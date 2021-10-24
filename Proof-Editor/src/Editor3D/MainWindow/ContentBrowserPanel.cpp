#include "Proofprch.h"
#include "ContentBrowserPanel.h"

#include <ImGui/imgui.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Platform/Window/WindowPlatformUtils.cpp"
#include "Proof/Utils/PlatformUtils.h"
#include <vector>
#include "Proof/Scene/Mesh.h"
#include "Proof/Resources/Asset/AssetManager.h"
#include "Proof/Resources/Asset/Asset.h"

#include <yaml-cpp/yaml.h>
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "../Proof-Editor/src/Editor3D/Editor3D.h"
namespace Proof
{ 
	static const std::filesystem::path s_AssetsPath = "content";
	static float padding = 16.0f; // space between 
	static float thumbnailSize = 60;
	static uint32_t IterPosition = 0;
	static std::string NewFolderName; 
	static std::string NewFileName;
	static std::string FileDragSource;
	static std::string FileDragSourceName;


	ContentBrowserPanel::ContentBrowserPanel(class Editore3D* owner):
		m_CurrentDirectory(s_AssetsPath), 
		m_owner(owner)
	{
		m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		m_MeshIcon = Texture2D::Create("Resources/Icons/ContentBrowser/MeshComponentIcon.png");
		m_ArrowIcon = Texture2D::Create("Resources/Icons/ContentBrowser/ArrowIcon.png");
	}
	void ContentBrowserPanel::ImGuiRender() {
		ImGui::Begin("Content Browser");
		{
			ImGui::BeginChild("Folders",{200,ImGui::GetContentRegionAvail().y});
			{
				if (ImGui::ImageButton((ImTextureID)m_FolderIcon->GetID(),{60,60})) {
					m_CurrentDirectory = {s_AssetsPath};
				}
				ImGui::Text("Content");
				for (auto& It : std::filesystem::directory_iterator(s_AssetsPath)) {
					if (It.is_directory()) {
						std::string Path = It.path().filename().string();
						if(ImGui::TreeNode(Path.c_str())){
							for(auto& It : std::filesystem::directory_iterator(It.path())){
								std::string temp = It.path().stem().string();
								ImGui::Text((It.is_directory() ? "(Folder)"+temp: temp).c_str());
							}
							ImGui::TreePop();
						}
					}
				}
			}
			ImGui::EndChild();
			ImGui::SameLine();

			ImGui::BeginChild("Sub Window");
			{
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
							ImGui::TextColored({1.0,0.0,0.0,1},"Folder already exist");
							ImGui::EndTooltip();
						}
						ImGui::SameLine();
						if (ImGui::Button("Done") || ImGui::IsKeyPressed((int)KeyBoardKey::Enter)) {
							if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFolderName) == false) {
								std::filesystem::create_directories({m_CurrentDirectory.string() + "\\" + NewFolderName});
								NewFolderName = "";
								ImGui::CloseCurrentPopup();
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
							if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset") == false) {
								std::ofstream({m_CurrentDirectory.string() + "\\" + NewFileName+".ProofAsset"});
								ImGui::CloseCurrentPopup();
								NewAsset(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset");
								NewFileName = "";
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
								ImGui::CloseCurrentPopup();
								NewMeshAsset(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset");
								NewFileName = "";
							}
						}
						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("New Material")) {
						char buffer[256];
						memset(buffer,0,sizeof(buffer));
						strcpy_s(buffer,sizeof(buffer),NewFolderName.c_str());
						if (ImGui::InputTextWithHint("##Name","Name of Material",buffer,sizeof(buffer))) {
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
								ImGui::CloseCurrentPopup();
								NewMaterialAsset(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset");
								NewFileName = "";
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
					/* We are not passing just as string cause life time of object is lost */
					/* we need to understand what is wrong */
					/* THIS IS THE PIECE OF CODE
					std::string Path = It.path().string();
					ImGui::Text("%s",Path.c_str());
					*/
					bool isScene = IsScene(It.path().string());
					AssetID ID = 0;
					std::string Path = It.path().string();
					std::string filename = It.is_directory() ? It.path().filename().string() : It.path().filename().stem().string(); // returns the file name with ending like hallo.txt, stem removes the .txt
					std::string filenameNoStem = It.path().filename().string();
					if(It.is_directory()){
						if (ImGui::ImageButton((ImTextureID)m_FolderIcon->GetID(),{thumbnailSize,thumbnailSize})) {} // there are more paremter to flip image and to add a tint colour
					}else{
				
						if(isScene ==false){

							ID = GetIDCurrentDirectory(m_CurrentDirectory.string() + "\\" + filenameNoStem);
							Asset* Temp =nullptr;
							Temp = AssetManager::GetAsset<Asset>(ID);
							if(Temp != nullptr){
								if (ImGui::ImageButton((ImTextureID)(Temp->IsImageIDNUll() ==false? Temp->GetImageID(): m_FileIcon->GetID()),{thumbnailSize,thumbnailSize})) {
								}
								if(ImGui::IsItemHovered()&& ImGui::IsMouseDown(0)==false){
									FileDragSource = m_CurrentDirectory.string() + "\\" + filenameNoStem;
									FileDragSourceName = filename;
								}
								if(ImGui::BeginDragDropSource()){
									AssetID staticID =GetIDCurrentDirectory(FileDragSource);
									ImGui::SetDragDropPayload(AssetManager::GetAsset<Asset>(staticID)->GetAssetTypeName().c_str(),&staticID,sizeof(AssetID));

									ImGui::Image((ImTextureID)(Temp->IsImageIDNUll() == false ? Temp->GetImageID() : m_FileIcon->GetID()),{60,60}); 
									ImGui::Text(FileDragSourceName.c_str());
									ImGui::EndDragDropSource();
								}

						
								if(Temp->GetAssetTypeName() == MaterialAsset::GetAssetTypeStaticName()&& ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)){
									MaterialAsset * TempMaterial = dynamic_cast<MaterialAsset*>(Temp);
									m_owner->CreateMaterialEdtior(TempMaterial);
									//ImGui::Begin("Material Editor");
									//MaterialAsset* TempMaterial = dynamic_cast<MaterialAsset*>(Temp);
									//ImGui::ColorEdit4("##Colour",glm::value_ptr(TempMaterial->m_Material.Colour));
									//
									//ImGui::End();
								}

								if (Temp->GetAssetTypeName() == MeshAsset::GetAssetTypeStaticName() && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
									MeshAsset* tempAsset= dynamic_cast<MeshAsset*>(Temp);
									m_owner->CreateMeshEditor(tempAsset);
									
								}
						
							}
						}else{
							if (ImGui::ImageButton((ImTextureID)m_FileIcon->GetID() ,{thumbnailSize,thumbnailSize})){
						
							}
							if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) == false) {
								FileDragSource = m_CurrentDirectory.string() + "\\" + filenameNoStem;
								FileDragSourceName = filename;
							}

							if (ImGui::BeginDragDropSource()) {
								ImGui::SetDragDropPayload("World",FileDragSource.c_str(),sizeof(FileDragSource));
								ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(),{60,60});
								ImGui::Text(FileDragSourceName.c_str());
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
							if(It.is_directory() ==false && isScene==false){
								AssetManager::Remove(GetIDCurrentDirectory(m_CurrentDirectory.string() + "\\" + (It.is_directory() ? filename : filenameNoStem)));
							}
							std::filesystem::remove_all(m_CurrentDirectory.string() + "\\" + (It.is_directory() ? filename : filenameNoStem));
						}

						if (ImGui::BeginMenu("Rename")) {
							char buffer[256];
							memset(buffer,0,sizeof(buffer));
							strcpy_s(buffer,sizeof(buffer),RenameVariable.c_str());
							if (ImGui::InputTextWithHint("##Name",NewFolderName.empty() == false ? "Name of file" : "Name of file",buffer,sizeof(buffer))) {
								RenameVariable = buffer;
							}

							if(RenameVariable != filename && RenameVariable.empty() ==false){
								if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + RenameVariable + (It.is_directory() ? " " : ".ProofAsset")) == true) {
									ImGui::BeginTooltip();
									ImGui::TextColored({1.0,0.0,0.0,1},"File already exist");
									ImGui::EndTooltip();
									ImGui::SetMouseCursor(8); // this is set to not allowed
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
										if(It.is_directory() == false && isScene ==false){
											auto* asset = AssetManager::GetAsset<Asset>(GetIDCurrentDirectory(m_CurrentDirectory.string() + "\\" + RenameVariable + ".ProofAsset"));
											asset->m_AssetName = RenameVariable;
											asset->SetPath(m_CurrentDirectory.string() + "\\" + RenameVariable + ".ProofAsset");
											asset->SaveAsset();
										}
										RenameVariable = "";
										ImGui::CloseCurrentPopup();
									}
								}

							}
							ImGui::EndMenu();
						}
						ImGui::EndPopup();

					}
					if(ID !=0 || It.is_directory()){
						ImGui::Text(filename.c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered
						ImGui::NextColumn();
					}

					if(isScene ==true){
						ImGui::Text(filename.c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered
						ImGui::NextColumn();
					}
				}
				ImGui::Columns(1);
				ImGui::SliderFloat("Thumbnail Size",&thumbnailSize,16,512);
				ImGui::SliderFloat("Padding",&padding,0,32);
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
	void ContentBrowserPanel::NewAsset(const std::string& NewFilePath) {
		std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.png)\0 *.png\0 (*.jpg)\0 *.jpg\0");
		if (FIle.empty() == false) {
			Texture2DAsset* TempAsset = new Texture2DAsset(FIle,NewFilePath);
			AssetManager::NewAsset(TempAsset->GetID(),TempAsset);
		}
	}
	void ContentBrowserPanel::NewMeshAsset(const std::string& NewFilePath) {
		std::string FIle = Utils::FileDialogs::OpenFile("Mesh (*.obj)\0 *.obj\0 (*.gltf)\0 *.gltf\0 (*.fbx)\0 *.fbx\0");
		if (FIle.empty() == false) {
			MeshAsset* TempAsset = new MeshAsset(FIle,NewFilePath);
			AssetManager::NewAsset(TempAsset->GetID(),TempAsset);
		}
	}
	void ContentBrowserPanel::NewMaterialAsset(const std::string& NewFilePath) {
			MaterialAsset* TempAsset = new MaterialAsset("null",NewFilePath);
			AssetManager::NewAsset(TempAsset->GetID(),TempAsset);
	}
	AssetID ContentBrowserPanel::GetIDCurrentDirectory(const std::string& Path) {
		std::ifstream testFile(Path);
		std::string line;
		AssetID ID;

		while (std::getline(testFile,line)) {
			if (line.empty() ==false) { // first line
				if(line.substr(0,16) == "AssetTypeString:"){
					break;
				}
				return 0;
			}
		}
		while (std::getline(testFile,line)) {
			if(line.substr(0,3) =="ID:"){
				std::stringstream ss(line.substr(3,line.size()));
				ss>>ID;
				return ID;
			}
		}
		return 0;
	}
	bool ContentBrowserPanel::IsScene(const std::string& Path) {
		std::ifstream testFile(Path);
		std::string line;
		while (std::getline(testFile,line)) {
			if (line.empty() == false) { // first line
				if (line.substr(0,6) == "World:") {
					return true;
				}
				return false;
			}
		}
	}
}