#pragma once
#include "Proof/Core/Core.h"
#include <unordered_map>
#include "Proof/Core/UUID.h"
#include <set>
#include <fstream>
#include <string>
#include <filesystem>
#include "Proof/Utils/PlatformUtils.h"
#include "Asset.h"
namespace Proof
{
	
	class Proof_API AssetManager {
	public:
		static AssetManager& Get() { return *s_AssetManager; }
		static void NewAsset(UUID ID,const Count<class Asset>& asset);
		static bool HasID(UUID ID);
		static bool IsAssetLoaded(UUID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return it->second.second != nullptr;
			}
			return false;
		}
		static bool LoadAsset(UUID ID);
		template<class T>
		static T* GetAsset(UUID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				if (it->second.second.get() == nullptr)
					LoadAsset(ID);
				return dynamic_cast<T*>(it->second.second.get());
			}
			return nullptr;
		}
		template<class T>
		static Count<T>GetAssetShared(UUID ID){
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				if(it->second.second == nullptr)
					LoadAsset(ID);
				return std::dynamic_pointer_cast<T>(it->second.second);
			}
			return nullptr;
		}
		static const std::string& GetAssetType(UUID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return it->second.first.Type;
			}
			return std::string();
		}
		/**
		*  gets an asset without checking if the asset exist, would cause a crash if the asset does not exit
		* @param UUID:id the uniqe id of teh acces we are getting
		*/
		template<class T>
		static T* ForceGetAsset(UUID ID) { 
			if(s_AssetManager->m_AllAssets.at(ID).second == nullptr)
				LoadAsset(ID);

			return dynamic_cast<T*>(s_AssetManager->m_AllAssets.at(ID).second.get());
		}
		/**
		* gets an asset without checking if the asset exist, would cause a crash if the asset does not exit
		* @param UUID:id the uniqe id of teh acces we are getting
		*/
		template<class T>
		static Count<T>ForceGetAssetShared(UUID ID) {
			if (s_AssetManager->m_AllAssets.at(ID).second == nullptr)
				LoadAsset(ID);
			return std::dynamic_pointer_cast<T>(s_AssetManager->m_AllAssets.at(ID).second);
		}
		static UUID CreateID();
		static void Remove(UUID ID);
		AssetManager(const AssetManager&) = delete;

		static void NotifyOpenedNewLevel(std::set<UUID> assetLoadIn);
		static void NotifyOpenedNewAsset(UUID ID);
		static const std::string& GetAssetName(UUID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return it->second.first.GetName();
			}
			return std::string();
		}
		static AssetType GetAssetFromFilePath(const std::filesystem::path& path) {
			const std::string fileFullExtension = Utils::FileDialogs::GetFullFileExtension(path);
			if (fileFullExtension == "Mesh.ProofAsset")
				return AssetType::MeshAsset;
			if (fileFullExtension == "Material.ProofAsset")
				return AssetType::Material;
			if (fileFullExtension == "Texture.ProofAsset")
				return AssetType::TextureAsset;
			if (fileFullExtension == "ProofWorld")
				return AssetType::WorldAsset;
			const std::string fileDirectExtension = Utils::FileDialogs::GetFileExtension(path);
		
			for (const std::string& temp : s_PermitableMeshSourceFile) {
				if (temp == fileDirectExtension)
					return AssetType::MeshSourceFile;
			}
			return AssetType::None;
		}

		
	private:
		static bool ResetAssetInfo(UUID ID,const std::string& name, const std::string path) {
			
			if (HasID(ID) == false)return false;
			Asset* asset = ForceGetAsset<Asset>(ID);
			if (IsAssetLoaded(ID)) {
				asset->m_AssetName = name;
				asset->SetPath(path);
				asset->SaveAsset();
			}
			auto it = Get().m_AllAssets.at(ID);
			it.first.Path = path;
			it.first.Name = name;
			return true;
		}
		struct AssetInfo {
			std::filesystem::path Path;
			std::string Type;
			const std::string& GetName()const {
				return Name;
			}
			AssetInfo(const std::string& path, const std::string& type) :
				Path(path), Type(type)
			{
				Name = Utils::FileDialogs::GetFileName(path);

			}
		private:
			friend class AssetManager;
			std::string Name;
		};
		static std::vector<std::string> s_PermitableMeshSourceFile;
	
		static void SaveAllAsset();
		static AssetManager* s_AssetManager;
		std::unordered_map<UUID,std::pair<AssetInfo,Count<class Asset>>> m_AllAssets;// path
		AssetManager() {};
		static void InitilizeAssets(const std::string& Path);
		static void MakeDirectory(const std::string& path);
		static bool IsFileValid(const std::string& Path);
		static std::string GetAssetType(const std::string& Path);
		friend class Application;
		friend class AssetManagerPanel;
		friend class ContentBrowserPanel;
		static void NewInitilizeAssets(const std::string& Path);
		static void GenerateAsset(std::set<UUID> assetLoadIn);
	};
}	