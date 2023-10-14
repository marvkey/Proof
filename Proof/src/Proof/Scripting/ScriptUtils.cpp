#include "Proofprch.h"
#include "ScriptUtils.h"
#include "ScriptRegistry.h"
#include "ScriptField.h"
#include "ScriptTypes.h"
#include "Proof/Utils/StringUtils.h"

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
			case MONO_TYPE_VALUETYPE:
				{
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
	
}

