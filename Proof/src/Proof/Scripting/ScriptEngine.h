#pragma once
//Reference
//https://github.com/TheCherno/Hazel/blob/scripting/Hazel/src/Hazel/Scripting/ScriptEngine.h
#include "Proof/Core/Core.h"
#include "proof/Scene/Entity.h"
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
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
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
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
	};
	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static void StartWorld();
		static void EndWorld();

		static void CallOnUpdate(float ts, Entity entity);
		static void CallOnCreate(Entity entity);
		static void CallOnSpawn(Entity entity);
		static void CallOnPlace(Entity entity);

		static void CallOnDestroy(Entity entity);
		static bool EntityClassExists(const std::string& fullClassName);

		static void RemoveEntity(Entity entity);
		static void RemoveScript(const std::string& fullClassName, Entity entity);
	private:
		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void InitMono();
		static void LoadAssemblyClasses(MonoAssembly* assembly);
		friend class ScriptClass;
	};
}
