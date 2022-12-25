#include "Proofprch.h"
#include "ContentBrowserPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"

#include <ImGui/imgui.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Asset/TextureAsset/TextureAsset.h"
#include "Proof/Asset/MeshAsset.h"
#include "Proof/Asset/MaterialAsset.h"
#include "Proof/Asset/PhysicsMaterialAsset.h"


#include "Proof/Utils/PlatformUtils.h"
#include <vector>
#include "Proof/Scene/Mesh.h"


#include <yaml-cpp/yaml.h>

#include "Editor3D/Editor3D.h"
#include "Proof/Core/FrameTime.h"
#include "Editor3D/ImGUIAPI.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/AssetThumbnailGenerator.h"

#include "Proof/Asset/AssetSupport.h"
namespace Proof
{
	static float padding = 16.0f; // space between 
	static float thumbnailSize = 60;
	static std::string FileRenameName;
	//original name of file being renamed
	static std::string NameofFileRename;
	static std::filesystem::path s_AssetsPath;
	ContentBrowserPanel::ContentBrowserPanel(Editore3D* owner) :
		m_CurrentDirectory(Application::Get()->GetProject()->GetAssetDir()),
		m_Owner(owner) {
		s_AssetsPath = Application::Get()->GetProject()->GetAssetDir();

		m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		m_MeshIcon = Texture2D::Create("Resources/Icons/ContentBrowser/MeshComponentIcon.png");
		m_ArrowIcon = Texture2D::Create("Resources/Icons/ContentBrowser/ArrowIcon.png");
	}
	void ContentBrowserPanel::ImGuiRender(FrameTime deltaTime) {
		if (m_ShowWindow == false)
			return;

		PF_PROFILE_FUNC();
		//auto relativePath = std::filesystem::relative(path, g_AssetPath);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Content Browser", &m_ShowWindow);
		{
			// Content folder structure
			{
				ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0,0,0,1 });
				ImGui::BeginChild("Folders", { 200,ImGui::GetContentRegionAvail().y });
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
					if (ImGui::TreeNodeEx(s_AssetsPath.filename().string().c_str(), flags)) {
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
							m_CurrentDirectory = s_AssetsPath; // reseting before adding a new path
						}
						ImGui::TreePop();
					}
					ImGui::Separator();
					GetAllSubFolders(s_AssetsPath.string());
				}
				ImGui::EndChild();
				ImGui::PopStyleColor();
			}
			ImGui::SameLine();
			// Content Images
			{
				ImGui::BeginChild("Sub Window");
				{
					if (ImGui::Button("<-", { 50,50 })) {
						if (m_CurrentDirectory != s_AssetsPath) {
							m_CurrentDirectory = m_CurrentDirectory.parent_path();
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("Regenerate Asset Source", { 50,50 })) {
						AssetManager::GenerateAllSourceAssets();
					}
					if (ImGui::BeginPopupContextWindow(0, 1)) { // right click 
						if (ImGui::MenuItem("Folder")) {
							NameofFileRename = Utils::FileDialogs::GetFileName(NewFolder("folder"));
							FileRenameName = NameofFileRename;
						}

						if (AddAssetPopupMenuItem<MeshAsset>("Mesh", "Mesh(*.obj)\0 * .obj\0 (*.gltf)\0 * .gltf\0 (*.fbx)\0 * .fbx\0", NameofFileRename))
							FileRenameName = NameofFileRename;
						else if (AddAssetPopupMenuItem<Texture2DAsset>("Texture2D", "Texture (*.png)\0 *.png\0 (*.jpg)\0 *.jpg\0", NameofFileRename))
							FileRenameName = NameofFileRename;
						else if (AddAssetPopupMenuItem<MaterialAsset>("Material", NameofFileRename))
							FileRenameName = NameofFileRename;
						else if (AddAssetPopupMenuItem<PhysicsMaterialAsset>("PhysicsMaterial", NameofFileRename))
							FileRenameName = NameofFileRename;
						ImGui::EndPopup();
					}
					// ImageButton
					{
						float cellSize = thumbnailSize + padding;
						float panelWidth = ImGui::GetContentRegionAvail().x;
						int columnCount = (int)(panelWidth / cellSize); // amount of folder we an fit in a row
						if (columnCount < 1)
							columnCount = 1;
						ImGui::Columns(columnCount, 0, false);

						for (auto& It : std::filesystem::directory_iterator(m_CurrentDirectory)) {
							const auto& path = It.path();
							ImGui::PushID(path.filename().string().c_str());
							AssetType assetType = AssetManager::GetAssetTypeFromFilePath(It);
							if (It.is_directory()) {
								ImGui::ImageButton((ImTextureID)m_FolderIcon->GetID(), { thumbnailSize,thumbnailSize });
								if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
									m_CurrentDirectory /= path.filename(); // /= is the operator overloaded  to the next folder
								}
								goto outDir;
							}
							/*
							if (AssetThumbnailGenerator::HasThumbnail(GetIDCurrentDirectory(path.string()))) {
								ImGui::ImageButton((ImTextureID)AssetThumbnailGenerator::GetThumbnail(GetIDCurrentDirectory(path.string())), {thumbnailSize,thumbnailSize});
							}
							else {

								if (assetType == AssetType::Mesh) {
									auto info = AssetManager::GetAssetInfo(GetIDCurrentDirectory(path.string()));
									if (info.Loaded) {
										AssetThumbnailGenerator::GenerateThumbnail(GetIDCurrentDirectory(path.string()));
									}
									ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(), { thumbnailSize,thumbnailSize });
								}
							}
							*/
							ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(), { thumbnailSize,thumbnailSize });
							if (ImGui::BeginDragDropSource()) {
								std::string fileDragSourcePath = path.string();
								UUID staticID = GetIDCurrentDirectory(fileDragSourcePath);
								std::string assetTypestring = EnumReflection::EnumString(AssetManager::GetAssetTypeFromFilePath(fileDragSourcePath));
								ImGui::SetDragDropPayload(assetTypestring.c_str(), &staticID, sizeof(UUID));
								ImGui::Image((ImTextureID)m_FileIcon->GetID(), { 60,60 });
								ImGui::Text(Utils::FileDialogs::GetFileName(fileDragSourcePath).c_str());
								ImGui::EndDragDropSource();
							}
							if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
								UUID staticID = GetIDCurrentDirectory(path.string());
								if (staticID == 0)
									staticID = AssetManager::GetAssetSourceID(path,true);
								m_Owner->CreateAssetEditor(staticID);
								
							}
							outDir:
							if (ImGui::BeginPopupContextItem(path.string().c_str())) {
								if (ImGui::MenuItem("Rename")) {
									NameofFileRename = Utils::FileDialogs::GetFileName(path);
									FileRenameName = NameofFileRename;
								}
								if (assetType == AssetType::MeshSourceFile) {
									if (ImGui::MenuItem("Mesh asset")) {
										if (ImGui::BeginPopupModal("popup")) {
											ImGui::Text("Lorem ipsum");
											ImGui::EndPopup();
										}

									}
								}
								if (assetType != AssetType::MeshSourceFile && assetType != AssetType::TextureSourceFile) {
									if (ImGui::MenuItem("Reload")) {
										if (It.is_directory() == false) {
											if (AssetManager::HasID(GetIDCurrentDirectory(It.path().string()))) {
												if (AssetManager::IsAssetLoaded(GetIDCurrentDirectory(It.path().string()))) {
													AssetManager::GetAsset<Asset>(GetIDCurrentDirectory(It.path().string()))->LoadAsset();
												}
											}
										}
									}
								}
								if (ImGui::MenuItem("Delete")) {
									if (It.is_directory() == false) {
										AssetManager::Remove(GetIDCurrentDirectory(m_CurrentDirectory.string() + "\\" + Utils::FileDialogs::GetFullFileName(path)));
										std::filesystem::remove_all(m_CurrentDirectory.string() + "\\" + Utils::FileDialogs::GetFullFileName(path));
									}
									else if (It.is_directory()) {
										DeleteFolder(path.string());
									}
									else {  // if it is a scene or any other file
										//std::filesystem::remove_all(m_CurrentDirectory.string() + "\\" + fullFileName);
									}
								}
								ImGui::EndPopup();
							}
							if (NameofFileRename != Utils::FileDialogs::GetFileName(path))// file renamed
								ImGui::TextWrapped(Utils::FileDialogs::GetFileName(path).c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered
							else
								Rename(Utils::FileDialogs::GetFileName(path), Utils::FileDialogs::GetFullFileExtension(path), It.is_directory());

							ImGui::NextColumn();
							ImGui::PopID();
						}
						ImGui::Columns(1);
						ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
						ImGui::SliderFloat("Padding", &padding, 0, 32);
					}
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
	void ContentBrowserPanel::GetAllSubFolders(const std::string& path) {
		for (auto& It : std::filesystem::directory_iterator(path)) {
			if (It.is_directory() == false)continue;
			bool subFolder = false;
			for (auto& subIt : std::filesystem::directory_iterator(It.path())) { // simple way to chekc if it has another sub folder
				if (subIt.is_directory()) {
					subFolder = true;
					break;
				}
			}
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			if (subFolder == false)
				flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow
			flags |= ImGuiTreeNodeFlags_SpanFullWidth;

			std::string fileFullName = Utils::FileDialogs::GetFullFileName(It.path());
			bool opened = ImGui::TreeNodeEx((void*)fileFullName.c_str(), flags, fileFullName.c_str()); // ID is the path cause it is unique
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				m_CurrentDirectory = s_AssetsPath; // reseting before adding a new path
				m_CurrentDirectory = It.path(); // Go into the sub direcotyr 
			}
			if (opened) {
				GetAllSubFolders(It.path().string());
				ImGui::TreePop();
			}
		}
	}
	std::string ContentBrowserPanel::NewFolder(const std::string& folderName) {
		std::string name = folderName.empty() ? "folder" : folderName;
		if (folderName.empty()) {
			uint32_t index = 0;
			while (std::filesystem::exists(m_CurrentDirectory.string() + "\\" + folderName)) {
				index++;
				name = "folder(" + std::to_string(index) + ")";
			}
		}
		std::filesystem::create_directories({ m_CurrentDirectory.string() + "\\" + name });
		return { m_CurrentDirectory.string() + "\\" + name }; // retuns the full path of the folder name
	}
	void ContentBrowserPanel::RenameFolder(const std::string& fileName, const std::string& fileFullExension) {
		std::string newFullName = FileRenameName + "." + fileFullExension;

		bool nameExist = std::filesystem::exists(m_CurrentDirectory.string() + "\\" + newFullName);
		if (newFullName == NameofFileRename || nameExist == false)
			ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });
		else
			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0,0.0,0.0,1 }); // error name not valuable

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), FileRenameName.c_str());
		ImGui::SetKeyboardFocusHere(0);
		if (ImGui::InputTextWithHint("##Name", "Name", buffer, sizeof(buffer)), 0, ImGuiInputTextFlags_CallbackAlways) // do not remove text callback
			FileRenameName = buffer;
		ImGui::PopStyleColor();
		if ((ImGui::IsItemHovered() == false && ImGui::IsAnyMouseDown())) // the text no longer edited
			goto a;
		if (ImGui::IsKeyPressed((ImGuiKey)KeyBoardKey::Enter)) {
			a:
			if (nameExist == true) { // so basically if it is the same name do not change anything
				FileRenameName = "";
				ExternalAPI::ImGUIAPI::SetKeyboardFocusOff();
				NameofFileRename = "";
				return;
			}
			const auto& fileOldFullName = m_CurrentDirectory.string() + "\\" + fileName + "." + fileFullExension; // includes path
			const auto& fileNewFullName = m_CurrentDirectory.string() + "\\" + newFullName; // includes path
			std::filesystem::rename(fileOldFullName, fileNewFullName);
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(fileNewFullName)) {// through every file
				if (AssetManager::GetAssetTypeFromFilePath(dirEntry.path()) != AssetType::None) {
					Asset* asset = AssetManager::GetAsset<Asset>(GetIDCurrentDirectory(dirEntry.path().string())).get();
					if (asset != nullptr)
						AssetManager::ResetAssetInfo(GetIDCurrentDirectory(dirEntry.path().string()), dirEntry.path().string());
				}
			}
			ExternalAPI::ImGUIAPI::SetKeyboardFocusOff();
			NameofFileRename = "";
			FileRenameName = "";
			return;
		}
	}
	void ContentBrowserPanel::RenameFile(const std::string& fileName, const std::string& fileFullExension) {

		std::string newFullName = FileRenameName + "." + fileFullExension;

		bool nameExist = std::filesystem::exists(m_CurrentDirectory.string() + "\\" + newFullName);
		if (newFullName == NameofFileRename || nameExist == false)
			ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });
		else
			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0,0.0,0.0,1 }); // error name not valuable

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), FileRenameName.c_str());
		ImGui::SetKeyboardFocusHere(0);
		if (ImGui::InputTextWithHint("##Name", "Name", buffer, sizeof(buffer)), 0, ImGuiInputTextFlags_CallbackAlways) // do not remove text callback
			FileRenameName = buffer;
		ImGui::PopStyleColor();

		if ((ImGui::IsItemHovered() == false && ImGui::IsAnyMouseDown())) // the text no longer edited
			goto a;
		if (ImGui::IsKeyPressed((ImGuiKey)KeyBoardKey::Enter)) {
			a:
			if (nameExist == true) {// so basically if it is the same name do not change anything
				FileRenameName = "";
				ExternalAPI::ImGUIAPI::SetKeyboardFocusOff();
				NameofFileRename = "";
				return;
			}
			const auto& fileOldFullName = m_CurrentDirectory.string() + "\\" + fileName + "." + fileFullExension; // includes path
			const auto& fileNewFullName = m_CurrentDirectory.string() + "\\" + newFullName; // includes path
			std::filesystem::rename(fileOldFullName, fileNewFullName);
			AssetManager::ResetAssetInfo(GetIDCurrentDirectory(fileNewFullName), fileNewFullName);

			FileRenameName = "";
			ExternalAPI::ImGUIAPI::SetKeyboardFocusOff();
			NameofFileRename = "";
			return;
		}
	}

	void ContentBrowserPanel::DeleteFolder(const std::string& path) {
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(path)) {// through every file
			if (dirEntry.is_directory()) {
				DeleteFolder(dirEntry.path().string());
			}
			if (AssetManager::GetAssetTypeFromFilePath(dirEntry.path()) != AssetType::None) {
				AssetManager::Remove(GetIDCurrentDirectory(dirEntry.path().string()));
			}
		}

		std::filesystem::remove_all(path); // DELETING THE FOLDER
	}
	UUID ContentBrowserPanel::GetIDCurrentDirectory(const std::string& path) {
		std::ifstream testFile(path);
		std::string line;
		uint64_t ID;

		while (std::getline(testFile, line)) {
			if (line.empty() == false) { // first line
				if (line.substr(0, 10) == "AssetType:") {
					break;
				}
				return 0;
			}
		}
		while (std::getline(testFile, line)) {
			if (line.substr(0, 3) == "ID:") {
				std::stringstream ss(line.substr(3, line.size()));
				ss >> ID;
				return ID;
			}
		}
		return 0;
	}

	void GenerateThumbnail(UUID ID) {

	}

}