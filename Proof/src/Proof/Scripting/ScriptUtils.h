#pragma once
#include "ScriptField.h"
#include <mono/utils/mono-error.h>
#include <vector>
#include "Proof/Scene/Entity.h"
#include "ValueWrapper.h"
extern "C" {
	typedef struct _MonoType MonoType;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoException MonoException;
}
namespace Proof
{
	struct ManagedClass;
}
namespace Proof::ScriptUtils
{
	ScriptFieldType	GetFieldTypeFromMonoType(MonoType* monoType);
	std::string ResolveMonoClassName(MonoClass* monoClass);
	Buffer GetFieldValue(MonoObject* classInstance, const std::string& fieldName, ScriptFieldType fieldType, bool isProperty);
	MonoObject* GetFieldValueObject(MonoObject* classInstance, const std::string& fieldName, bool isProperty);
	void SetFieldValue(MonoObject* classInstance, const ScriptField* fieldInfo, const void* data);
	void HandleException(MonoObject* exception);
	void SplitFullClassName(const std::string& fullClassName, std::string& outNamespace, std::string& outName);

	Buffer MonoObjectToValue(MonoObject* obj, ScriptFieldType fieldType);
	void* UnBoxInternal(MonoObject* obj);

	template<typename TValueType>
	TValueType Unbox(MonoObject* obj) { return *(TValueType*)UnBoxInternal(obj); }
	template<typename TValueType>
	TValueType UnboxAddress(MonoObject* obj) { return (TValueType*)UnBoxInternal(obj); }
	MonoObject* BoxValue(MonoClass* valueClass, const void* value);

	MonoString* EmptyMonoString(bool appDomain = true);
	std::string MonoStringToUTF8(MonoString* monoString);
	MonoString* UTF8StringToMono(const std::string& str);

	bool CheckMonoError(MonoError& error);

	MonoObject* ValueToMonoObject(const void* data, ScriptFieldType dataType);

	class ManagedArrayUtils
	{
	public:
		static Utils::ValueWrapper GetValue(MonoArray* arr, uintptr_t index);

		template<typename TValueType>
		static void SetValue(MonoArray* arr, uintptr_t index, TValueType value)
		{
			if constexpr (std::is_same<TValueType, MonoObject*>::value)
				SetValueInternal(arr, index, value);
			else
				SetValueInternal(arr, index, &value);
		}

		static uintptr_t Length(MonoArray* arr);
		static void Resize(MonoArray** arr, uintptr_t newLength);
		static void RemoveAt(MonoArray** arr, uintptr_t index);
		static MonoArray* Copy(MonoArray* arr);

		template<typename TValueType>
		static MonoArray* FromVector(const std::vector<TValueType>& vec)
		{
			MonoArray* arr = Create<TValueType>(vec.size());
			for (size_t i = 0; i < vec.size(); i++)
				SetValue<TValueType>(arr, i, vec[i]);
			return arr;
		}

		template<typename TValueType>
		static std::vector<TValueType> ToVector(MonoArray* arr)
		{
			uintptr_t length = Length(arr);

			std::vector<TValueType> vec;
			vec.resize(length);

			if constexpr (std::is_same_v<TValueType, Utils::ValueWrapper>)
			{
				for (uintptr_t i = 0; i < length; i++)
					vec[i] = GetValue(arr, i);
			}
			else
			{
				for (uintptr_t i = 0; i < length; i++)
					vec[i] = GetValue(arr, i).Get<TValueType>();
			}

			return vec;
		}

	public:
		static MonoArray* Create(const std::string& arrayClass, uintptr_t length);
		static MonoArray* Create(ManagedClass* arrayClass, uintptr_t length);

		template<typename T>
		static MonoArray* Create(uintptr_t length) {
			PF_CORE_ASSERT(false);
			return nullptr;
		}


	private:
		static void SetValueInternal(MonoArray* arr, uintptr_t index, void* data);
		static void SetValueInternal(MonoArray* arr, uintptr_t index, MonoObject* value);
	};

	template<> inline MonoArray* ManagedArrayUtils::Create<bool>(uintptr_t length) { return Create("System.Boolean", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<int8_t>(uintptr_t length) { return Create("System.SByte", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<int16_t>(uintptr_t length) { return Create("System.Int16", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<int32_t>(uintptr_t length) { return Create("System.Int32", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<int64_t>(uintptr_t length) { return Create("System.Int64", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<uint8_t>(uintptr_t length) { return Create("System.Byte", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<uint16_t>(uintptr_t length) { return Create("System.UInt16", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<uint32_t>(uintptr_t length) { return Create("System.UInt32", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<uint64_t>(uintptr_t length) { return Create("System.UInt64", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<float>(uintptr_t length) { return Create("System.Single", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<double>(uintptr_t length) { return Create("System.Double", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<char>(uintptr_t length) { return Create("System.Char", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<std::string>(uintptr_t length) { return Create("System.String", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<Entity>(uintptr_t length) { return Create("PF.Entity", length); }
	//template<> inline MonoArray* ManagedArrayUtils::Create<Prefab>(uintptr_t length) { return Create("Proof.Prefab", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<glm::vec2>(uintptr_t length) { return Create("PF.Vector2", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<glm::vec3>(uintptr_t length) { return Create("PF.Vector3", length); }
	template<> inline MonoArray* ManagedArrayUtils::Create<glm::vec4>(uintptr_t length) { return Create("PF.Vector4", length); }
}