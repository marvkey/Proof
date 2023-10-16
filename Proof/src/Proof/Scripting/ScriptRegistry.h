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
	class AssemblyInfo;

	class ScriptClass : public RefCounted
	{
	public:
		ScriptClass() = default;
		ScriptClass(std::string fullName) :
			m_FullName(fullName)
		{

		}
		//ScriptAsset(uint32_t classID)
		//	: m_ClassID(classID) {}

		//uint32_t GetClassID() const { return m_ClassID; }

		const std::string& GetFullName() const { return m_FullName; }
	private:
		//uint32_t m_ClassID = 0;

		std::string m_FullName = "";
	};
	struct ManagedClass;
	struct ManagedMethod;
	class ScriptRegistry
	{
	public:
		static void Init();
		static void ShutDown();
		static ManagedClass* GetManagedClassByName(const std::string& className);
		static ManagedClass* GetManagedClass(MonoClass* monoClass);
		static ManagedClass* GetMonoObjectClass(MonoObject* monoObject);

		static bool IsInitialized();

		static void GenerateRegistryForAppAssembly(Count<AssemblyInfo> assemblyInfo);

		static ManagedMethod* GetSpecificManagedMethod(ManagedClass* managedClass, const std::string& name, uint32_t parameterCount, bool ignoreParent = false);

	private:
		static void RegisterClasss(std::string_view className, MonoClass* monoClass);
		static void RegisterClassMethods(Count<class AssemblyInfo> assemblyInfo, ManagedClass& managedClass);
		static void RegisterClassFields(Count<AssemblyInfo> assemblyInfo, ManagedClass& managedClass);
		static void RegisterClassProperties(Count<AssemblyInfo> assemblyInfo, ManagedClass& managedClass);

		static void RegisterCoreClasses();
		static void BuildClassMetadata(Count<AssemblyInfo>& assemblyInfo, MonoClass* monoClass);
	};
}