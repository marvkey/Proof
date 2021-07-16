#include "Proofprch.h"
#include "ContentBrowserPanel.h"

#include <ImGui/imgui.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Platform/Window/WindowPlatformUtils.cpp"
#include "Proof/Utils/PlatformUtils.h"
#include <vector>
#include "Proof3D/Scene/Model.h"

namespace Proof
{
	static const std::filesystem::path s_AssetsPath = "../";
	static float padding = 16.0f; // space between 
	static float thumbnailSize = 128;
	static uint32_t IterPosition =0;
	ContentBrowserPanel::ContentBrowserPanel():
		m_CurrentDirectory(s_AssetsPath) 	{	
		s_ContentBrowserAssets = new std::vector<Asset*>;
		m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		m_MeshIcon = Texture2D::Create("Resources/Icons/ContentBrowser/MeshComponentIcon.png");
	}
	void ContentBrowserPanel::ImGuiRender() {
		ImGui::Begin("Content Browser");
		if (ImGui::BeginPopupContextWindow(0,1,false)) { // right click adn open a new entitiy
			if (ImGui::MenuItem("New Folder")){}
			if(ImGui::MenuItem("New Texture")){
				std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.png)\0 *.png\0 (*.jpg)\0 *.jpg\0");
				if(FIle.empty() ==false){
					Texture2DAsset* TempAsset = new Texture2DAsset(FIle);
					s_ContentBrowserAssets->emplace_back(TempAsset);
				} 
			}

			if(ImGui::MenuItem("New Mesh")){
				std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.Mesh)\0* *.obj\0 ");
				if (FIle.empty() == false) {
					MeshAsset* TempAsset = new MeshAsset(FIle);
					s_ContentBrowserAssets->emplace_back(TempAsset);
				}
			}
			ImGui::EndPopup();
		}
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize); // amount of folder we an fit in a row
		if (columnCount < 1)
			columnCount = 1;
		ImGui::Columns(columnCount,0,false);
		IterPosition =0;
		for (Asset* It :*s_ContentBrowserAssets){
			Texture2DAsset* TextureIterate = dynamic_cast<Texture2DAsset*>(It);
			if(TextureIterate != nullptr){
				if (ImGui::ImageButton((ImTextureID)TextureIterate->m_Texture->GetID(),{thumbnailSize,thumbnailSize}));
				if(ImGui::BeginDragDropSource()){
					// whatever we put between setdragDropPayload set it as the payload
					ImGui::SetDragDropPayload(TextureIterate->GetName().c_str(),&TextureIterate->m_Texture,sizeof(TextureIterate->m_Texture));

					ImGui::Image((ImTextureID)TextureIterate->m_Texture->GetID(),{60,60}); // whatever w
					ImGui::EndDragDropSource();
				}
				IterPosition++;
				ImGui::Text("Default");
				ImGui::NextColumn();
				continue;
			}
			MeshAsset* meshIterate = dynamic_cast<MeshAsset*>(It);
			if(meshIterate != nullptr){
				if (ImGui::ImageButton((ImTextureID)m_MeshIcon->GetID(),{thumbnailSize,thumbnailSize},{0,1},{1,0}));
				if (ImGui::BeginPopupContextItem("RemoveMesh")) {
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopup("RemoveMesh")) {
					if (ImGui::MenuItem("Remove Mesh")) {
						meshIterate->m_Model= nullptr;
					}
					ImGui::EndPopup();
				}
				if (ImGui::BeginDragDropSource()) {
					// whatever we put between setdragDropPayload set it as the payload
					ImGui::SetDragDropPayload(MeshAsset::GetName().c_str(),&IterPosition,sizeof(uint32_t));

					ImGui::Image((ImTextureID)m_MeshIcon->GetID(),{60,60},{0,1},{1,0}); // whatever w
					ImGui::EndDragDropSource();
				}
				IterPosition++;
				ImGui::Text("Default");
				ImGui::NextColumn();
				continue;
			}
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size",&thumbnailSize,16,512);
		ImGui::SliderFloat("Padding",&padding,0,32);
		ImGui::End();

	/*
		if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath)) {
			if (ImGui::Button("<-")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		static float padding = 16.0f; // space between 
		static float thumbnailSize = 128;

		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize); // amount of folder we an fit in a row
		if (columnCount < 1)
			columnCount = 1;
		ImGui::Columns(columnCount,0,false);
		*/

		//for (auto& It : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			/* We are not passing just as string cause life time of object is lost */
			/* we need to understand what is wrong */
			/* THIS IS THE PIECE OF CODE
			std::string Path = It.path().string();
			ImGui::Text("%s",Path.c_str());
			*/
		/*
			std::filesystem::relative(It.path(),s_AssetsPath);
			std::string Path = It.path().string();
			std::string filename = It.path().filename().string(); // returns the file name with ending like hallo.txt, stem removes the .txt

			Count<Texture2D> Icon = It.is_directory() ? m_FolderIcon : m_FileIcon;
			if (ImGui::ImageButton((ImTextureID) Icon->GetID(),{thumbnailSize,thumbnailSize})); // there are more paremter to flip image and to add a tint colour
			if(ImGui::IsItemHovered()&&ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
				if (It.is_directory()){ // directory is a folder
					m_CurrentDirectory /= filename; // /= is the operator overloaded
				}
			}
			ImGui::Text(Path.c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size",&thumbnailSize,16,512);
		ImGui::SliderFloat("Padding",&padding,0,32);
		
		ImGui::End();
		*/
	}
}