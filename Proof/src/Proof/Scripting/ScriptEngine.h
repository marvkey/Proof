#pragma once
//Reference
//https://github.com/TheCherno/Hazel/blob/scripting/Hazel/src/Hazel/Scripting/ScriptEngine.h
#include "Proof/Core/Core.h"
#include "proof/Scene/Entity.h"
#include "ScriptTypes.h"
#include "ScriptRegistry.h"
#include "ScriptGCManager.h"
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
	//struct AssemblyMetadata;
	
	class ScriptEngine {
	public:
		static void Init();
		static void ShutDown();

		static void BeginRuntime(Count<World> world);
		static void EndRuntime();
		static bool IsRuntime();

		static Count<World> GetWorldContext();
		static AssemblyMetadata GetMetadataForImage(MonoImage* image);
		static std::vector<AssemblyMetadata> GetReferencedAssembliesMetadata(MonoImage* image);
		static const std::unordered_map<std::string, Count<ScriptClass>>& GetEntityScripts();
		static void ReloadppAssembly();

		// vecotr of classes, adn its fields for an entity
		static bool IsModuleValid(const std::string& classFullName);

		template<typename... TArgs>
		static void CallMethod(MonoObject* managedObject, const std::string& methodName, TArgs&&... args)
		{

			if (managedObject == nullptr)
			{
				PF_ENGINE_WARN("ScriptEngine Attempting to call method {0} on an invalid instance!", methodName);
				return;
			}

			constexpr size_t argsCount = sizeof...(args);

			ManagedClass* clazz = ScriptRegistry::GetMonoObjectClass(managedObject);
			if (clazz == nullptr)
			{
				PF_ENGINE_ERROR("ScriptEngine Failed to find ManagedClass!");
				return;
			}

			ManagedMethod* method = ScriptRegistry::GetSpecificManagedMethod(clazz, methodName, argsCount);
			if (method == nullptr)
			{
				PF_ENGINE_ERROR("ScriptEngine Failed to find a C# method called {0} with {1} parameters", methodName, argsCount);
				return;
			}

			if constexpr (argsCount > 0)
			{
				const void* data[] = { &args... };
				CallMethod(managedObject, method, data);
			}
			else
			{
				CallMethod(managedObject, method, nullptr);
			}
		}
		#if 0
		template<typename... TArgs>
		static void CallMethod(GCHandle instance, const std::string& methodName, TArgs&&... args)
		{
			if (instance == nullptr)
			{
				PF_ENGINE_WARN("ScriptEngine Attempting to call method {0} on an invalid instance!", methodName);
				return;
			}

			CallMethod(GCManager::GetReferencedObject(instance), methodName, std::forward<TArgs>(args)...);
		}
		#endif
		static Count<AssemblyInfo> GetCoreAssemblyInfo();
		static Count<AssemblyInfo> GetAppAssemblyInfo();

		static MonoDomain* GetAppDomain();
		static MonoDomain* GetCoreDomain();

		template<typename... TConstructorArgs>
		static MonoObject* CreateManagedObject(const std::string& className, TConstructorArgs&&... args)
		{
			return CreateManagedObject_Internal(GetManagedClass(className), true,std::forward<TConstructorArgs>(args)...);
		}
		template<typename... TArgs>
		static void CallMethod(ScriptGCHandle instance, const std::string& methodName, TArgs&&... args)
		{
			if (instance == nullptr)
			{
				PF_ENGINE_WARN("ScriptEngine Attempting to call method {0} on an invalid instance!", methodName);
				return;
			}

			CallMethod(ScriptGCManager::GetReferencedObject(instance), methodName, std::forward<TArgs>(args)...);
		}


	private:
		static void InitMono();
		static void ShutDownMono();
		static bool LoadCoreAssembly();
		static bool LoadAppAssembly();
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);

		static MonoObject* CreateManagedObject(ManagedClass* managedClass, bool appDomain =true);
		static MonoObject* CreateManagedObject(const std::string& className, bool appDomain = true);
		static ManagedClass* GetManagedClass(const std::string& className);

		static void InitRuntimeObject(MonoObject* monoObject);

		static void CallMethod(MonoObject* monoObject, ManagedMethod* managedMethod, const void** parameters);

	private:

		template<typename... TConstructorArgs>
		static MonoObject* CreateManagedObject_Internal(ManagedClass* managedClass,bool appDomain, TConstructorArgs&&... args)
		{

			if (managedClass == nullptr)
			{
				PF_ENGINE_ERROR("ScriptEngine Attempting to create managed object with a null class!");
				return nullptr;
			}

			if (managedClass->IsAbstract)
				return nullptr;

			MonoObject* obj = CreateManagedObject(managedClass, appDomain);

			if (managedClass->IsStruct)
				return obj;

			//if (ManagedType::FromClass(managedClass).IsValueType())
			//	return obj;

			constexpr size_t argsCount = sizeof...(args);
			ManagedMethod* ctor = ScriptRegistry::GetSpecificManagedMethod(managedClass, ".ctor", argsCount);

			InitRuntimeObject(obj);

			if constexpr (argsCount > 0)
			{
				if (ctor == nullptr)
				{
					PF_ENGINE_ERROR("ScriptEngine Failed to call constructor with {} parameters for class '{}'.", argsCount, managedClass->FullName);
					return obj;
				}

				const void* data[] = { &args... };
				CallMethod(obj, ctor, data);
			}

			return obj;
		}

	private:
		friend class ScriptRegistry;
	};


}

