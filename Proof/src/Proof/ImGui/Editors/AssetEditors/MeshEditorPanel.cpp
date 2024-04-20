#include "Proofprch.h"
#include "MeshEditorPanel.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/Editors/EditorWorkspace/ViewPortEditorWorkspace.h"
#include "Proof/ImGui/Editors/Panels/DetailsPanel.h"
#include "Proof/ImGui/UIHandlers.h"
#include "Proof/Utils/ContainerUtils.h"
namespace Proof
{

	MeshEditorPanel::MeshEditorPanel()
		:AssetEditor("MeshEditorPanel")
	{

	}
	void MeshEditorPanel::OnUpdate(FrameTime deltaTime)
	{
		if (!m_MeshBase)return;

		AssetEditor::OnUpdate(deltaTime);
		m_WorkSpaceManager->OnUpdate(deltaTime);
	}
	void MeshEditorPanel::OnImGuiRender()
	{
		if (!m_MeshBase)return;

		UI::PushModified(m_NeedsSaving);

		static bool detailsOpen = true;
		m_DetailsPanel->OnImGuiRender(m_DetailsPanelName.c_str(), detailsOpen);
		m_WorkSpaceManager->OnImGuiRender();
		UI::PopModified();
	}

	void MeshEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::Mesh && asset->GetAssetType() != AssetType::DynamicMesh)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to MeshEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}

		m_MeshBase = asset.As<MeshBase>();
		UpdatePreviewEntity();
	}
	void MeshEditorPanel::Save()
	{
		if (!m_MeshBase)
			return;

		m_NeedsSaving = false;
		AssetManager::SaveAsset(m_MeshBase->GetID());
	}

	void MeshEditorPanel::SetDefaultLayout()
	{
	#if 1
		ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id);
		ImGui::DockBuilderSetNodeSize(dockspace_id, GetWindowSize());

		auto dock_id_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);
		auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.5f, nullptr, &dockspace_id);
		ImGui::DockBuilderDockWindow(m_DetailsPanelName.c_str(), dock_id_up);
		ImGui::DockBuilderDockWindow(m_ViewportPanelName.c_str(), dock_id_down);

		ImGui::DockBuilderFinish(dockspace_id);
	#endif

		//ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());
		//ImGuiWindow* window = ImGui::FindWindowByName(m_DetailsPanelName.c_str());
		//if (m_DetailsPanel->GetImGuiWindow())
		//{
		//	ImGui::SetWindowDock(m_DetailsPanel->GetImGuiWindow(), dockspace_id, 0);
		//}
		//auto viewport = m_WorkSpaceManager->GetWorkspace<ViewPortEditorWorkspace>(m_ViewportPanelName.c_str());
		//if(viewport->GetImGuiWindow())
		//	ImGui::SetWindowDock(viewport->GetImGuiWindow(), dockspace_id, 0);
	}
	bool MeshEditorPanel::IsSubWindowsHovered()
	{
		for (auto workspaceData : m_WorkSpaceManager->GetWorkspaceData())
		{
			if (workspaceData.second.EditorWorkspace->IsHovered())
				return true;
		}

		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
	}
	bool MeshEditorPanel::IsSubWindowsFocused()
	{

		for (auto workspaceData : m_WorkSpaceManager->GetWorkspaceData())
		{
			if (workspaceData.second.EditorWorkspace->IsFocused())
				return true;
		}

		if (m_DetailsPanel->IsFocused())
			return true;

		return false;
	}

	void MeshEditorPanel::UpdatePreviewEntity()
	{
		m_World = Count<World>::Create();
		Entity entity = m_World->CreateEntity("Mesh");

		if (AssetManager::HasAsset(m_MeshBase))
		{
			const auto& assetInfo = AssetManager::GetAssetInfo(m_MeshBase);
			if (assetInfo.Type == AssetType::Mesh)
				entity.AddComponent<MeshComponent>().SetMesh(m_MeshBase->GetID());
			else
				entity.AddComponent<DynamicMeshComponent>().SetMesh(m_MeshBase->GetID());

		}

		{
			Entity light = m_World->CreateEntity("Light");

			light.AddComponent<DirectionalLightComponent>().Intensity = 2.5;
			light.GetComponent<TransformComponent>().SetRotationEuler(glm::radians(glm::vec3{ 0.400, 5.400, 0.100 }));

			Entity skyLight = m_World->CreateEntity("SkyLight");
			skyLight.AddComponent<SkyLightComponent>();
		}

		if (m_WorkSpaceManager)
			m_WorkSpaceManager.release();
		m_WorkSpaceManager = CreateSpecial<EditorWorkspaceManager>();
		m_DetailsPanelName = fmt::format("Details##Mesh: {}", m_MeshBase->GetID());
		m_ViewportPanelName = fmt::format("Viewport##Mesh: {}", m_MeshBase->GetID());

		ViewPortEditorData editorData;
		editorData.EnableSelection = false;
		auto viewport = m_WorkSpaceManager->AddWorkspace<ViewPortEditorWorkspace>(m_ViewportPanelName.c_str(), true, m_ViewportPanelName, editorData);
		m_WorkSpaceManager->SetWorldContext(m_World);
		m_DetailsPanel = Count<DetailsPanel>::Create(std::bind(&MeshEditorPanel::RenderSettingsPanel, this));
	}
	void MeshEditorPanel::RenderSettingsPanel()
	{
		if (m_MeshBase->GetAssetType() == AssetType::Mesh)
			RenderStaticMeshPanel();
		else
			RenderDynamicMeshPanel();
	}
	void MeshEditorPanel::RenderStaticMeshPanel()
	{
		auto staticMesh = m_MeshBase.As<Mesh>();

		{
			auto location = staticMesh->GetTranslation();
			if (UI::AttributeDrag("Translation", location, 0.25))
				staticMesh->SetTranslation(location);


			auto rotation = staticMesh->GetRotationDeg();
			if (UI::AttributeDrag("RotationDeg", rotation, 0.25))
				staticMesh->SetRotationDeg(rotation);

			auto scale = staticMesh->GetScale();
			if (UI::AttributeDrag("Scale", scale))
				staticMesh->SetScale(scale);
		}

		DrawMeshNode();
		UI::AttributeDrawMaterialTable(m_MeshBase->GetMaterialTable(), m_MeshBase->GetMeshSource()->GetMaterials());
	}
	void MeshEditorPanel::RenderDynamicMeshPanel()
	{


	}

	void MeshNodeHierarchy(Count<MeshBase> meshBase, const MeshNode& node, const glm::mat4& parentTransform, uint32_t level)
	{
		glm::mat4 transform = parentTransform * node.LocalTransform;
		auto& nodeSubMeshIndices = node.Submeshes;

		if (!nodeSubMeshIndices.empty())
		{
			ImGui::PushID(node.Name.c_str());

			uint32_t meshIndex = nodeSubMeshIndices.front();
			const std::vector<uint32_t>& submeshes = meshBase->GetSubMeshes();

			bool checked = std::find(submeshes.begin(), submeshes.end(), meshIndex) != submeshes.end();

			if (ImGui::Checkbox("##checkbox", &checked))
			{
				std::vector<uint32_t> subMeshCopy;

				// doing this because if we have large amoutn dont want to be creating a copy every time
				// memory and time expensive 

				subMeshCopy = submeshes;
				PF_ENGINE_INFO("{}", subMeshCopy);

				if (checked)
					subMeshCopy.emplace_back(meshIndex);
				else
					Utils::Remove(subMeshCopy,meshIndex);

				meshBase->SetSubMeshes(subMeshCopy);
			} 
			ImGui::PopID();
			ImGui::SameLine();

		}
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (node.Children.size() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (UI::AttributeTreeNode(node.Name, flags))
		{
		#if TRANSFORM_INFO
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				ImGui::Text("World Transform");
				ImGui::Text("  Translation: %.2f, %.2f, %.2f", translation.x, translation.y, translation.z);
				ImGui::Text("  Scale: %.2f, %.2f, %.2f", scale.x, scale.y, scale.z);
			}
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				ImGui::Text("Local Transform");
				ImGui::Text("  Translation: %.2f, %.2f, %.2f", translation.x, translation.y, translation.z);
				ImGui::Text("  Scale: %.2f, %.2f, %.2f", scale.x, scale.y, scale.z);
			}
		#endif
			for (uint32_t i = 0; i < node.Children.size(); i++)
				MeshNodeHierarchy(meshBase, meshBase->GetMeshSource()->GetNodes().at(node.Children[i]), transform, level + 1);

			UI::EndTreeNode();
		}
	}

	void MeshEditorPanel::DrawMeshNode()
	{
		const MeshNode& rootNode = m_MeshBase->GetMeshSource()->GetRootNode();
		MeshNodeHierarchy(m_MeshBase, rootNode, glm::mat4(1.0), 0);
	}

}

