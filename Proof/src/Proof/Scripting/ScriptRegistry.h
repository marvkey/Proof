#pragma once
#include "Proof/Core/Core.h"

extern "C" {
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoProperty MonoProperty;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoType MonoType;
}
namespace Proof
{

	struct ManagedClass;
	class ScriptRegistry
	{
	public:
		static void Init();
		static void ShutDown();
		static ManagedClass* GetManagedClassByName(const std::string& className);
		static ManagedClass* GetManagedClass(MonoClass* monoClass);
		static bool IsInitialized();
	private:
		static void RegisterClasss(std::string_view className, MonoClass* monoClass);
		static void RegisterClassMethods(Count<class AssemblyInfo> assemblyInfo, ManagedClass& managedClass);
		static void RegisterClassFields(Count<AssemblyInfo> assemblyInfo, ManagedClass& managedClass);
		static void RegisterClassProperties(Count<AssemblyInfo> assemblyInfo, ManagedClass& managedClass);

		static void RegisterCoreClasses();
	};
}