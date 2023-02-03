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
		PF_CORE_ASSERT(std::filesystem::is_directory(projectconfig.Project) == false, "Pass direcoty as a directory");

		Special<Project> project =CreateSpecial<Project>();
		//std::ofstream outfile(projectconfig.Project);
		//outfile.close();
		//
		//ProjectSerilizer projectserilizer(project.get());
		//projectserilizer.SerilizeText(projectconfig.Project.string());
		
		return project;
	}
	
}
