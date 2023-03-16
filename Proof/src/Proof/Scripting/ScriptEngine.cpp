// all Help to this wonderful guide
//https://peter1745.github.io/introduction.html
#include "Proofprch.h"
#include "ScriptEngine.h"


#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

#include "mono/metadata/attrdefs.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "ScriptFunc.h"
#include <fmt/format.h>
#include "Proof/Core/Buffer.h"
namespace Proof
{
    static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
    {
        { "System.Single", ScriptFieldType::Float },
        { "System.Double", ScriptFieldType::Double },
        { "System.Boolean", ScriptFieldType::Bool },
        { "System.Char", ScriptFieldType::Char },
        { "System.Int16", ScriptFieldType::Short },
        { "System.Int32", ScriptFieldType::Int },
        { "System.Int64", ScriptFieldType::Long },
        { "System.Byte", ScriptFieldType::Byte },
        { "System.UInt16", ScriptFieldType::UShort },
        { "System.UInt32", ScriptFieldType::UInt },
        { "System.UInt64", ScriptFieldType::ULong },

        { "Proof.Vector2", ScriptFieldType::Vector2 },
        { "Proof.Vector3", ScriptFieldType::Vector3 },
        { "Proof.Vector4", ScriptFieldType::Vector4 },

        { "Proof.Entity", ScriptFieldType::Entity },
    };
   
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
    namespace Utils {

        static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB = false)
        {
            ScopedBuffer fileData = FileSystem::ReadFileBinary(assemblyPath);

            // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size(), 1, &status, 0);

            if (status != MONO_IMAGE_OK)
            {
                const char* errorMessage = mono_image_strerror(status);
                // Log some error message using the errorMessage data
                return nullptr;
            }

            if (loadPDB)
            {
                std::filesystem::path pdbPath = assemblyPath;
                pdbPath.replace_extension(".pdb");

                if (std::filesystem::exists(pdbPath))
                {
                    ScopedBuffer pdbFileData = FileSystem::ReadFileBinary(pdbPath);
                    mono_debug_open_image_from_memory(image, pdbFileData.As<const mono_byte>(), pdbFileData.Size());
                    PF_ENGINE_INFO("Loaded PDB {}", pdbPath.string());
                }
            }

            std::string pathString = assemblyPath.string();
            MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
            mono_image_close(image);

            return assembly;
        }

