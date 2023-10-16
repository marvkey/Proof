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

namespace Proof
{
	
	struct ScriptRegistryData
	{
		// maybe change the string here to a hash number 
		std::unordered_map<std::string, ManagedClass> Classes;
		std::unordered_map<std::string, ScriptField> Fields;
		std::unordered_map<std::string, std::vector<ManagedMethod>> Methods;
		//app assembly
		std::unordered_map<std::string, Count<ScriptClass>> EntityScripts;
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
			return nullptr;

		return &s_ScriptRegistryData->Classes[className];
	}
	ManagedClass* ScriptRegistry::GetManagedClass(MonoClass* monoClass)
	{
		if (!IsInitialized())return nullptr;

		if (monoClass == nullptr)
			return nullptr;

		return GetManagedClassByName(ScriptUtils::ResolveMonoClassName(monoClass));
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
	bool ScriptRegistry::IsInitialized()
	{
		return s_ScriptRegistryData != nullptr;
	}
	void ScriptRegistry::GenerateRegistryForAppAssembly(Count<AssemblyInfo> assemblyInfo)
	{
		ScopeTimer scopeTimer(__FUNCTION__);

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
				//ScriptFieldType fieldType = ScriptUtils::GetFieldTypeFromMonoType(monoType);
				ScriptFieldType fieldType = ScriptUtils::GetFieldTypeFromMonoType(monoType);

				if (fieldType == ScriptFieldType::Void)
					continue;

				MonoCustomAttrInfo* attributes = mono_custom_attrs_from_field(currentClass, field);

				//uint32_t fieldID = Hash::GenerateFNVHash(fmt::format("{0}:{1}", managedClass.FullName, name));

				std::string realName = fmt::format("{0}:{1}", managedClass.FullName, name);
				int32_t typeEncoding = mono_type_get_type(monoType);

				//ScriptField& managedField = s_ScriptRegistryData->Fields[fieldID];
				ScriptField& managedField = s_ScriptRegistryData->Fields[realName];
				managedField.Name = name;
				//managedField.ID = fieldID;
				managedField.Type = fieldType;
				managedField.IsProperty = false;

				if (typeEncoding == MONO_TYPE_ARRAY || typeEncoding == MONO_TYPE_SZARRAY)
					managedField.Flags |= (uint64_t)FieldFlag::IsArray;

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
				#if 0
				if (attributes && mono_custom_attrs_has_attr(attributes, GetManagedClassByName("Proof.ShowInEditorAttribute")->Class))
				{
					managedField.Flags &= ~(uint64_t)FieldFlag::Protected;
					managedField.Flags &= ~(uint64_t)FieldFlag::Internal;
					managedField.Flags &= ~(uint64_t)FieldFlag::Private;
					managedField.Flags |= (uint64_t)FieldFlag::Public;

					MonoObject* attrib = mono_custom_attrs_get_attr(attributes, GetManagedClassByName("Proof.ShowInEditorAttribute")->Class);
					//ANT_TRY_GET_FIELD_VALUE(managedField.DisplayName, "Ant.ShowInEditorAttribute", "DisplayName", attrib);

					bool isReadOnly = false;
					//ANT_TRY_GET_FIELD_VALUE(isReadOnly, "Ant.ShowInEditorAttribute", "IsReadOnly", attrib);

					if (isReadOnly)
						managedField.Flags |= (uint64_t)FieldFlag::ReadOnly;
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
				#endif
				managedClass.Size += managedField.Size;

				//if (std::find(managedClass.Fields.begin(), managedClass.Fields.end(), fieldID) == managedClass.Fields.end())
				//	managedClass.Fields.push_back(managedField.ID);


				if (std::find(managedClass.Fields.begin(), managedClass.Fields.end(), realName) == managedClass.Fields.end())
					managedClass.Fields.push_back(realName);

				PF_ENGINE_TRACE("	Register Field {}", realName);

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
}

