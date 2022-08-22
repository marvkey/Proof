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
    bool CheckMonoError(MonoError& error) {
        bool hasError = !mono_error_ok(&error);
        if (hasError) {
            uint32_t errorCode = mono_error_get_error_code(&error);
            const char* errorMessage = mono_error_get_message(&error);
            PF_ERROR("Code: {} Message: {}", errorCode, errorMessage);
            mono_error_cleanup(&error);
        }
        return hasError;
    }
    namespace Utils
    {
        void PrintAssemblyTypes(MonoAssembly* assembly) {
            MonoImage* image = mono_assembly_get_image(assembly);
            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

            for (int32_t i = 0; i < numTypes; i++) {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
                PF_ENGINE_TRACE("{}.{}", nameSpace, name);
            }
        }
        static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize) {
            std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

            if (!stream) {
                // Failed to open the file
                return nullptr;
            }

            std::streampos end = stream.tellg();
            stream.seekg(0, std::ios::beg);
            uint64_t size = end - stream.tellg();

            if (size == 0) {
                // File is empty
                return nullptr;
            }

            char* buffer = new char[size];
            stream.read((char*)buffer, size);
            stream.close();

            *outSize = (uint32_t)size;
            return buffer;
        }

        static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath) {
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

            std::string pathString = assemblyPath.string();
            MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
            mono_image_close(image);

            // Don't forget to free the file data
            delete[] fileData;

            return assembly;
        }
    }
    struct MonoData {
        MonoDomain* RootDomain = nullptr;
        MonoDomain* AppDomain = nullptr;

        MonoAssembly* CoreAssembly = nullptr;
        MonoImage* CoreAssemblyImage = nullptr;

        MonoAssembly* AppAssembly = nullptr;
        MonoImage* AppAssemblyImage = nullptr;
        Count<ScriptClass> EntityClass;

        std::unordered_map<std::string, Count<ScriptClass>> ScriptEntityClasses;
        std::unordered_map<UUID, std::vector<Count<ScriptInstance>>> EntityInstances;
        
        World* CurrentWorld = nullptr;
    };
    template <class Type>
    void LoadData(MonoObject* object, MonoClassField* classField, ScriptField& monoData) {
        Type val;
        mono_field_get_value(object, classField, &val);
        monoData.Data = val;
    }
    static MonoData* s_Data;
    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore):
        m_ClassNamespace(classNamespace), m_ClassName(className)
    {
        MonoImage* image = isCore == false ? s_Data->AppAssemblyImage : s_Data->CoreAssemblyImage;
        //data changes it to class
        m_MonoClass = mono_class_from_name(image, classNamespace.data(), className.data());
        MonoObject* defaultClass = Instantiate();

        MonoClassField* classFields;
        void* fieldsIter = nullptr;
        while (classFields = mono_class_get_fields(m_MonoClass, &fieldsIter)) {
            if (ScriptEngine::GetFieldAccessibility(classFields) ^ (uint8_t)ProofMonoAccessibility::Public)continue;

            MonoType* monoType = mono_field_get_type(classFields);
            //if(appAssembly==true) // dont want data for entity class preety annoying
            //    PF_ENGINE_INFO("Name: {} Type: {}", mono_field_get_name(classFields), EnumReflection::EnumString<MonoTypeEnum>((MonoTypeEnum)mono_type_get_type(monoType)));
            ScriptField data;
            data.Name = mono_field_get_name(classFields);
            data.Type = (ProofMonoType)mono_type_get_type(monoType);
            
            if ((int)data.Type == MonoTypeEnum::MONO_TYPE_CLASS) {
                MonoClass* type = mono_type_get_class(monoType);
                if (type == s_Data->EntityClass->m_MonoClass ) {
                    data.Type = ProofMonoType::Entity;
                }
                
            }
            
            switch (data.Type) {
                case Proof::ProofMonoType::Bool:
                    LoadData<bool>(defaultClass, classFields, data);
                    break;
                case Proof::ProofMonoType::Char: 
                    {
                        
                        void* gotData;
                        mono_field_get_value(defaultClass, classFields, &gotData);
                        data.Data= (char)gotData;
                        break;
                    }
                case Proof::ProofMonoType::String:
                    {
                        MonoString* stringValue;
                        mono_field_get_value(defaultClass, classFields, &stringValue);
                        data.Data = ScriptEngine::MonoToString(stringValue);
                        break;
                    }
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
                    {
                      //  mono_class_get_name(mono_type_get_class(monoType));
                        //format is {EnumNameSpace}{EnumName}:{EnumType(int,uint32_t)..}: {Name of variable}
                        std::string nameSpace = mono_class_get_namespace(mono_type_get_class(monoType));
                        if (nameSpace.empty() == false) {
                            data.Name = fmt::format("{}.{}:{}:{}", nameSpace, mono_class_get_name(mono_type_get_class(monoType)), EnumReflection::EnumString<ProofMonoType>((ProofMonoType)mono_type_get_type(mono_class_enum_basetype(mono_type_get_class(monoType)))), mono_field_get_name(classFields));
                        }
                        else {
                            data.Name = fmt::format("{}:{}:{}", mono_class_get_name(mono_type_get_class(monoType)), EnumReflection::EnumString<ProofMonoType>((ProofMonoType)mono_type_get_type(mono_class_enum_basetype(mono_type_get_class(monoType)))), mono_field_get_name(classFields));
                        }
                  
                        switch ((ProofMonoType)mono_type_get_type(mono_class_enum_basetype(mono_type_get_class(monoType)))) {
                            case Proof::ProofMonoType::Uint8_t:
                                break;
                            case Proof::ProofMonoType::Uint16_t:
                                break;
                            case Proof::ProofMonoType::Uint32_t:
                                break;
                            case Proof::ProofMonoType::Uint64_t:
                                break;
                            case Proof::ProofMonoType::Int8_t:
                                break;
                            case Proof::ProofMonoType::Int16_t:
                                break;
                            case Proof::ProofMonoType::Int32_t:
                                {
                                    int32_t gotData;
                                    mono_field_get_value(defaultClass, classFields, &gotData);
                                    data.Data = (int32_t)gotData;
                                    break;
                                }
                            case Proof::ProofMonoType::Int64_t:
                                break;
                            case Proof::ProofMonoType::Float:
                                break;
                            case Proof::ProofMonoType::Double:
                                break;
                          
                            default:
                                break;
                        }
                        break;
                    }
                case Proof::ProofMonoType::Entity:
                    data.Data = (uint64_t)0; //we know it woudl always be zero when initilized 
                    break;
                case Proof::ProofMonoType::Array:
                    {
                        
                        break;
                    }
                default:
                    continue;
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

        m_Constructor = s_Data->EntityClass->GetMethod(".ctor", 1);
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
        LoadAssembly("Resources/Scripts/ProofScriptCore.dll");
        LoadAppAssembly("GameProject/Asset/Scripts/Binaries/Game.dll");
        LoadAssemblyClasses();

        ScriptFunc::RegisterAllComponents();
        ScriptFunc::RegisterFunctions();

        s_Data->EntityClass = CreateCount<ScriptClass>("Proof", "Entity", true);
    }
    void ScriptEngine::Shutdown() {

    }

    void ScriptEngine::StartWorld(World* world) {
        s_Data->CurrentWorld = world;
    }
    void ScriptEngine::EndWorld() {
        s_Data->CurrentWorld = nullptr;
        s_Data->EntityInstances.clear();
    }
    void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath) {
                // Create an App Domain
        s_Data->AppDomain = mono_domain_create_appdomain("ProofScriptRuntime", nullptr);
        mono_domain_set(s_Data->AppDomain, true);

        // Move this maybe
        s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
        s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
        // Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
    }
    void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath) {
        s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
        //auto assemb = s_Data->AppAssembly;
        s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
       // auto assembi = s_Data->AppAssemblyImage;
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
        for (auto& script : sc.m_Scripts) {
            if (ScriptEngine::EntityClassExists(script.ClassName) == false)continue;
            Count<ScriptInstance> instance = CreateCount<ScriptInstance>(s_Data->ScriptEntityClasses[script.ClassName], entity);
            s_Data->EntityInstances[entity.GetID()].emplace_back(instance);
            for (auto& prop : script.Fields) {
                MonoClassField* currentField = mono_class_get_field_from_name(instance->m_ScriptClass->GetMonoClass(), prop.Name.c_str());
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
                        mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<float>());
                        break;
                    case Proof::ProofMonoType::Double:
                        mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<double>());
                        break;
                    case Proof::ProofMonoType::Class:
                        break;
                    case Proof::ProofMonoType::Bool:
                        mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<bool>());
                        break;
                    case Proof::ProofMonoType::Char:
                        mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<char>());
                        break;
                    case Proof::ProofMonoType::String:
                        {
                            MonoString* string = mono_string_new(s_Data->AppDomain,prop.Data._Cast<std::string>()->c_str());
                            mono_field_set_value(instance->m_Instance, currentField, string);
                            break;
                        }
                    case Proof::ProofMonoType::Enum:
                        {
                            auto fieldName = prop.Name.substr(prop.Name.find_last_of(":")+1);
                            //cause mono enum names the names are treated differently
                            currentField = mono_class_get_field_from_name(instance->m_ScriptClass->GetMonoClass(), fieldName.c_str());
                            MonoType* monoType = mono_field_get_type(currentField);

                            switch ((ProofMonoType)mono_type_get_type(mono_class_enum_basetype(mono_type_get_class(monoType)))) {
                                case Proof::ProofMonoType::Uint8_t:
                                    break;
                                case Proof::ProofMonoType::Uint16_t:
                                    break;
                                case Proof::ProofMonoType::Uint32_t:
                                    break;
                                case Proof::ProofMonoType::Uint64_t:
                                    break;
                                case Proof::ProofMonoType::Int8_t:
                                    break;
                                case Proof::ProofMonoType::Int16_t:
                                    break;
                                case Proof::ProofMonoType::Int32_t:
                                    mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<int32_t>());
                                    break;
                                case Proof::ProofMonoType::Int64_t:
                                    break;
                                case Proof::ProofMonoType::Float:
                                    break;
                                case Proof::ProofMonoType::Double:
                                    break;

                                default:
                                    break;
                            }
                            break;
                        }
                    case Proof::ProofMonoType::Entity: 
                        {
                            if(s_Data->CurrentWorld->HasEnitty(*prop.Data._Cast<uint64_t>()))
                                mono_field_set_value(instance->m_Instance, currentField, prop.Data._Cast<uint64_t>());
                            break;
                        }
                    default:
                        break;
                }
            }
            instance->CallOnCreate();
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

    void ScriptEngine::SetValue(UUID ID,const std::string& className, const std::string& varName, void* data) {
        const ScriptInstance* instance = nullptr;
        auto& temp = s_Data->EntityInstances[ID];
        for (auto& classes : temp) {
            auto fullName = classes->m_ScriptClass->GetFullName();

            if (fullName == className) {
                instance = classes.get();
                break;
            }
        }

        MonoClassField* currentField = mono_class_get_field_from_name(instance->m_ScriptClass->GetMonoClass(), varName.c_str());
        mono_field_set_value(instance->m_Instance, currentField, data);
    }

  

    bool ScriptEngine::IsFieldAvailable(const std::string& className, const std::string& varName) {
        if (EntityClassExists(className) == false) return false;
        auto& scriptClass = s_Data->ScriptEntityClasses[className];
        MonoClassField* field = mono_class_get_field_from_name(scriptClass->GetMonoClass(), varName.c_str());
        return field != nullptr;
    }

    bool ScriptEngine::IsFieldAvailable(const std::string& className, const std::string& varName, ProofMonoType type) {
        if (EntityClassExists(className) == false) return false;
        auto& scriptClass = s_Data->ScriptEntityClasses[className];
        MonoClassField* field = mono_class_get_field_from_name(scriptClass->GetMonoClass(), varName.c_str());
        if (field == nullptr)return false;
        MonoType* monoType = mono_field_get_type(field);

        if (type == ProofMonoType::Entity)
            type = ProofMonoType::Class;
        if (type == (ProofMonoType)mono_type_get_type(monoType))
            return true;
        return false;
    }
    


    void ScriptEngine::ReloadAssembly(World* world) {
        PF_CORE_ASSERT(world);
        LoadAssembly("Resources/Scripts/ProofScriptCore.dll");
        LoadAppAssembly("GameProject/Asset/Scripts/Binaries/Game.dll");
        LoadAssemblyClasses();

        ScriptFunc::RegisterAllComponents();
        ScriptFunc::RegisterFunctions();

        world->ForEachComponent<ScriptComponent>([](ScriptComponent& comp) {
            //using normal for loop since we might be removing the data in theri
            for (int i = 0; i < comp.m_Scripts.size(); i++) {
                auto& script = comp.m_Scripts[i];
                auto& fields = script.Fields;

                if (EntityClassExists(script.ClassName) == false) {
                    comp.m_Scripts.erase(comp.m_Scripts.begin() + i);
                    continue;
                }
                for (int j = 0; j < script.Fields.size(); j++) {
                    auto& field = fields[j];
                    // first get rid of unecesary fields
                    // make sure we are ching teh same type of each
                    if (IsFieldAvailable(script.ClassName, field.Name,field.Type) == false) {
                        script.Fields.erase(script.Fields.begin() + j);
                        continue;
                    }
                }
                const ScriptClass* scriptClass = ScriptEngine::GetScriptClass(script.ClassName);
                if (scriptClass != nullptr) {
                    for (auto& data : scriptClass->m_FieldData) {
                        if (script.HasField(data.Name) == true)
                            continue;
                        ScriptField field;
                        field.Name = data.Name;
                        field.Type = data.Type;
                        field.Data = data.Data;
                        script.Fields.emplace_back(field);
                    }
                }
            }
        });
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

        MonoDomain* rootDomain = mono_jit_init("ProofJITRuntime");
        PF_CORE_ASSERT(rootDomain);

        // Store the root domain pointer
        s_Data->RootDomain = rootDomain;

    }
    void ScriptEngine::LoadAssemblyClasses() {

        s_Data->ScriptEntityClasses.clear();

        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
        MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Proof", "Entity");
        PF_ENGINE_TRACE("C# Script Classes");
        for (int32_t i = 0; i < numTypes; i++) {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
            const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

            MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);
            //one reason it would be null is bcause it is an enum and the namespace kindd affects it
            if (monoClass == nullptr)continue;
            if (mono_class_is_enum(monoClass))continue;
            std::string fullName;
            MonoImage* monoImage = mono_class_get_image(monoClass);
            if (strlen(nameSpace) != 0) // length of a string
                fullName = fmt::format("{}.{}", nameSpace, name);
            else
                fullName = name;
            if (monoClass == entityClass)
                continue;

            bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
            ;
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
    const std::unordered_map<std::string, Count<ScriptClass>>const& ScriptEngine::GetScripts() {
        return s_Data->ScriptEntityClasses;
    }
    std::string ScriptEngine::MonoToString(MonoString* monoString){
        if (monoString == nullptr || mono_string_length(monoString) == 0)
            return "";

        MonoError error;
        char* utf8 = mono_string_to_utf8_checked(monoString, &error);
        if (CheckMonoError(error))
            return "";
        std::string result(utf8);
        mono_free(utf8);
        return result;

    }
    MonoString* ScriptEngine::StringToMono(const std::string& data) {
        MonoString* string = mono_string_new(s_Data->AppDomain, data.c_str());
        return string;
    }

    void ScriptEngine::ForEachEnumType(const std::string& classFullName, const std::function<void(const std::string&, std::any)>& func) {
        std::string nameSpaceData = classFullName.substr(0, classFullName.find_first_of("."));
        std::string name = classFullName.substr(classFullName.find_first_of(".") + 1);
        MonoImage* image;
        MonoClass* monoClass;

        if (nameSpaceData == "Proof") {
            MonoImage* image = s_Data->CoreAssemblyImage;
            if (nameSpaceData.empty())
                monoClass = mono_class_from_name(image, "", name.c_str());
            else
                monoClass = mono_class_from_name(image, nameSpaceData.c_str(), name.c_str());
        }

        else if (monoClass == nullptr) {
            if (nameSpaceData.empty())
                monoClass = mono_class_from_name(image, "", name.c_str());
            else
                monoClass = mono_class_from_name(image, nameSpaceData.c_str(), name.c_str());
        }
        
        if (monoClass == nullptr)return;
        MonoClassField* classFields;
        void* fieldsIter = nullptr;
        int index = 0;

        //GONNA HAVE A LIST OF ALL ENUMS AND their possible values 
        // the enums will have to be labelled public though
        MonoObject* enumObject =mono_object_new(s_Data->AppDomain, monoClass);
        while (classFields = mono_class_get_fields(monoClass, &fieldsIter)) {
            // the first field is some wierd "value_" probably a built in variable for the enums
            if (index == 0) {
                index++;
                continue;
            }
            MonoType* monoType = mono_field_get_type(classFields);
            const std::string val = mono_field_get_name(classFields);
            switch ((ProofMonoType)mono_type_get_type(mono_class_enum_basetype(mono_type_get_class(monoType)))) {
                case Proof::ProofMonoType::Uint8_t:
                    {
                        uint8_t* data;
                        mono_field_get_value(enumObject, classFields, data);
                        std::any anyData = *data;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Uint16_t:
                    {
                        uint16_t* data;
                        mono_field_get_value(enumObject, classFields, data);
                        std::any anyData = *data;
                        func(val, anyData);
                        break;
                    } 
                case Proof::ProofMonoType::Uint32_t:
                    {
                        uint32_t* data;
                        mono_field_get_value(enumObject, classFields, data);
                        std::any anyData = *data;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Uint64_t:
                    {
                        uint64_t* data;
                        mono_field_get_value(enumObject, classFields, data);
                        std::any anyData = *data;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Int8_t:
                    {
                        int8_t* data;
                        mono_field_get_value(enumObject, classFields, data);
                        std::any anyData = *data;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Int16_t:
                    {
                        int16_t* data;
                        mono_field_get_value(enumObject, classFields, data);
                        std::any anyData = *data;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Int32_t:
                    {
                        int32_t gotData;
                        
                       // mono_field_get_value(enumObject, classFields, &gotData);
                        mono_field_static_get_value(mono_class_vtable(s_Data->AppDomain, monoClass), classFields, &gotData);

                        std::any anyData = gotData;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Int64_t:
                    {
                        int64_t data;
                        mono_field_get_value(enumObject, classFields, &data);
                        std::any anyData = data;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Float:
                    {
                        float data;
                        mono_field_get_value(enumObject, classFields, &data);
                        std::any anyData = data;
                        func(val, anyData);
                        break;
                    }
                case Proof::ProofMonoType::Double:
                    {
                        double data;
                        mono_field_get_value(enumObject, classFields,& data);
                        std::any anyData = data;
                        func(val, anyData);
                        break;
                    }
                default:
                    break;
            }
            index++;
        }
    }
}