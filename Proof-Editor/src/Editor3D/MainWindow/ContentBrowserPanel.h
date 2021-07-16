#pragma once
#include <filesystem>
#include "Proof/Core/Core.h"
#include <vector>
#include "Proof/Resources/Asset/Asset.h"
namespace Proof{
	/* THIS CLASS WILL NEED TO BE HANDLED BY CURRENT PROJECT */
	class Proof_API ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		void ImGuiRender();
		std::vector<Proof::Asset*>* s_ContentBrowserAssets;
	private:
		 std::filesystem::path m_CurrentDirectory;
		 Count<Texture2D> m_FolderIcon;
		 Count<Texture2D> m_FileIcon;
		 Count<Texture2D> m_MeshIcon;
	};
}
