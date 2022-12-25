#pragma once
#include "Proof/Core/Core.h"
#include <filesystem>
#include "Proof/Asset/AssetManager.h"
namespace Proof{
	class Project {
	public:
		Project(class Application* App) :
			m_App(App)
		{

		}
		std::string Name = "Proof";
		std::filesystem::path GetProjectDir();
		const std::filesystem::path& GetAssetDir()const {
			return m_AssetDirectory;
		}
		void SetStartWorld(AssetID ID) {
			if (AssetManager::HasID(ID) == false)return;
			auto info = AssetManager::GetAssetInfo(ID);
			if (info.Type == AssetType::World)
				m_StartWorld = ID;
		}
		AssetID GetStartWorld() {
			return m_StartWorld;
		}
	private:
		uint64_t m_StartWorld =0;
		std::filesystem::path m_AssetDirectory = "Proof/Assets";
		std::filesystem::path m_AssetManager = "Proof/Assets/AssetManager.ProofAssetManager";
		std::filesystem::path m_Path ="Proof/Proof.ProofProject";
		Project() {};
		Project* s_Project;

		class Application* m_App = nullptr;
		void GenerateDefault();
		friend class ProjectSerilizer;
		friend class Application;
	};

}
