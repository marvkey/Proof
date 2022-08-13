// all Help to this wonderful guide
//https://peter1745.github.io/introduction.html
#include "Proofprch.h"
#include "ScriptEngine.h"


#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
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
    static MonoData* s_Data;

    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className) {
        MonoImage* image = mono_assembly_get_image(s_Data->CSharpAssembly);
        //data changes it to class
        m_MonoClass = mono_class_from_name(image, classNamespace.data(), className.data());
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

    ScriptInstance::ScriptInstance(Count<ScriptClass> scriptClass, Entity entity):
    m_ScriptClass(scriptClass)
    {
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
        if(m_OnCreate)
            m_ScriptClass->CallMethod(m_Instance, m_OnCreate);
    }

    void ScriptInstance::CallOnUpdate(float ts) {
        if (m_OnUpdate == nullptr)return;
        void* param = &ts;
        m_ScriptClass->CallMethod(m_Instance, m_OnUpdate, &param);

    }
    void ScriptInstance::CallOnPlace() {
        if (m_OnPlaced)
            m_ScriptClass->CallMethod(m_Instance,m_OnPlaced);
    }
    void ScriptInstance::CallOnSpawn() {
        if (m_OnSpawn)
            m_ScriptClass->CallMethod(m_Instance, m_OnSpawn);
    }
    void ScriptInstance::CallOnDestroy() {
        if(m_OnDestroy)
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
        sc.ForEachScript([&](auto& className) {
            if (ScriptEngine::EntityClassExists(className)) {
                Count<ScriptInstance> instance = CreateCount<ScriptInstance>(s_Data->ScriptEntityClasses[className], entity);
                s_Data->EntityInstances[entity.GetID()].emplace_back(instance);
                instance->CallOnCreate();
            }
        });
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
            std::string fullName;

            if (strlen(nameSpace) != 0) // length of a string
                fullName = fmt::format("{}.{}", nameSpace, name);
            else
                fullName = name;

            MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);
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
}
