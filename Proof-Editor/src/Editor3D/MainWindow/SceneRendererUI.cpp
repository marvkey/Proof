#include "SceneRendererUI.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "SceneHierachyPanel.h"
namespace Proof{
	SceneRendererUI::SceneRendererUI(MeshAsset* asset) {
		m_World = new World();
		tempEntity =m_World->CreateEntity(asset->GetAssetName());
		mesh =tempEntity.AddComponent<MeshComponent>();
		tempEntity.GetComponent<TransformComponent>()->Location.Z-=10;
		mesh->AssetID = asset->GetID();
		m_WorldRenderer = {m_World,1300,800};
		m_Type= SceneRendererType::MeshAsset;
	}

	void SceneRendererUI::ImGuiRender(FrameTime deltaTime) {
		if(m_ShowWindow==false)
			return;

		if(m_Type == SceneRendererType::MeshAsset || m_Type  == SceneRendererType::EntityPrefab){
			RenderAsset(deltaTime);
		}
	}
	void SceneRendererUI::MeshUI() {
		float width = ImGui::GetWindowWidth();
		width *= 0.3;
		ImGui::BeginChild(m_ID,{width,ImGui::GetContentRegionAvail().y});
		{
			ExternalAPI::ImGUIAPI::CheckBox("FaceCulling",&mesh->GetAsset()->GetMesh()->m_FaceCulling);
			SceneHierachyPanel::DrawVectorControl("Location",tempEntity.GetComponent<TransformComponent>()->Location,0,width * 0.25);
			SceneHierachyPanel::DrawVectorControl("Rotation",tempEntity.GetComponent<TransformComponent>()->Rotation,0,width * 0.25);
			SceneHierachyPanel::DrawVectorControl("Scale",tempEntity.GetComponent<TransformComponent>()->Scale,1,width * 0.25);
			if (mesh != nullptr) {
				ExternalAPI::ImGUIAPI::CheckBox(mesh->GetAsset()->GetAssetName(),&mesh->GetAsset()->GetMesh()->m_Enabled);
				ImGui::NewLine();
				for (SubMesh& subMesh : mesh->GetMesh()->meshes) {
					ExternalAPI::ImGUIAPI::CheckBox(subMesh.GetName(),&subMesh.m_Enabled);
				}
			}
		}
		ImGui::EndChild();

	}
	void SceneRendererUI::RenderAsset(FrameTime deltaTime){
		ImGui::PushID(m_ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin(mesh->GetAsset()->GetAssetName().c_str(),&m_ShowWindow)) {
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
				m_WorldRenderer.SetRendererPause(false);
			}
			else {
				m_WorldRenderer.SetRendererPause(true);
			}
			if (m_Type == SceneRendererType::MeshAsset)
				MeshUI();
			ImGui::SameLine();
			ImGui::BeginChild(mesh->GetAsset()->GetAssetName().c_str());
			m_WorldRenderer.Renderer();
			if (m_LastWidht != ImGui::GetWindowSize().x || m_LastHeight != ImGui::GetWindowSize().y) {
				m_WorldRenderer.Resize(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);

				m_LastWidht = ImGui::GetWindowSize().x; m_LastHeight = ImGui::GetWindowSize().y;
				if (m_WorldRenderer.GetRendererPaused() == true) {
					m_WorldRenderer.SetRendererPause(false);
					m_WorldRenderer.Renderer();
					m_WorldRenderer.SetRendererPause(true);
				}
			}
			uint32_t Text = m_WorldRenderer.GetWorldTexture();
			ImGui::Image((ImTextureID)Text,ImVec2{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
			m_World->OnUpdateEditor(deltaTime,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
			m_LastWidht = ImGui::GetWindowSize().x; m_LastHeight = ImGui::GetWindowSize().y;
			ImGui::EndChild();
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopID();
	};
}