#include "Proofprch.h"
#include "PrefabEditor.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/ImGui/Editors/Panels/SceneHierachyPanel.h"
#include <imgui_internal.h>
namespace Proof
{
	PrefabEditorPanel::PrefabEditorPanel()
		:
		AssetEditor("PrefabEditorPanel")
	{
	}
	void PrefabEditorPanel::OnUpdate(FrameTime ts)
	{
		if (!m_Prefab)
			return;

		AssetEditor::OnUpdate(ts);
		m_WorkSpaceManager->OnUpdate(ts);
	}
	void PrefabEditorPanel::OnImGuiRender()
	{
		if (!m_Prefab)
			return;

		static bool hierachyOpen = true;

		ImGuiID dockspace_id = ImGui::GetID((m_TitleAndId +"Dockspace").c_str());
		//ImGui::DockSpace(dockspace_id,ImVec2(0,0),)

		static auto first_time = true;
#if 1
		if (first_time)
		{
			first_time = false;
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id);
			ImGui::DockBuilderSetNodeSize(dockspace_id, GetWindowSize());

			auto dock_id_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);
			auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.5f, nullptr, &dockspace_id);
			ImGui::DockBuilderDockWindow(fmt::format(" WorldHierarchyPanel##Prefab: {}", m_Prefab->GetID()).c_str(), dock_id_up);
			ImGui::DockBuilderDockWindow(fmt::format("Viewport##PrefabEditor{}", m_Prefab->GetID()).c_str(), dock_id_down);

			ImGui::DockBuilderFinish(dockspace_id);
		}
#endif
		
		//ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_Once);
		m_WorldHierarchyPanel->OnImGuiRender(fmt::format( " WorldHierarchyPanel##Prefab: {}",  m_Prefab->GetID()).c_str(), hierachyOpen);
		//ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_Once);
		m_WorkSpaceManager->OnImGuiRender();

	}
	void PrefabEditorPanel::OnEvent(Event& e)
	{
		m_WorldHierarchyPanel->OnEvent(e);
		m_WorkSpaceManager->OnEvent(e);
	}
	void PrefabEditorPanel::OnWindowStylePush()
	{
	}
	void PrefabEditorPanel::OnWindowStylePop()
	{
	}
	void PrefabEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::Prefab)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to PrefabEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_Prefab = asset.As<Prefab>();
		m_WorkSpaceManager = CreateSpecial< EditorWorkspaceManager>();
		m_WorkSpaceManager->AddWorkspace<ViewPortEditorWorkspace>(fmt::format("Viewport##PrefabEditor{}", m_Prefab->GetID()).c_str(), true,
			fmt::format("Viewport##PrefabEditor{}", m_Prefab->GetID()).c_str(),
			ViewPortEditorData{ false,false,nullptr,false,AssetSelectionContext::Prefab,m_Prefab->GetID() })->SetWorldContext(m_Prefab->GetWorld());

		m_WorldHierarchyPanel = Count<SceneHierachyPanel>::Create(false, m_Prefab->GetID());
		m_WorldHierarchyPanel->SetWorldContext(m_Prefab->GetWorld());
	}
	void PrefabEditorPanel::Save()
	{
		m_NeedsSaving = false;
		AssetManager::SaveAsset(m_Prefab->GetID());
	}
	
}