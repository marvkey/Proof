#include "Proofprch.h"
#include "ScriptUtils.h"
#include "ScriptRegistry.h"
#include "ScriptEngine.h"
#include "ScriptField.h"
#include "ScriptTypes.h"
#include "Proof/Utils/StringUtils.h"

#include "Proof/Asset/AssetManager.h"
#include <glm/glm.hpp>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/tokentype.h>

#define PF_CORE_CLASS(clazz) ScriptRegistry::GetManagedClassByName("Proof."#clazz)

namespace Proof::ScriptUtils
{

    ScriptFieldType GetFieldTypeFromMonoType(MonoType* monoType)
    {
		int32_t typeEncoding = mono_type_get_type(monoType);
		MonoClass* typeClass = mono_type_get_class(monoType);

		switch (typeEncoding)
		{
			case MONO_TYPE_VOID:		return ScriptFieldType::Void;
			case MONO_TYPE_BOOLEAN:		return ScriptFieldType::Bool;
			case MONO_TYPE_CHAR:		return ScriptFieldType::Char;
			case MONO_TYPE_I1:			return ScriptFieldType::Int8;
			case MONO_TYPE_I2:			return ScriptFieldType::Int16;
			case MONO_TYPE_I4:			return ScriptFieldType::Int32;
			case MONO_TYPE_I8:			return ScriptFieldType::Int64;
			case MONO_TYPE_U1:			return ScriptFieldType::UInt8;
			case MONO_TYPE_U2:			return ScriptFieldType::UInt16;
			case MONO_TYPE_U4:			return ScriptFieldType::UInt32;
			case MONO_TYPE_U8:			return ScriptFieldType::UInt64;
			case MONO_TYPE_R4:			return ScriptFieldType::Float;
			case MONO_TYPE_R8:			return ScriptFieldType::Double;
			case MONO_TYPE_STRING:		return ScriptFieldType::String;
			case MONO_TYPE_ENUM:
				{
					MonoType* enumType = mono_class_enum_basetype(typeClass);
					return ScriptUtils::GetFieldTypeFromMonoType(enumType);
					break;

				}
			case MONO_TYPE_VALUETYPE:
				{
					if (mono_class_is_enum(typeClass))
					{
						MonoType* enumType = mono_class_enum_basetype(typeClass);
						return ScriptUtils::GetFieldTypeFromMonoType(enumType);
						break;
					}
					if (PF_CORE_CLASS(AssetID) && typeClass == PF_CORE_CLASS(AssetID)->Class)
						return ScriptFieldType::AssetID;

					if (PF_CORE_CLASS(Vector2) && typeClass == PF_CORE_CLASS(Vector2)->Class)
						return ScriptFieldType::Vector2;

					if (PF_CORE_CLASS(Vector3) && typeClass == PF_CORE_CLASS(Vector3)->Class)
						return ScriptFieldType::Vector3;

					if (PF_CORE_CLASS(Vector4) && typeClass == PF_CORE_CLASS(Vector4)->Class)
						return ScriptFieldType::Vector4;

					break;
				}
			case MONO_TYPE_CLASS:
				{
					auto entityClass = PF_CORE_CLASS(Entity);

					if (entityClass && mono_class_is_assignable_from(typeClass, entityClass->Class))
						return ScriptFieldType::Entity;

					if (PF_CORE_CLASS(Prefab) && typeClass == PF_CORE_CLASS(Prefab)->Class)
						return ScriptFieldType::Prefab;

					if (PF_CORE_CLASS(Mesh) && typeClass == PF_CORE_CLASS(Mesh)->Class)
						return ScriptFieldType::Mesh;

					if (PF_CORE_CLASS(DynamicMesh) && typeClass == PF_CORE_CLASS(StaticMesh)->Class)
						return ScriptFieldType::DynamicMesh;

					if (PF_CORE_CLASS(Material) && typeClass == PF_CORE_CLASS(Material)->Class)
						return ScriptFieldType::Material;

					if (PF_CORE_CLASS(PhysicsMaterial) && typeClass == PF_CORE_CLASS(PhysicsMaterial)->Class)
						return ScriptFieldType::PhysicsMaterial;

					if (PF_CORE_CLASS(Texture2D) && typeClass == PF_CORE_CLASS(Texture2D)->Class)
						return ScriptFieldType::Texture2D;

					break;
				}
			case MONO_TYPE_SZARRAY:
			case MONO_TYPE_ARRAY:
				{
					MonoClass* elementClass = mono_class_get_element_class(typeClass);
					if (elementClass == nullptr)
						break;

					ManagedClass* managedElementClass = ScriptRegistry::GetManagedClass(elementClass);
					if (managedElementClass == nullptr)
						break;

					return GetFieldTypeFromMonoType(mono_class_get_type(elementClass));
				}
		
		}

		return ScriptFieldType::Void;
    }

