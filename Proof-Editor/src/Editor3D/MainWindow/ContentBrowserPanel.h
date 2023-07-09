#pragma once
//#include "Proof/Core/Core.h"
#include "Panel.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Utils/PlatformUtils.h"

#include <fstream>
#include <string>
#include "Panel.h"
#include <type_traits>
#include <vector>
#include <filesystem>
namespace Proof
{
	class Texture2D;
	/* THIS CLASS WILL NEED TO BE HANDLED BY CURRENT PROJECT */
	class Proof_API ContentBrowserPanel :public Panel {
	public:
		ContentBrowserPanel();
		void ImGuiRender(class FrameTime deltaTime);
		//returns false when done rendering
		// when the window pop up context is done	
		// returns true if still need rendering
		// the bool is for don, and assetId is the asset id of the mesh
		static std::pair<bool, AssetID> AddMesh(Count<class MeshSource> meshSource, const std::vector<uint32_t>& excludeIndex = {});
			//returns false when done rendering
		// when the window pop up context is done
		// returns true if still need rendering
		// the bool is for don, and assetId is the asset id of the mesh
		static std::pair<bool,AssetID> AddWorld(Count<class World> world);
	private:
		std::filesystem::path m_CurrentDirectory;
		Count<Texture2D> m_FolderIcon;
		Count<Texture2D> m_FileIcon;
		Count<Texture2D> m_MeshIcon;
		Count<Texture2D> m_ArrowIcon;
		void GetAllSubFolders(const std::filesystem::path& path);

		/**
		* adds a asset to menu item to add to content broweser
		* name: the asset we want to add texture, mesh, so one
		* openFilePath : path of the openFIle 
		* @returns if asset was added sucessfuly
		*/
		std::string NewFolder(const std::string& folderName);
		template <typename T>
		bool AddAssetPopupMenuItem(const std::string& name, const char* filter, std::string& setFileToRename) {
			if (ImGui::MenuItem(name.c_str())) {
				std::string extensionFilePathSource = Utils::FileDialogs::OpenFile(filter);
				if (extensionFilePathSource.empty())return false;
				//	setFileToRename = Utils::FileDialogs::GetFileName(AddAssetSource<T>(extensionFilePathSource));
				return true;
			}
			return false;
		}
		/**
		* Adds an asset and retuns the file path of the asset
		* assetName: name of the asset
		* @return returns the path to the asset
		*/
		template <typename T>
		std::string AddAssetName(const std::string& assetName = typeid(T).name()) {
			std::string fileFullName; // name of the full file including extension
			std::string fileDefaultName; // name that is what we get when first importaed
			std::string fileName; // name that we can add a 1 or 2 at the end

			fileDefaultName = assetName;
			fileName = fileDefaultName;
			fileFullName = fileName + "." + AssetManager::GetExtension(T::GetStaticType());
			uint32_t endIndex = 0; // the ending index of a file like file(0) or file(1)
			while (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + fileFullName))
			{
				endIndex++;
				fileName = fileDefaultName + "(" + std::to_string(endIndex) + ")";
				fileFullName = fileName + "." + AssetManager::GetExtension(T::GetStaticType());
			}
			std::ofstream({ m_CurrentDirectory.string() + "\\" + fileFullName });

			auto asset = AssetManager::NewAsset<T>(m_CurrentDirectory.string() + "\\" + fileFullName);
			return { m_CurrentDirectory.string() + "\\" + fileFullName };
		}
		/**
		* adds a asset to menu item to add to content broweser
		* name: the asset we want to add texture, mesh, so one
		* @returns if asset was added sucessfuly
		*/
		template <typename T>
		bool AddAssetPopupMenuItem(const std::string& name, std::string& setFileToRename) {
			if (ImGui::MenuItem(name.c_str())) {
				setFileToRename = Utils::FileDialogs::GetFileName(AddAssetName<T>(name));
				return true;
			}
			return false;
		}
		// NEEDS TO LOOP EVERY ASSET AND CHANGE ANY THING THAT LINKS TO THIS ASSET
		// LIKE ASSET WICH LINKK TO Stuff
		void RenameFolder(const std::string& fileName, const std::string& fileFullExension);
		void RenameFile(const std::string& fileName,const std::string& fileFullExension);

		void Rename(const std::string& fileName, const std::string& fileFullExension, bool isFolder) {
			if (isFolder)
				RenameFolder(fileName, fileFullExension);
			else
				RenameFile(fileName, fileFullExension);
		}

		void ContentSubWindow();
		void GenerateThumbnail(UUID ID);
		void DeleteFolder(const std::string& path);
		friend class Editore3D;
	};
}
