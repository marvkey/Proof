#pragma once
#include <filesystem>
#include "Proof/Core/Core.h"
#include <vector>
#include "Proof/Resources/Asset/Asset.h"
#include <fstream>
#include <string>
namespace Proof{
	/* THIS CLASS WILL NEED TO BE HANDLED BY CURRENT PROJECT */
	class Proof_API ContentBrowserPanel {
	public:
		ContentBrowserPanel(class Editore3D* owner);
		void ImGuiRender();
	private:
		 std::filesystem::path m_CurrentDirectory;
		 Count<Texture2D> m_FolderIcon;
		 Count<Texture2D> m_FileIcon;
		 Count<Texture2D> m_MeshIcon;
		 Count<Texture2D> m_ArrowIcon;
		 void NewAsset(const std::string&NewFilePath);
		 void NewMeshAsset(const std::string& NewFilePath);
		 void NewMaterialAsset(const std::string& NewFilePath);
		__forceinline uint32_t GetIDCurrentDirectory(const std::string& Path);
		__forceinline  bool IsScene (const std::string& Path);
		Editore3D* m_owner =nullptr;
	};
}
