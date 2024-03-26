#include "Proofprch.h"
#include "AssetEditorDetail.h"
#include "Proof/ImGui/Editors/Panels/DetailsPanel.h"
#include "Proof/Asset/AssetManager.h"
namespace Proof
{
	AssetEditorDetail::AssetEditorDetail(const std::string& windowTitle)
		:
		AssetEditor(windowTitle.c_str())
	{
	}
	void AssetEditorDetail::SetAsset(const Count<class Asset>& asset)
	{
		m_DetailsPanel = Count<DetailsPanel>::Create(std::bind(&AssetEditorDetail::RenderDetailSettings, this));
		m_DetailsPanelName = fmt::format("Details##{}: {}",EnumReflection::EnumString( asset->GetAssetType()), asset->GetID());
	}
	void AssetEditorDetail::OnImGuiRender()
	{
		// save if any data is changed
		PF_PROFILE_FUNC();
		if (!GetAsset())return;
		m_DetailsPanel->OnImGuiRender(m_DetailsPanelName.c_str(), m_IsDetailsPanelOpen);
	}

	void AssetEditorDetail::SetDefaultLayout()
	{
		ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());
		ImGuiWindow* window = ImGui::FindWindowByName(m_DetailsPanelName.c_str());
		if (m_DetailsPanel->GetImGuiWindow())
		{
			ImGui::SetWindowDock(m_DetailsPanel->GetImGuiWindow(), dockspace_id, 0);
		}
	}
	bool AssetEditorDetail::IsSubWindowsHovered()
	{
		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
	}
	bool AssetEditorDetail::IsSubWindowsFocused()
	{
		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
	}

	void AssetEditorDetail::Save()
	{
		if (!GetAsset())return;

		m_NeedsSaving = false;
		AssetManager::SaveAsset(GetAsset()->GetID());
	}
}
