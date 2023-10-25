#pragma once
#include "Proof/Project/Project.h"

namespace Proof
{
	class ScriptBuilder
	{
	public:
		static void BuildCSProject(const std::filesystem::path& filepath);
		static void BuildCSProject(Count<class Project> project);
		static void RegenerateProjectScriptSolution(const std::filesystem::path& projectDirectory, const std::string& projectName);
		static void RegenerateProjectScriptSolution(Count<class Project> project);
	};
}