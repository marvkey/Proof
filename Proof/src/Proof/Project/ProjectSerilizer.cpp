#include "Proofprch.h"
#include "ProjectSerilizer.h"
#include "Project.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Proof
{
	ProjectSerilizer::ProjectSerilizer(Project* project) {
		PF_CORE_ASSERT(project !=  nullptr, "Project cannot be nulltptr");
		m_Project = project;
	}
	void ProjectSerilizer::SerilizeText(const std::string& filePath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << YAML::Key << "Project";
			out << YAML::BeginMap;
			{
				out << YAML::Key << "Name" << m_Project->m_ProjectConfig.Name;
				out << YAML::Key << "AssetDirectory" << m_Project->m_ProjectConfig.AssetDirectory.string();
				out << YAML::Key << "AssetManager" << m_Project->m_ProjectConfig.AssetManager.string();
				out << YAML::Key << "StartWorld" << m_Project->m_ProjectConfig.StartWorld;
			}
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
		std::ofstream foud(filePath);
		foud << out.c_str();
		foud.close();
	}
	bool ProjectSerilizer::DeSerilizeText(const std::string& filePath) {
		if (std::filesystem::exists(filePath) == false)
			PF_CORE_ASSERT(false);
		YAML::Node data = YAML::LoadFile(filePath);
		if (!data["Project"])
			return false;
		auto projectData = data["Project"];
		
		m_Project->m_ProjectConfig.Name = projectData["Name"].as<std::string>();
		m_Project->m_ProjectConfig.AssetDirectory = projectData["AssetDirectory"].as<std::string>();
		m_Project->m_ProjectConfig.AssetManager = projectData["AssetManager"].as<std::string>();
		m_Project->m_ProjectConfig.StartWorld = projectData["StartWorld"].as<uint64_t>();
		return true;
	}
}
