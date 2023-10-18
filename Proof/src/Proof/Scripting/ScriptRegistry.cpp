#include "Proofprch.h"
#include "ScriptRegistry.h"
#include "ScriptTypes.h"
#include "ScriptField.h"
#include "ScriptUtils.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include "mono/metadata/attrdefs.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>

#define PF_CACHED_CLASS_RAW(clazz) ScriptRegistry::GetManagedClassByName(clazz)->Class
#define PF_REGISTERED_CLASS(clazz) ScriptRegistry::GetManagedClassByName(clazz)
//dont use for strings
#define PF_TRY_GET_ATTRIBUTE(refValue,className,fieldName,attributeObject)\
{\
	 MonoClass* classType = ScriptRegistry::GetManagedClassByName(className)->Class; \
    if (classType) { \
        MonoClassField* field = mono_class_get_field_from_name(classType, fieldName); \
        if (field) { \
            mono_field_get_value(attributeObject, field, &refValue); \
        } else { \
            PF_ENGINE_ERROR("AttributeClass: {} Does not have Field: {}",className,fieldName);\
			PF_CORE_ASSERT(false);\
        } \
    } else { \
        PF_ENGINE_ERROR("Script Registry does not contain AttributeClass: {} ",className);\
		PF_CORE_ASSERT(false);\
    } \
}\

#define PF_TRY_GET_ATTRIBUTE_STRING(refValue,className,fieldName,attributeObject)\
{\
	 MonoClass* classType = ScriptRegistry::GetManagedClassByName(className)->Class; \
    if (classType) { \
        MonoClassField* field = mono_class_get_field_from_name(classType, fieldName); \
        if (field) { \
            refValue = ScriptUtils::MonoStringToUTF8( (MonoString*)mono_field_get_value_object(ScriptEngine::GetAppDomain(), field, attrib));\
        } else { \
            PF_ENGINE_ERROR("AttributeClass: {} Does not have Field: {}",className,fieldName);\
			PF_CORE_ASSERT(false);\
        } \
    } else { \
        PF_ENGINE_ERROR("Script Registry does not contain AttributeClass: {} ",className);\
		PF_CORE_ASSERT(false);\
    } \
}\

namespace Proof
{
	
	struct ScriptRegistryData
	{
		// maybe change the string here to a hash number 
		std::unordered_map<std::string, ManagedClass> Classes;
		std::unordered_map<std::string, ScriptField> Fields;
		// meathod name and its overlaod
		std::unordered_map<std::string, std::vector<ManagedMethod>> Methods;
		//app assembly
		std::unordered_map<std::string, Count<ScriptClass>> EntityScripts;

