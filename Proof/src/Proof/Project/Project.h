#pragma once
#include "Proof/Core/Core.h"
#include <filesystem>

namespace Proof{
	class Project {
	public:
		std::string Name = "Proof";
		std::filesystem::path StartWorld;
		static Project* Get() {
			return s_Project;
		}
		static std::filesystem::path GetProjectDir();
		const std::filesystem::path& GetAssetDir()const {
			return m_AssetDirectory;
		}
	private:
		static void SetApp(class Application* app) {
			if (app != nullptr) {
				delete s_Project;
				s_Project = new Project();
				s_Project->m_App = app;
			}
			else {
				delete s_Project;
			}
		}
		std::filesystem::path m_AssetDirectory = "Proof/Assets";
		std::filesystem::path m_AssetManager = "Proof/Assets/AssetManager.ProofAssetManager";
		std::filesystem::path m_Path ="Proof/Proof.ProofProject";
		Project() {};
		static Project* s_Project;

		class Application* m_App = nullptr;
		void GenerateDefault();
		friend class ProjectSerilizer;
		friend class Application;
	};

}
