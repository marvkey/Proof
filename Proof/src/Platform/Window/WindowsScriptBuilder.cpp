#include "Proofprch.h"
#include "Proof/Scripting/ScriptBuilder.h"
#include <ShlObj.h>

namespace Proof
{
	void ScriptBuilder::BuildCSProject(const std::filesystem::path& filepath)
	{
		TCHAR programFilesFilePath[MAX_PATH];
		SHGetSpecialFolderPath(0, programFilesFilePath, CSIDL_PROGRAM_FILES, FALSE);
		std::filesystem::path msBuildPath = std::filesystem::path(programFilesFilePath) / "Microsoft Visual Studio" / "2022" / "Community" / "Msbuild" / "Current" / "Bin" / "MSBuild.exe";

		std::string command = fmt::format("cd \"{}\" && \"{}\" \"{}\" -property:Configuration=Debug", filepath.parent_path().string(), msBuildPath.string(), filepath.filename().string());
		system(command.c_str());
	}

	void ScriptBuilder::BuildCSProject(Count<Project> project)
	{
		auto projectAssemblyFile = std::filesystem::absolute(project->GetProjectDirectory().string()  + project->GetConfig().Name + ".csproj");
		BuildCSProject(projectAssemblyFile);
	}
	void ScriptBuilder::RegenerateProjectScriptSolution(const std::filesystem::path& projectPath)
	{
		std::string batchFilePath = projectPath.string();
		std::replace(batchFilePath.begin(), batchFilePath.end(), '/', '\\'); // Only windows
		batchFilePath += "\\Win-CreateScriptProjects.bat";
		system(batchFilePath.c_str());
	}
	void ScriptBuilder::RegenerateProjectScriptSolution(Count<class Project> project)
	{
		RegenerateProjectScriptSolution(project->GetProjectDirectory());
	}
}