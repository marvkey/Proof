#pragma once
#include <filesystem>
#include "Proof/Core/Core.h"
#include <vector>
#include "Proof/Resources/Asset/Asset.h"
#include <fstream>
#include <string>
#include "Panel.h"
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
		Editore3D* m_owner =nullptr;
	};
}
