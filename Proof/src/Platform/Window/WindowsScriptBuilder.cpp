#include "Proofprch.h"
#include "Proof/Scripting/ScriptBuilder.h"
#include <ShlObj.h>
#include <map>
#include "Proof/Utils/StringUtils.h"
namespace Proof
{
	static void GenerateBatchFile(const std::filesystem::path& path)
	{
		const std::string templateString = R"(
@echo off
pushd %~dp0
call %PROOF_DIR%\vendor\premake\premake5.exe vs2022

REM Check the error level
if %errorlevel% neq 0 (
    echo An error occurred.
    pause
)

popd

		)";
		std::ofstream outputFile(path.string());
		if (outputFile.is_open())
		{
			outputFile << templateString;
			outputFile.close();
			PF_ENGINE_INFO("Premake Flle {}", path.string());
		}
		else
		{
			PF_ENGINE_ERROR("Failed to crate premake file {}", path.string());
		}
	}
	static void GeneratePremakeFile(const std::filesystem::path& projectDir,const std::string& projectName)
	{
		const std::string templateString = 
R"(FileVersion = 1.2
require "vstudio"

--just need t work//https://github.com/premake/premake-core/issues/1963
function platformsElement(cfg)
	_p(2,'<Platforms>x64</Platforms>')
end

premake.override(premake.vstudio.cs2005.elements, "projectProperties", function (oldfn, cfg)
	return table.join(oldfn(cfg), {
	platformsElement,
	})
end)

ProofRoootDirectory = os.getenv("PROOF_DIR")
include (path.join(ProofRoootDirectory, "Proof-Editor", "Resources", "LUA", "Proof.lua"))
workspace "$PROJECT_NAME$"
	targetdir "build"
	startproject "$PROJECT_NAME$"
	
	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}

group "Proof"
project "ProofScriptCore"
	location "%{ProofRoootDirectory}/ProofScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{ProofRoootDirectory}/Proof-Editor/Resources/Scripts/Binaries/ScriptCore")
	objdir ("%{ProofRoootDirectory}/Proof-Editor/Resources/Scripts/Intermediates/ScriptCore")

	files
	{
		"%{ProofRoootDirectory}/ProofScriptCore/Src/**.cs",
		--"%{ProofRoootDirectory}/ProofScriptCore/Properties/**.cs",
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group ""

project "$PROJECT_NAME$"
	location "Source/ScriptApp"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetname "$PROJECT_NAME$"
	targetdir ("%{wks.location}/Binaries/ScriptApp")
	objdir ("%{wks.location}/Intermediates/ScriptApp")

	files 
	{
		"Assets/**.cs", 
	}
	links
	{
		"ProofScriptCore"
	}
	linkAppReferences()

	print("\nDone linking app references")    
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
)";
		std::map<std::string, std::string> replacements = { {"$PROJECT_NAME$", projectName} };
		auto newString = Utils::String::ReplaceInString(templateString, replacements);

		std::string filePath = projectDir.string() + "/premake5.lua";
		std::ofstream outputFile(filePath);
		if (outputFile.is_open())
		{
			outputFile << newString;
			outputFile.close();
			PF_ENGINE_INFO("Premake Flle {}", filePath);
		}
		else
		{
			PF_ENGINE_ERROR("Failed to crate premake file {}", filePath);
		}
	}
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
		auto projectAssemblyFile = std::filesystem::absolute(project->GetProjectDirectory().string() + project->GetConfig().Name + ".csproj");
		BuildCSProject(projectAssemblyFile);
	}
	void ScriptBuilder::RegenerateProjectScriptSolution(const std::filesystem::path& projectDir, const std::string& projectName)
	{
		GeneratePremakeFile(projectDir, projectName);
		std::string batchFilePath = projectDir.string();
		std::replace(batchFilePath.begin(), batchFilePath.end(), '/', '\\'); // Only windows
		batchFilePath += "\\Win-CreateScriptProjects.bat";
		if (!FileSystem::Exists(batchFilePath))
		{
			GenerateBatchFile(batchFilePath);
		}
		system(batchFilePath.c_str());
	}
	void ScriptBuilder::RegenerateProjectScriptSolution(Count<class Project> project)
	{
		RegenerateProjectScriptSolution(project->GetProjectDirectory(), project->GetConfig().Name);
	}
}