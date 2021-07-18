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
#include "Proof/Resources/Asset/AssetManager.h"
namespace Proof
{
	static const std::filesystem::path s_AssetsPath = "content";
	static float padding = 16.0f; // space between 
	static float thumbnailSize = 128;
	static uint32_t IterPosition = 0;
	static std::string NewFolderName;
	static std::string NewFileName;
	ContentBrowserPanel::ContentBrowserPanel():
		m_CurrentDirectory(s_AssetsPath) {
		m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		m_MeshIcon = Texture2D::Create("Resources/Icons/ContentBrowser/MeshComponentIcon.png");
		m_ArrowIcon = Texture2D::Create("Resources/Icons/ContentBrowser/ArrowIcon.png");
	}
	void ContentBrowserPanel::ImGuiRender() {
		ImGui::ShowDemoWindow();
		ImGui::Begin("Content Browser");
		ImGui::BeginChild("Folders",{100,ImGui::GetContentRegionAvail().y});
		if (ImGui::ImageButton((ImTextureID)m_FolderIcon->GetID(),{60,60})) {
			m_CurrentDirectory = {s_AssetsPath};
		}
		ImGui::Text("Content");
		for (auto& It : std::filesystem::directory_iterator(s_AssetsPath)) {
			if (It.is_directory()) {
				Count<Texture2D> Icon = m_FolderIcon;
				if (ImGui::ImageButton((ImTextureID)Icon->GetID(),{80,50})) {
				}
				std::string Path = It.path().filename().string();
				ImGui::Text("%s",Path.c_str());
				ImGui::NewLine();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("Sub Window");
		if (ImGui::Button("<-",{50,50})) {
			if (m_CurrentDirectory != s_AssetsPath) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		if (ImGui::BeginPopupContextWindow(0,1,false)) { // right click adn open a new entitiy
			if (ImGui::BeginMenu("Create Folder")) {
				char buffer[256];
				memset(buffer,0,sizeof(buffer));
				strcpy_s(buffer,sizeof(buffer),NewFolderName.c_str());
				if (ImGui::InputTextWithHint("##Name",NewFolderName.empty() == false ? "Name of Folder" : "Name of Folder",buffer,sizeof(buffer))) {
					NewFolderName = buffer;
				}
				if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFolderName) == true && NewFolderName.empty() == false) {
					ImGui::BeginTooltip();
					ImGui::TextColored({1.0,0.0,0.0,1},"Folder already exisst");
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
				if (ImGui::Button("Done") || ImGui::IsKeyPressed((int)KeyBoardKey::Enter)) {
					if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFolderName) == false) {
						std::filesystem::create_directories({m_CurrentDirectory.string() + "\\" + NewFolderName});
						NewFolderName = "";
						ImGui::CloseCurrentPopup();
					}
					else {

						ImGui::BeginTooltip();
						ImGui::TextColored({1.0,0.0,0.0,1},"Folder already exisst");
						ImGui::EndTooltip();
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("New Texture")) {
				char buffer[256];
				memset(buffer,0,sizeof(buffer));
				strcpy_s(buffer,sizeof(buffer),NewFolderName.c_str());
				if (ImGui::InputTextWithHint("##Name","Name of Texture",buffer,sizeof(buffer))) {
					NewFileName = buffer;
				}

				if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFileName +".ProofAsset") == true && NewFileName.empty() == false) {
					ImGui::BeginTooltip();
					ImGui::TextColored({1.0,0.0,0.0,1},"File already exist");
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
				if (ImGui::Button("Done") || ImGui::IsKeyPressed((int)KeyBoardKey::Enter)) {
					if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFileName) == false) {
						std::ofstream({m_CurrentDirectory.string() + "\\" + NewFileName+".ProofAsset"});
						ImGui::CloseCurrentPopup();
						NewAsset(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset");
						NewFileName = "";
					}
					else {

						ImGui::BeginTooltip();
						ImGui::TextColored({1.0,0.0,0.0,1},"File already exist");
						ImGui::EndTooltip();
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("New Mesh")) {
				char buffer[256];
				memset(buffer,0,sizeof(buffer));
				strcpy_s(buffer,sizeof(buffer),NewFolderName.c_str());
				if (ImGui::InputTextWithHint("##Name","Name of Mesh",buffer,sizeof(buffer))) {
					NewFileName = buffer;
				}

				if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset") == true && NewFileName.empty() == false) {
					ImGui::BeginTooltip();
					ImGui::TextColored({1.0,0.0,0.0,1},"File already exist");
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
				if (ImGui::Button("Done") || ImGui::IsKeyPressed((int)KeyBoardKey::Enter)) {
					if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFileName) == false) {
						std::ofstream({m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset"});
						ImGui::CloseCurrentPopup();
						NewMeshAsset(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset");
						NewFileName = "";
					}
					else {
						ImGui::BeginTooltip();
						ImGui::TextColored({1.0,0.0,0.0,1},"File already exist");
						ImGui::EndTooltip();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize); // amount of folder we an fit in a row
		if (columnCount < 1)
			columnCount = 1;
		ImGui::Columns(columnCount,0,false);
		for (auto& It : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			static std::string RenameVariable;
			uint32_t ID;
			/* We are not passing just as string cause life time of object is lost */
			/* we need to understand what is wrong */
			/* THIS IS THE PIECE OF CODE
			std::string Path = It.path().string();
			ImGui::Text("%s",Path.c_str());
			*/
			std::filesystem::relative(It.path(),s_AssetsPath);
			std::string Path = It.path().string();
			std::string filename = It.is_directory() ? It.path().filename().string() : It.path().filename().stem().string(); // returns the file name with ending like hallo.txt, stem removes the .txt
			std::string filenameNoStem = It.path().filename().string();
			Count<Texture2D> Icon = It.is_directory() ? m_FolderIcon : m_FileIcon;
			if(It.is_directory()){
				if (ImGui::ImageButton((ImTextureID)m_FolderIcon->GetID(),{thumbnailSize,thumbnailSize})) {} // there are more paremter to flip image and to add a tint colour
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FileToFolder")) {
					}
					ImGui::EndDragDropTarget();
				}

			}else{
				ID = GetAllIDCurrentDirectory(m_CurrentDirectory.string() + "\\" + filenameNoStem);

				Asset* Temp =nullptr;
				Temp = AssetManager::GetAsset(ID);
				if(Temp != nullptr){
					if (ImGui::ImageButton((ImTextureID)(Temp->IsImageIDNUll() ==false? Temp->GetImageID(): m_FileIcon->GetID()),{thumbnailSize,thumbnailSize})) {
					
					}
					if(ImGui::BeginDragDropSource()){
						ImGui::SetDragDropPayload(Temp->GetName().c_str(),&ID,sizeof(uint32_t));

						ImGui::Image((ImTextureID)(Temp->IsImageIDNUll() == false ? Temp->GetImageID() : m_FileIcon->GetID()),{60,60}); 
						ImGui::EndDragDropSource();
					}
				}
			}
			 

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (It.is_directory()) { // directory is a folder
					m_CurrentDirectory /= filename; // /= is the operator overloaded
				}
			}
			if (ImGui::BeginPopupContextItem(filename.c_str())) {
				if (ImGui::MenuItem("Delete")) {
					std::filesystem::remove_all(m_CurrentDirectory.string() + "\\" + (It.is_directory() ? filename : filenameNoStem));
					if(It.is_directory() ==false){
						AssetManager::Remove(ID);
					}
				}

				if (ImGui::BeginMenu("Rename")) {
					char buffer[256];
					memset(buffer,0,sizeof(buffer));
					strcpy_s(buffer,sizeof(buffer),RenameVariable.c_str());
					if (ImGui::InputTextWithHint("##Name",NewFolderName.empty() == false ? "Name of file" : "Name of file",buffer,sizeof(buffer))) {
						RenameVariable = buffer;
					}

					if(RenameVariable != filename){
						if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + RenameVariable) == true && RenameVariable.empty() == false) {
							ImGui::BeginTooltip();
							ImGui::TextColored({1.0,0.0,0.0,1},"File already exisst");
							ImGui::EndTooltip();
						}
					}
					
					ImGui::SameLine();
					if (ImGui::Button("Done") || ImGui::IsKeyPressed((int)KeyBoardKey::Enter)) {
						if(filenameNoStem == RenameVariable){
							RenameVariable = "";
							ImGui::CloseCurrentPopup();
						}else{
							if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + RenameVariable + (It.is_directory() ? " " : ".ProofAsset")) == false) {
								std::filesystem::rename(m_CurrentDirectory.string() + "\\" + filenameNoStem,m_CurrentDirectory.string() + "\\"+ RenameVariable + (It.is_directory() ? " ":".ProofAsset"));
								RenameVariable = "";
								ImGui::CloseCurrentPopup();
							}
						}

					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();

			}
			ImGui::Text(filename.c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered

			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size",&thumbnailSize,16,512);
		ImGui::SliderFloat("Padding",&padding,0,32);
		ImGui::EndChild();

		ImGui::End();
	}
	void ContentBrowserPanel::NewAsset(const std::string& NewFilePath) {
		std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.png)\0 *.png\0 (*.jpg)\0 *.jpg\0");
		if (FIle.empty() == false) {
			Texture2DAsset* TempAsset = new Texture2DAsset(FIle);
			AssetManager::NewAsset(TempAsset->GetID(),TempAsset);
			std::ofstream NewFIle(NewFilePath);
			NewFIle << TempAsset->GetID();
			NewFIle.close();
		}else{
			std::filesystem::remove_all(NewFilePath); // BECAUSE WWE CREATED A PATH AND WE ARE GONNA WRITE TO IT
		}
	}
	void ContentBrowserPanel::NewMeshAsset(const std::string& NewFilePath) {
		std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.png)\0 *.obj\0 ");
		if (FIle.empty() == false) {
			MeshAsset* TempAsset = new MeshAsset(FIle);
			AssetManager::NewAsset(TempAsset->GetID(),TempAsset);
			std::ofstream NewFIle(NewFilePath);
			NewFIle << TempAsset->GetID();
			NewFIle.close();
		}
		else {
			std::filesystem::remove_all(NewFilePath); // BECAUSE WWE CREATED A PATH AND WE ARE GONNA WRITE TO IT
		}
	}
	uint32_t ContentBrowserPanel::GetAllIDCurrentDirectory(const std::string& Path) {
		std::ifstream MyReadFile(Path);
		uint32_t ID;
		while (MyReadFile>>ID);
		MyReadFile.close();
		return ID;
	}
}