#pragma once
#include "Proof/Core/Core.h"
#include <filesystem>
#include "Proof/Asset/AssetManager.h"
namespace Proof{
	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path Project ={};

		std::filesystem::path AssetDirectory = {};
		std::filesystem::path AssetManager = {};

		std::filesystem::path ScriptModuleDirectory = {};

		uint64_t StartWorld = 0;
		uint64_t StartWorldEdit = 0;
		bool OnCloseStartWorldEditLastOpen = true;
		ProjectConfig(std::filesystem::path project, const std::string& name = "Untitled"):
			Project(project), Name(name)
		{
			AssetDirectory = "Assets";
			AssetManager = AssetDirectory / "AssetManager.ProofAssetManager";
			ScriptModuleDirectory = "Resources/Scripts/Binaries";
		}
		ProjectConfig()
		{

		}

	};

	class Project {
	public:
		Project();
		virtual ~Project() {};
		static Special<Project> New(const ProjectConfig& project);
		static Special<Project> Load(const std::filesystem::path& path);
		
		const std::filesystem::path& GetProjectDirectory()
		{
			return m_ProjectDirectory;
		}

		std::filesystem::path GetAssetDirectory()
		{
			return GetProjectDirectory() / m_ProjectConfig.AssetDirectory;
		}

		std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			return GetAssetDirectory() / path;
		}	

		std::filesystem::path GetFromSystemProjectDirectory(const std::filesystem::path& path)
		{
			return GetProjectDirectory() / path;
		}
		const ProjectConfig& GetConfig() {
			return m_ProjectConfig;
		};

	private:
		ProjectConfig m_ProjectConfig;
		std::filesystem::path m_ProjectDirectory;
		friend class ProjectSerilizer;
		friend class Application;
	};

}
