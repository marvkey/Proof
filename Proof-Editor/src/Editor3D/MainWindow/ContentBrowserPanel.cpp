#include "Proofprch.h"
#include "ContentBrowserPanel.h"
#include "Proof/ImGui/ImGuiLayer.h"

#include <ImGui/imgui.h>
#include "Proof/Renderer/Texture.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Asset/Asset.h"


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
	struct CurrentFileInfo {
		AssetType AssetType = AssetType::None;
		// if assetManager Has asset surce
		bool HasAsset = false;
		AssetID ID{ 0 };
	};
	static float padding = 16.0f; // space between 
	static float thumbnailSize = 60;
	static std::string FileRenameName;
	//original name of file being renamed
	static std::string NameofFileRename;
	static std::filesystem::path s_PathCreateMesh = {};
	static std::filesystem::path s_AssetsDir;
	ContentBrowserPanel::ContentBrowserPanel(Editore3D* owner) :
		m_CurrentDirectory(Application::Get()->GetProject()->GetAssetDirectory()),
		m_Owner(owner) {
		s_AssetsDir = Application::Get()->GetProject()->GetAssetDirectory();
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
		// Content folder structure
		ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0,0,0,1 });
		ImGui::BeginChild("Folders", { 200,ImGui::GetContentRegionAvail().y });
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
			if (ImGui::TreeNodeEx(s_AssetsDir.filename().string().c_str(), flags)) {
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
					m_CurrentDirectory = s_AssetsDir; // reseting before adding a new path
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
			GetAllSubFolders(s_AssetsDir);
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ContentSubWindow();
		ImGui::End();
		ImGui::PopStyleVar();
	}

	AssetID AddMeshFunc(const std::filesystem::path& meshPath, std::filesystem::path savePath, const std::vector<uint32_t>& excludeIndex) {
		if (std::filesystem::is_directory(savePath)) {
			if (std::filesystem::exists(savePath) == false)
				std::filesystem::create_directory(savePath);
			// making it a file
			savePath = std::filesystem::relative(savePath /= Utils::FileDialogs::GetFileName(savePath),s_AssetsDir);
		}
		else {
			//fix / backlash issue
			savePath = std::filesystem::relative(savePath, s_AssetsDir);
		}
		std::string finalPath = savePath.string();

		if (std::filesystem::exists(std::filesystem::path(finalPath).parent_path()) == false) {
			std::filesystem::create_directory(std::filesystem::path(finalPath).parent_path());
		}
		uint32_t endIndex = 0; // the ending index of a file like file(0) or file(1)
		while (std::filesystem::exists(finalPath + fmt::format("({})", endIndex))) {
			endIndex++;
		}
		if (endIndex != 0)
			finalPath += fmt::format("({})", endIndex);

		PF_CORE_ASSERT(false);
		return 0;
		//return AssetManager::NewAsset<Mesh>(finalPath, meshPath.string())->GetID();
	}
	std::pair<bool, AssetID> ContentBrowserPanel::AddMesh(const std::filesystem::path& meshPath, const std::vector<uint32_t>& excludeIndex) {
		static std::string meshAddedSavePath = "Mesh\\";
		bool returnValue = false;
		if(ImGui::IsPopupOpen("Add Mesh") ==false)
			ImGui::OpenPopup("Add Mesh");
		AssetID ID = 0;
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Add Mesh", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			returnValue = true;
			char buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), meshAddedSavePath.c_str());
			ImGui::SameLine();
			ImGui::Text(s_AssetsDir.string().c_str());
			if (ImGui::InputText("##Path",buffer, sizeof(buffer))) // do not remove text callback
				meshAddedSavePath = buffer;
			ImGui::SetItemDefaultFocus();

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) {
				auto assetDirCopy = s_AssetsDir;
				// we are copyying because when we apply /= opperator it affects teh s_AssetDir to be dfferent
				std::string savedPath = std::filesystem::path{ assetDirCopy /= meshAddedSavePath }.string();
				ID =AddMeshFunc(meshPath, savedPath, excludeIndex);
				returnValue = false;
				meshAddedSavePath = "Mesh";
				s_PathCreateMesh = "";
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				meshAddedSavePath = "Mesh";
				returnValue = false;
				s_PathCreateMesh = "";
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::EndPopup();
		}
		return { returnValue,ID };
	}
	AssetID AddWorldFunc(Count<World> world,std::filesystem::path savePath) {
		if (std::filesystem::is_directory(savePath))
		{
			if (std::filesystem::exists(savePath) == false)
				std::filesystem::create_directory(savePath);
			// making it a file
			savePath = std::filesystem::relative(savePath /= Utils::FileDialogs::GetFileName(savePath));
		}
		else
		{
	  //fix / backlash issue
			savePath = std::filesystem::relative(savePath);
		}
		std::string finalPath = savePath.string();

		if (std::filesystem::exists(std::filesystem::path(finalPath).parent_path()) == false)
		{
			std::filesystem::create_directory(std::filesystem::path(finalPath).parent_path());
		}
		uint32_t endIndex = 0; // the ending index of a file like file(0) or file(1)
		while (std::filesystem::exists(finalPath + fmt::format("({})", endIndex)))
		{
			endIndex++;
		}
		if (endIndex != 0)
			finalPath += fmt::format("({})", endIndex);

		SceneSerializer serilizer(world.Get());
		serilizer.SerilizeText(finalPath);

		AssetID ID = world->GetID();
		AssetManager::AddWorldAsset(ID, finalPath);
		return ID;
	}
	std::pair<bool, AssetID> ContentBrowserPanel::AddWorld(Count<World> world)
	{
		static std::string worldAddedSavePath = "Scenes\\";
		bool returnValue = false;
		if (ImGui::IsPopupOpen("Add World") == false)
			ImGui::OpenPopup("Add World");
		AssetID ID = 0;
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Add World", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			returnValue = true;
			char buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), worldAddedSavePath.c_str());
			ImGui::SameLine();
			ImGui::Text(s_AssetsDir.string().c_str());
			if (ImGui::InputText("##Path", buffer, sizeof(buffer))) // do not remove text callback
				worldAddedSavePath = buffer;
			ImGui::SetItemDefaultFocus();

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				auto assetDirCopy = s_AssetsDir;
				// we are copyying because when we apply /= opperator it affects teh s_AssetDir to be dfferent
				std::string savedPath = std::filesystem::path{ assetDirCopy /= worldAddedSavePath }.string();
				ID = AddWorldFunc(world, savedPath);
				returnValue = false;
				worldAddedSavePath = "Scenes\\";
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				worldAddedSavePath = "Scenes\\";
				returnValue = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		return { returnValue,ID };
	}

	void ContentBrowserPanel::ContentSubWindow() {
		ImGui::BeginChild("Sub Window");
		if (ImGui::Button("<-", { 50,50 })) {
			if (m_CurrentDirectory != s_AssetsDir) {
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

			if (AddAssetPopupMenuItem<Texture2D>("Texture2D", "Texture (*.png)\0 *.png\0 (*.jpg)\0 *.jpg\0", NameofFileRename))
				FileRenameName = NameofFileRename;
			else if (AddAssetPopupMenuItem<Material>("Material", NameofFileRename))
				FileRenameName = NameofFileRename;
			else if (AddAssetPopupMenuItem<PhysicsMaterial>("PhysicsMaterial", NameofFileRename))
				FileRenameName = NameofFileRename;
			ImGui::EndPopup();
		}
		// ImageButton
		float cellSize = thumbnailSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize); // amount of folder we an fit in a row
		if (columnCount < 1)
			columnCount = 1;
		static bool createMesh = false;
		ImGui::Columns(columnCount, 0, false);

		for (auto& It : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			// wierd bug when we dont use relateive the first "\" becomes "/"
			const auto path = std::filesystem::relative(It.path(),s_AssetsDir);
			CurrentFileInfo currentFileInfo;
			if (AssetManager::HasAsset(path)) {
				auto info = AssetManager::GetAssetInfo(path);
				currentFileInfo.AssetType = info.Type;
				currentFileInfo.HasAsset = true;
				currentFileInfo.ID = info.ID;
			}
			else {
				std::string extension = Utils::FileDialogs::GetFileExtension(path);
				if (TextureHasFormat(extension))
					currentFileInfo.AssetType = AssetType::TextureSourceFile;
				else if (MeshHasFormat(extension))
					currentFileInfo.AssetType = AssetType::MeshSourceFile;
				currentFileInfo.HasAsset = false;
			}
			// has to be an assettype
			if (currentFileInfo.AssetType == AssetType::None && It.is_directory() == false)
				continue;
			ImGui::PushID(path.filename().string().c_str());

			if (It.is_directory()) {
				ImGui::ImageButton((ImTextureID)m_FolderIcon->GetImage().SourceImage, { thumbnailSize,thumbnailSize });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
					// wierd bug when we dont use relateive the first "\" becomes "/"
					m_CurrentDirectory =std::filesystem::relative( m_CurrentDirectory /= path.filename()); // /= is the operator overloaded  to the next folder
				}
				goto FolderorFile;
			}
			// file only
			//we have to use this m_fileicon get for some reason crashing
			/*
			switch (currentFileInfo.AssetType)
			{
			case Proof::AssetType::Mesh:
				ImGui::Button("Mesh", { thumbnailSize,thumbnailSize });
				break;
			case Proof::AssetType::Texture:
				ImGui::Button("Texture", { thumbnailSize,thumbnailSize });
				break;
			case Proof::AssetType::Material:
				ImGui::Button("Material", { thumbnailSize,thumbnailSize });
				break;
			case Proof::AssetType::World:
				ImGui::Button("World", { thumbnailSize,thumbnailSize });
				break;
			case Proof::AssetType::MeshSourceFile:
				ImGui::Button("MeshSourceFile", { thumbnailSize,thumbnailSize });
				break;
			case Proof::AssetType::PhysicsMaterial:
				ImGui::Button("PhysicsMaterial", { thumbnailSize,thumbnailSize });
				break;
			case Proof::AssetType::TextureSourceFile:
				ImGui::Button("TextureSourceFile", { thumbnailSize,thumbnailSize });
				break;
			default:
				ImGui::Button("File", { thumbnailSize,thumbnailSize });
				break;
			}
			*/
			// not working for somereason 
			ImGui::ImageButton((ImTextureID)m_FileIcon->GetImage().SourceImage, {thumbnailSize,thumbnailSize});
			if (ImGui::BeginDragDropSource()) {
				std::string fileDragSourcePath = path.string();
				// we doingthis becausefor loop and dragsurce may change
				if (AssetManager::HasAsset(fileDragSourcePath) == false) {
					//AssetManager::NewAssetSource(fileDragSourcePath,AssetManager::GetAssetTypeFromFilePath(fileDragSourcePath));
				}
				auto staticAssetInfo = AssetManager::GetAssetInfo(fileDragSourcePath);
				UUID staticID = AssetManager::GetAssetInfo(fileDragSourcePath).ID;
				std::string assetTypestring = EnumReflection::EnumString(staticAssetInfo.Type);
				ImGui::SetDragDropPayload(assetTypestring.c_str(), &staticID, sizeof(UUID));

				//ImGui::Image((ImTextureID)m_FileIcon->GetID(), { 60,60 });
				ImGui::Button(Utils::FileDialogs::GetFileName(fileDragSourcePath).c_str(), { 60,60 });
				ImGui::EndDragDropSource();
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				UUID staticID = AssetManager::GetAssetInfo(path).ID;
				m_Owner->CreateAssetEditor(staticID);
			}
			FolderorFile:
			if (ImGui::BeginPopupContextItem(path.string().c_str())) {
				if (ImGui::MenuItem("Rename")) {
					NameofFileRename = Utils::FileDialogs::GetFileName(path);
					FileRenameName = NameofFileRename;
				}
				if (currentFileInfo.AssetType != AssetType::MeshSourceFile && currentFileInfo.AssetType != AssetType::TextureSourceFile && It.is_directory() == false) {
					if (ImGui::MenuItem("Reload")) {
						if (AssetManager::IsAssetLoaded(currentFileInfo.ID)) {
							//AssetManager::GetAsset<Asset>(currentFileInfo.ID)->LoadAsset();
						}
					}

				}
				if (currentFileInfo.AssetType == AssetType::MeshSourceFile) {
					if (ImGui::MenuItem("Create Mesh")) {
						createMesh = true;
						s_PathCreateMesh = path;
					}
				}
				if (ImGui::MenuItem("Delete")) {
					if (It.is_directory() == false) {
						AssetManager::Remove(currentFileInfo.ID);
						std::filesystem::remove_all(m_CurrentDirectory.string() + "\\" + Utils::FileDialogs::GetFullFileName(path));
					}
					else {
						DeleteFolder(path.string());
					}
				}
				ImGui::EndPopup();
			}

			if (NameofFileRename != Utils::FileDialogs::GetFileName(path))// file renamed
			{	
				if (currentFileInfo.AssetType == AssetType::MeshSourceFile || currentFileInfo.AssetType == AssetType::TextureSourceFile) {
					ImGui::TextWrapped(Utils::FileDialogs::GetFullFileName(path).c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered
				
				}
				else {
					ImGui::TextWrapped(Utils::FileDialogs::GetFileName(path).c_str());// HAS TO BE HERE BECAUSE it will mess up item hovered
				}
			}
			else
				Rename(Utils::FileDialogs::GetFileName(path), Utils::FileDialogs::GetFullFileExtension(path), It.is_directory());

			
			ImGui::NextColumn();
			ImGui::PopID();
		}
		if (createMesh == true) {
			AssetID id;
			std::tie(createMesh,id) = AddMesh(s_PathCreateMesh);
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);
		ImGui::EndChild();
	}
	void ContentBrowserPanel::GetAllSubFolders(const std::filesystem::path& path) {
		auto relateivePath = std::filesystem::relative(path);
		for (auto& It : std::filesystem::directory_iterator(relateivePath)) {
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
				m_CurrentDirectory = s_AssetsDir; // reseting before adding a new path
				m_CurrentDirectory = std::filesystem::relative(It.path()); // /= is the operator overloaded  to the next folder
			}
			if (opened) {
				GetAllSubFolders(It.path());
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

		char buffer[1024];
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
				if (AssetManager::HasAsset(dirEntry.path())) {
					AssetInfo assetInfo = AssetManager::GetAssetInfo(dirEntry.path());
					AssetManager::ResetAssetPath(assetInfo.ID, dirEntry.path().string());
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

		char buffer[1024];
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
			const auto fileOldFullName = m_CurrentDirectory.string() + "\\" + fileName + "." + fileFullExension; // includes path
			const auto fileNewFullName = m_CurrentDirectory.string() + "\\" + newFullName; // includes path
			std::filesystem::rename(fileOldFullName, fileNewFullName);
			if (AssetManager::HasAsset(fileOldFullName)) {
				AssetInfo assetInfo = AssetManager::GetAssetInfo(fileOldFullName);
				AssetManager::ResetAssetPath(assetInfo.ID, fileNewFullName);
			}
			FileRenameName = "";
			ExternalAPI::ImGUIAPI::SetKeyboardFocusOff();
			NameofFileRename = "";
			return;
		}
	}

	void ContentBrowserPanel::DeleteFolder(const std::string& path) {
		for (const auto& dirEntry : std::filesystem::directory_iterator(path)) {// through every file
			if (dirEntry.is_directory()) {
				DeleteFolder(dirEntry.path().string());
			}
			auto path = dirEntry.path().string();
			if (AssetManager::HasAsset(path)) {
				AssetInfo assetInfo = AssetManager::GetAssetInfo(path);
				AssetManager::Remove(assetInfo.ID);
			}
		}

		std::filesystem::remove_all(path); // DELETING THE FOLDER
	}

	void GenerateThumbnail(UUID ID) {

	}

}