#pragma once
//Reference
//https://github.com/TheCherno/Hazel/blob/scripting/Hazel/src/Hazel/Scripting/ScriptEngine.h
#include "Proof/Core/Core.h"
#include "proof/Scene/Entity.h"
#include <any>
#include <functional>
#include <filesystem>
#include <string>
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoProperty MonoProperty;
	typedef struct _MonoString MonoString;
}
namespace Proof
{
	struct AssemblyMetadata;

	class ScriptEngine {
	public:
		static void Init();
		static void ShutDown();

		static void BeginRuntime(World* world) {};
		static void EndRuntime() {};

		static World* GetWorldContext() { return NULL; }
		static std::string MonoToString(MonoString* monoString);
		static MonoString* StringToMono(const std::string& data);
		static AssemblyMetadata GetMetadataForImage(MonoImage* image);
		static std::vector<AssemblyMetadata> GetReferencedAssembliesMetadata(MonoImage* image);
	private:
		static void InitMono();
		static void ShutDownMono();
		static bool LoadCoreAssembly();
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);

	};


}

