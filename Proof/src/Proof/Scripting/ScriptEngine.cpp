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
#include "ScriptRegistry.h"

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

    struct ScriptEngineData
    {
        MonoDomain* CoreDomain = nullptr;
        MonoDomain* AppDomain = nullptr;

        Count<AssemblyInfo> CoreAssemblyInfo = nullptr;
        Count<AssemblyInfo> AppAssemblyInfo = nullptr;

        bool IsMonoInitialized = false;
       // std::unordered_map<UUID, std::unordered_map<uint32_t, Coun<FieldStorageBase>>> EntityFieldMap;
        //std::unordered_map<AssetID, std::unordered_map<uint32_t, Coun<FieldStorageBase>>> PrefabFieldMap;

    };

    struct ScriptEngineRuntimeData
    {

    };

    ScriptEngineData* s_ScriptEngineData = nullptr;
    void ScriptEngine::Init() {

        PF_CORE_ASSERT(!s_ScriptEngineData, "[ScriptEngine]: Trying to call ScriptEngine::Init multiple times!");

        ScopeTimer scopeTime("ScriptEngine::Init");

        s_ScriptEngineData = new ScriptEngineData();

        s_ScriptEngineData->CoreAssemblyInfo = Count<AssemblyInfo>::Create();
        s_ScriptEngineData->AppAssemblyInfo = Count<AssemblyInfo>::Create();

        InitMono();

        ScriptGCManager::Init();

        LoadCoreAssembly();
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
        mono_domain_set(s_ScriptEngineData->CoreDomain, true);
        //mono_domain_set_config(s_ScriptEngineData->CoreDomain, ".", "");

        s_ScriptEngineData->CoreAssemblyInfo->FilePath = corePath;
        s_ScriptEngineData->CoreAssemblyInfo->Assembly = LoadMonoAssembly(s_ScriptEngineData->CoreAssemblyInfo->FilePath);
        s_ScriptEngineData->CoreAssemblyInfo->Classes.clear();

        s_ScriptEngineData->CoreAssemblyInfo->AssemblyImage = mono_assembly_get_image(s_ScriptEngineData->CoreAssemblyInfo->Assembly);
        s_ScriptEngineData->CoreAssemblyInfo->IsCoreAssembly = true;
        s_ScriptEngineData->CoreAssemblyInfo->Metadata = GetMetadataForImage(s_ScriptEngineData->CoreAssemblyInfo->AssemblyImage);
        s_ScriptEngineData->CoreAssemblyInfo->ReferencedAssemblies = GetReferencedAssembliesMetadata(s_ScriptEngineData->CoreAssemblyInfo->AssemblyImage);

        ScriptRegistry::Init();
    }
    std::string ScriptEngine::MonoToString(MonoString* monoString) {
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
       // MonoString* string = mono_string_new(s_Data->AppDomain, data.c_str());
        MonoString* string = nullptr;
        return string;
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
    Count<AssemblyInfo> ScriptEngine::GetCoreAssemblyInfo()
    {
        return s_ScriptEngineData->CoreAssemblyInfo;
    }
    Count<AssemblyInfo> ScriptEngine::GetAppAssemblyInfo()
    {
        return s_ScriptEngineData->AppAssemblyInfo;

    }
}
