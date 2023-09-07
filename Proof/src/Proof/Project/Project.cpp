#include "Proofprch.h"
#include "Project.h"
#include <fstream>
#include "ProjectSerilizer.h" 
namespace Proof
{
	
	Special<Project> Project::Load(const std::filesystem::path& path) {
		Special<Project> project = CreateSpecial<Project>();

		ProjectSerilizer serializer(project.get());
		if (serializer.DeSerilizeText(path.string()))
		{
			project->m_ProjectDirectory = path.parent_path();
			return project;
		}

		return nullptr;
	}

	Project::Project()
	{
		m_ProjectDirectory = m_ProjectDirectory.parent_path();
	}

	Special<Project> Project::New(const ProjectConfig& projectconfig)
	{

		Special<Project> project =CreateSpecial<Project>();
		ProjectSerilizer projectserilizer(project.get());
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
