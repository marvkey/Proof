#pragma once
#include "ScriptField.h"
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
}