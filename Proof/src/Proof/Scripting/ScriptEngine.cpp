// all Help to this wonderful guide
//https://peter1745.github.io/introduction.html
#include "Proofprch.h"
#include "ScriptEngine.h"


#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/attrdefs.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "ScriptFunc.h"
namespace Proof
{
    namespace Utils
    {
        void PrintAssemblyTypes(MonoAssembly* assembly) {
            MonoImage* image = mono_assembly_get_image(assembly);
            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
            PF_ENGINE_INFO("C# Assembly Types");
            for (int32_t i = 0; i < numTypes; i++) {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

                PF_ENGINE_INFO("{}.{}", nameSpace, name);
            }
        }
        char* ReadBytes(const std::string& filepath, uint32_t* outSize) {
            std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

            if (!stream) {
                // Failed to open the file
                return nullptr;
            }

            std::streampos end = stream.tellg();
            stream.seekg(0, std::ios::beg);
            uint32_t size = end - stream.tellg();

            if (size == 0) {
                // File is empty
                return nullptr;
            }

            char* buffer = new char[size];
            stream.read((char*)buffer, size);
            stream.close();

            *outSize = size;
            return buffer;
        }

        MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath) {
            uint32_t fileSize = 0;
            char* fileData = ReadBytes(assemblyPath, &fileSize);

            // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

            if (status != MONO_IMAGE_OK) {
                const char* errorMessage = mono_image_strerror(status);
                // Log some error message using the errorMessage data
                return nullptr;
            }

            MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
            mono_image_close(image);

            // Don't forget to free the file data
            delete[] fileData;

            return assembly;
        }
    }
    struct MonoData {
        MonoDomain* AppDomain = nullptr;
        MonoDomain* RootDomain = nullptr;
        MonoAssembly* CSharpAssembly = nullptr;
        MonoImage* CoreAssemblyImage = nullptr;
        ScriptClass EntityClass;
        World* CurrentWorld = nullptr;
        std::unordered_map<std::string, Count<ScriptClass>> ScriptEntityClasses;
        std::unordered_map<UUID, std::vector<Count<ScriptInstance>>> EntityInstances;
    };
    template <class Type>
    void LoadData(MonoObject* object, MonoClassField* classField, MonoFieldData& monoData) {
        Type val;
        mono_field_get_value(object, classField, &val);
        monoData.Data = val;
    }
    static MonoData* s_Data;
    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className) {
        MonoImage* image = mono_assembly_get_image(s_Data->CSharpAssembly);
        //data changes it to class
        m_MonoClass = mono_class_from_name(image, classNamespace.data(), className.data());
        MonoObject* defaultClass = Instantiate();

        MonoClassField* classFields;
        void* fieldsIter = nullptr;
        while (classFields = mono_class_get_fields(m_MonoClass, &fieldsIter)) {
            if (ScriptEngine::GetFieldAccessibility(classFields) ^ (uint8_t)ProofMonoAccessibility::Public)continue;

            MonoType* monoType = mono_field_get_type(classFields);
            PF_ENGINE_INFO("Name: {} Type: {}", mono_field_get_name(classFields), EnumReflection::EnumString<MonoTypeEnum>((MonoTypeEnum)mono_type_get_type(monoType)));
            MonoFieldData data;
            data.Name = mono_field_get_name(classFields);
            data.Type = (ProofMonoType)mono_type_get_type(monoType);
            switch (data.Type) {
                case Proof::ProofMonoType::None:
                    break;
                case Proof::ProofMonoType::Bool:
                    LoadData<bool>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Char:
                    break;
                case Proof::ProofMonoType::String:
                    break;
                case Proof::ProofMonoType::Uint8_t:
                    LoadData<uint8_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Uint16_t:
                    LoadData<uint16_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Uint32_t:
                    LoadData<uint32_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Uint64_t:
                    LoadData<uint64_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Int8_t:
                    LoadData<int8_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Int16_t:
                    LoadData<int16_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Int32_t:
                    LoadData<int32_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Int64_t:
                    LoadData<int64_t>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Float:
                    LoadData<float>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Double:
                    LoadData<double>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Class:
                    break;
                case Proof::ProofMonoType::Enum:
                    break;
                default:
                    break;
            }
            m_FieldData.emplace_back(data);
        }
    }
    MonoObject* ScriptClass::Instantiate() {
        PF_CORE_ASSERT(m_MonoClass, "Mono Class is nullptr");
        return ScriptEngine::InstantiateClass(m_MonoClass);
    }
    MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount) {
        return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
    }
    MonoObject* ScriptClass::CallMethod(MonoObject* instance, MonoMethod* method, void** params) {
        return mono_runtime_invoke(method, instance, params, nullptr);
    }

    ScriptInstance::ScriptInstance(Count<ScriptClass> scriptClass, Entity entity) :
        m_ScriptClass(scriptClass) {
        m_Instance = scriptClass->Instantiate();

        m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
        m_OnCreate = scriptClass->GetMethod("OnCreate", 0);
        m_OnUpdate = scriptClass->GetMethod("OnUpdate", 1);
        m_OnPlaced = scriptClass->GetMethod("OnPlace", 0);
        m_OnSpawn = scriptClass->GetMethod("OnSpawn", 0);
        m_OnDestroy = scriptClass->GetMethod("OnDestroy", 0);
        // Call Entity constructor
        {
            UUID entityID = entity.GetID();
            void* param = &entityID;
            m_ScriptClass->CallMethod(m_Instance, m_Constructor, &param);
        }
    }

    void ScriptInstance::CallOnCreate() {
        if (m_OnCreate)
            m_ScriptClass->CallMethod(m_Instance, m_OnCreate);
    }

    void ScriptInstance::CallOnUpdate(float ts) {
        if (m_OnUpdate == nullptr)return;
        void* param = &ts;
        m_ScriptClass->CallMethod(m_Instance, m_OnUpdate, &param);

    }
    void ScriptInstance::CallOnPlace() {
        if (m_OnPlaced)
            m_ScriptClass->CallMethod(m_Instance, m_OnPlaced);
    }
    void ScriptInstance::CallOnSpawn() {
        if (m_OnSpawn)
            m_ScriptClass->CallMethod(m_Instance, m_OnSpawn);
    }
    void ScriptInstance::CallOnDestroy() {
        if (m_OnDestroy)
            m_ScriptClass->CallMethod(m_Instance, m_OnDestroy);
    }
    void ScriptEngine::Init() {
        s_Data = new MonoData();
        InitMono();
    }
    void ScriptEngine::Shutdown() {

    }

    void ScriptEngine::StartWorld(World* world) {
        s_Data->CurrentWorld = world;
    }
    void ScriptEngine::EndWorld() {
        s_Data->EntityInstances.clear();
        s_Data->CurrentWorld = nullptr;
    }
    World* ScriptEngine::GetWorldContext() {
        return s_Data->CurrentWorld;
    }
    void ScriptEngine::OnUpdate(float ts, Entity entity) {
        PF_CORE_ASSERT(s_Data->EntityInstances.find(entity.GetID()) != s_Data->EntityInstances.end());
        //holding reference so we do not make a copy each iteration
        for (auto& a : s_Data->EntityInstances[entity.GetID()]) {
            a->CallOnUpdate(ts);
        }

    }
    void ScriptEngine::OnCreate(Entity entity) {
        auto sc = *entity.GetComponent<ScriptComponent>();
        for (auto& Class : sc.m_Scripts) {
            if (ScriptEngine::EntityClassExists(Class.ClassName) == true) {
                Count<ScriptInstance> instance = CreateCount<ScriptInstance>(s_Data->ScriptEntityClasses[Class.ClassName], entity);
                s_Data->EntityInstances[entity.GetID()].emplace_back(instance);
                for (auto& scriptData : sc.m_Scripts) {
                    for (auto& prop : scriptData.Fields) {
                        MonoClassField* currentField = mono_class_get_field_from_name(instance->m_ScriptClass->GetMonoClass(), prop.Name.c_str());
                        //mono_field_set_value(instance->m_Instance, currentField, &prop.Data);
                        switch (prop.Type) {
                            case Proof::ProofMonoType::Uint8_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<uint8_t>());
                                break;
                            case Proof::ProofMonoType::Uint16_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<uint16_t>());
                                break;
                            case Proof::ProofMonoType::Uint32_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<uint32_t>());
                                break;
                            case Proof::ProofMonoType::Uint64_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<uint64_t>());
                                break;
                            case Proof::ProofMonoType::Int8_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<int8_t>());
                                break;
                            case Proof::ProofMonoType::Int16_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<int16_t>());
                                break;
                            case Proof::ProofMonoType::Int32_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<int32_t>());
                                break;
                            case Proof::ProofMonoType::Int64_t:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<int64_t>());
                                break;
                            case Proof::ProofMonoType::Float:
                                mono_field_set_value(instance->m_Instance, currentField, &pro.Data._Cast<float>();
                                break;
                            case Proof::ProofMonoType::Double:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<double>());
                                break;
                            case Proof::ProofMonoType::Class:
                                break;
                            case Proof::ProofMonoType::None:
                                break;
                            case Proof::ProofMonoType::Bool:
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<bool>());
                                break;
                            case Proof::ProofMonoType::Char:
                                break;
                            case Proof::ProofMonoType::String:
                                break;
                            case Proof::ProofMonoType::Enum:
                                break;
                            default:
                                break;
                        }
                    }
                }
                instance->CallOnCreate();
            }
        }
      
    }
    void ScriptEngine::OnSpawn(Entity entity) {
        PF_CORE_ASSERT(s_Data->EntityInstances.find(entity.GetID()) != s_Data->EntityInstances.end());
        //holding reference so we do not make a copy each iteration
        for (auto& a : s_Data->EntityInstances[entity.GetID()]) {
            a->CallOnSpawn();
        }
    }
    void ScriptEngine::OnPlace(Entity entity) {
        PF_CORE_ASSERT(s_Data->EntityInstances.find(entity.GetID()) != s_Data->EntityInstances.end());
        //holding reference so we do not make a copy each iteration
        for (auto& a : s_Data->EntityInstances[entity.GetID()]) {
            a->CallOnPlace();
        }
    }
    void ScriptEngine::OnDestroy(Entity entity) {
        PF_CORE_ASSERT(s_Data->EntityInstances.find(entity.GetID()) != s_Data->EntityInstances.end());
        //holding reference so we do not make a copy each iteration
        for (auto& a : s_Data->EntityInstances[entity.GetID()]) {
            a->CallOnDestroy();
        }
    }
    const ScriptClass* ScriptEngine::GetScriptClass(const std::string& name) {
        if (EntityClassExists(name) == false)return nullptr;
        return s_Data->ScriptEntityClasses[name].get();
    }
    void ScriptEngine::SetValue(UUID ID, const std::string& className,const std::string& varName, void* data) {
        const ScriptInstance* instance = nullptr;
        auto& temp = s_Data->EntityInstances[ID];
        for (auto& classes : temp) {
            if (classes->m_ScriptClass->GeClassName() == className)
                instance = classes.get();
        }
        MonoClassField* currentField = mono_class_get_field_from_name(instance->m_ScriptClass->GetMonoClass(), varName.c_str());
        mono_field_set_value(instance->m_Instance, currentField, data);
    }
    template<class T>
    T ScriptEngine::GetValue(UUID ID, const std::string& className, const std::string& varName) {
        const ScriptInstance* instance = nullptr;
        auto& temp = s_Data->EntityInstances[ID];
        for (auto& classes : temp) {
            if (classes->m_ScriptClass->GeClassName() == className)
                instance = classes.get();
        }
        MonoClassField* currentField = mono_class_get_field_from_name(instance->m_ScriptClass->GetMonoClass(), varName.c_str());
        T val;
        mono_field_get_value(instance->m_Instance, currentField, &val);
        return val;
    }
    MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass) {
        MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
        //calling default constructor
        mono_runtime_object_init(instance);
        return instance;
    }
    bool ScriptEngine::EntityClassExists(const std::string& fullClassName) {
        return s_Data->ScriptEntityClasses.find(fullClassName) != s_Data->ScriptEntityClasses.end();
    }
    void ScriptEngine::InitMono() {
        mono_set_assemblies_path("mono/lib");

        s_Data->RootDomain = mono_jit_init("ProofJITRuntime");
        PF_CORE_ASSERT(s_Data->RootDomain, "failed to initilize mono domain");

        // Create an App Domain
        s_Data->AppDomain = mono_domain_create_appdomain("ProofDomainRuntime", nullptr);
        mono_domain_set(s_Data->RootDomain, true);

        s_Data->CSharpAssembly = Utils::LoadCSharpAssembly("Resources/Scripts/ProofScript.dll");
        s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CSharpAssembly);
        // Utils::PrintAssemblyTypes(s_Data->CSharpAssembly);
        LoadAssemblyClasses(s_Data->CSharpAssembly);
        s_Data->EntityClass = ScriptClass("Proof", "Entity");

        ScriptFunc::RegisterAllComponents();
        ScriptFunc::RegisterFunctions();
    }
    void ScriptEngine::LoadAssemblyClasses(MonoAssembly* assembly) {

        s_Data->ScriptEntityClasses.clear();
        MonoImage* image = mono_assembly_get_image(assembly);


        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
        MonoClass* entityClass = mono_class_from_name(image, "Proof", "Entity");
        PF_ENGINE_TRACE("C# Script Classes");
        for (int32_t i = 0; i < numTypes; i++) {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);
            //one reason it would be null is bcause it is an enum and the namespace kindd affects it
            if (monoClass == nullptr)continue;
            if (mono_class_is_enum(monoClass))continue;
            std::string fullName;

            if (strlen(nameSpace) != 0) // length of a string
                fullName = fmt::format("{}.{}", nameSpace, name);
            else
                fullName = name;
            if (monoClass == entityClass)
                continue;

            bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
            if (isEntity) {
                s_Data->ScriptEntityClasses[fullName] = CreateCount<ScriptClass>(nameSpace, name);
                PF_ENGINE_TRACE("   Added To Script Class {}", fullName);
            }
        }
    }

    MonoImage* ScriptEngine::GetCoreAssemblyImage() {
        return s_Data->CoreAssemblyImage;
    }

    // Gets the accessibility level of the given property
    uint8_t ScriptEngine::GetFieldAccessibility(MonoClassField* field) {
        uint8_t accessibility = (uint8_t)ProofMonoAccessibility::None;
        uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

        switch (accessFlag) {
            case MONO_FIELD_ATTR_PRIVATE:
                {
                    accessibility = (uint8_t)ProofMonoAccessibility::Private;
                    break;
                }
            case MONO_FIELD_ATTR_FAM_AND_ASSEM:
                {
                    accessibility |= (uint8_t)ProofMonoAccessibility::Protected;
                    accessibility |= (uint8_t)ProofMonoAccessibility::Internal;
                    break;
                }
            case MONO_FIELD_ATTR_ASSEMBLY:
                {
                    accessibility = (uint8_t)ProofMonoAccessibility::Internal;
                    break;
                }
            case MONO_FIELD_ATTR_FAMILY:
                {
                    accessibility = (uint8_t)ProofMonoAccessibility::Protected;
                    break;
                }
            case MONO_FIELD_ATTR_FAM_OR_ASSEM:
                {
                    accessibility |= (uint8_t)ProofMonoAccessibility::Private;
                    accessibility |= (uint8_t)ProofMonoAccessibility::Protected;
                    break;
                }
            case MONO_FIELD_ATTR_PUBLIC:
                {
                    accessibility = (uint8_t)ProofMonoAccessibility::Public;
                    break;
                }
        }
        return accessibility;
    }
    uint8_t ScriptEngine::GetPropertyAccessbility(MonoProperty* property) {
        uint8_t accessibility = (uint8_t)ProofMonoAccessibility::None;

        // Get a reference to the property's getter method
        MonoMethod* propertyGetter = mono_property_get_get_method(property);
        if (propertyGetter != nullptr) {
            // Extract the access flags from the getters flags
            uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

            switch (accessFlag) {
                case MONO_FIELD_ATTR_PRIVATE:
                    {
                        accessibility = (uint8_t)ProofMonoAccessibility::Private;
                        break;
                    }
                case MONO_FIELD_ATTR_FAM_AND_ASSEM:
                    {
                        accessibility |= (uint8_t)ProofMonoAccessibility::Protected;
                        accessibility |= (uint8_t)ProofMonoAccessibility::Internal;
                        break;
                    }
                case MONO_FIELD_ATTR_ASSEMBLY:
                    {
                        accessibility = (uint8_t)ProofMonoAccessibility::Internal;
                        break;
                    }
                case MONO_FIELD_ATTR_FAMILY:
                    {
                        accessibility = (uint8_t)ProofMonoAccessibility::Protected;
                        break;
                    }
                case MONO_FIELD_ATTR_FAM_OR_ASSEM:
                    {
                        accessibility |= (uint8_t)ProofMonoAccessibility::Private;
                        accessibility |= (uint8_t)ProofMonoAccessibility::Protected;
                        break;
                    }
                case MONO_FIELD_ATTR_PUBLIC:
                    {
                        accessibility = (uint8_t)ProofMonoAccessibility::Public;
                        break;
                    }
            }
        }
        // C# DOES NOT REQUIRE Both getter and setter to be present only one
        // so no setter means that we cannot write to dat so we can jsut mark it as nullptr
        // Get a reference to the property's setter method
        MonoMethod* propertySetter = mono_property_get_set_method(property);
        if (propertySetter != nullptr) {
            // Extract the access flags from the setters flags
            uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
            if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
                accessibility = (uint8_t)ProofMonoAccessibility::Private;
        }
        else {
            accessibility = (uint8_t)ProofMonoAccessibility::Private;
        }

        return accessibility;
    }
}
