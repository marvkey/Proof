#include "ContentBrowserUtils.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIColors.h"
#include "Proof/ImGui/UiUtilities.h"
#include "../ApplicationSettings.h"
#include "Proof/Utils/StringUtils.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Utils/StringUtils.h"
#include "Editor3D/EditorResources.h"
#include "ContentBrowserPanel.h"

#include "Proof/Asset/AssetManager.h"
#include <imgui.h>
#include "imgui_internal.h"

namespace Proof
{
	static char s_RenameBuffer[MAX_INPUT_BUFFER_LENGTH];

	ContentBrowserItem::ContentBrowserItem(ItemType type, AssetID id, const std::string& name, const Count<Texture2D>& icon):
		m_Type(type),m_ID(id), m_FileName(name), m_Icon(icon)
	{
		m_DisplayName = m_FileName;
		if (m_FileName.size() > 25)
			m_DisplayName = m_FileName.substr(0, 25) + "...";
	}

	void ContentBrowserItem::OnRenderBegin()
	{
		ImGui::PushID(&m_ID);
		ImGui::BeginGroup();
	}
	CBItemActionResult ContentBrowserItem::OnRender()
	{
		CBItemActionResult result;

		const auto& editorSettings = ApplicationSettings::Get();
		const float thumbnailSize = editorSettings.ContentBrowserThumbnailSize;
		const bool displayAssetType = editorSettings.ContentBrowserShowAssetTypes;

		SetDisplayNameFromFileName();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		const float edgeOffset = 4.0f;

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + edgeOffset * 2.0f;
		const float infoPanelHeight = std::max(displayAssetType ? thumbnailSize * 0.5f : textLineHeight, textLineHeight);


		const bool isFocused = ImGui::IsWindowFocused();

		const bool isSelected = SelectionManager::IsSelected(SelectionContext::ContentBrowser, m_ID);

		ImGui::InvisibleButton("##thumbnailButton", ImVec2{ thumbnailSize, thumbnailSize });
		UI::DrawButtonImage(m_Icon, IM_COL32(255, 255, 255, 225),
			IM_COL32(255, 255, 255, 255),
			IM_COL32(255, 255, 255, 255),
			UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));

		// Info Panel
		//-----------

		auto renamingWidget = [&]
		{
			ImGui::SetKeyboardFocusHere();
			ImGui::InputText("##rename", s_RenameBuffer, MAX_INPUT_BUFFER_LENGTH);

			if (ImGui::IsItemDeactivatedAfterEdit() || Input::IsKeyHold(KeyBoardKey::Enter))
			{
				Rename(s_RenameBuffer);
				m_IsRenaming = false;
				SetDisplayNameFromFileName();
				result.Set(ContentBrowserAction::Renamed, true);
			}
		};

		UI::ShiftCursor(edgeOffset, edgeOffset);

		if (m_Type == ItemType::Directory)
		{

			ImGui::BeginVertical((std::string("InfoPanel") + m_DisplayName).c_str(), ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
			{
				ImGui::BeginHorizontal(m_FileName.c_str(), ImVec2(thumbnailSize - 2.0f, 0.0f));
				ImGui::Spring();
				{
					ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 3.0f));
					const float textWidth = std::min(ImGui::CalcTextSize(m_DisplayName.c_str()).x, thumbnailSize);
					if (m_IsRenaming)
					{
						ImGui::SetNextItemWidth(thumbnailSize - edgeOffset * 3.0f);
						renamingWidget();
					}
					else
					{
						ImGui::SetNextItemWidth(textWidth);
						ImGui::Text(m_DisplayName.c_str());
					}
					ImGui::PopTextWrapPos();
				}
				ImGui::Spring();
				ImGui::EndHorizontal();

				ImGui::Spring();
				ImGui::EndVertical();
			}
		}
		else
		{
			ImGui::BeginVertical((std::string("InfoPanel") + m_DisplayName).c_str(), ImVec2(thumbnailSize - edgeOffset * 3.0f, infoPanelHeight - edgeOffset));
			{
				ImGui::BeginHorizontal("label", ImVec2(0.0f, 0.0f));

				ImGui::SuspendLayout();
				ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 2.0f));
				if (m_IsRenaming)
				{
					ImGui::SetNextItemWidth(thumbnailSize - edgeOffset * 3.0f);
					renamingWidget();
				}
				else
				{
					ImGui::Text(m_DisplayName.c_str());
				}
				ImGui::PopTextWrapPos();
				ImGui::ResumeLayout();

				ImGui::Spring();
				ImGui::EndHorizontal();
			}
			ImGui::Spring();
