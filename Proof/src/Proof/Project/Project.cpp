#include "Proofprch.h"
#include "Project.h"
#include <fstream>
#include "ProjectSerilizer.h" 
namespace Proof
{
	Project* Project::s_Project = nullptr;
	std::filesystem::path Project::GetProjectDir() {
		return Project::Get()->m_Path.parent_path();
	}
	void Project::GenerateDefault() {
		std::ofstream outfile(m_Path);
		outfile.close();
		
		ProjectSerilizer project(this);
		project.SerilizeText(m_Path.string());
	}
}
