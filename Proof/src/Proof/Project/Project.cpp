#include "Proofprch.h"
#include "Project.h"
#include <fstream>
#include "ProjectSerilizer.h" 
namespace Proof
{
		
	Count<Project> Project::Load(const std::filesystem::path& path) {
		Count<Project> project = Count<Project>::Create();

		ProjectSerilizer serializer(project.Get());
		if (serializer.DeSerilizeText(path.string()))
		{
			project->m_ProjectDirectory = path.parent_path();
			return project;
		}

		return nullptr;
	}
	std::filesystem::path Project::GetScriptProjectSolutionPath() const
	{
		return GetProjectDirectory() / std::filesystem::path(GetProjectName() + ".sln");
	}
	std::filesystem::path Project::GetScriptCoreDll()const
	{
		return GetFromSystemProjectDirectory(m_ProjectConfig.ScriptModuleDirectory).string() + "/ProofScriptCore.dll";

	}
	
	std::filesystem::path Project::GetScriptAppDll()const
	{
		return GetFromSystemProjectDirectory(m_ProjectConfig.ScriptModuleDirectory.string() + fmt::format("/{}.dll", m_ProjectConfig.Name));
	}
	Project::Project()
	{
		m_ProjectDirectory = m_ProjectDirectory.parent_path();
	}

	Count<Project> Project::New(const ProjectConfig& projectconfig)
	{

		Count<Project> project = Count<Project>::Create();
		ProjectSerilizer projectserilizer(project.Get());
		projectserilizer.SerilizeText(projectconfig.Project.string());
		
		return project;
	}
	
	ProjectConfig::ProjectConfig(const std::string& name, std::filesystem::path directory):
		Name(name)
	{
		PF_CORE_ASSERT(FileSystem::IsDirectory(directory)," Has to be directory");
		Project += directory.string() + "/" + FileSystem::GetFileName(name) +"/" + FileSystem::GetFileName(name); // dir/ project dir(NAME)/ PROject Name.prrofPRoject
		Project += ".ProofProject";
		AssetDirectory = "Assets";
		AssetManager = AssetDirectory / "AssetManager.ProofAssetManager";
		ScriptModuleDirectory = "Resources/Scripts/Binaries";
	}

}