	std::string ResolveMonoClassName(MonoClass* monoClass)
	{
		const char* classNamePtr = mono_class_get_name(monoClass);
		std::string className = classNamePtr != nullptr ? classNamePtr : "";

		if (className.empty())
			return "Unknown Class";

		MonoClass* nestingClass = mono_class_get_nesting_type(monoClass);
		if (nestingClass != nullptr)
		{
			className = ResolveMonoClassName(nestingClass) + "/" + className;
		}
		else
		{
			const char* classNamespacePtr = mono_class_get_namespace(monoClass);
			if (classNamespacePtr)
				className = std::string(classNamespacePtr) + "." + className;
		}

		MonoType* classType = mono_class_get_type(monoClass);
		if (mono_type_get_type(classType) == MONO_TYPE_SZARRAY || mono_type_get_type(classType) == MONO_TYPE_ARRAY)
			Utils::String::Erase(className, "[]");

		return className;
	}

	Buffer GetFieldValue(MonoObject* classInstance, const std::string& fieldName, ScriptFieldType fieldType, bool isProperty)
	{
		PF_PROFILE_FUNC();
		MonoObject* object =  GetFieldValueObject(classInstance, fieldName, isProperty);
		return MonoObjectToValue(object, fieldType);
	}

	MonoObject* GetFieldValueObject(MonoObject* classInstance, const std::string& fieldName, bool isProperty)
	{
		PF_PROFILE_FUNC();
		
		MonoClass* objectClass = mono_object_get_class(classInstance);

		MonoObject* valueObject = nullptr;

		if (isProperty)
		{
			MonoProperty* classProperty = mono_class_get_property_from_name(objectClass, fieldName.c_str());
			valueObject = mono_property_get_value(classProperty, classInstance, nullptr, nullptr);
		}
		else
		{
			MonoClassField* classField = mono_class_get_field_from_name(objectClass, fieldName.c_str());
			PF_CORE_ASSERT(classField);
			valueObject = mono_field_get_value_object(mono_domain_get(), classField, classInstance);
		}

		return valueObject;
	}
	MonoObject* ValueToMonoObject(const void* data, ScriptFieldType dataType)
	{
		PF_PROFILE_FUNC();

		if (IsPrimitiveType(dataType))
		{
			return BoxValue(ScriptRegistry::GetFieldTypeClass(dataType), data);
		}
		else
		{
			switch (dataType)
			{
				case ScriptFieldType::String: return (MonoObject*)UTF8StringToMono(*(std::string*)data);
				case ScriptFieldType::Prefab: return ScriptEngine::CreateManagedObject("Proof.Prefab", *(AssetID*)data);
				case ScriptFieldType::Entity: return ScriptEngine::CreateManagedObject("Proof.Entity", *(UUID*)data);
				case ScriptFieldType::Mesh: return ScriptEngine::CreateManagedObject("Proof.Mesh", *(AssetID*)data);
				case ScriptFieldType::DynamicMesh: return ScriptEngine::CreateManagedObject("Proof.DynamicMesh", *(AssetID*)data);
				case ScriptFieldType::Material: return ScriptEngine::CreateManagedObject("Proof.Material", *(AssetID*)data);
				case ScriptFieldType::PhysicsMaterial: return ScriptEngine::CreateManagedObject("Proof.PhysicsMaterial", *(AssetID*)data);
				case ScriptFieldType::Texture2D: return ScriptEngine::CreateManagedObject("Proof.Texture2D", *(AssetID*)data);
			}
		}

		PF_CORE_ASSERT(false, "Unsupported value type!");
		return nullptr;
	}

