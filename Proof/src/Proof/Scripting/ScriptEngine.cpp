// all Help to this wonderful guide
//https://peter1745.github.io/introduction.html
#include "Proofprch.h"
#include "ScriptEngine.h"


#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include "mono/metadata/attrdefs.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "ScriptFunc.h"
#include <fmt/format.h>
#include "Proof/Core/Buffer.h"
#include "Proof/Utils/FileSystem.h"
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
namespace Proof
{

    static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
    {
        { "System.Single", ScriptFieldType::Float },
        { "System.Double", ScriptFieldType::Double },
        { "System.Boolean", ScriptFieldType::Bool },
        { "System.Char", ScriptFieldType::Char },
        { "System.SByte", ScriptFieldType::Int8_t },
        { "System.Int16", ScriptFieldType::Int16_t },
        { "System.Int32", ScriptFieldType::Int32_t },
        { "System.Int64", ScriptFieldType::Int64_t },

        { "System.Byte", ScriptFieldType::Uint8_t },
        { "System.UInt16", ScriptFieldType::Uint16_t },
        { "System.UInt32", ScriptFieldType::Uint32_t },
        { "System.UInt64", ScriptFieldType::Uint64_t },

        { "Proof.Vector2", ScriptFieldType::Vector2 },
        { "Proof.Vector3", ScriptFieldType::Vector3 },
        { "Proof.Vector4", ScriptFieldType::Vector4 },

        { "Proof.Entity", ScriptFieldType::Entity },
        { "Proof.Prefab", ScriptFieldType::Prefab },
        { "Proof.Texture", ScriptFieldType::Texture },
    };
    namespace Utils
    {
       
   }
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
            Buffer fileData = FileSystem::ReadBytes(assemblyPath);

            // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.GetSize(), 1, &status, 0);

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
                    Buffer pdbFileData = FileSystem::ReadBytes(pdbPath);
                    mono_debug_open_image_from_memory(image, pdbFileData.As<const mono_byte>(), pdbFileData.GetSize());
                    PF_ENGINE_INFO("Loaded PDB {}", pdbPath.string());
                }
            }

            std::string pathString = assemblyPath.string();
            MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
            mono_image_close(image);
            fileData.Release();
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
        ScriptFieldType MonoTypeToScriptFieldTypeInt(MonoTypeEnum monoType) {
            switch (monoType)
            {
                case MONO_TYPE_VOID: return ScriptFieldType::None;
                case MONO_TYPE_BOOLEAN: return ScriptFieldType::Bool;
                case MONO_TYPE_CHAR: return ScriptFieldType::Char;
                case MONO_TYPE_I1: return ScriptFieldType::Int8_t;
                case MONO_TYPE_U1: return ScriptFieldType::Uint8_t;
                case MONO_TYPE_I2: return ScriptFieldType::Int16_t;
                case MONO_TYPE_U2: return ScriptFieldType::Uint16_t;
                case MONO_TYPE_I4: return ScriptFieldType::Int32_t;
                case MONO_TYPE_U4: return ScriptFieldType::Uint32_t;
                case MONO_TYPE_I8: return ScriptFieldType::Int64_t;
                case MONO_TYPE_U8: return ScriptFieldType::Uint64_t;
                case MONO_TYPE_R4: return ScriptFieldType::Float;
                case MONO_TYPE_R8: return ScriptFieldType::Double;
                case MONO_TYPE_ENUM: return ScriptFieldType::Enum;
               // case MONO_TYPE_STRING: return ScriptFieldType::String;
               // case MONO_TYPE_CLASS: return ScriptFieldType::Class; // Modify this to match your enum.
                // Add more cases for the other enum values as needed.
                default: return ScriptFieldType::None; break;
            }
            PF_CORE_ASSERT(false);
        };

        ScriptFieldType MonoTypeToScriptFieldTypeString(MonoType* monoType)
        {
            
            std::string typeName = mono_type_get_name(monoType);
            //if (typeName == "Demos.PhysicsCube.ErrorCode")
            //    PF_CORE_ASSERT(false);
            MonoClass* monoClass = mono_class_from_mono_type(monoType);
            if (monoClass != nullptr && mono_class_is_enum(monoClass))
            {
                return ScriptFieldType::Enum;
            }


            auto it = s_ScriptFieldTypeMap.find(typeName);
            if (it == s_ScriptFieldTypeMap.end())
            {
                PF_ENGINE_ERROR("Unknown type: {}", typeName);
                return ScriptFieldType::None;
            }

            return it->second;
        }

    }
    struct ScriptSubclassData {
        std::string ParentName;
        std::set<std::string> SubClasses;
    };

    
    struct MonoData {
        MonoDomain* RootDomain = nullptr;
        MonoDomain* AppDomain = nullptr;

        MonoAssembly* CoreAssembly = nullptr;
        MonoImage* CoreAssemblyImage = nullptr;

        MonoAssembly* AppAssembly = nullptr;
        MonoImage* AppAssemblyImage = nullptr;
        Count<ScriptClass> EntityClass;

        //(ENitty ID), (class name,{script name, script inctance)
        std::unordered_map<UUID, std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>> EntityScriptFields;
        std::unordered_map<std::string, Count<ScriptClass>> ScriptEntityClasses;

        //in a vector because we want to have teh order in wich the enum properties are in

        // enum name, enum type of int, enum values
        std::unordered_map<std::string, std::pair<ScriptFieldType,std::vector<ScriptEnumData>>> EnumClasses;
        // entity id, (name of class, instance of it)
        std::unordered_map<UUID, std::unordered_map<std::string,Count<ScriptInstance>>> EntityInstances;
        
        World* CurrentWorld = nullptr;
        //(scirpt name) (script direct Parent name, script subclasses)
        std::unordered_map<std::string, ScriptSubclassData> ScriptSubClasses;


        //prefab Id, (Prefab Entity ID), class Name, {field name, field
        std::unordered_map<AssetID, std::unordered_map<UUID,std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>>> PrefabScriptFields;
        void Clear () {
            EntityClass = nullptr;
            EntityScriptFields.clear();
            ScriptEntityClasses.clear();
            EntityInstances.clear();
            ScriptSubClasses.clear();
            EnumClasses.clear();
        }
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
    MonoMethod* GetMeathod(const std::string& methodName, int parameterCount, MonoClass* monoClass) {
        // Search for the method in the current class
        MonoMethod* method = mono_class_get_method_from_name(monoClass, methodName.c_str(), parameterCount);
        
        // If the method was not found in the current class, search in the parent class

        if (method != nullptr)
            return method;
        
        // if parent class has the meathod
        MonoClass* parentClass = mono_class_get_parent(monoClass);
        if (parentClass)
        {
            return GetMeathod(methodName, parameterCount, parentClass);
        }
        return nullptr;
    }
    MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount) {
        return GetMeathod(name.c_str(), parameterCount,m_MonoClass);
    }
    MonoObject* ScriptClass::CallMethod(MonoObject* instance, MonoMethod* method, void** params) {

       MonoObject* exception = nullptr;
       mono_runtime_invoke(method, instance, params, &exception);
       if (exception != nullptr)
       {
           MonoClass* excClass = mono_object_get_class(exception);
           MonoString* msg = mono_object_to_string(exception, nullptr);
           PF_EC_ERROR("C#: {}", mono_string_to_utf8(msg));
       }
       return exception;
    }
    ScriptInstance::ScriptInstance(Count<ScriptClass> scriptClass, Entity entity) :
        m_ScriptClass(scriptClass) {
        m_Instance = scriptClass->Instantiate();

       	m_Constructor = s_Data->EntityClass->GetMethod(".ctor", 1);
		m_OnCreate = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdate = scriptClass->GetMethod("OnUpdate", 1);

        m_OnTriggerEnter = scriptClass->GetMethod("OnTriggerEnter", 1);
        m_OnCollisionEnter = scriptClass->GetMethod("OnCollisionEnter", 1);
        m_OnCollisionLeave = scriptClass->GetMethod("OnCollisionLeave", 1);

        m_OnOverlapTriggerEnter = scriptClass->GetMethod("OnOverllapTriggerEnter", 1);
       // m_OnPlaced = scriptClass->GetMethod("OnPlace", 0);
       // m_OnSpawn = scriptClass->GetMethod("OnSpawn", 0);
        m_OnDestroy = scriptClass->GetMethod("OnDestroy", 0);
        // Call Entity constructor
        {
            uint64_t entityID = entity.GetUUID();
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
    void ScriptInstance::CallOnCollisionEnter(Entity otherEntity)
    {
        if (!m_OnCollisionEnter)return;
        uint64_t enittyID = otherEntity.GetUUID();
        void* param = &enittyID;

        {
            MonoObject* instance = m_ScriptClass->Instantiate();
            m_ScriptClass->CallMethod(instance, m_Constructor, &param);


            param = instance;
            m_ScriptClass->CallMethod(m_Instance, m_OnCollisionEnter, &param);
        }
    }
    void ScriptInstance::CallOnCollissionLeave(Entity otherEntity)
    {
        if (!m_OnCollisionLeave)return;
        uint64_t enittyID = otherEntity.GetUUID();
        void* param = &enittyID;

        {
            MonoObject* instance = m_ScriptClass->Instantiate();
            m_ScriptClass->CallMethod(instance, m_Constructor, &param);


            param = instance;
            m_ScriptClass->CallMethod(m_Instance, m_OnCollisionLeave, &param);
        }

    }
    void ScriptInstance::CallOnTriggerEnter(Entity otherEntity)
    {
        if (!m_OnTriggerEnter)return;
        UUID enittyID = otherEntity.GetUUID();
        void* param = &enittyID;

        {
            MonoObject* instance = m_ScriptClass->Instantiate();
            m_ScriptClass->CallMethod(instance, m_Constructor, &param);


            param = instance;
            m_ScriptClass->CallMethod(m_Instance, m_OnTriggerEnter, &param);
        }
    }
    void ScriptInstance::CallOnOverllapTriggerEnter(Entity otherEntity)
    {
        if (!m_OnOverlapTriggerEnter)return;
        UUID enittyID = otherEntity.GetUUID();
        void* param = &enittyID;

        {
            MonoObject* instance = m_ScriptClass->Instantiate();
            m_ScriptClass->CallMethod(instance, m_Constructor, &param);


            param = instance;
            m_ScriptClass->CallMethod(m_Instance, m_OnOverlapTriggerEnter, &param);
        }
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
        mono_field_set_value(m_Instance, field.ClassField,(void*) value);

        return true;
    }
    void ScriptEngine::Init() {

        Timer time;
        s_Data = new MonoData();

        InitMono();
        ScriptFunc::RegisterFunctions();
        
        auto assemlblyPath = Application::Get()->GetProject()->GetFromSystemProjectDirectory(Application::Get()->GetProject()->GetConfig().ScriptModuleDirectory).string() + "/ProofScriptCore.dll";
        auto appAssemblyPath = Application::Get()->GetProject()->GetFromSystemProjectDirectory(Application::Get()->GetProject()->GetConfig().ScriptModuleDirectory).string() + "/Game.dll";

      
        LoadAssembly(assemlblyPath);
        LoadAppAssembly(appAssemblyPath);

        LoadAssemblyClasses();

        ScriptFunc::RegisterAllComponents();

        s_Data->EntityClass = Count<ScriptClass>::Create("Proof", "Entity", true);

        PF_ENGINE_INFO("Script Engine Initialized {}m/s", time.ElapsedMillis());

    }
    void ScriptEngine::Shutdown() 
    {
        Timer time;

        mono_domain_set(mono_get_root_domain(), false);

        mono_domain_unload(s_Data->AppDomain);
        s_Data->AppDomain = nullptr;

        mono_jit_cleanup(s_Data->RootDomain);
        s_Data->RootDomain = nullptr;
        delete s_Data;

        PF_ENGINE_INFO("Script Engine Shutdown {}m/s", time.ElapsedMillis());
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
       // s_Data->Clear();
        mono_domain_set(mono_get_root_domain(), false);
        mono_domain_unload(s_Data->AppDomain);

        /*
        LoadAssembly("Resources/Scripts/ProofScriptCore.dll");
        LoadAppAssembly("GameProject/Asset/Scripts/Binaries/Game.dll");
        LoadAssemblyClasses();

        ScriptFunc::RegisterAllComponents();
        ScriptFunc::RegisterFunctions();
        // Retrieve and instantiate class
        s_Data->EntityClass = Count<ScriptClass>::Create("Proof", "Entity", true);
        */

        {
            ScriptFunc::RegisterFunctions();


            auto assemlblyPath = Application::Get()->GetProject()->GetFromSystemProjectDirectory(Application::Get()->GetProject()->GetConfig().ScriptModuleDirectory).string() + "/ProofScriptCore.dll";
            auto appAssemblyPath = Application::Get()->GetProject()->GetFromSystemProjectDirectory(Application::Get()->GetProject()->GetConfig().ScriptModuleDirectory).string() + "/Game.dll";


            LoadAssembly(assemlblyPath);
            LoadAppAssembly(appAssemblyPath);


            LoadAssemblyClasses();

            ScriptFunc::RegisterAllComponents();

            s_Data->EntityClass = Count<ScriptClass>::Create("Proof", "Entity", true);
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
        //mono_set_assemblies_path("mono/lib");
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

    static void ReflectEnums(const std::string& fullname, MonoClass* enumClass,MonoDomain* domain)
    {
        MonoClassField* classFields;
        void* fieldsIter = nullptr;
        int index = 0;

        //GONNA HAVE A LIST OF ALL ENUMS AND their possible values 
        // the enums will have to be labelled public though
        MonoObject* enumObject = mono_object_new(domain, enumClass);
        PF_ENGINE_INFO("Added To Enums {} ", fullname);
        while (classFields = mono_class_get_fields(enumClass, &fieldsIter))
        {
// the first field is some wierd "value_" probably a built in variable for the enums
            if (index == 0)
            {
               // PF_CORE_ASSERT(false);
                index++;
                continue;
            }
            const std::string enumName = mono_field_get_name(classFields);

            ScriptEnumData enumData;
            enumData.Name = enumName;
            enumData.EnumType = Utils::MonoTypeToScriptFieldTypeInt((MonoTypeEnum)mono_type_get_type(mono_class_enum_basetype(enumClass))); // using int cause i am 100% sure it is an enum

            //int value;
            mono_field_static_get_value(mono_class_vtable(domain, enumClass), classFields, enumData.Value);

            std::string data = mono_field_get_data(classFields);
            PF_ENGINE_TRACE("   {} ", enumName);

            s_Data->EnumClasses[fullname].first = enumData.EnumType;
            s_Data->EnumClasses[fullname].second.emplace_back(enumData);
        }
    }
    void ScriptEngine::LoadAssemblyClasses() {

        s_Data->ScriptEntityClasses.clear();
        s_Data->EnumClasses.clear();
        {
            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
            MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Proof", "Entity");

            PF_ENGINE_TRACE("C# Script Classes");
            for (int32_t i = 0; i < numTypes; i++)
            {
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

                if (monoClass == entityClass || monoClass == nullptr)
                    continue;
                MonoImage* monoImage = mono_class_get_image(monoClass);

                if (mono_class_is_enum(monoClass))
                {
                    ReflectEnums(fullName, monoClass, s_Data->AppDomain);
                }
                bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);

                if (!isEntity) continue;

                s_Data->ScriptEntityClasses[fullName] = Count<ScriptClass>::Create(nameSpace, className);
                s_Data->ScriptSubClasses[fullName];

                auto scriptClass = s_Data->ScriptEntityClasses[fullName];
                PF_ENGINE_TRACE("   Added To Script Class {}", fullName);

                int fieldCount = mono_class_num_fields(monoClass);
                PF_ENGINE_WARN("{} has {} fields:", className, fieldCount);

                MonoClass* parent = mono_class_get_parent(monoClass);

                if (parent != nullptr && parent != entityClass && mono_class_is_subclass_of(parent, entityClass, false))
                {
                    const char* nameSpace = mono_class_get_namespace(parent);
                    const char* name = mono_class_get_name(parent);

                    // Combine the namespace and name to form the full class name
                    std::string parentfullName = nameSpace;
                    parentfullName += ".";
                    parentfullName += name;

                    s_Data->ScriptSubClasses[fullName].ParentName = parentfullName;
                    s_Data->ScriptSubClasses[parentfullName].SubClasses.insert(fullName);
                }

                AssemblyLoadFields(scriptClass, monoClass);
            }

        }
        return;
        // just laoding enums
        {
            const MonoTableInfo* coreTypeDefinitionsTable = mono_image_get_table_info(s_Data->CoreAssemblyImage, MONO_TABLE_TYPEDEF);
            int32_t coreNumTypes = mono_table_info_get_rows(coreTypeDefinitionsTable);

            PF_ENGINE_TRACE("C# Core Script Enums");
            for (int32_t i = 0; i < coreNumTypes; i++)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(coreTypeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
                const char* nameSpace = mono_metadata_string_heap(s_Data->CoreAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* className = mono_metadata_string_heap(s_Data->CoreAssemblyImage, cols[MONO_TYPEDEF_NAME]);
                 //one reason it would be null is bcause it is an enum and the namespace kindd affects it
                std::string fullName;
                if (strlen(nameSpace) != 0)
                    fullName = fmt::format("{}.{}", nameSpace, className);
                else
                    fullName = className;

                MonoClass* monoClass = mono_class_from_name(s_Data->CoreAssemblyImage, nameSpace, className);

                if (monoClass == nullptr)
                    continue;
                MonoImage* monoImage = mono_class_get_image(monoClass);

                if (mono_class_enum_basetype(monoClass))
                {
                    ReflectEnums(fullName, monoClass, s_Data->RootDomain);
                    continue;
                }
            }
        }
    }
    void ScriptEngine::AssemblyLoadFields(Count<ScriptClass> scriptClass, MonoClass* monoClass)
    {
        MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Proof", "Entity");
        void* iterator = nullptr;
        while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
        {
            const char* fieldName = mono_field_get_name(field);
            uint32_t flags = mono_field_get_flags(field);
            if (flags & MONO_FIELD_ATTR_PUBLIC )
            {
                MonoType* type = mono_field_get_type(field);
                ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldTypeString(type); //using string because i am not sure if it is an enum or an array or so n
                if (fieldType == ScriptFieldType::None)
                    continue;
                PF_ENGINE_WARN("  {} ({})", fieldName, Utils::ScriptFieldTypeToString(fieldType));

                scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
            }
        }
        MonoClass* parentClass = mono_class_get_parent(monoClass);
        if (parentClass != nullptr && parentClass != entityClass)
        {
            if (mono_class_is_subclass_of(parentClass, entityClass, false))
                AssemblyLoadFields(scriptClass, parentClass);
        }
    }
    MonoImage* ScriptEngine::GetCoreAssemblyImage() {
        return s_Data->CoreAssemblyImage;
    }

    const std::unordered_map<std::string, Count<ScriptClass>>const& ScriptEngine::GetScripts() {
        return s_Data->ScriptEntityClasses;
    }
    std::unordered_map<std::string, Count<ScriptInstance>> const& ScriptEngine::GetScriptInstnace(Entity entity)
    {
        return s_Data->EntityInstances[entity.GetUUID()];
    }
    bool ScriptEngine::EntityHasScripts(Entity entity)
    {
        return s_Data->EntityInstances.contains(entity.GetUUID());
    }

    const std::unordered_map<UUID, std::unordered_map<std::string, Count<ScriptInstance>>>& ScriptEngine::EachEntityScript()
    {
        return s_Data->EntityInstances;
    }
    
    std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>& ScriptEngine::GetScriptFieldMap(Entity entity)
    {
        UUID entityID = entity.GetUUID();
        return s_Data->EntityScriptFields[entityID];
    }
    std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>& ScriptEngine::GetScriptFieldMap(UUID id)
    {
        return s_Data->EntityScriptFields[id];
    }
    std::unordered_map<AssetID, std::unordered_map<UUID, std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>>>& ScriptEngine::GetPrefabFieldMap(AssetID ID)
    {
        return s_Data->PrefabScriptFields;
    }
    const std::unordered_map<std::string, std::pair<ScriptFieldType, std::vector<ScriptEnumData>>>& ScriptEngine::GetEnumClasses()
    {
        return s_Data->EnumClasses;
    }
    bool ScriptEngine::HasScriptFieldMap(Entity entity) {
        return s_Data->EntityScriptFields.contains(entity.GetUUID());
    }
    bool ScriptEngine::HasScriptFieldMap(UUID Id)
    {
        return s_Data->EntityScriptFields.contains(Id);
    }
    void ScriptEngine::CreateScriptFieldMap(Entity entity) {
        s_Data->EntityScriptFields[entity.GetUUID()] = {};
    }

    std::string ScriptEngine::GetFieldEnumName(ScriptField field)
    {

        MonoType* fieldType = mono_field_get_type(field.ClassField);

        MonoClass* fieldClass = mono_type_get_class(fieldType);
        bool isEnum = (fieldClass);
        if (isEnum)
        {

            std::string name = mono_type_get_name(fieldType);
            return name;
        }
        return "";
        PF_CORE_ASSERT(false);
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
    MonoObject* ScriptEngine::GetMonoManagedObject(UUID uuid, const std::string& fullName)
    {
        PF_CORE_ASSERT(s_Data->EntityInstances.contains(uuid));
        if (!s_Data->EntityInstances.contains(uuid))return nullptr;

        auto& entData = s_Data->EntityInstances.at(uuid);

        if (entData.contains(fullName))
        {
            return entData.at(fullName)->GetMonoObject();
        } 

        if(!s_Data->ScriptSubClasses.contains(fullName))
            return nullptr;

        for (auto& [scriptName,instnace] :entData)
        {
            // need to do this recursivelya
            if (s_Data->ScriptSubClasses[fullName].SubClasses.contains(scriptName))
            {
                return instnace->GetMonoObject();
            }
            // not sure
           return GetMonoManagedObject(uuid, scriptName);
        }
        /*
        // checking if it is a subclass
        for (const auto& subclassName : s_Data->ScriptSubClasses[fullName].SubClasses)
        {
            
            if(entData.contains(subclassName))
                 return entData.at(subclassName)->GetMonoObject();
            if(s_Data->ScriptSubClasses[subclassName].ParentName)
            GetMonoManagedObject(uuid, subclassName);
            //std::string parentName = s_Data->ScriptSubClasses.at(subclassName).ParentName;

            //if(entData.contains(parentName))
        }
        */
        return nullptr;
    }
    MonoDomain* ScriptEngine::GetDomain()
    {
        return s_Data->AppDomain;
    }
    void ScriptMeathod::OnCreate(Entity entity)
    {
        const auto& sc = entity.GetComponent<ScriptComponent>();

        for (auto& className : sc.ScriptsNames)
        {
            if (ScriptEngine::EntityClassExists(className))
            {
                UUID entityID = entity.GetUUID();

                Count<ScriptInstance> instance = Count<ScriptInstance>::Create(s_Data->ScriptEntityClasses[className], entity);
                if (instance == nullptr)
                    continue;
                s_Data->EntityInstances[entity.GetUUID()][className] = instance;


                if (s_Data->EntityScriptFields.contains(entityID))
                {
                    const auto& fieldMap = s_Data->EntityScriptFields.at(entityID);
                    for (const auto& [scriptName, scriptData] : fieldMap)
                    {
                        for (const auto& [fieldName, fieldData] : scriptData)
                        {
                            if (fieldData.Field.Type == ScriptFieldType::Prefab)
                            {
                                uint64_t id = *(uint64_t*)fieldData.m_Buffer;
                                if (id == 0)
                                    continue;
                                void* param = &id;
                                Count<ScriptClass> prefab = Count<ScriptClass>::Create("Proof", "Prefab", true);
                                MonoObject* prefabinstnace = prefab->Instantiate();
                                MonoMethod* constructo = prefab->GetMethod(".ctor", 1);
                                prefab->CallMethod(prefabinstnace, constructo, &param);

                               // MonoClassField* field = mono_class_get_field_from_name(prefab->GetMonoClass(), fieldData.Field.Name.c_str());
                                // this is how we do it 
                                mono_field_set_value(instance->GetMonoObject(), fieldData.Field.ClassField, prefabinstnace);
                                continue;
                            }

                            if (fieldData.Field.Type == ScriptFieldType::Entity)
                            {
                                uint64_t id = *(uint64_t*)fieldData.m_Buffer;
                                if (id == 0)
                                    continue;

                                void* param = &id;
                                MonoObject* entityInstnace = s_Data->EntityClass->Instantiate();
                                MonoMethod* constructo = s_Data->EntityClass->GetMethod(".ctor", 1);
                                s_Data->EntityClass->CallMethod(entityInstnace, constructo, &param);

                                MonoClassField* field = mono_class_get_field_from_name(s_Data->EntityClass->GetMonoClass(), fieldData.Field.Name.c_str());
                                // this is how we do it 
                                mono_field_set_value(instance->GetMonoObject(), fieldData.Field.ClassField, entityInstnace);
                                continue;
                            }
                            instance->SetFieldValueInternal(fieldName, fieldData.m_Buffer);
                        }
                    }
                }

                instance->CallOnCreate();
            }
        }
    }
    void ScriptMeathod::OnDestroy(Entity entity)
    {
        const auto& sc = entity.GetComponent<ScriptComponent>();

        UUID entityUUID = entity.GetUUID();

        if (s_Data->EntityInstances.contains(entityUUID))
        {
            for (auto& [scriptName, instance] : s_Data->EntityInstances[entityUUID])
            {
                instance->CallOnDestroy();
            }
            s_Data->EntityInstances.erase(entityUUID);
        }

    }
    void ScriptMeathod::OnUpdate(Entity entity, FrameTime time)
    {
        PF_PROFILE_FUNC();
        UUID entityUUID = entity.GetUUID();
        if (s_Data->EntityInstances.contains(entityUUID))
        {
            ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

            for (auto& scriptName : scriptComponent.ScriptsNames)
            {
                auto it = s_Data->EntityInstances[entityUUID].find(scriptName);

                // means that in script component
                //this script is found on script meathod 


                if (it != s_Data->EntityInstances[entityUUID].end())
                {
                    it->second->CallOnUpdate(time);
                }
                else 
                {
                    // TODO CALL ON CREATE ON THE NEW SCRIPT ADDED TO SCRIPT COMPONENT
                }
            }

            for (auto& [scriptName, instance] : s_Data->EntityInstances[entityUUID])
            {
                if (!scriptComponent.ScriptsNames.contains(scriptName))
                {
                    // CALL ON DELETE ON THESE Scripts
                }
            }
        }
        else
        {
            //PF_ENGINE_ERROR("Could not find ScriptInstance for entity {}  entityy Tag {}", entityUUID, entity.GetName());
        }
    }

    void ScriptMeathod::CallMeathod(Count<ScriptInstance> instnace, MonoMethod* meathod, void** params)
    {
        MonoObject* exception = nullptr;
        mono_runtime_invoke(meathod, instnace->m_Instance, params, &exception);
    }
    
    void ScriptMeathod::OnCollisionEnter(Entity currentEntity, Entity collidingEntity)
    {
        UUID entityUUID = currentEntity.GetUUID();
        if (s_Data->EntityInstances.contains(entityUUID))
        {
            for (auto& [scriptName, instance] : s_Data->EntityInstances[entityUUID])
            {
                instance->CallOnCollisionEnter(collidingEntity);
            }
        }
        else
        {
            PF_ENGINE_ERROR("Could not find ScriptInstance for entity {}  entityy Tag {}", entityUUID, currentEntity.GetName());
        }
    }
    void ScriptMeathod::OnCollisionLeave(Entity currentEntity, Entity leavingEntity)
    {

        UUID entityUUID = currentEntity.GetUUID();
        if (s_Data->EntityInstances.contains(entityUUID))
        {
            for (auto& [scriptName, instance] : s_Data->EntityInstances[entityUUID])
            {
                instance->CallOnCollissionLeave(leavingEntity);
            }
        }
        else
        {
            PF_ENGINE_ERROR("Could not find ScriptInstance for entity {}  entityy Tag {}", entityUUID, currentEntity.GetName());
        }
    }
    void ScriptMeathod::OnTriggerEnter(Entity currentEntity, Entity triggerEntity)
    {
        UUID entityUUID = currentEntity.GetUUID();
        if (s_Data->EntityInstances.contains(entityUUID))
        {
            for (auto& [scriptName, instance] : s_Data->EntityInstances[entityUUID])
            {
                instance->CallOnTriggerEnter(triggerEntity);
            }
        }
        else
        {
            PF_ENGINE_ERROR("Could not find ScriptInstance for entity {}  entityy Tag {}", entityUUID, currentEntity.GetName());
        }
    }
    void ScriptMeathod::OnOverlapTriggerEnter(Entity currentEntity, Entity entityWithTriggerBox)
    {

        UUID entityUUID = currentEntity.GetUUID();
        if (s_Data->EntityInstances.contains(entityUUID))
        {
            for (auto& [scriptName, instance] : s_Data->EntityInstances[entityUUID])
            {
                instance->CallOnOverllapTriggerEnter(entityWithTriggerBox);
            }
        }
        else
        {
            PF_ENGINE_ERROR("Could not find ScriptInstance for entity {}  entityy Tag {}", entityUUID, currentEntity.GetName());
        }
    }

}