		std::unordered_map<std::string, ManageEnumClass> EnumClasses;
	};
	ScriptRegistryData* s_ScriptRegistryData = nullptr;
	void ScriptRegistry::Init()
	{
		ScopeTimer scopeTime("ScriptRegistry::Init");
		PF_CORE_ASSERT(!s_ScriptRegistryData, "Trying to initialize ScriptCache multiple times!");

		s_ScriptRegistryData = new ScriptRegistryData();

		RegisterCoreClasses();
	}
	void ScriptRegistry::ShutDown()
	{
		delete s_ScriptRegistryData;
	}
	ManagedClass* ScriptRegistry::GetManagedClassByName(const std::string& className)
	{
		if (!IsInitialized())
			return nullptr;

		//uint32_t classID = Hash::GenerateFNVHash(className);
		if (s_ScriptRegistryData->Classes.find(className) == s_ScriptRegistryData->Classes.end())
		{
			PF_ENGINE_ERROR("Script Registry does not contain {}", className);
			return nullptr;
		}
		return &s_ScriptRegistryData->Classes[className];
	}
	ManagedClass* ScriptRegistry::GetManagedClass(MonoClass* monoClass)
	{
		if (!IsInitialized())return nullptr;

		if (monoClass == nullptr)
			return nullptr;

		return GetManagedClassByName(ScriptUtils::ResolveMonoClassName(monoClass));
	}
	ManageEnumClass* ScriptRegistry::GetManagedEnumClassByName(const std::string& className)
	{
		if (s_ScriptRegistryData->EnumClasses.contains(className))
			return &s_ScriptRegistryData->EnumClasses.at(className);
		return nullptr;
	}
	ManagedClass* ScriptRegistry::GetMonoObjectClass(MonoObject* monoObject)
	{
		PF_PROFILE_FUNC();

		if (!IsInitialized())
			return nullptr;

		MonoClass* objectClass = mono_object_get_class(monoObject);
		if (objectClass == nullptr)
			return nullptr;

		return GetManagedClassByName(ScriptUtils::ResolveMonoClassName(objectClass));
	}
	MonoClass* ScriptRegistry::GetFieldTypeClass(ScriptFieldType fieldType)
	{
		switch (fieldType)
		{
			case ScriptFieldType::Bool: return PF_REGISTERED_CLASS("System.Bool")->Class;
			case ScriptFieldType::Int8: return PF_REGISTERED_CLASS("System.SByte")->Class;
			case ScriptFieldType::Int16: return PF_REGISTERED_CLASS("System.Int16")->Class;
			case ScriptFieldType::Int32: return PF_REGISTERED_CLASS("System.Int32")->Class;
			case ScriptFieldType::Int64: return PF_REGISTERED_CLASS("System.Int64")->Class;
			case ScriptFieldType::UInt8: return PF_REGISTERED_CLASS("System.Byte")->Class;
			case ScriptFieldType::UInt16: return PF_REGISTERED_CLASS("System.UInt16")->Class;
			case ScriptFieldType::UInt32: return PF_REGISTERED_CLASS("System.UInt32")->Class;
			case ScriptFieldType::UInt64: return PF_REGISTERED_CLASS("System.UInt64")->Class;
			case ScriptFieldType::Float: return PF_REGISTERED_CLASS("System.Single")->Class;
			case ScriptFieldType::Double: return PF_REGISTERED_CLASS("System.Double")->Class;
			case ScriptFieldType::String: return PF_REGISTERED_CLASS("System.String")->Class;
			case ScriptFieldType::Vector2: return PF_REGISTERED_CLASS("PF.Vector2")->Class;
			case ScriptFieldType::Vector3: return PF_REGISTERED_CLASS("PF.Vector3")->Class;
			case ScriptFieldType::Vector4: return PF_REGISTERED_CLASS("PF.Vector4")->Class;
			case ScriptFieldType::AssetID: return PF_REGISTERED_CLASS("PF.AssetID")->Class;
			case ScriptFieldType::Prefab: return PF_REGISTERED_CLASS("PF.Prefab")->Class;
			case ScriptFieldType::Entity: return PF_REGISTERED_CLASS("PF.Entity")->Class;
			case ScriptFieldType::Mesh: return PF_REGISTERED_CLASS("PF.Mesh")->Class;
			case ScriptFieldType::DynamicMesh: return PF_REGISTERED_CLASS("PF.DynamicMesh")->Class;
			case ScriptFieldType::Material: return PF_REGISTERED_CLASS("PF.Material")->Class;
			case ScriptFieldType::PhysicsMaterial: return PF_REGISTERED_CLASS("PF.PhysicsMaterial")->Class;
			case ScriptFieldType::Texture2D: return PF_REGISTERED_CLASS("PF.Texture2D")->Class;
		}
		PF_ENGINE_ERROR("NOt supported type {}", EnumReflection::EnumString(fieldType));
		PF_CORE_ASSERT(false);
		return nullptr;
	}
	ScriptField* ScriptRegistry::GetFieldByName(const std::string& fieldName)
	{
		if (s_ScriptRegistryData == nullptr)
			return nullptr;

		if (s_ScriptRegistryData->Fields.find(fieldName) == s_ScriptRegistryData->Fields.end())
			return nullptr;
		return &s_ScriptRegistryData->Fields.at(fieldName);
	}
	bool ScriptRegistry::IsInitialized()
	{
		return s_ScriptRegistryData != nullptr;
	}
	void ScriptRegistry::GenerateRegistryForAppAssembly(Count<AssemblyInfo> assemblyInfo)
	{
		ScopeTimer scopeTimer(__FUNCTION__);

		RegisterEnums(assemblyInfo);

		const MonoTableInfo* tableInfo = mono_image_get_table_info(assemblyInfo->AssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t tableRowCount = mono_table_info_get_rows(tableInfo);
		for (int32_t i = 1; i < tableRowCount; i++)
		{
			MonoClass* monoClass = mono_class_get(assemblyInfo->AssemblyImage, (i + 1) | MONO_TOKEN_TYPE_DEF);
			BuildClassMetadata(assemblyInfo, monoClass);
		}

		// Process fields and properties after all classes have been parsed.
		for (auto classID : assemblyInfo->Classes)
		{
			ManagedClass& managedClass = s_ScriptRegistryData->Classes.at(classID);

			RegisterClassMethods(assemblyInfo, managedClass);

			MonoObject* tempInstance = ScriptEngine::CreateManagedObject_Internal(&managedClass,true);
			if (tempInstance == nullptr)
				continue;

			RegisterClassFields(assemblyInfo, managedClass);
			RegisterClassProperties(assemblyInfo, managedClass);

			if (mono_class_is_subclass_of(managedClass.Class, PF_CACHED_CLASS_RAW("Proof.Entity"), false))
			{
				//AssetID handle = AssetManager::CreateMemoryOnlyAssetWithHandle<ScriptAsset>(Hash::GenerateFNVHash(managedClass.FullName), classID);


				s_ScriptRegistryData->EntityScripts[managedClass.FullName] = Count<ScriptClass>::Create(managedClass.FullName);
			}
		}
		
		for (auto className : assemblyInfo->Classes)
		{
			ManagedClass& managedClass = s_ScriptRegistryData->Classes.at(className);

			if (!mono_class_is_subclass_of(managedClass.Class, PF_CACHED_CLASS_RAW("Proof.Entity"), false))
				continue;

			MonoObject* tempInstance = ScriptEngine::CreateManagedObject_Internal(&managedClass,true);

			for (auto fieldName : managedClass.Fields)
			{
				ScriptField& fieldInfo = s_ScriptRegistryData->Fields.at(fieldName);
				if (!fieldInfo.IsArray())
				{
					fieldInfo.DefaultValueBuffer = ScriptUtils::GetFieldValue(tempInstance, fieldInfo.Name, fieldInfo.Type, fieldInfo.IsProperty);
				}
				else
				{
					MonoArray* arr = (MonoArray*)ScriptUtils::GetFieldValueObject(tempInstance, fieldInfo.Name, fieldInfo.IsProperty);

					if (arr != nullptr)
					{
						fieldInfo.DefaultValueBuffer.Allocate(mono_array_length(arr) * fieldInfo.Size);
						fieldInfo.DefaultValueBuffer.ZeroInitialize();
					}
				}
			}
		}
	}
	static void ReflectEnumFields(const std::string& enumName, MonoClass* enumClass, Count<AssemblyInfo> assemblyInfo)
	{
		MonoClassField* classFields;
		void* fieldsIter = nullptr;
		int index = 0;
		while (classFields = mono_class_get_fields(enumClass, &fieldsIter))
		{
			// the first field is some wierd "value_" probably a built in variable for the enums
			if (index == 0)
			{
				index++;
				continue;
			}
			const std::string fieldName = mono_field_get_name(classFields);
			
			auto& enumField = s_ScriptRegistryData->EnumClasses[enumName].EnumFields.emplace_back();
			enumField.Name = fieldName;
			
			uint8_t value[16];
			mono_field_static_get_value(mono_class_vtable(ScriptEngine::GetAppDomain(), enumClass), classFields, value);

			Buffer buffer(value, sizeof(value) , true);
			enumField.SetValueBuffer(buffer);
			buffer.Release();
			std::string data = mono_field_get_data(classFields);
			PF_ENGINE_TRACE("		{} ", fieldName);

		}
	}
	void ScriptRegistry::RegisterEnums(Count<AssemblyInfo> assemblyInfo)
	{
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(assemblyInfo->AssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
			const char* nameSpace = mono_metadata_string_heap(assemblyInfo->AssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(assemblyInfo->AssemblyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(assemblyInfo->AssemblyImage, nameSpace, className);

			if (monoClass == nullptr)
				continue;

			if (!mono_class_is_enum(monoClass))
				continue;
			auto& managedEnumClass = s_ScriptRegistryData->EnumClasses[fullName];

			MonoType* enumType = mono_class_enum_basetype(monoClass);
			managedEnumClass.Class = monoClass;
			managedEnumClass.FullName = fullName;
			//managedEnumClass.

			auto type = ScriptUtils::GetFieldTypeFromMonoType(enumType);
			PF_ENGINE_INFO("	Registering Enums Fields{} Type: {}", fullName,EnumReflection::EnumString( type));
			ReflectEnumFields(fullName, monoClass, assemblyInfo);
		}
	}
	ManagedMethod* ScriptRegistry::GetSpecificManagedMethod(ManagedClass* managedClass, const std::string& name, uint32_t parameterCount, bool ignoreParent)
	{
		PF_PROFILE_FUNC();

		if (!IsInitialized())
			return nullptr;

		if (managedClass == nullptr)
		{
			PF_ENGINE_ERROR("ScriptEngine Attempting to get method {0} from a nullptr class!", name);
			return nullptr;
		}

		ManagedMethod* method = nullptr;

		//uint32_t methodID = Hash::GenerateFNVHash(managedClass->FullName + ":" + name);
		std::string methodName = managedClass->FullName + ":" + name;
		if (s_ScriptRegistryData->Methods.find(methodName) != s_ScriptRegistryData->Methods.end())
		{
			for (auto& methodCandiate : s_ScriptRegistryData->Methods.at(methodName))
			{
				if (methodCandiate.ParameterCount == parameterCount)
				{
					method = &methodCandiate;
					break;
				}
			}
		}

		//if (method == nullptr && !ignoreParent && managedClass->ParentID != 0)
		//	method = GetSpecificManagedMethod(&s_Cache->Classes.at(managedClass->ParentID), name, parameterCount);

		if (method == nullptr && !ignoreParent && managedClass->ParentName.size() > 0)
			method = GetSpecificManagedMethod(&s_ScriptRegistryData->Classes.at(managedClass->ParentName), name, parameterCount);
		if (method == nullptr)
			PF_ENGINE_WARN("ScriptEngine Failed to find method with name: {0} and parameter count: {1} in class {2}", name, parameterCount, managedClass->FullName);

		return method;
	}
	const std::unordered_map<std::string, Count<ScriptClass>>& ScriptRegistry::GetEntityScripts()
	{
		if (s_ScriptRegistryData == nullptr)
			return {};
		return s_ScriptRegistryData->EntityScripts;
	}
	void ScriptRegistry::RegisterClasss(std::string_view className, MonoClass* monoClass)
	{
		MonoType* classType = mono_class_get_type(monoClass);
		ManagedClass managedClass;
		managedClass.FullName = className;
		//managedClass.ID = Hash::GenerateFNVHash(managedClass.FullName);

		int alignment = 0;
		managedClass.Size = mono_type_size(classType, &alignment);
		managedClass.Class = monoClass;
		s_ScriptRegistryData->Classes[managedClass.FullName] = managedClass;

		PF_ENGINE_INFO("Registering Class {}", managedClass.FullName);
		if (managedClass.FullName.find("Proof.") != std::string::npos)
		{
			Count<AssemblyInfo> coreAssembly = ScriptEngine::GetCoreAssemblyInfo();
			RegisterClassMethods(coreAssembly, managedClass);
			RegisterClassFields(coreAssembly, managedClass);
			RegisterClassProperties(coreAssembly, managedClass);
		}
	}
	void ScriptRegistry::RegisterClassMethods(Count<class AssemblyInfo> assemblyInfo, ManagedClass& managedClass)
	{
		MonoMethod* monoMethod = nullptr;
		void* methodPtr = 0;
		while ((monoMethod = mono_class_get_methods(managedClass.Class, &methodPtr)) != NULL)
		{
			MonoMethodSignature* sig = mono_method_signature(monoMethod);

			uint32_t flags = mono_method_get_flags(monoMethod, nullptr);
			char* fullName = mono_method_full_name(monoMethod, FALSE);

			ManagedMethod method;
			//method.ID = Hash::GenerateFNVHash(fullName);
			method.FullName = fullName;
			method.IsStatic = flags & MONO_METHOD_ATTR_STATIC;
			method.IsVirtual = flags & MONO_METHOD_ATTR_VIRTUAL;
			method.ParameterCount = mono_signature_get_param_count(sig);
			method.Method = monoMethod;

			// NOTE: We can enable this if we want information about a methods parameters later
			/*if (method.ParameterCount > 0)
			{
				const char** parameterNames = new const char*[method.ParameterCount];
				mono_method_get_param_names(monoMethod, parameterNames);

				MonoType* parameterType = nullptr;
				void* parameterIter = 0;
				uint32_t parameterIndex = 0;
				while ((parameterType = mono_signature_get_params(sig, &parameterIter)) != NULL)
				{
					const char* name = parameterNames[parameterIndex];

					FieldInfo parameter;
					parameter.ID = Hash::GenerateFNVHash(name);
					parameter.FullName = name;
					parameter.Type = ManagedType::FromType(parameterType);
					parameter.Attributes = 0;

					int alignment = 0;
					parameter.Size = mono_type_size(parameterType, &alignment);

					method.Parameters[parameter.ID] = parameter;

					parameterIndex++;
				}

				delete[] parameterNames;
			}*/

			s_ScriptRegistryData->Methods[method.FullName].push_back(method);
			managedClass.Methods.push_back(method.FullName);
			PF_ENGINE_TRACE("	Register Meathod {}", method.FullName);

			mono_free(fullName);
		}
	}
	static bool HasAttribute(MonoCustomAttrInfo* attributes, const char* className) {
		ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(className);
		if (managedClass)
		{
			return mono_custom_attrs_has_attr(attributes, managedClass->Class);
		}
		else
		{
			PF_ENGINE_ERROR("Script Registry does not contain AttributeClass: {}", className);
			PF_CORE_ASSERT(false);
			return false;
		}
	}
	void ScriptRegistry::RegisterClassFields(Count<AssemblyInfo> assemblyInfo, ManagedClass& managedClass)
	{
		MonoClass* currentClass = managedClass.Class;
		while (currentClass != nullptr)
		{
			std::string className = mono_class_get_name(currentClass);
			std::string classNameSpace = mono_class_get_namespace(currentClass);

			if (classNameSpace.find("Proof") != std::string::npos && className.find("Entity") != std::string::npos)
			{
				currentClass = nullptr;
				continue;
			}

			MonoClassField* field = nullptr;
			void* fieldPtr = 0;
			while ((field = mono_class_get_fields(currentClass, &fieldPtr)) != NULL)
			{
				std::string name = mono_field_get_name(field);

				// Properties have a backing field called <PropertyName>k__BackingField. We don't want to include those in the class fields list.
				if (name.find("k__BackingField") != std::string::npos)
					continue;
				MonoType* monoType = mono_field_get_type(field);
				ScriptFieldType fieldType = ScriptUtils::GetFieldTypeFromMonoType(monoType);

				if (fieldType == ScriptFieldType::Void)
					continue;


				//uint32_t fieldID = Hash::GenerateFNVHash(fmt::format("{0}:{1}", managedClass.FullName, name));

				std::string realName = fmt::format("{0}.{1}", managedClass.FullName, name);
				int32_t typeEncoding = mono_type_get_type(monoType);

				//ScriptField& managedField = s_ScriptRegistryData->Fields[fieldID];
				ScriptField& managedField = s_ScriptRegistryData->Fields[realName];
				managedField.Name = name;
				//managedField.ID = fieldID;
				managedField.Type = fieldType;
				managedField.IsProperty = false;

				if (typeEncoding == MONO_TYPE_ARRAY || typeEncoding == MONO_TYPE_SZARRAY)
					managedField.Flags |= (uint64_t)FieldFlag::IsArray;

				if (typeEncoding == MONO_TYPE_VALUETYPE)
				{
					if (mono_class_is_enum(mono_type_get_class(monoType)))
					{
						managedField.Flags |= (uint64_t)FieldFlag::IsEnum;
					}
				}
				if (typeEncoding == MONO_TYPE_ENUM)
					managedField.Flags |= (uint64_t)FieldFlag::IsEnum;


				uint32_t visibility = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;
				switch (visibility)
				{
					case MONO_FIELD_ATTR_PUBLIC:
						{
							managedField.Flags &= ~(uint64_t)FieldFlag::Protected;
							managedField.Flags &= ~(uint64_t)FieldFlag::Private;
							managedField.Flags &= ~(uint64_t)FieldFlag::Internal;
							managedField.Flags |= (uint64_t)FieldFlag::Public;
							break;
						}
					case MONO_FIELD_ATTR_FAMILY:
						{
							managedField.Flags &= ~(uint64_t)FieldFlag::Public;
							managedField.Flags &= ~(uint64_t)FieldFlag::Private;
							managedField.Flags &= ~(uint64_t)FieldFlag::Internal;
							managedField.Flags |= (uint64_t)FieldFlag::Protected;
							break;
						}
					case MONO_FIELD_ATTR_ASSEMBLY:
						{
							managedField.Flags &= ~(uint64_t)FieldFlag::Public;
							managedField.Flags &= ~(uint64_t)FieldFlag::Protected;
							managedField.Flags &= ~(uint64_t)FieldFlag::Private;
							managedField.Flags |= (uint64_t)FieldFlag::Internal;
							break;
						}
					case MONO_FIELD_ATTR_PRIVATE:
						{
							managedField.Flags &= ~(uint64_t)FieldFlag::Public;
							managedField.Flags &= ~(uint64_t)FieldFlag::Protected;
							managedField.Flags &= ~(uint64_t)FieldFlag::Internal;
							managedField.Flags |= (uint64_t)FieldFlag::Private;
							break;
						}
				}


				MonoCustomAttrInfo* attributes = mono_custom_attrs_from_field(currentClass, field);

				if (attributes)
				{
					if (HasAttribute(attributes, "Proof.ShowInEditorAttribute"))
					{
						managedField.Flags &= ~(uint64_t)FieldFlag::Protected;
						managedField.Flags &= ~(uint64_t)FieldFlag::Internal;
						managedField.Flags &= ~(uint64_t)FieldFlag::Private;
						managedField.Flags |= (uint64_t)FieldFlag::Public;
						MonoObject* attrib = mono_custom_attrs_get_attr(attributes, GetManagedClassByName("Proof.ShowInEditorAttribute")->Class);
						#if 0
						MonoClassField* field = mono_class_get_field_from_name(GetManagedClassByName("Proof.ShowInEditorAttribute")->Class, "DisplayName");
						if (field)
						{
							managedField.DisplayName = ScriptUtils::MonoStringToUTF8( (MonoString*)mono_field_get_value_object(ScriptEngine::GetAppDomain(), field, attrib));
						}
						#endif

						PF_TRY_GET_ATTRIBUTE_STRING(managedField.DisplayName, "Proof.ShowInEditorAttribute", "DisplayName", attrib);

						bool isReadOnly = false;
						PF_TRY_GET_ATTRIBUTE(isReadOnly, "Proof.ShowInEditorAttribute", "IsReadOnly", attrib);

						if (isReadOnly == true)
							managedField.Flags |= (uint64_t)FieldFlag::ReadOnly;
					}

					if (HasAttribute(attributes, "Proof.ClampValueAttribute"))
					{
						MonoObject* attrib = mono_custom_attrs_get_attr(attributes, GetManagedClassByName("Proof.ClampValueAttribute")->Class);

						managedField.FieldRangeAttribute.RangeFlags |= ScriptFieldRangeAttribute::RangeSet::All;
						PF_TRY_GET_ATTRIBUTE(managedField.FieldRangeAttribute.MinValue, "Proof.ClampValueAttribute", "Min", attrib);
						PF_TRY_GET_ATTRIBUTE(managedField.FieldRangeAttribute.MaxValue, "Proof.ClampValueAttribute", "Max", attrib);
					}

					if (HasAttribute(attributes, "Proof.MaxAttribute"))
					{
						MonoObject* attrib = mono_custom_attrs_get_attr(attributes, GetManagedClassByName("Proof.MaxAttribute")->Class);
						managedField.FieldRangeAttribute.RangeFlags |= ScriptFieldRangeAttribute::RangeSet::Max;
						PF_TRY_GET_ATTRIBUTE(managedField.FieldRangeAttribute.MaxValue, "Proof.MaxAttribute", "Max", attrib);
					}

					if (HasAttribute(attributes, "Proof.MinAttribute"))
					{
						MonoObject* attrib = mono_custom_attrs_get_attr(attributes, GetManagedClassByName("Proof.MinAttribute")->Class);
						managedField.FieldRangeAttribute.RangeFlags |= ScriptFieldRangeAttribute::RangeSet::Min;
						PF_TRY_GET_ATTRIBUTE(managedField.FieldRangeAttribute.MaxValue, "Proof.MinAttribute", "Min", attrib);
					}

					if (HasAttribute(attributes, "Proof.ToolTipAttribute"))
					{
						MonoObject* attrib = mono_custom_attrs_get_attr(attributes, GetManagedClassByName("Proof.ToolTipAttribute")->Class);
						PF_TRY_GET_ATTRIBUTE_STRING(managedField.ToolTip, "Proof.ToolTipAttribute", "ToolTip", attrib);
					}
				}

				if (managedField.IsArray())
				{
					MonoClass* fieldArrayClass = mono_type_get_class(monoType);
					MonoClass* elementClass = mono_class_get_element_class(fieldArrayClass);
					MonoType* elementType = mono_class_get_type(elementClass);

					int align;
					managedField.Size = mono_type_size(elementType, &align);
				}
				else
				{
					int align;
					managedField.Size = mono_type_size(monoType, &align);
				}
				managedClass.Size += managedField.Size;

				//if (std::find(managedClass.Fields.begin(), managedClass.Fields.end(), fieldID) == managedClass.Fields.end())
				//	managedClass.Fields.push_back(managedField.ID);

				//registyr name
				{
					MonoClass* fieldClass = mono_type_get_class(monoType);
					if (fieldClass != nullptr)
					{
						const char* name = mono_class_get_name(fieldClass);
						const char* nameSpace = mono_class_get_namespace(fieldClass);

						managedField.RegistryClassName = fmt::format("{}.{}", nameSpace, name);
					}
					
					
				}
				if (std::find(managedClass.Fields.begin(), managedClass.Fields.end(), realName) == managedClass.Fields.end())
					managedClass.Fields.push_back(realName);

				if(managedField.IsEnum())
					PF_ENGINE_TRACE("	Register Field {} Enum Type:{}, RawType: {}", realName, managedField.RegistryClassName, EnumReflection::EnumString(fieldType));
				else if(managedField.IsArray())
					PF_ENGINE_TRACE("	Register Field {} Array Type: {}", realName, EnumReflection::EnumString(fieldType));
				else 
					PF_ENGINE_TRACE("	Register Field {} Type: {}", realName, EnumReflection::EnumString(fieldType));

			}

			currentClass = mono_class_get_parent(currentClass);
		}
	}
	void ScriptRegistry::RegisterClassProperties(Count<AssemblyInfo> assemblyInfo, ManagedClass& managedClass)
	{
	}
	

	void ScriptRegistry::RegisterCoreClasses()
	{
		#define REGISTER_CORELIB_CLASS(name) RegisterClasss("System." ##name, mono_class_from_name(mono_get_corlib(), "System", name))

		REGISTER_CORELIB_CLASS("Object");
		REGISTER_CORELIB_CLASS("ValueType");
		REGISTER_CORELIB_CLASS("Boolean");
		REGISTER_CORELIB_CLASS("SByte");
		REGISTER_CORELIB_CLASS("Int16");
		REGISTER_CORELIB_CLASS("Int32");
		REGISTER_CORELIB_CLASS("Int64");
		REGISTER_CORELIB_CLASS("Byte");
		REGISTER_CORELIB_CLASS("UInt16");
		REGISTER_CORELIB_CLASS("UInt32");
		REGISTER_CORELIB_CLASS("UInt64");
		REGISTER_CORELIB_CLASS("Single");
		REGISTER_CORELIB_CLASS("Double");
		REGISTER_CORELIB_CLASS("Char");
		REGISTER_CORELIB_CLASS("String");



		PF_ENGINE_TRACE("Registering Script Core classes");
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(ScriptEngine::GetCoreAssemblyInfo()->AssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
			const char* nameSpace = mono_metadata_string_heap(ScriptEngine::GetCoreAssemblyInfo()->AssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(ScriptEngine::GetCoreAssemblyInfo()->AssemblyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(ScriptEngine::GetCoreAssemblyInfo()->AssemblyImage, nameSpace, className);

			if (monoClass == nullptr)
				continue;

			if (mono_class_is_enum(monoClass))
				continue;

			RegisterClasss(fullName, monoClass);
		}

		RegisterEnums(ScriptEngine::GetCoreAssemblyInfo());
	}
	void ScriptRegistry::BuildClassMetadata(Count<AssemblyInfo>& assemblyInfo, MonoClass* monoClass)
	{
		PF_CORE_ASSERT(monoClass);

		const std::string fullName = ScriptUtils::ResolveMonoClassName(monoClass);

		// C# adds a .<PrivateImplementationDetails> class for some reason?
		if (fullName.find("<PrivateImpl") != std::string::npos)
			return;

		//uint32_t classID = Hash::GenerateFNVHash(fullName);
		ManagedClass& managedClass = s_ScriptRegistryData->Classes[fullName];
		managedClass.FullName = fullName;
		//managedClass.ID = classID;
		managedClass.Class = monoClass;
		uint32_t classFlags = mono_class_get_flags(monoClass);
		managedClass.IsAbstract = classFlags & MONO_TYPE_ATTR_ABSTRACT;
		managedClass.IsStruct = mono_class_is_valuetype(monoClass);

		MonoClass* parentClass = mono_class_get_parent(monoClass);
		if (parentClass != nullptr && parentClass != PF_CACHED_CLASS_RAW("System.Object"))
		{
			std::string parentName = ScriptUtils::ResolveMonoClassName(parentClass);
			//managedClass.ParentID = Hash::GenerateFNVHash(parentName);
			managedClass.ParentName = parentName;
		}

		//assemblyInfo->Classes.push_back(managedClass.ID);
		assemblyInfo->Classes.push_back(managedClass.FullName);
	}
	const std::string ScriptClass::GetName() const
	{
		const auto first = m_FullName.find_first_of(':');
		if (first == std::string_view::npos)
			return m_FullName;

		return m_FullName.substr(first + 1);
	}
	const std::string ScriptClass::GetNameSpace() const
	{
		const auto first = m_FullName.find_first_of(':');
		if (first == std::string_view::npos)
			return "";

		return m_FullName.substr(0, first);
	}
}

