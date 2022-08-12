#pragma once
#include "Proof/Core/Core.h"
extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}
namespace Proof
{
	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();
	private:
		static void InitMono();
	};
}
