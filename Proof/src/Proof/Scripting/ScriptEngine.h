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
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoProperty MonoProperty;
	typedef struct _MonoString MonoString;
}
namespace Proof
{

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,Enum,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};
	struct ScriptField {
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};
	// ScriptField + data storage
	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];

		friend class ScriptEngine;
		friend class ScriptInstance;
	};
	class ScriptClass {
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* CallMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
		MonoClass* GetMonoClass() {
			return m_MonoClass;
		}
		std::string GetFullName() {
			return fmt::format("{}.{}", m_ClassNamespace, m_ClassName);
		}

		template<typename T>
		T GetFieldDefaultValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			bool success = GetFieldDefaultValueInternal(name, buffer);
			if (!success)
				return T();

			return *(T*)buffer;
		}
		bool GetFieldDefaultValueInternal(const std::string& name, void* buffer);

		const std::unordered_map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		inline static uint8_t buffer[16];

		std::unordered_map<std::string, ScriptField>  m_Fields;
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass = nullptr;

		// tempary for seeing values not gonna stand
		MonoObject* m_Instance = nullptr;
		friend class ScriptEngine;

	};
	class ScriptInstance {
	public:
		ScriptInstance(Count<ScriptClass> scriptClass, Entity entity);
		ScriptInstance(std::string className, Entity entity);

		void CallOnCreate();
		void CallOnUpdate(float ts);

		void CallOnPlace();
		void CallOnSpawn();
		void CallOnDestroy();

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			SetFieldValueInternal(name, &value);
		}
		Count<ScriptClass> GetScriptClass()const  {
			return m_ScriptClass;
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
		Count<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreate = nullptr;
		MonoMethod* m_OnUpdate = nullptr;
		MonoMethod* m_OnPlaced = nullptr;
		MonoMethod* m_OnSpawn = nullptr;
		MonoMethod* m_OnDestroy = nullptr;
		inline static uint8_t s_FieldValueBuffer[16];
		friend class ScriptEngine;
	};

	// need a better system cause when we create prefabs may run into a problem for fields
	
	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static void BeginRuntime(World* world);
		static void EndRuntime();

		static bool EntityClassExists(const std::string& fullClassName);
		static World* GetWorldContext();
		static MonoImage* GetCoreAssemblyImage();
		static Count<ScriptClass> GetScriptClass(const std::string& name);

		static const std::unordered_map<std::string, Count<ScriptClass>>const& GetScripts();
		// scirpt name, instnace
		static std::unordered_map<std::string, Count<ScriptInstance>>const GetScriptInstnace(Entity enitty);
		static bool EntityHasScripts(Entity enitty);

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, float ts);

		// class name, field, data
		static std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>& GetScriptFieldMap(Entity entity);
		static bool HasScriptFieldMap(Entity entity);
		static void CreateScriptFieldMap(Entity entity);
		static std::string MonoToString(MonoString* monoString);
		static MonoString* StringToMono(const std::string& data);

		// reloads the assembly and updates all the script components
		// if scene is being played scene does not run
		static void ReloadAssembly(World* world);
	private:
		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);
		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void InitMono();
		static void LoadAssemblyClasses();
		friend class ScriptClass;
		friend class ScriptFunc;
	};


	namespace Utils {

		inline const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
		{
			switch (fieldType)
			{
				case ScriptFieldType::None:    return "None";
				case ScriptFieldType::Float:   return "Float";
				case ScriptFieldType::Double:  return "Double";
				case ScriptFieldType::Bool:    return "Bool";
				case ScriptFieldType::Char:    return "Char";
				case ScriptFieldType::Byte:    return "Byte";
				case ScriptFieldType::Short:   return "Short";
				case ScriptFieldType::Int:     return "Int";
				case ScriptFieldType::Long:    return "Long";
				case ScriptFieldType::UByte:   return "UByte";
				case ScriptFieldType::UShort:  return "UShort";
				case ScriptFieldType::UInt:    return "UInt";
				case ScriptFieldType::ULong:   return "ULong";
				case ScriptFieldType::Vector2: return "Vector2";
				case ScriptFieldType::Vector3: return "Vector3";
				case ScriptFieldType::Vector4: return "Vector4";
				case ScriptFieldType::Entity:  return "Entity";
			}
			PF_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")    return ScriptFieldType::None;
			if (fieldType == "Float")   return ScriptFieldType::Float;
			if (fieldType == "Double")  return ScriptFieldType::Double;
			if (fieldType == "Bool")    return ScriptFieldType::Bool;
			if (fieldType == "Char")    return ScriptFieldType::Char;
			if (fieldType == "Byte")    return ScriptFieldType::Byte;
			if (fieldType == "Short")   return ScriptFieldType::Short;
			if (fieldType == "Int")     return ScriptFieldType::Int;
			if (fieldType == "Long")    return ScriptFieldType::Long;
			if (fieldType == "UByte")   return ScriptFieldType::UByte;
			if (fieldType == "UShort")  return ScriptFieldType::UShort;
			if (fieldType == "UInt")    return ScriptFieldType::UInt;
			if (fieldType == "ULong")   return ScriptFieldType::ULong;
			if (fieldType == "Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Entity")  return ScriptFieldType::Entity;

			PF_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}

	}

}

