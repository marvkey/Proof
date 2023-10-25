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
			ScriptModuleDirectory = "Binaries/ScriptApp";
		}

		// name and the direcotyr, it creates a sub directory with the name then sotres the 
		ProjectConfig(const std::string& name, std::filesystem::path directory);

		ProjectConfig()
		{

		}
	
	};

	class Project : RefCounted
	{
	public:
		Project();
		virtual ~Project() {};
		static Count<Project> New(const ProjectConfig& project);
		static Count<Project> Load(const std::filesystem::path& path);
		
		const std::filesystem::path& GetProjectDirectory()const
		{
			return m_ProjectDirectory;
		}
		const std::filesystem::path& GetProjectDirectory()
		{
			return m_ProjectDirectory;
		}
		std::filesystem::path GetAssetDirectory()const
		{
			return GetProjectDirectory() / m_ProjectConfig.AssetDirectory;
		}

		std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)const
		{
			return GetAssetDirectory() / path;
		}	

		std::filesystem::path GetFromSystemProjectDirectory(const std::filesystem::path& path)const
		{
			return GetProjectDirectory() / path;
		}
		std::filesystem::path GetCacheDirectory()
		{
			return std::filesystem::path(GetProjectDirectory()) / "Cache";
		}

		std::filesystem::path GetScriptProjectSolutionPath()const;
		std::filesystem::path GetScriptCoreDll()const;
		std::filesystem::path GetScriptAppDll()const;
		const ProjectConfig& GetConfig()const 
		{
			return m_ProjectConfig;
		};
		const ProjectConfig& GetConfig()
		{
			return m_ProjectConfig;
		};
		const std::string& GetProjectName()const
		{
			return m_ProjectConfig.Name;
		}
	private:
		ProjectConfig m_ProjectConfig;
		std::filesystem::path m_ProjectDirectory;
		friend class ProjectSerilizer;
		friend class Application;
		friend class Editore3D;
	};

}
