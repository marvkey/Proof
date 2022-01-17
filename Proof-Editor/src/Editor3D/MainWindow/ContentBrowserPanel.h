#pragma once
#include <filesystem>
#include "Proof/Core/Core.h"
#include <vector>
#include "Proof/Resources/Asset/Asset.h"
#include <fstream>
#include <string>
#include "Panel.h"
#include <type_traits>
namespace Proof{
	/* THIS CLASS WILL NEED TO BE HANDLED BY CURRENT PROJECT */
	class Proof_API ContentBrowserPanel:public Panel {
	public:
		ContentBrowserPanel(class Editore3D* owner);
		void ImGuiRender(class FrameTime deltaTime);
	private:
		 std::filesystem::path m_CurrentDirectory;
		 Count<Texture2D> m_FolderIcon;
		 Count<Texture2D> m_FileIcon;
		 Count<Texture2D> m_MeshIcon;
		 Count<Texture2D> m_ArrowIcon;
		__forceinline UUID GetIDCurrentDirectory(const std::string& Path);
		__forceinline  bool IsScene (const std::string& Path);
		void RenameFileName(bool isDirectory);
		void ChangeFileName();
		void ChangeFolderName();
		void AllSubFolders(const std::string& path);
		/**
		* Deletes all asset in a folder
		* Path: path of the folder
		*/
		void DeleteFolder(const std::string& path);
		/**
		* Creates a new folder
		* @return the full path of teh folder
		*/
		std::string NewFolder();
		/**
		* Adds an asset and retuns the file path of the asset
		* assetSourcePath: the source file of the asset
		* @return returns the path to the mateiral assest
		*/
		template<class T>
		std::string AddAsset(const std::string& assetSourcePath) {
			std::string fileFullName; // name of the full file including extension
			std::string fileDefaultName; // name that is what we get when first importaed
			std::string fileName; // name that we can add a 1 or 2 at the end
			if (assetSourcePath.empty())return {};
			
			fileDefaultName = Utils::FileDialogs::GetFileName(assetSourcePath);
			fileName = fileDefaultName;
			fileFullName = fileName + "." + T::StaticGetExtension();
			

			uint32_t endIndex = 0; // the ending index of a file like file(0) or file(1)
			while (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + fileFullName)) {
				endIndex++;
				fileName = fileDefaultName + "(" + std::to_string(endIndex) + ")";
				fileFullName = fileName + "." + T::StaticGetExtension();
			}
			std::ofstream({ m_CurrentDirectory.string() + "\\" + fileFullName });
			
			Count<T> tempAsset = CreateCount<T>(assetSourcePath, m_CurrentDirectory.string() + "\\" + fileFullName);
			AssetManager::NewAsset(tempAsset->GetID(), tempAsset);
			return { m_CurrentDirectory.string() + "\\" + fileFullName };
		}
		/**
		* Adds an asset and retuns the file path of the asset
		* only for material asset as it does not need a file path
		* assetSourcePath: the source file of the asset
		* @return returns the path to the mateiral assest
		*/
		std::string AddMaterialAsset();
		Editore3D* m_owner =nullptr;
	};
}
