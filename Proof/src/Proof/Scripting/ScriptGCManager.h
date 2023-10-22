#pragma once
#include "Proof/Scripting/ScriptRawTypes.h"
extern "C" {
	typedef struct _MonoObject MonoObject;
}

namespace Proof
{
	// script garabage collector manager
	class ScriptGCManager
	{
	public:
		static ScriptGCHandle CreateObjectReference(MonoObject* managedObject, bool weakReference, bool pinned = false, bool track = true);
		static bool IsHandleValid(ScriptGCHandle handle);
		static MonoObject* GetReferencedObject(ScriptGCHandle handle);
		static void ReleaseObjectReference(ScriptGCHandle handle);

	private:
		static void Init();
		static void Shutdown();
		static void CollectGarbage(bool blockUntilFinalized = true);
		
		friend class ScriptEngine;
	};
}