        void PrintAssemblyTypes(MonoAssembly* assembly)
        {
            MonoImage* image = mono_assembly_get_image(assembly);
            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

            for (int32_t i = 0; i < numTypes; i++)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
                PF_ENGINE_TRACE("{}.{}", nameSpace, name);
            }
        }

        ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
        {
            std::string typeName = mono_type_get_name(monoType);

            auto it = s_ScriptFieldTypeMap.find(typeName);
            if (it == s_ScriptFieldTypeMap.end())
            {
                PF_ENGINE_ERROR("Unknown type: {}", typeName);
                return ScriptFieldType::None;
            }

            return it->second;
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

        //(ENitty ID), (class name,{script name, script inctance)
        std::unordered_map<EntityID, std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>> EntityScriptFields;
        std::unordered_map<std::string, Count<ScriptClass>> ScriptEntityClasses;

        // entity id, (name of class, instance of it)
        std::unordered_map<EntityID, std::unordered_map<std::string,Count<ScriptInstance>>> EntityInstances;
        
        World* CurrentWorld = nullptr;

        #ifdef PF_ENABLE_DEBUG 
            bool EnableDebugging = true;
        #else
            bool EnableDebugging = false;
        #endif
    };
    
    static MonoData* s_Data;
    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore):
        m_ClassNamespace(classNamespace), m_ClassName(className)
    {
        m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
        m_Instance = mono_object_new(s_Data->AppDomain, m_MonoClass);
        mono_runtime_object_init(m_Instance);
    }
    bool ScriptClass::GetFieldDefaultValueInternal(const std::string& name, void* buffer)
    {

        if (!GetFields().contains(name))
            return false;

        MonoClassField* field = mono_class_get_field_from_name(m_MonoClass, name.c_str());
        
        mono_field_get_value(m_Instance, field, buffer);
        return true;
    }
    MonoObject* ScriptClass::Instantiate() {
        PF_CORE_ASSERT(m_MonoClass, "Mono Class is nullptr");
        return ScriptEngine::InstantiateClass(m_MonoClass);
    }
    MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount) {
        return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
    }
    MonoObject* ScriptClass::CallMethod(MonoObject* instance, MonoMethod* method, void** params) {

        MonoObject* exception = nullptr;
        return mono_runtime_invoke(method, instance, params, &exception);
    }
    ScriptInstance::ScriptInstance(Count<ScriptClass> scriptClass, Entity entity) :
        m_ScriptClass(scriptClass) {
        m_Instance = scriptClass->Instantiate();

       	m_Constructor = s_Data->EntityClass->GetMethod(".ctor", 1);
		m_OnCreate = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdate = scriptClass->GetMethod("OnUpdate", 1);
       // m_OnPlaced = scriptClass->GetMethod("OnPlace", 0);
       // m_OnSpawn = scriptClass->GetMethod("OnSpawn", 0);
       // m_OnDestroy = scriptClass->GetMethod("OnDestroy", 0);
        // Call Entity constructor
        {
            uint64_t entityID = entity.GetEntityID();
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
    bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
    {
        const auto& fields = m_ScriptClass->GetFields();
        auto it = fields.find(name);
        if (it == fields.end())
            return false;

        const ScriptField& field = it->second;
        mono_field_get_value(m_Instance, field.ClassField, buffer);
        return true;
    }
    bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
    {
        const auto& fields = m_ScriptClass->GetFields();
        auto it = fields.find(name);
        if (it == fields.end())
            return false;

        const ScriptField& field = it->second;
        mono_field_set_value(m_Instance, field.ClassField, (void*)value);
        return true;
    }
    void ScriptEngine::Init() {
        s_Data = new MonoData();

        InitMono();
        ScriptFunc::RegisterFunctions();

        LoadAssembly("Resources/Scripts/ProofScriptCore.dll");
        LoadAppAssembly("GameProject/Asset/Scripts/Binaries/Game.dll");

        LoadAssemblyClasses();

        ScriptFunc::RegisterAllComponents();

        s_Data->EntityClass = Count<ScriptClass>::Create("Proof", "Entity", true);
    }
    void ScriptEngine::Shutdown() {
        mono_domain_set(mono_get_root_domain(), false);

        mono_domain_unload(s_Data->AppDomain);
        s_Data->AppDomain = nullptr;

        mono_jit_cleanup(s_Data->RootDomain);
        s_Data->RootDomain = nullptr;
    }

    void ScriptEngine::BeginRuntime(World* world) {
        s_Data->CurrentWorld = world;
    }
    void ScriptEngine::EndRuntime() {
        s_Data->CurrentWorld = nullptr;
        s_Data->EntityInstances.clear();
    }
    void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath) {
                // Create an App Domain
        std::string name = "ProofScriptRuntime";
        s_Data->AppDomain = mono_domain_create_appdomain(name.data(), nullptr);
        mono_domain_set(s_Data->AppDomain, true);

        // Move this maybe
        s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath,s_Data->EnableDebugging);
        s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
        // Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
    }
    void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath) {
        s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath, s_Data->EnableDebugging);
        s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
    }
    World* ScriptEngine::GetWorldContext() {
        return s_Data->CurrentWorld;
    }
    
    Count<ScriptClass> ScriptEngine::GetScriptClass(const std::string& name) {
        if (EntityClassExists(name) == false)return nullptr;
        return s_Data->ScriptEntityClasses[name];
    }

    void ScriptEngine::ReloadAssembly(World* world) {
        PF_CORE_ASSERT(world);
        mono_domain_set(mono_get_root_domain(), false);

        mono_domain_unload(s_Data->AppDomain);
        LoadAssembly("Resources/Scripts/ProofScriptCore.dll");
        LoadAppAssembly("GameProject/Asset/Scripts/Binaries/Game.dll");
        LoadAssemblyClasses();

        ScriptFunc::RegisterAllComponents();
        ScriptFunc::RegisterFunctions();
        // Retrieve and instantiate class
        s_Data->EntityClass = Count<ScriptClass>::Create("Proof", "Entity", true);
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
        if (s_Data->EnableDebugging)
        {
            const char* argv[2] = {
                "--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
                "--soft-breakpoints"
            };

            mono_jit_parse_options(2, (char**)argv);
            mono_debug_init(MONO_DEBUG_FORMAT_MONO);
        }
        MonoDomain* rootDomain = mono_jit_init("ProofJITRuntime");
        PF_CORE_ASSERT(rootDomain);
        // Store the root domain pointer
        s_Data->RootDomain = rootDomain;


        if (s_Data->EnableDebugging)
            mono_debug_domain_create(s_Data->RootDomain);

        mono_thread_set_main(mono_thread_current());

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
            const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

            //one reason it would be null is bcause it is an enum and the namespace kindd affects it
            std::string fullName;
            if (strlen(nameSpace) != 0)
                fullName = fmt::format("{}.{}", nameSpace, className);
            else
                fullName = className;

            MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);

            if (monoClass == entityClass)
                continue;
            MonoImage* monoImage = mono_class_get_image(monoClass);

            bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
            if (!isEntity) continue;

            s_Data->ScriptEntityClasses[fullName] = Count<ScriptClass>::Create(nameSpace, className);
            auto scriptClass = s_Data->ScriptEntityClasses[fullName];
            PF_ENGINE_TRACE("   Added To Script Class {}", fullName);

            int fieldCount = mono_class_num_fields(monoClass);
            PF_ENGINE_WARN("{} has {} fields:", className, fieldCount);
            void* iterator = nullptr;
            while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
            {
                const char* fieldName = mono_field_get_name(field);
                uint32_t flags = mono_field_get_flags(field);
                if (flags & MONO_FIELD_ATTR_PUBLIC)
                {
                    MonoType* type = mono_field_get_type(field);
                    ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);
                    PF_ENGINE_WARN("  {} ({})", fieldName, Utils::ScriptFieldTypeToString(fieldType));

                    scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
                }
            }
        }
    }
    MonoImage* ScriptEngine::GetCoreAssemblyImage() {
        return s_Data->CoreAssemblyImage;
    }

    const std::unordered_map<std::string, Count<ScriptClass>>const& ScriptEngine::GetScripts() {
        return s_Data->ScriptEntityClasses;
    }
    std::unordered_map<std::string, Count<ScriptInstance>> const ScriptEngine::GetScriptInstnace(Entity entity)
    {
        return s_Data->EntityInstances[entity.GetEntityID()];
    }
    bool ScriptEngine::EntityHasScripts(Entity entity)
    {
        return s_Data->EntityInstances.contains(entity.GetEntityID());
    }
    void ScriptEngine::OnCreateEntity(Entity entity)
    {
        const auto& sc = *entity.GetComponent<ScriptComponent>();

        for (auto& className : sc.ScriptsNames)
        {
            if (ScriptEngine::EntityClassExists(className))
            {
                UUID entityID = entity.GetEntityID();

                Count<ScriptInstance> instance = Count<ScriptInstance>::Create(s_Data->ScriptEntityClasses[className], entity);
                s_Data->EntityInstances[entity.GetEntityID()][className] = instance;


                if (s_Data->EntityScriptFields.contains(entityID))
                {
                    const auto& fieldMap = s_Data->EntityScriptFields.at(entityID);
                    for (const auto& [scriptName, scriptData] : fieldMap)
                    {
                        for(const auto& [fieldName, fieldData]: scriptData)
                            instance->SetFieldValueInternal(fieldName, fieldData.m_Buffer);
                    }
                }

                instance->CallOnCreate();
            }
        }
    }  
    void ScriptEngine::OnUpdateEntity(Entity entity, float ts)
    {
        UUID entityUUID = entity.GetEntityID();
        if (s_Data->EntityInstances.contains(entityUUID))
        {
            for (auto& [scriptName, instance]: s_Data->EntityInstances[entityUUID])
            {
                instance->CallOnUpdate(ts);
            }
        }
        else
        {
            PF_ENGINE_ERROR("Could not find ScriptInstance for entity {}  entityy Tag {}", entityUUID,entity.GetName());
            PF_CORE_ASSERT(false);
        }
    }
    std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>& ScriptEngine::GetScriptFieldMap(Entity entity)
    {
        UUID entityID = entity.GetEntityID();
        return s_Data->EntityScriptFields[entityID];
    }
    bool ScriptEngine::HasScriptFieldMap(Entity entity) {
        return s_Data->EntityScriptFields.contains(entity.GetEntityID());
    }

    void ScriptEngine::CreateScriptFieldMap(Entity entity) {
        s_Data->EntityScriptFields[entity.GetEntityID()] = {};
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
}
