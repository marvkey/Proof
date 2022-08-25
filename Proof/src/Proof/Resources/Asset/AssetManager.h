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
#include "PhysicsMaterialAsset.h"
namespace Proof
{
	
	class Proof_API AssetManager {
	public:
		static AssetManager& Get() { return *s_AssetManager; }
		static void NewAsset(AssetID ID,const Count<class Asset>& asset);
		static bool HasID(AssetID ID);
		static bool IsAssetLoaded(AssetID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return it->second.second != nullptr;
			}
			return false;
		}
		struct AssetInfo {
			std::filesystem::path Path;
			AssetType Type;
			AssetID ID;
			bool IsLoaded() {
				return AssetManager::IsAssetLoaded(ID);
			}
			std::string GetName()const {
				return Utils::FileDialogs::GetFileName(Path);
			}
			AssetInfo(const std::string& path, AssetType type, AssetID id) :
				Path(path), Type(type), ID(id) {
			}

			AssetInfo() {

			}
		private:
			friend class AssetManager;
		};
		static AssetManager::AssetInfo GetAssetInfo(AssetID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return it->second.first;
			}
			return AssetManager::AssetInfo();
		}
		static bool LoadAsset(AssetID ID);
		template<class T>
		static T* GetAsset(AssetID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				if (it->second.second.get() == nullptr)
					LoadAsset(ID);
				return dynamic_cast<T*>(it->second.second.get());
			}
			return nullptr;
		}
		template<class T>
		static Count<T>GetAssetShared(AssetID ID){
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				if(it->second.second == nullptr)
					LoadAsset(ID);
				return std::dynamic_pointer_cast<T>(it->second.second);
			}
			return nullptr;
		}
		static AssetType GetAssetType(AssetID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return it->second.first.Type;
			}
			return AssetType::None;
		}
		/**
		*  gets an asset without checking if the asset exist, would cause a crash if the asset does not exit
		* @param AssetID:id the uniqe id of teh acces we are getting
		*/
		template<class T>
		static T* ForceGetAsset(AssetID ID) { 
			if(s_AssetManager->m_AllAssets.at(ID).second == nullptr)
				LoadAsset(ID);

			return dynamic_cast<T*>(s_AssetManager->m_AllAssets.at(ID).second.get());
		}
		/**
		* gets an asset without checking if the asset exist, would cause a crash if the asset does not exit
		* @param AssetID:id the uniqe id of teh acces we are getting
		*/
		template<class T>
		static Count<T>ForceGetAssetShared(AssetID ID) {
			if (s_AssetManager->m_AllAssets.at(ID).second == nullptr)
				LoadAsset(ID);
			return std::dynamic_pointer_cast<T>(s_AssetManager->m_AllAssets.at(ID).second);
		}
		static AssetID CreateID();
		static void Remove(AssetID ID);
		AssetManager(const AssetManager&) = delete;

		static void NotifyOpenedNewLevel(std::set<AssetID> assetLoadIn);
		static void NotifyOpenedNewAsset(AssetID ID);
		static const std::string& GetAssetName(AssetID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return it->second.first.GetName();
			}
			return std::string();
		}
		static AssetType GetAssetFromFilePath(const std::filesystem::path& path) {
			const std::string fileFullExtension = Utils::FileDialogs::GetFullFileExtension(path);


			if (fileFullExtension == "Mesh.ProofAsset")
				return AssetType::Mesh;
			if (fileFullExtension == "Material.ProofAsset")
				return AssetType::Material;
			if (fileFullExtension == "Texture.ProofAsset")
				return AssetType::Texture;
			if (fileFullExtension == "ProofWorld")
				return AssetType::World;
			if (fileFullExtension == PhysicsMaterialAsset::StaticGetExtension())
				return AssetType::PhysicsMaterial;
			const std::string fileDirectExtension = Utils::FileDialogs::GetFileExtension(path);
		
			//for (const std::string& temp : s_PermitableMeshSourceFile) {
			//	if (temp == fileDirectExtension)
			//		return AssetType::MeshSourceFile;
			//}
			return AssetType::None;
		}

		
	private:
		static bool ResetAssetInfo(AssetID ID,const std::string& path) {
			
			if (HasID(ID) == false)return false;
			Asset* asset = ForceGetAsset<Asset>(ID);
			if (IsAssetLoaded(ID)) {
				asset->SetPath(path);
				asset->SaveAsset();
				asset->m_AssetName = Utils::FileDialogs::GetFileName(path);
			}
			auto it = Get().m_AllAssets.at(ID);
			it.first.Path = path;
			return true;
		}

		static std::vector<std::string> s_PermitableMeshSourceFile;
	
		static void SaveAllAsset();
		static AssetManager* s_AssetManager;
		std::unordered_map<AssetID,std::pair<AssetInfo,Count<class Asset>>> m_AllAssets;// path
		std::unordered_map<AssetID, Count<class PhysicsMaterialAsset>> m_AllPhysicsMaterialAsset;// path
		AssetManager() {};
		static void InitilizeAssets(const std::string& Path);
		static void MakeDirectory(const std::string& path);
		static bool IsFileValid(const std::string& Path);
		static AssetType GetAssetType(const std::string& Path);

		static void NewInitilizeAssets(const std::string& Path);
		static void NewSaveAllAsset(const std::string& Path);
		static void GenerateAsset(std::set<AssetID> assetLoadIn);

		friend class Application;
		friend class AssetManagerPanel;
		friend class ContentBrowserPanel;
		friend class NewContentBrowserPanel;
		friend class PhysicsEngine;
	};
}	