	void SetFieldValue(MonoObject* classInstance, const ScriptField* fieldInfo, const void* data)
	{
		PF_PROFILE_FUNC();

		if (classInstance == nullptr || fieldInfo == nullptr || data == nullptr)
			return;

		if (!fieldInfo->IsWritable())
			return;

		MonoClass* objectClass = mono_object_get_class(classInstance);

		if (fieldInfo->IsProperty)
		{
			MonoProperty* classProperty = mono_class_get_property_from_name(objectClass, fieldInfo->Name.c_str());
			void* propertyData = nullptr;

			if (fieldInfo->IsArray() || IsPrimitiveType(fieldInfo->Type))
				propertyData = const_cast<void*>(data);
			else
				propertyData = ValueToMonoObject(data, fieldInfo->Type);

			mono_property_set_value(classProperty, classInstance, &propertyData, nullptr);
		}
		else
		{
			MonoClassField* classField = mono_class_get_field_from_name(objectClass, fieldInfo->Name.c_str());
			void* fieldData = nullptr;

			if (fieldInfo->IsArray() || IsPrimitiveType(fieldInfo->Type))
				fieldData = (void*)data;
			else
				fieldData = ValueToMonoObject(data, fieldInfo->Type);

			mono_field_set_value(classInstance, classField, fieldData);
		}
	}

	Buffer MonoObjectToValue(MonoObject* obj, ScriptFieldType fieldType)
	{
		PF_PROFILE_FUNC();

		if (obj == nullptr)
			return Buffer();

		Buffer result;
		result.Allocate(GetFieldTypeSize(fieldType));
		result.ZeroInitialize();

		switch (fieldType)
		{
			case ScriptFieldType::Bool:
				{
					bool value = (bool)Unbox<MonoBoolean>(obj);
					result.Write(&value, sizeof(bool));
					break;
				}
			case ScriptFieldType::Int8:
				{
					int8_t value = Unbox<int8_t>(obj);
					result.Write(&value, sizeof(int8_t));
					break;
				}
			case ScriptFieldType::Int16:
				{
					int16_t value = Unbox<int16_t>(obj);
					result.Write(&value, sizeof(int16_t));
					break;
				}
			case ScriptFieldType::Int32:
				{
					int32_t value = Unbox<int32_t>(obj);
					result.Write(&value, sizeof(int32_t));
					break;
				}
			case ScriptFieldType::Int64:
				{
					int64_t value = Unbox<int64_t>(obj);
					result.Write(&value, sizeof(int64_t));
					break;
				}
			case ScriptFieldType::UInt8:
				{
					uint8_t value = Unbox<uint8_t>(obj);
					result.Write(&value, sizeof(uint8_t));
					break;
				}
			case ScriptFieldType::UInt16:
				{
					uint16_t value = Unbox<uint16_t>(obj);
					result.Write(&value, sizeof(uint16_t));
					break;
				}
			case ScriptFieldType::UInt32:
				{
					uint32_t value = Unbox<uint32_t>(obj);
					result.Write(&value, sizeof(uint32_t));
					break;
				}
			case ScriptFieldType::UInt64:
				{
					uint64_t value = Unbox<uint64_t>(obj);
					result.Write(&value, sizeof(uint64_t));
					break;
				}
			case ScriptFieldType::Float:
				{
					float value = Unbox<float>(obj);
					result.Write(&value, sizeof(float));
					break;
				}
			case ScriptFieldType::Double:
				{
					double value = Unbox<double>(obj);
					result.Write(&value, sizeof(double));
					break;
				}
			case ScriptFieldType::String:
				{
					std::string str = MonoStringToUTF8((MonoString*)obj);
					result.Allocate(str.size() + 1);
					result.ZeroInitialize();
					result.Write(str.data(), str.size());
					break;
				}
			case ScriptFieldType::Vector2:
				{
					glm::vec2 value = Unbox<glm::vec2>(obj);
					result.Write(glm::value_ptr(value), sizeof(glm::vec2));
					break;
				}
			case ScriptFieldType::Vector3:
				{
					glm::vec3 value = Unbox<glm::vec3>(obj);
					result.Write(glm::value_ptr(value), sizeof(glm::vec3));
					break;
				}
			case ScriptFieldType::Vector4:
				{
					glm::vec4 value = Unbox<glm::vec4>(obj);
					result.Write(glm::value_ptr(value), sizeof(glm::vec4));
					break;
				}
			case ScriptFieldType::AssetID:
				{
					AssetID value = Unbox<AssetID>(obj);
					result.Write(&value, sizeof(AssetID));
					break;
				}
			case ScriptFieldType::Entity:
				{
					Buffer idBuffer = GetFieldValue(obj, "ID", ScriptFieldType::UInt64, false);
					result.Write(idBuffer.Data, sizeof(UUID));
					idBuffer.Release();
					break;
				}
			case ScriptFieldType::Prefab:
			case ScriptFieldType::Mesh:
			case ScriptFieldType::DynamicMesh:
			case ScriptFieldType::Material:
			case ScriptFieldType::PhysicsMaterial:
			case ScriptFieldType::Texture2D:
				{
					Buffer handleBuffer = GetFieldValue(obj, "m_ID", ScriptFieldType::AssetID, false);
					result.Write(handleBuffer.Data, sizeof(AssetID));
					handleBuffer.Release();
					break;
				}
			default:
				PF_ENGINE_ERROR("Does not support {}", EnumReflection::EnumString(fieldType));
				PF_CORE_ASSERT(false);
				break;
		}

		return result;
	}

