#pragma once
#include "ScriptField.h"
#include <mono/utils/mono-error.h>
extern "C" {
	typedef struct _MonoType MonoType;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoException MonoException;
}

namespace Proof::ScriptUtils
{
	ScriptFieldType	GetFieldTypeFromMonoType(MonoType* monoType);
	std::string ResolveMonoClassName(MonoClass* monoClass);
	Buffer GetFieldValue(MonoObject* classInstance, const std::string& fieldName, ScriptFieldType fieldType, bool isProperty);
	MonoObject* GetFieldValueObject(MonoObject* classInstance, const std::string& fieldName, bool isProperty);
	Buffer MonoObjectToValue(MonoObject* obj, ScriptFieldType fieldType);
	void* UnBoxInternal(MonoObject* obj);

	template<typename TValueType>
	TValueType Unbox(MonoObject* obj) { return *(TValueType*)UnBoxInternal(obj); }
	template<typename TValueType>
	TValueType UnboxAddress(MonoObject* obj) { return (TValueType*)UnBoxInternal(obj); }

	MonoString* EmptyMonoString(bool appDomain = true);
	std::string MonoStringToUTF8(MonoString* monoString);
	MonoString* UTF8StringToMono(const std::string& str);

	bool CheckMonoError(MonoError& error);

}