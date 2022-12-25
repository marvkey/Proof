#include "Proofprch.h"
#include "Project.h"
#include <fstream>
#include "ProjectSerilizer.h" 
namespace Proof
{
	std::filesystem::path Project::GetProjectDir() {
		return m_Path.parent_path();
	}
	void Project::GenerateDefault() {
		std::ofstream outfile(m_Path);
		outfile.close();
		
		ProjectSerilizer project(this);
		project.SerilizeText(m_Path.string());
	}
}
