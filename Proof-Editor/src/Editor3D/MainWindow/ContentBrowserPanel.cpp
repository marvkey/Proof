#include "Proofprch.h"
#include "ContentBrowserPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"

#include <ImGui/imgui.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
#include "Proof/Resources/Asset/MeshAsset.h"

#include "Proof/Utils/PlatformUtils.h"
#include <vector>
#include "Proof/Scene/Mesh.h"
#include "Proof/Resources/Asset/AssetManager.h"
#include "Proof/Resources/Asset/Asset.h"

#include <yaml-cpp/yaml.h>
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "../Proof-Editor/src/Editor3D/Editor3D.h"
#include "Proof/Core/FrameTime.h"
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
	static std::string RenameFile;
	static std::string RenameFileNewName;
	static std::string RenameFileExtension;

	ContentBrowserPanel::ContentBrowserPanel(class Editore3D* owner):
		m_CurrentDirectory(s_AssetsPath), 
		m_owner(owner)
	{
		m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		m_MeshIcon = Texture2D::Create("Resources/Icons/ContentBrowser/MeshComponentIcon.png");
		m_ArrowIcon = Texture2D::Create("Resources/Icons/ContentBrowser/ArrowIcon.png");
	}
	void ContentBrowserPanel::ImGuiRender(FrameTime DeltaTime) {
		if (m_ShowWindow == false)
			return;
		ImGui::Begin("Content Browser", &m_ShowWindow);
		{
			ImGui::BeginChild("Folders",{200,ImGui::GetContentRegionAvail().y});
			{
				ImGui::Text("Content");
				for (auto& It : std::filesystem::directory_iterator(s_AssetsPath)) {
					if (It.is_directory()) {
						std::string Path = It.path().filename().string();
						if(ImGui::TreeNode(Path.c_str())){
							for(auto& SubIt : std::filesystem::directory_iterator(It.path())){
								if(SubIt.is_directory()) // TODO(MARV) ADD RECURSION TO THIS FOR SUB FOlders
									ImGui::Text(SubIt.path().stem().string().c_str());
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
							if (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + NewFileName + "."+Texture2DAsset::StaticGetExtension()) == false) {
								std::ofstream({m_CurrentDirectory.string() + "\\" + NewFileName+ "." + Texture2DAsset::StaticGetExtension() });
								ImGui::CloseCurrentPopup();

								std::string file= Utils::FileDialogs::OpenFile("Texture (*.png)\0 *.png\0 (*.jpg)\0 *.jpg\0");
								if (file.empty() == false) {
									Count<Texture2DAsset> TempAsset = CreateCount<Texture2DAsset>(file, m_CurrentDirectory.string() + "\\" + NewFileName + Texture2DAsset::StaticGetExtension());
									AssetManager::NewAsset(TempAsset->GetID(), TempAsset);
								}
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
								std::string FIle = Utils::FileDialogs::OpenFile("Mesh (*.obj)\0 *.obj\0 (*.gltf)\0 *.gltf\0 (*.fbx)\0 *.fbx\0");
								if (FIle.empty() == false) {
									Count<MeshAsset> TempAsset = CreateCount<MeshAsset>(FIle, m_CurrentDirectory.string() + "\\" + NewFileName + MeshAsset::StaticGetExtension());
									AssetManager::NewAsset(TempAsset->GetID(), TempAsset);
								}
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
								Count<MaterialAsset> TempAsset = CreateCount<MaterialAsset>(m_CurrentDirectory.string() + "\\" + NewFileName + ".ProofAsset");
								AssetManager::NewAsset(TempAsset->GetID(), TempAsset);
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

					std::string fileExtension =Utils::FileDialogs::GetFullFileExtension(It.path());
					AssetType assetType= AssetManager::GetAssetFromFilePath(It);
					bool isScene = IsScene(It.path().string());
					std::string filename = Utils::FileDialogs::GetFileName(It.path()); // returns the file name with ending like hallo.txt, stem removes the .txt
					std::string filenameNoStem = It.path().filename().string();
					if(It.is_directory()){
						if (ImGui::ImageButton((ImTextureID)m_FolderIcon->GetID(),{thumbnailSize,thumbnailSize})) {} // there are more paremter to flip image and to add a tint colour
					}else{
				
						if(fileExtension != "ProofWorld") {
							if (assetType == AssetType::MeshAsset)
								ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(), { thumbnailSize,thumbnailSize });
							else if(assetType == AssetType::Material)
								ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(), { thumbnailSize,thumbnailSize });
							if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) == false) {
								FileDragSource = It.path().string();
								FileDragSourceName = filename;
							}
							if (ImGui::BeginDragDropSource()) {
								UUID staticID = GetIDCurrentDirectory(FileDragSource);
								std::string assetTypestring;
								if (assetType == AssetType::MeshAsset)
									assetTypestring = MeshAsset::GetAssetType();
								else
									assetTypestring = MaterialAsset::GetAssetType();
								ImGui::SetDragDropPayload(assetTypestring.c_str(), &staticID, sizeof(UUID));

								ImGui::Image((ImTextureID)m_FileIcon->GetID(), { 60,60 });
								ImGui::Text(FileDragSourceName.c_str());
								ImGui::EndDragDropSource();
							}
							if (assetType == AssetType::MeshAsset && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
								UUID staticID = GetIDCurrentDirectory(It.path().string());
								m_owner->CreateMeshEditor(AssetManager::GetAssetShared<MeshAsset>(staticID).get());
							}
							if (assetType == AssetType::Material && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
								UUID staticID = GetIDCurrentDirectory(It.path().string());
								m_owner->CreateMaterialEdtior(AssetManager::GetAssetShared<MaterialAsset>(staticID).get());
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

						if (ImGui::MenuItem("Rename")) {
							RenameFile = Utils::FileDialogs::GetFullFileName(It.path());
							RenameFileNewName = Utils::FileDialogs::GetFileName(RenameFile);
							RenameFileExtension = Utils::FileDialogs::GetFullFileExtension(It.path());
						}
						if (ImGui::MenuItem("Reload")) {
							if (It.is_directory() == false && isScene == false) {
								AssetManager::ForceGetAssetShared<Asset>(GetIDCurrentDirectory(It.path().string()))->LoadAsset();
							}
						}
						ImGui::EndPopup();

					}
					if (Utils::FileDialogs::GetFileName(RenameFile) == Utils::FileDialogs::GetFileName(It.path()))
						Rename(It.is_directory());
					else
						ImGui::Text(Utils::FileDialogs::GetFileName(It.path()).c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered
					ImGui::NextColumn();
				}
				ImGui::Columns(1);
				ImGui::SliderFloat("Thumbnail Size",&thumbnailSize,16,512);
				ImGui::SliderFloat("Padding",&padding,0,32);
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
	UUID ContentBrowserPanel::GetIDCurrentDirectory(const std::string& Path) {
		std::ifstream testFile(Path);
		std::string line;
		uint64_t ID;

		while (std::getline(testFile,line)) {
			if (line.empty() ==false) { // first line
				if(line.substr(0,10) == "AssetType:"){
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
		if (Utils::FileDialogs::GetFileExtension(Path) == "ProofWorld")
			return true;
		return false;
	}
	void ContentBrowserPanel::Rename(bool directory){
		std::string newName = RenameFileNewName + "." + RenameFileExtension;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), RenameFileNewName.c_str());
		if (ImGui::InputTextWithHint("##Name", directory == true ? "New folder name" : "New file Name", buffer, sizeof(buffer)))
			RenameFileNewName = buffer;
		ImGui::SetKeyboardFocusHere(-1);

		if (ImGui::IsKeyPressed((int)KeyBoardKey::Enter) || ImGui::IsItemFocused() == true) {

			if (newName != RenameFile) {
				if (std::filesystem::exists(m_CurrentDirectory.string()+ "\\" +RenameFileNewName+"."+ RenameFileExtension) == false) {
					std::filesystem::rename(m_CurrentDirectory.string() + "\\" + RenameFile, m_CurrentDirectory.string() + "\\" + newName);
					AssetManager::ResetAssetInfo(GetIDCurrentDirectory(m_CurrentDirectory.string() + "\\" + newName), RenameFileNewName, m_CurrentDirectory.string() + "\\" + newName);
				}
			}
			RenameFileNewName = "";
			RenameFileExtension = "";
			RenameFile = "";
		}
	}
}