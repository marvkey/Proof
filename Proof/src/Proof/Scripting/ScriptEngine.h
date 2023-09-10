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

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, 
		Int8_t, Int16_t, Int32_t, Int64_t,Enum, //suports only integer types, they are basically integers,
		Uint8_t,Uint16_t, Uint32_t, Uint64_t,
		Vector2, Vector3, Vector4,
		Entity,
		Prefab,
		Texture,
	};
	struct ScriptEnumData
	{
		std::string Name;
		uint8_t Value[16];
		ScriptFieldType EnumType; // can only be integer values
		template<typename T>
		T GetValue()const
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)Value;
		}
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

		void SetValueRaw(uint8_t buffer[16])
		{
			memcpy(m_Buffer, buffer, sizeof(buffer));
		}
	private:
		uint8_t m_Buffer[16];

		friend class ScriptEngine;
		friend class ScriptInstance;
		friend class ScriptMeathod;
	};
	class ScriptClass : public RefCounted {
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

		std::array<uint8_t, 16> GetFieldDefaultValueRaw(const std::string& name)
		{
			std::array<uint8_t, 16> buffer;
			bool success = GetFieldDefaultValueInternal(name, buffer.data());
			if (!success)
				return {};

			return buffer;
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
	class ScriptInstance : public RefCounted {
	public:
		ScriptInstance(Count<ScriptClass> scriptClass, Entity entity);
	

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

		MonoObject* GetMonoObject() 
		{
			return m_Instance;
		}
	private:
		void CallOnCreate();
		void CallOnUpdate(float ts);

		void CallOnPlace();
		void CallOnSpawn();
		void CallOnDestroy();

		void CallOnCollisionEnter(Entity otherEntity);
		void CallOnCollissionLeave(Entity otherEntity);

		void CallOnTriggerEnter(Entity otherEntity);

		// entity box we are enterin g
		void CallOnOverllapTriggerEnter(Entity otherEntity);


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

		MonoMethod* m_OnCollisionEnter = nullptr;
		MonoMethod* m_OnCollisionLeave = nullptr;
		MonoMethod* m_OnTriggerEnter = nullptr;
		MonoMethod* m_OnOverlapTriggerEnter = nullptr;

		inline static uint8_t s_FieldValueBuffer[16];
		friend class ScriptEngine;
		friend class ScriptMeathod;
	};

	class ScriptMeathod 
	{
	public:
		static void OnCreate(Entity entity);
		static void OnDestroy(Entity entity);
		static void OnUpdate(Entity entity, FrameTime time);

		static void CallMeathod(Count<ScriptInstance> instnace, MonoMethod* meathod, void** params);
	//	static void OnDestroy(Entity entity, const std::string& name);
		// physics
		static void OnCollisionEnter(Entity currentEntity, Entity collidingEntity);
		static void OnCollisionLeave(Entity currentEntity, Entity leavingEntity);

		// current entier, entity that has enterd the current entity trigger box
		static void OnTriggerEnter(Entity currentEntity, Entity triggerEntity);

		//  current entity has entered anotehr entity trigger box and is notfieid
		static void OnOverlapTriggerEnter(Entity currentEntity, Entity entityWithTriggerBox);
	};
	// need a better system cause when we create prefabs may run into a problem for fields
	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static void BeginRuntime(World* world);
		static void EndRuntime();
				// reloads the assembly and updates all the script components
		// if scene is being played scene does not run
		static void ReloadAssembly(World* world);


		static bool EntityClassExists(const std::string& fullClassName);
		static World* GetWorldContext();
		static MonoImage* GetCoreAssemblyImage();
		static Count<ScriptClass> GetScriptClass(const std::string& name);

		static const std::unordered_map<std::string, Count<ScriptClass>>const& GetScripts();
		// scirpt name, instnace
		static std::unordered_map<std::string, Count<ScriptInstance>>const& GetScriptInstnace(Entity enitty);
		static bool EntityHasScripts(Entity enitty);
		static const std::unordered_map<UUID, std::unordered_map<std::string, Count<ScriptInstance>>>& EachEntityScript();
		// class name, field, data
		static std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>& GetScriptFieldMap(Entity entity);
		static std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>& GetScriptFieldMap(UUID id);
		//prefab Id, (Prefab Entity ID), class Name, {field name, field
		static std::unordered_map<AssetID, std::unordered_map<UUID, std::unordered_map<std::string, std::unordered_map<std::string, ScriptFieldInstance>>>>& GetPrefabFieldMap(AssetID ID);

		static const std::unordered_map<std::string,std::pair<ScriptFieldType, std::vector<ScriptEnumData>>>& GetEnumClasses();

		static std::string GetFieldEnumName(ScriptField field);
		static bool HasScriptFieldMap(Entity entity);
		static void CreateScriptFieldMap(Entity entity);
		static std::string MonoToString(MonoString* monoString);
		static MonoString* StringToMono(const std::string& data);
		static MonoObject* InstantiateClass(MonoClass* monoClass);

		static MonoObject* GetMonoManagedObject(UUID ID, const std::string& fullName);
		static MonoDomain* GetDomain();
	private:
		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);
		static void InitMono();
		static void LoadAssemblyClasses();
		static void AssemblyLoadFields(Count<ScriptClass> script, MonoClass* monoClass);
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
				case ScriptFieldType::Int8_t:    return "Byte";
				case ScriptFieldType::Int16_t:   return "Short";
				case ScriptFieldType::Int32_t:     return "Int";
				case ScriptFieldType::Int64_t:    return "Long";
				case ScriptFieldType::Uint8_t:   return "UByte";
				case ScriptFieldType::Uint16_t:  return "UShort";
				case ScriptFieldType::Uint32_t:    return "UInt";
				case ScriptFieldType::Uint64_t:   return "ULong";
				case ScriptFieldType::Vector2: return "Vector2";
				case ScriptFieldType::Vector3: return "Vector3";
				case ScriptFieldType::Vector4: return "Vector4";
				case ScriptFieldType::Entity:  return "Entity";
				case ScriptFieldType::Prefab:  return "Prefab";
				case ScriptFieldType::Texture:  return "ImageAsset";
				case ScriptFieldType::Enum:  return "Enum";
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
			if (fieldType == "Byte")    return ScriptFieldType::Int8_t;
			if (fieldType == "Short")   return ScriptFieldType::Int16_t;
			if (fieldType == "Int")     return ScriptFieldType::Int32_t;
			if (fieldType == "Long")    return ScriptFieldType::Int64_t;
			if (fieldType == "UByte")   return ScriptFieldType::Uint8_t;
			if (fieldType == "UShort")  return ScriptFieldType::Uint16_t;
			if (fieldType == "UInt")    return ScriptFieldType::Uint32_t;
			if (fieldType == "ULong")   return ScriptFieldType::Uint64_t;
			if (fieldType == "Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Entity")  return ScriptFieldType::Entity;
			if (fieldType == "Prefab")  return ScriptFieldType::Prefab;
			if (fieldType == "Image")  return ScriptFieldType::Texture;
			if (fieldType == "Enum")  return ScriptFieldType::Enum;

			PF_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}

	}

}

