// all Help to this wonderful guide
//https://peter1745.github.io/introduction.html
#include "Proofprch.h"
#include "ScriptEngine.h"
#include "Proof/Core/Buffer.h"
#include "Proof/Utils/FileSystem.h"
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
#include "ScriptTypes.h"
#include "ScriptGCManager.h"
#include "ScriptFunc.h"
#include "ScriptField.h"
#include "ScriptRegistry.h"
#include "ScriptFile.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <fmt/format.h>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include "mono/metadata/attrdefs.h"

#include "ScriptUtils.h"

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
    }

    // Scirpt Engien during runtiem will not create any new field map for 
    // entities taht where not aroudn during eidtor time
    //but if an entity ask fo r a field map and it only exist during runtime
    // we will generate a storage for its field
   // using FieldMap = std::unordered_map < UUID, ScriptClassesContainerMetaData>;
    struct ScriptEngineData
    {
        MonoDomain* CoreDomain = nullptr;
        MonoDomain* AppDomain = nullptr;

        Count<AssemblyInfo> CoreAssemblyInfo = nullptr;
        Count<AssemblyInfo> AppAssemblyInfo = nullptr;

        bool IsMonoInitialized = false;
        // entity id, vecotr of all the scripts and its fields
       // FieldMap EntityFieldMap;
       // std::unordered_map<UUID, std::unordered_map<uint32_t, Coun<FieldStorageBase>>> EntityFieldMap;
        //std::unordered_map<AssetID, std::unordered_map<uint32_t, Coun<FieldStorageBase>>> PrefabFieldMap;

    };

    struct ScriptEngineRuntimeData
    {
        Count<World> WorldContext =nullptr;
        std::unordered_map<UUID, std::vector<ScriptGCHandle>> InstanceScripts;
    };

    ScriptEngineData* s_ScriptEngineData = nullptr;
    ScriptEngineRuntimeData* s_ScriptEngineRuntimeData = nullptr;

    void ScriptEngine::Init() {

        PF_CORE_ASSERT(!s_ScriptEngineData, "[ScriptEngine]: Trying to call ScriptEngine::Init multiple times!");

        ScopeTimer scopeTime("ScriptEngine::Init");

        s_ScriptEngineData = new ScriptEngineData();

        s_ScriptEngineData->CoreAssemblyInfo = Count<AssemblyInfo>::Create();
        s_ScriptEngineData->AppAssemblyInfo = Count<AssemblyInfo>::Create();

        InitMono();

        ScriptGCManager::Init();

        LoadCoreAssembly();
        LoadAppAssembly();
        ScriptFunc::RegisterFunctions();
        ScriptFunc::RegisterAllComponents();
    }
    void ScriptEngine::ShutDown()
    {
        ScopeTimer scopeTime("ScriptEngine::ShutDown");
        ScriptGCManager::Shutdown();
        ScriptRegistry::ShutDown();

        ScriptEngine::ShutDownMono();
    }

    void ScriptEngine::InitMono()
    {
        if (s_ScriptEngineData->IsMonoInitialized)
            return;

        //mono_set_dirs("mono/lib", "mono/etc");

        s_ScriptEngineData->CoreDomain = mono_jit_init("ProofJITRuntime");
        PF_CORE_ASSERT(s_ScriptEngineData->CoreDomain);

        mono_thread_set_main(mono_thread_current());
        s_ScriptEngineData->IsMonoInitialized = true;

        PF_ENGINE_INFO("ScriptEngine InitMono");
    }
    void ScriptEngine::ShutDownMono()
    {
        if (!s_ScriptEngineData->IsMonoInitialized)
        {
            PF_ENGINE_WARN("ScriptEngine Trying to shutdown Mono multiple times!");
            return;
        }

        s_ScriptEngineData->AppDomain = nullptr;
        mono_jit_cleanup(s_ScriptEngineData->AppDomain);
        s_ScriptEngineData->AppDomain = nullptr;

        s_ScriptEngineData->IsMonoInitialized = false;
    }
   
    bool ScriptEngine::LoadCoreAssembly()
    {
        auto corePath = Application::Get()->GetProject()->GetFromSystemProjectDirectory(Application::Get()->GetProject()->GetConfig().ScriptModuleDirectory).string() + "/ProofScriptCore.dll";

        PF_ENGINE_INFO("ScriptEngine Trying to load core assembly: {}", corePath);
        if (!FileSystem::Exists(corePath))
        {
            PF_ENGINE_ERROR("ScriptEngine Could not load core assembly! Path: {}", corePath);
            return false;
        }

        s_ScriptEngineData->CoreAssemblyInfo->ReferencedAssemblies.clear();

        std::string domainName = "ProofScriptRuntime";
        s_ScriptEngineData->CoreDomain = mono_domain_create_appdomain(domainName.data(), nullptr);
       // mono_domain_set(s_ScriptEngineData->CoreDomain, true);
        //mono_domain_set_config(s_ScriptEngineData->CoreDomain, ".", "");

        {
            std::string appDomainname = "ProofScriptRuntime";

            s_ScriptEngineData->AppDomain = mono_domain_create_appdomain(appDomainname.data(), nullptr);
            mono_domain_set(s_ScriptEngineData->AppDomain, true);
            //mono_domain_set_config(s_State->ScriptsDomain, ".", "");
        }
        s_ScriptEngineData->CoreAssemblyInfo->FilePath = corePath;
        s_ScriptEngineData->CoreAssemblyInfo->Assembly = LoadMonoAssembly(s_ScriptEngineData->CoreAssemblyInfo->FilePath);
        s_ScriptEngineData->CoreAssemblyInfo->Classes.clear();

        s_ScriptEngineData->CoreAssemblyInfo->AssemblyImage = mono_assembly_get_image(s_ScriptEngineData->CoreAssemblyInfo->Assembly);
        s_ScriptEngineData->CoreAssemblyInfo->IsCoreAssembly = true;
        s_ScriptEngineData->CoreAssemblyInfo->Metadata = GetMetadataForImage(s_ScriptEngineData->CoreAssemblyInfo->AssemblyImage);
        s_ScriptEngineData->CoreAssemblyInfo->ReferencedAssemblies = GetReferencedAssembliesMetadata(s_ScriptEngineData->CoreAssemblyInfo->AssemblyImage);

        ScriptRegistry::Init();
    }

    bool ScriptEngine::LoadAppAssembly()
    {
        auto appPath = Application::Get()->GetProject()->GetFromSystemProjectDirectory(Application::Get()->GetProject()->GetConfig().ScriptModuleDirectory).string() + "/Game.dll";

        if (!FileSystem::Exists(appPath))
        {
            PF_ENGINE_ERROR("ScriptEngine, Failed to load app assembly! Invalid filepath");
            PF_ENGINE_ERROR("ScriptEngine, Filepath = {}", appPath);
            return false;
        }

        auto appAssemblyInfo = s_ScriptEngineData->AppAssemblyInfo;

        appAssemblyInfo->FilePath = appPath;
        appAssemblyInfo->Assembly = LoadMonoAssembly(appPath);
        appAssemblyInfo->Classes.clear();

        appAssemblyInfo->AssemblyImage = mono_assembly_get_image(appAssemblyInfo->Assembly);
        appAssemblyInfo->IsCoreAssembly = false;
        appAssemblyInfo->Metadata = GetMetadataForImage(appAssemblyInfo->AssemblyImage);
        appAssemblyInfo->ReferencedAssemblies = GetReferencedAssembliesMetadata(appAssemblyInfo->AssemblyImage);

        ScriptRegistry::GenerateRegistryForAppAssembly(appAssemblyInfo);

    }

    AssemblyMetadata ScriptEngine::GetMetadataForImage(MonoImage* image)
    {
        AssemblyMetadata metadata;

        const MonoTableInfo* t = mono_image_get_table_info(image, MONO_TABLE_ASSEMBLY);
        uint32_t cols[MONO_ASSEMBLY_SIZE];
        mono_metadata_decode_row(t, 0, cols, MONO_ASSEMBLY_SIZE);

        metadata.Name = mono_metadata_string_heap(image, cols[MONO_ASSEMBLY_NAME]);
        metadata.MajorVersion = cols[MONO_ASSEMBLY_MAJOR_VERSION] > 0 ? cols[MONO_ASSEMBLY_MAJOR_VERSION] : 1;
        metadata.MinorVersion = cols[MONO_ASSEMBLY_MINOR_VERSION];
        metadata.BuildVersion = cols[MONO_ASSEMBLY_BUILD_NUMBER];
        metadata.RevisionVersion = cols[MONO_ASSEMBLY_REV_NUMBER];

        return metadata;
    }

    std::vector<AssemblyMetadata> ScriptEngine::GetReferencedAssembliesMetadata(MonoImage* image)
    {
        const MonoTableInfo* t = mono_image_get_table_info(image, MONO_TABLE_ASSEMBLYREF);
        int rows = mono_table_info_get_rows(t);

        std::vector<AssemblyMetadata> metadata;
        for (int i = 0; i < rows; i++)
        {
            uint32_t cols[MONO_ASSEMBLYREF_SIZE];
            mono_metadata_decode_row(t, i, cols, MONO_ASSEMBLYREF_SIZE);

            auto& assemblyMetadata = metadata.emplace_back();
            assemblyMetadata.Name = mono_metadata_string_heap(image, cols[MONO_ASSEMBLYREF_NAME]);
            assemblyMetadata.MajorVersion = cols[MONO_ASSEMBLYREF_MAJOR_VERSION];
            assemblyMetadata.MinorVersion = cols[MONO_ASSEMBLYREF_MINOR_VERSION];
            assemblyMetadata.BuildVersion = cols[MONO_ASSEMBLYREF_BUILD_NUMBER];
            assemblyMetadata.RevisionVersion = cols[MONO_ASSEMBLYREF_REV_NUMBER];
        }

        return metadata;
    }

    MonoAssembly* ScriptEngine::LoadMonoAssembly(const std::filesystem::path& assemblyPath)
    {
        if (!FileSystem::Exists(assemblyPath))
            return nullptr;

        Buffer fileData = FileSystem::ReadBytes(assemblyPath);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            PF_ENGINE_ERROR("ScriptEngine Failed to open C# assembly '{0}'\n\t\tMessage: {1}", assemblyPath.string(), errorMessage);
            return nullptr;
        }
        #if 0
        // Load C# debug symbols if debugging is enabled
        if (s_State->Config.EnableDebugging)
        {
            // First check if we have a .dll.pdb file
            bool loadDebugSymbols = true;
            std::filesystem::path pdbPath = assemblyPath.string() + ".pdb";
            if (!FileSystem::Exists(pdbPath))
            {
                // Otherwise try just .pdb
                pdbPath = assemblyPath;
                pdbPath.replace_extension("pdb");

                if (!FileSystem::Exists(pdbPath))
                {
                    ANT_CORE_WARN_TAG("ScriptEngine", "Couldn't find .pdb file for assembly {0}, no debug symbols will be loaded.", assemblyPath.string());
                    loadDebugSymbols = false;
                }
            }

            if (loadDebugSymbols)
            {
                ANT_CORE_INFO_TAG("ScriptEngine", "Loading debug symbols from '{0}'", pdbPath.string());
                Buffer buffer = FileSystem::ReadBytes(pdbPath);
                mono_debug_open_image_from_memory(image, buffer.As<mono_byte>(), buffer.Size);
                buffer.Release();
            }
        }
        #endif
        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
        mono_image_close(image);
        return assembly;
    }
    struct EntityClassReloadMetadata
    {
        std::string ClassName;
        std::unordered_map<std::string, Buffer> Fields;
    };
    void ScriptEngine::ReloadppAssembly()
    {
        #if 0
        PF_PROFILE_FUNC();

        ScopeTimer scopeTimer(__FUNCTION__);
        PF_ENGINE_INFO("ScriptEngine Reloading App: {}", s_ScriptEngineData->CoreAssemblyInfo->FilePath.string(), s_ScriptEngineData->AppAssemblyInfo->FilePath.string());

        std::unordered_map<UUID, std::vector<EntityClassReloadMetadata>> oldFieldValues;

        for (auto& [entityId, enittyclassMetadata] : s_ScriptEngineData->EntityFieldMap)
        {
            auto& fieldsclassReloadMetadata = oldFieldValues[entityId];
            
            for (auto& savedMetaData : enittyclassMetadata)
            {
                auto& fieldReloadMetaData = fieldsclassReloadMetadata.emplace_back();
                fieldReloadMetaData.ClassName = savedMetaData.className;
                for (auto& [field, fieldStorage] : savedMetaData.Fields)
                {
                    if (!fieldStorage)
                        continue;

                    if (!fieldStorage->GetFieldInfo()->IsWritable())continue;

                    fieldReloadMetaData.Fields[field] = Buffer::Copy(fieldStorage->GetValueBuffer());
                }
            }
           // DestroyScriptEntity(y, false);

        }
        #endif
    }

    MonoObject* ScriptEngine::CreateManagedObject(ManagedClass* managedClass, bool appDomain)
    {
        MonoDomain* domain;
        if (appDomain)
            domain = s_ScriptEngineData->AppDomain;
        else
            domain = s_ScriptEngineData->CoreDomain;
        MonoObject* monoObject = mono_object_new(domain, managedClass->Class);
        PF_CORE_ASSERT(monoObject, "Failed to create MonoObject!");
        return monoObject;
    }
    void ScriptEngine::InitRuntimeObject(MonoObject* monoObject)
    {
        /// calles default runtime constroctor
        mono_runtime_object_init(monoObject);
    }

    MonoDomain* ScriptEngine::GetAppDomain()
    {
        return s_ScriptEngineData->AppDomain ;
    }
    MonoDomain* ScriptEngine::GetCoreDomain()
    {
        return s_ScriptEngineData->CoreDomain;
    }
    MonoObject* ScriptEngine::CreateManagedObject(const std::string& className, bool appDomain)
    {
        ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(className);
        
        PF_CORE_ASSERT(managedClass);

        return CreateManagedObject(managedClass, appDomain);
    }
    #if 0
    void ScriptEngine::DestroyScriptEntity(Entity entity)
    {
        DestroyScriptEntity(entity, true);
    }

    void ScriptEngine::DestroyScriptEntity(Entity entity, bool erase)
    {
        PF_PROFILE_FUNC();

        if (!entity.HasComponent<ScriptComponent>())
            return;
        auto& sc = entity.GetComponent<ScriptComponent>();

        if (s_ScriptEngineData->EntityFieldMap.contains(entity.GetUUID()) && erase)
        {
            s_ScriptEngineData->EntityFieldMap.erase(entity.GetUUID());
            sc.ScriptMetadates.clear();
        }
    }
    #endif

    void ScriptEngine::RuntimeInstantiateScriptEntity(Entity entity)
    {
        PF_PROFILE_FUNC();

        auto scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!entity.HasComponent<ScriptComponent>())
            return;
        ScriptComponent& script = entity.GetComponent<ScriptComponent>();


        bool hasScripts = false;
        for (int i = 0; i < script.ScriptMetadates.size(); i++)
        {
            if (AssetManager::HasAsset(script.ScriptMetadates[i].ScriptClassID))
            {
                hasScripts = true;
                //s_ScriptEngineData->EntityFieldMap[entity.GetUUID()] = {};
                break;
            }
        }

        if (!hasScripts)
        {
            PF_ENGINE_WARN("Entity {} contains no valid ScriptModule ", entity.GetName());
            script.ScriptMetadates.clear();
            return;
        }
        for (int i = 0; i < script.ScriptMetadates.size(); i++)
        {
            if (AssetManager::HasAsset(script.ScriptMetadates[i].ScriptClassID))
            {
                RuntimeScriptEntityPushScript(entity, AssetManager::GetAsset<ScriptFile>(script.ScriptMetadates[i].ScriptClassID));
            }
            else
            {
                script.ScriptMetadates.erase(script.ScriptMetadates.begin() + i);
            }
        }
    }
    

    void ScriptEngine::RuntimeScriptEntityPushScript(Entity entity, Count<class ScriptFile> scriptFile)
    {
        #if 0
        PF_CORE_ASSERT(s_ScriptEngineData->EntityFieldMap.contains(entity.GetUUID()), fmt::format("Entity {} is not contained in Script Engine ", entity.GetName()));

        ScriptComponent& scriptComponent = entity.GetComponent<ScriptComponent>();

        if (!IsModuleValid(scriptFile))
        {
            PF_ENGINE_ERROR("Trying to add invalid script to Entity {}", entity.GetName());
            return;
        }


        if (!scriptComponent.HasScript(scriptFile->GetID()))
        {
            PF_ENGINE_ERROR("Trying to Script:{} to Entity:{} that already contains script", scriptFile->GetFullName(), entity.GetName());
            return;
        }
        ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(scriptFile->GetFullName());
        if (!managedClass)
            return;

        MonoObject* runtimeInstance = CreateManagedObject(managedClass, entity.GetUUID());
        ScriptGCHandle instanceHandle = ScriptGCManager::CreateObjectReference(runtimeInstance, false);

        for (auto fieldName : managedClass->Fields)
        {
            if (s_ScriptEngineData->EntityFieldMap.contains(entity.GetUUID()))
            {
                auto& fields = s_ScriptEngineData->EntityFieldMap[entity.GetUUID()].Classes[fieldName].Fields;
                for (auto& [name, field] : fields)
                {
                    if (field)
                        field->SetRuntimeInstance(instanceHandle);
                }
            }
        }
        CallMethod(instanceHandle, "OnCreate");

        if (!scriptComponent.HasScript(scriptFile->GetID()))
            scriptComponent.ScriptMetadates.emplace_back(ScriptComponentsClassesData{ scriptFile->GetID(),instanceHandle });
        else
            scriptComponent.ScriptMetadates.back() = ScriptComponentsClassesData{ scriptFile->GetID(),instanceHandle };

        #endif
    }

    void ScriptEngine::BeginRuntime(Count<World> world)
    {
        PF_PROFILE_FUNC();

        s_ScriptEngineRuntimeData = new ScriptEngineRuntimeData();
        s_ScriptEngineRuntimeData->WorldContext = world;
        PF_CORE_ASSERT(s_ScriptEngineRuntimeData->WorldContext->IsPlaying(), "Don't call ScriptEngine::BeginRuntime if the scene isn't being played!");
        auto view = s_ScriptEngineRuntimeData->WorldContext->GetAllEntitiesWith<ScriptComponent>();
        for (auto enttID : view)
        {
            Entity entity{ enttID,s_ScriptEngineRuntimeData->WorldContext.Get() };
            RuntimeInstantiateScriptEntity(entity);
        }
        // copying all the values 
        // it uses shared pointer so the 

        
    }
    void ScriptEngine::EndRuntime()
    {
        
        delete s_ScriptEngineRuntimeData;
    }
    bool ScriptEngine::IsRuntime()
    {
        return s_ScriptEngineRuntimeData != nullptr;
    }
    Count<World> ScriptEngine::GetWorldContext()
    {
        if(IsRuntime())
            return s_ScriptEngineRuntimeData->WorldContext;

        return nullptr;
    }

    void ScriptEngine::CallMethod(MonoObject* monoObject, ManagedMethod* managedMethod, const void** parameters)
    {
        PF_PROFILE_FUNC();

        MonoObject* exception = NULL;
        mono_runtime_invoke(managedMethod->Method, monoObject, const_cast<void**>(parameters), &exception);
        
        ScriptUtils::HandleException(exception);
    }

    bool ScriptEngine::IsModuleValid(AssetID id)
    {
        return IsModuleValid(AssetManager::GetAsset<ScriptFile>(id));
    }
    bool ScriptEngine::IsModuleValid(Count<ScriptFile>  file)
    {
        if (!AssetManager::HasAsset(file))
            return false;

        return GetEntityScripts().contains(file->GetFullName());
    }
    
    ManagedClass* ScriptEngine::GetManagedClass(const std::string& className)
    {
        return ScriptRegistry::GetManagedClassByName(className);
    }


    Count<AssemblyInfo> ScriptEngine::GetCoreAssemblyInfo()
    {
        return s_ScriptEngineData->CoreAssemblyInfo;
    }
    Count<AssemblyInfo> ScriptEngine::GetAppAssemblyInfo()
    {
        return s_ScriptEngineData->AppAssemblyInfo;

    }
    const std::unordered_map<std::string, Count<ScriptClass>>& ScriptEngine::GetEntityScripts()
    {
        return ScriptRegistry::GetEntityScripts();
    }
}
