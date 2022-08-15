#pragma once
//Reference
//https://github.com/TheCherno/Hazel/blob/scripting/Hazel/src/Hazel/Scripting/ScriptEngine.h
#include "Proof/Core/Core.h"
#include "MonoTypes.h"
#include "proof/Scene/Entity.h"
#include <any>
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoProperty MonoProperty;
}
namespace Proof
{


	class ScriptClass {
	public:
		ScriptClass() = default;
		ScriptClass(const std::string & classNamespace, const std::string & className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* CallMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
		std::vector<ScriptField> m_FieldData;
		MonoClass* GetMonoClass() {
			return m_MonoClass;
		}
		std::string GetFullName() {
			return fmt::format("{}.{}", m_ClassNamespace, m_ClassName);
		}
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass = nullptr;
		friend class ScriptEngine;

	};

	class ScriptInstance {
	public:
		ScriptInstance(Count<ScriptClass> scriptClass, Entity entity);

		void CallOnCreate();
		void CallOnUpdate(float ts);

		void CallOnPlace();
		void CallOnSpawn();
		void CallOnDestroy();
	private:
		Count<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreate = nullptr;
		MonoMethod* m_OnUpdate = nullptr;
		MonoMethod* m_OnPlaced = nullptr;
		MonoMethod* m_OnSpawn = nullptr;
		MonoMethod* m_OnDestroy = nullptr;

		friend class ScriptEngine;
	};
	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static void StartWorld(World* world);
		static void EndWorld();

		static void OnUpdate(float ts, Entity entity);
		static void OnCreate(Entity entity);
		static void OnSpawn(Entity entity);
		static void OnPlace(Entity entity);
		static void OnDestroy(Entity entity);

		static bool EntityClassExists(const std::string& fullClassName);

		static World* GetWorldContext();
		static void RemoveEntity(Entity entity);
		static void RemoveScript(const std::string& fullClassName, Entity entity);
		static MonoImage* GetCoreAssemblyImage();
		// any variable
		static uint8_t GetFieldAccessibility(MonoClassField* field);
		// Getters and setters variables 
		static uint8_t GetPropertyAccessbility(MonoProperty* property);

		static const ScriptClass* GetScriptClass(const std::string& name);
		static void SetValue(UUID ID, const std::string& className, const std::string& varName, void* data);

		static bool IsFieldAvailable(const std::string& className, const std::string& varName);
		static const std::unordered_map<std::string, Count<ScriptClass>>const& GetScripts();
	private:
		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void InitMono();
		static void LoadAssemblyClasses(MonoAssembly* assembly);
		friend class ScriptClass;
		friend class ScriptFunc;
	};
}
