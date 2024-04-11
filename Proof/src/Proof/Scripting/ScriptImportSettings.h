#pragma once
#include "Proof/Core/Core.h"
#include<iostream>
#include <unordered_map>
namespace Proof
{
	struct ScriptImportClassMetaData
	{
		std::string className;
		std::unordered_map<std::string, Count<class FieldStorageBase>> Fields;
	};
	class ScriptImportSettings
	{
	public:
		static bool HasClass(std::string className) { return s_Classes.contains(className); }
		static const ScriptImportClassMetaData& GetClassMetaData(std::string className);
	private:
		static std::unordered_map<std::string, ScriptImportClassMetaData> s_Classes;
		friend class ScriptEngine;
		friend class ScriptImportSavedSettingSerializer;
		static void LoadAssembly();
		static void Init();
		// if we swap project it may still hold current project data and that could affect new project fields
		// just to be safe we will clear the map
		static void ShutDown();
	};
}