#if 0
			if (displayAssetType)
			{
				UI::ShiftCursorX(edgeOffset);
				ImGui::BeginHorizontal("assetType", ImVec2(0.0f, 0.0f));
				ImGui::Spring();
				{
					const AssetInfo& metadata = AssetManager::GetAssetInfo(m_ID);
					std::string& assetType = Utils::String::ToUpper(Utils::AssetTypeToString(metadata.Type));
					UI::ScopedColour textColour(ImGuiCol_Text, Colours::Theme::textDarker);
					if (thumbnailSize < 128)
					{
						UI::Fonts::PushFont("ExtraSmall");
					}
					else
						UI::Fonts::PushFont("Small");
					ImGui::TextUnformatted(assetType.c_str());
					UI::Fonts::PopFont();
				}
				ImGui::EndHorizontal();

				ImGui::Spring(-1.0f, edgeOffset);
			}
#endif

			ImGui::EndVertical();
		}


		UI::ShiftCursor(-edgeOffset, -edgeOffset);
		if (!m_IsRenaming)
		{
			if (Input::IsKeyHold(KeyBoardKey::F2) && isSelected && isFocused)
				StartRenaming();
		}

		ImGui::PopStyleVar(); // ItemSpacing

		// End of the Item Group
		//======================
		ImGui::EndGroup();


		if (isSelected || ImGui::IsItemHovered())
		{
			ImRect itemRect = UI::GetItemRect();
			auto* drawList = ImGui::GetWindowDrawList();

			if (isSelected)
			{
				const bool mouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered();
				ImColor colTransition = UI::ColourWithMultipliedValue(UI::Colours::Theme::Selection, 0.8f);

				drawList->AddRect(itemRect.Min, itemRect.Max,
					mouseDown ? UI::Colours::ConvertToColorRGBAuint( colTransition) : UI::Colours::Theme::Selection, 6.0f,
					m_Type == ItemType::Directory ? 0 : ImDrawFlags_RoundCornersBottom, 1.0f);
			}
			else // isHovered
			{
				if (m_Type != ItemType::Directory)
				{
					drawList->AddRect(itemRect.Min, itemRect.Max,
						UI::Colours::Theme::Muted, 6.0f,
						ImDrawFlags_RoundCornersBottom, 1.0f);
				}
			}
		}

		UpdateDrop(result);

		bool dragging = false;
		if (dragging = ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			m_IsDragging = true;

			const auto& selectionStack = SelectionManager::GetSelections(SelectionContext::ContentBrowser);
			if (!SelectionManager::IsSelected(SelectionContext::ContentBrowser, m_ID))
				result.Set(ContentBrowserAction::ClearSelections, true);

			auto& currentItems = ContentBrowserPanel::Get().GetCurrentItems();

			if (selectionStack.size() > 0)
			{
				for (const auto& selectedItemHandles : selectionStack)
				{
					size_t index = currentItems.FindItem(selectedItemHandles);
					if (index == ContentBrowserItemList::InvalidItem)
						continue;

					const auto& item = currentItems[index];
					UI::Image(item->GetIcon(), ImVec2(20, 20));
					ImGui::SameLine();
					const auto& name = item->GetName();
					ImGui::TextUnformatted(name.c_str());
				}

				ImGui::SetDragDropPayload("asset_payload", selectionStack.data(), sizeof(AssetID) * selectionStack.size());
			}

			result.Set(ContentBrowserAction::Selected, true);
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered())
		{
			result.Set(ContentBrowserAction::Hovered, true);

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !m_IsRenaming)
			{
				result.Set(ContentBrowserAction::Activated, true);
			}
			else
			{
				bool action = SelectionManager::GetSelectionCount(SelectionContext::ContentBrowser) > 1 ? ImGui::IsMouseReleased(ImGuiMouseButton_Left) : ImGui::IsMouseClicked(ImGuiMouseButton_Left);
				const bool isSelected = SelectionManager::IsSelected(SelectionContext::ContentBrowser, m_ID);
				bool skipBecauseDragging = m_IsDragging && isSelected;
				if (action && !skipBecauseDragging)
				{
					if (isSelected && Input::IsKeyHold(KeyBoardKey::LeftControl) && !m_JustSelected)
					{
						result.Set(ContentBrowserAction::Deselected, true);
					}

					if (m_JustSelected)
						m_JustSelected = false;

					if (!isSelected)
					{
						result.Set(ContentBrowserAction::Selected, true);
						m_JustSelected = true;
					}

					if (!Input::IsKeyHold(KeyBoardKey::LeftControl) && !Input::IsKeyHold(KeyBoardKey::LeftShift) && m_JustSelected)
						result.Set(ContentBrowserAction::ClearSelections, true);

					if (Input::IsKeyHold(KeyBoardKey::LeftShift))
						result.Set(ContentBrowserAction::SelectToHere, true);
				}
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
		if (ImGui::BeginPopupContextItem("CBItemContextMenu"))
		{
			result.Set(ContentBrowserAction::Selected, true);
			OnContextMenuOpen(result);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		m_IsDragging = dragging;

		return result;
	}
	void ContentBrowserItem::OnRenderEnd()
	{
		ImGui::PopID();
		ImGui::NextColumn();
	}

	void ContentBrowserItem::StartRenaming()
	{
		if (m_IsRenaming)
			return;

		memset(s_RenameBuffer, 0, MAX_INPUT_BUFFER_LENGTH);
		memcpy(s_RenameBuffer, m_FileName.c_str(), m_FileName.size());
		m_IsRenaming = true;
	}

	void ContentBrowserItem::StopRenaming()
	{
		m_IsRenaming = false;
		SetDisplayNameFromFileName();
		memset(s_RenameBuffer, 0, MAX_INPUT_BUFFER_LENGTH);
	}

	void ContentBrowserItem::Rename(const std::string& newName)
	{
		OnRenamed(newName);
	}

	void ContentBrowserItem::SetDisplayNameFromFileName()
	{
		const auto& editorSettings = ApplicationSettings::Get();
		const float thumbnailSize = editorSettings.ContentBrowserThumbnailSize;

		int maxCharacters = 0.00152587f * (thumbnailSize * thumbnailSize); // 0.00152587f is a magic number that is gained from graphing this equation in desmos and setting the y=25 at x=128

		if (m_FileName.size() > maxCharacters)
			m_DisplayName = m_FileName.substr(0, maxCharacters) + " ...";
		else
			m_DisplayName = m_FileName;
	}

	void ContentBrowserItem::OnContextMenuOpen(CBItemActionResult& actionResult)
	{
		if (ImGui::MenuItem("Reload"))
			actionResult.Set(ContentBrowserAction::Reload, true);

		if (SelectionManager::GetSelectionCount(SelectionContext::ContentBrowser) == 1 && ImGui::MenuItem("Rename"))
			actionResult.Set(ContentBrowserAction::StartRenaming, true);

		if (ImGui::MenuItem("Rename"))
			actionResult.Set(ContentBrowserAction::StartRenaming, true);


		if (ImGui::MenuItem("Copy"))
			actionResult.Set(ContentBrowserAction::Copy, true);

		if (ImGui::MenuItem("Duplicate"))
			actionResult.Set(ContentBrowserAction::Duplicate, true);

		if (ImGui::MenuItem("Delete"))
			actionResult.Set(ContentBrowserAction::OpenDeleteDialogue, true);

		ImGui::Separator();

		if (ImGui::MenuItem("Show In Explorer"))
			actionResult.Set(ContentBrowserAction::ShowInExplorer, true);

		if (ImGui::MenuItem("Open Externally"))
			actionResult.Set(ContentBrowserAction::OpenExternal, true);

		RenderCustomContextItems();
	}

	ContentBrowserDirectory::ContentBrowserDirectory(const Count<DirectoryInfo>& directoryInfo)
		: ContentBrowserItem(ContentBrowserItem::ItemType::Directory, directoryInfo->Handle, directoryInfo->FilePath.filename().string(), EditorResources::FolderIcon), m_DirectoryInfo(directoryInfo)
	{
	}

	ContentBrowserDirectory::~ContentBrowserDirectory()
	{
	}

	void ContentBrowserDirectory::OnRenamed(const std::string& newName)
	{
		auto target = Application::Get()->GetProject()->GetAssetDirectory() / m_DirectoryInfo->FilePath;
		auto destination = Application::Get()->GetProject()->GetAssetDirectory() / m_DirectoryInfo->FilePath.parent_path() / newName;

		if (Utils::String::ToLower(newName) == Utils::String::ToLower(target.filename().string()))
		{
			auto tmp = Application::Get()->GetProject()->GetAssetDirectory() / m_DirectoryInfo->FilePath.parent_path() / "TempDir";
			FileSystem::Rename(target, tmp);
			target = tmp;
		}

		if (!FileSystem::Rename(target, destination))
		{
			PF_ENGINE_ERROR("Couldn't rename {0} to {1}!", m_DirectoryInfo->FilePath.filename().string(), newName);
		}
	}


	void ContentBrowserDirectory::UpdateDrop(CBItemActionResult& actionResult)
	{
		if (SelectionManager::IsSelected(SelectionContext::ContentBrowser, m_ID))
			return;

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("asset_payload");

			if (payload)
			{
				auto& currentItems = ContentBrowserPanel::Get().GetCurrentItems();
				uint32_t count = payload->DataSize / sizeof(AssetID);

				for (uint32_t i = 0; i < count; i++)
				{
					AssetID assetHandle = *(((AssetID*)payload->Data) + i);
					size_t index = currentItems.FindItem(assetHandle);
					if (index != ContentBrowserItemList::InvalidItem)
					{
						if (currentItems[index]->Move(m_DirectoryInfo->FilePath))
						{
							actionResult.Set(ContentBrowserAction::Refresh, true);
							currentItems.erase(assetHandle);
						}
					}
				}
			}

			ImGui::EndDragDropTarget();
		}
	}

	void ContentBrowserDirectory::Delete()
	{
		bool deleted = FileSystem::DeleteFile(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath);
		if (!deleted)
		{
			PF_ENGINE_ERROR("Failed to delete folder {0}", m_DirectoryInfo->FilePath.string());
			return;
		}

		for (auto asset : m_DirectoryInfo->Assets)
			AssetManager::Remove(asset);
	}

	bool ContentBrowserDirectory::Move(const std::filesystem::path& destination)
	{
		bool wasMoved = FileSystem::MoveFile(Project::GetActive()->GetAssetDirectory() / m_DirectoryInfo->FilePath, Project::GetActive()->GetAssetDirectory() / destination);
		if (!wasMoved)
			return false;

		return true;
	}

	ContentBrowserAsset::ContentBrowserAsset(const AssetInfo& assetInfo, const Count<Texture2D>& icon)
		: ContentBrowserItem(ContentBrowserItem::ItemType::Asset, assetInfo.ID, assetInfo.Path.stem().string(), icon), m_AssetInfo(assetInfo)
	{
	}

	ContentBrowserAsset::~ContentBrowserAsset()
	{

	}

	void ContentBrowserAsset::Delete()
	{
		auto filepath = AssetManager::GetAssetFileSystemPath(m_AssetInfo.Path);
		bool deleted = FileSystem::DeleteFile(filepath);
		if (!deleted)
		{
			PF_ENGINE_ERROR("Couldn't delete {0}", m_AssetInfo.Path.string());
			return;
		}

		auto currentDirectory = ContentBrowserPanel::Get().GetDirectory(m_AssetInfo.Path.parent_path());
		currentDirectory->Assets.erase(std::remove(currentDirectory->Assets.begin(), currentDirectory->Assets.end(), m_AssetInfo.ID), currentDirectory->Assets.end());
		AssetManager::Remove(m_AssetInfo.ID);
	}

	bool ContentBrowserAsset::Move(const std::filesystem::path& destination)
	{
		auto filepath = AssetManager::GetAssetFileSystemPath(m_AssetInfo.Path);
		bool wasMoved = FileSystem::MoveFile(filepath, Project::GetActive()->GetAssetDirectory() / destination);
		if (!wasMoved)
		{
			PF_ENGINE_ERROR("Couldn't move {0} to {1}", m_AssetInfo.Path.string(), destination.string());
			return false;
		}

		AssetManager::ChangeAssetPath(m_AssetInfo.ID, destination / filepath.filename());
		return true;
	}

	void ContentBrowserAsset::OnRenamed(const std::string& newName)
	{
		FileSystem::SkipNextFileSystemChange();

		auto filepath = AssetManager::GetAssetFileSystemPath(m_AssetInfo.Path);
		const std::string extension = filepath.extension().string();
		std::filesystem::path newFilepath = fmt::format("{0}\\{1}{2}", filepath.parent_path().string(), newName, extension);

		std::string targetName = fmt::format("{0}{1}", newName, extension);
		if (Utils::String::ToLower(targetName) == Utils::String::ToLower(filepath.filename().string()))
		{
			FileSystem::RenameFilename(filepath, "temp-rename");
			filepath = fmt::format("{0}\\temp-rename{1}", filepath.parent_path().string(), extension);
		}

		FileSystem::SkipNextFileSystemChange();

		if (FileSystem::RenameFilename(filepath, newName))
		{
			// Update AssetManager with new name
			//auto& metadata = Project::GetEditorAssetManager()->GetMetadata(m_AssetInfo.Handle);
			//const auto& info = AssetManager::GetAssetInfo(m_AssetInfo.ID);

			AssetManager::ChangeAssetPath(m_AssetInfo.ID, newFilepath);
			//Project::GetEditorAssetManager()->OnAssetRenamed(m_AssetInfo.Handle, newFilepath);
		}
		else
		{
			PF_ENGINE_ERROR("Couldn't rename {0} to {1}!", filepath.filename().string(), newName);
		}
	}
}