	MonoObject* BoxValue(MonoClass* valueClass, const void* value)
	{
		/*
		In the context of Mono, when you have a value (like a number) that's normally not an object but you need to treat it like one, you "box" it. 
		Boxing means putting that value into a special container, like a box. This allows you to use the value where objects (or references to objects) are expected.
		Here's how you might use `mono_value_box`:

		1. You have some value (e.g., an integer) that you want to work with in a Mono-based application.

		2. You call `mono_value_box` and provide:
		   - The type of the value (e.g., `int`).
		   - The actual value you want to box (e.g., the number 42).

		3. Mono takes that value, puts it in a box (a special object), and hands you back a reference to that box.

		4. Now, you can use that reference as if it were an object, even though it originally wasn't.

		In short, `mono_value_box` lets you work with non-object values in an environment where objects are expected, like in many .NET and Mono applications. 
		It's a way of making different types of data look like objects when you need them to.
		*/
		return mono_value_box(ScriptEngine::GetAppDomain(), valueClass, const_cast<void*>(value));
	}

	MonoString* EmptyMonoString(bool appDomain)
	{

		MonoDomain* domain;
		if (appDomain)
			domain = ScriptEngine::GetAppDomain();
		else
			domain = ScriptEngine::GetCoreDomain();
		return mono_string_empty(domain);
	}

	std::string MonoStringToUTF8(MonoString* monoString)
	{
		if (monoString == nullptr || mono_string_length(monoString) == 0)
			return "";

		MonoError error;
		char* utf8 = mono_string_to_utf8_checked(monoString, &error);
		if (ScriptUtils::CheckMonoError(error))
			return "";
		std::string result(utf8);
		mono_free(utf8);
		return result;
	}

	MonoString* UTF8StringToMono(const std::string& str)
	{
		return mono_string_new(ScriptEngine::GetAppDomain(), str.c_str());
	}

	bool CheckMonoError(MonoError& error)
	{
		bool hasError = !mono_error_ok(&error);

		if (hasError)
		{
			unsigned short errorCode = mono_error_get_error_code(&error);
			const char* errorMessage = mono_error_get_message(&error);


			PF_ENGINE_ERROR("ScriptEngine Mono Error!");
			PF_ENGINE_ERROR("ScriptEngine\tError Code: {0}", errorCode);
			PF_ENGINE_ERROR("ScriptEngine\tError Message: {0}", errorMessage);
			mono_error_cleanup(&error);
			PF_CORE_ASSERT(false);
		}

		return hasError;
	}

	void* UnBoxInternal(MonoObject* obj)
	{
		return mono_object_unbox(obj);
	}
	
}

