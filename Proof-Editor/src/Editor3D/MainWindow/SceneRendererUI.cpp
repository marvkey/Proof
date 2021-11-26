#include "SceneRendererUI.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "SceneHierachyPanel.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Scene/Component.h"
namespace Proof{
	SceneRendererUI::SceneRendererUI(MeshAsset* asset) {
		m_World = CreateSpecial<World>();
		m_MeshAsset = asset;
		tempEntity =m_World->CreateEntity(asset->GetName());
		mesh =tempEntity.AddComponent<MeshComponent>();
		tempEntity.AddComponent<LightComponent>()->m_Ambient ={1,1,1};
		tempEntity.GetComponent<TransformComponent>()->Location.Z-=10;
		mesh->m_MeshAssetPointerID = asset->GetID();
		m_WorldRenderer = {m_World.get(),1300,800};
		m_Type= SceneRendererType::MeshAsset;
		m_WorldRenderer.RenderData.RenderSettings.Technique = RenderTechnique::FowardRendering;
	}

	void SceneRendererUI::ImGuiRender(FrameTime deltaTime) {
		if(m_ShowWindow==false)
			return;

		RenderAsset(deltaTime);
		
	}
	void SceneRendererUI::MeshUI() {
		float width = ImGui::GetWindowWidth();
		width *= 0.3;
		ImGui::BeginChild(m_ID,{width,ImGui::GetContentRegionAvail().y});
		{
			if(ImGui::Button("Renstate mesh")){
				std::string filePath = Utils::FileDialogs::OpenFile("Mesh (*.obj)\0 *.obj\0 (*.gltf)\0 *.gltf\0 (*.fbx)\0 *.fbx\0");
				if (filePath.empty() == false) {
					m_MeshAsset->Reinstate(filePath);
				}
			}
			ExternalAPI::ImGUIAPI::CheckBox("FaceCulling",&mesh->GetAsset()->GetMesh()->m_FaceCulling);
			SceneHierachyPanel::DrawVectorControl("Location",tempEntity.GetComponent<TransformComponent>()->Location,0,width * 0.25);
			SceneHierachyPanel::DrawVectorControl("Rotation",tempEntity.GetComponent<TransformComponent>()->Rotation,0,width * 0.25);
			SceneHierachyPanel::DrawVectorControl("Scale",tempEntity.GetComponent<TransformComponent>()->Scale,1,width * 0.25);
			if (m_MeshAsset != nullptr) {
				ExternalAPI::ImGUIAPI::CheckBox(mesh->GetAsset()->GetName(),&mesh->GetAsset()->GetMesh()->m_Enabled);
				ImGui::NewLine();
				for (SubMesh& subMesh : mesh->GetMeshSource()->meshes) {
					ExternalAPI::ImGUIAPI::CheckBox(subMesh.GetName(),&subMesh.m_Enabled);
				}
			}
		}
		ImGui::EndChild();

	}
	void SceneRendererUI::RenderAsset(FrameTime deltaTime){
		ImGui::PushID(m_ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin(mesh->GetAsset()->GetName().c_str(),&m_ShowWindow)) {
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
				m_WorldRenderer.SetRendererPause(false);
			else
				m_WorldRenderer.SetRendererPause(true);

			MeshUI();
			ImGui::SameLine();
			ImGui::BeginChild(mesh->GetAsset()->GetName().c_str());
			m_WorldRenderer.Renderer();
			if (m_LastWidht != ImGui::GetWindowSize().x || m_LastHeight != ImGui::GetWindowSize().y) {
				m_WorldRenderer.m_ScreenFrameBuffer->Resize(ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
				m_LastWidht = ImGui::GetWindowSize().x; m_LastHeight = ImGui::GetWindowSize().y;
				if (m_WorldRenderer.GetRendererPaused() == true) {
					m_WorldRenderer.SetRendererPause(false);
					m_WorldRenderer.Renderer();
					m_WorldRenderer.SetRendererPause(true);
				}
			}
			uint32_t Text = m_WorldRenderer.GetWorldTexture();
			ImGui::Image((ImTextureID)Text,ImVec2{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
			m_World->m_EditorCamera.OnUpdate(deltaTime,ImGui::GetWindowSize().x,ImGui::GetWindowSize().y);
			m_LastWidht = ImGui::GetWindowSize().x; m_LastHeight = ImGui::GetWindowSize().y;
			ImGui::EndChild();
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopID();
	};
}