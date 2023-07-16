
#include "SceneRendererUI.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Editor3D/Editor3D.h"
#include "Editor3D/ImGUIAPI.h"
#include "SceneHierachyPanel.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Scene/Component.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/RenderPass.h"
namespace Proof{

	void SceneRendererUI::ImGuiRender(FrameTime deltaTime) {
		if(m_ShowWindow==false)
			return;

		PF_PROFILE_FUNC();
		RenderAsset(deltaTime);
		
	}
	SceneRendererUI::SceneRendererUI(AssetID id) {
		m_ID = id;
		m_World = Count<World>::Create();
		if (AssetManager::HasAsset(m_ID) == false)return;
		auto assetInfo = AssetManager::GetAssetInfo(m_ID);
		switch (assetInfo.Type) {
			case Proof::AssetType::Mesh:
				{
					m_Type = SceneRendererType::MeshAsset;
					MeshAssetSetUp();
					break;
				}
			case Proof::AssetType::MeshSourceFile:
				{
					m_Type = SceneRendererType::MeshSourceFile;
					MeshSourceAssetSetUp();
					break;
				}
		}
	}
	void SceneRendererUI::MeshAssetSetUp() {
		auto assetInfo= AssetManager::GetAssetInfo(m_ID);
		m_SceneEntity = m_World->CreateEntity(assetInfo.GetName());
		m_SceneEntity.AddComponent<DirectionalLightComponent>().Color = Vector{ 1,1,1 };
		m_SceneEntity.GetComponent<TransformComponent>().Location.Z -= 10;
		m_SceneEntity.AddComponent<MeshComponent>().SetMesh(m_ID);
		m_WorldRenderer = CreateSpecial<WorldRenderer>(m_World, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight() );
	}
	
	void SceneRendererUI::MeshAssetUI() {
		/*
		// there is a problem when resizing it may be due to command buffer simaltneous bit
		float width = ImGui::GetWindowWidth();
		width *= 0.3;
		ImGui::BeginChild(m_ID,{width,ImGui::GetContentRegionAvail().y});
		{
			auto mesh = AssetManager::GetAsset<Mesh>(m_ID);
			if(ImGui::Button("Renstate mesh")){
				std::string filePath = Utils::FileDialogs::OpenFile("Mesh (*.obj)\0 *.obj\0 (*.gltf)\0 *.gltf\0 (*.fbx)\0 *.fbx\0");
				if (filePath.empty() == false) {
					mesh->LoadModel(filePath);
				}
			}
			ExternalAPI::ImGUIAPI::CheckBox("FaceCulling",&meshAsset->GetMesh()->m_FaceCulling);
			//SceneHierachyPanel::DrawVectorControl("Location",m_SceneEntity.GetComponent<TransformComponent>().Location,0,width * 0.25);
			//SceneHierachyPanel::DrawVectorControl("Rotation", m_SceneEntity.GetComponent<TransformComponent>().Rotation,0,width * 0.25);
			//SceneHierachyPanel::DrawVectorControl("Scale", m_SceneEntity.GetComponent<TransformComponent>().Scale,1,width * 0.25);
			ExternalAPI::ImGUIAPI::CheckBox(meshAsset->GetName(),&meshAsset->GetMesh()->Enabled);
			ImGui::NewLine();
			for (SubMesh& subMesh : meshAsset->GetMesh()->meshes) {
				ExternalAPI::ImGUIAPI::CheckBox(subMesh.GetName(),&subMesh.Enabled);
			}
		}
		ImGui::EndChild
		*/
	}
	void SceneRendererUI::MeshSourceAssetSetUp() {
		/*
		auto meshAsset = AssetManager::GetAsset<MeshSourceFileAsset>(m_ID);
		m_SceneEntity = m_World->CreateEntity(meshAsset->GetName());
		m_SceneEntity.AddComponent<DirectionalLightComponent>()->Color = Vector{ 1,1,1 };
		m_SceneEntity.GetComponent<TransformComponent>().Location.Z -= 10;
		m_SceneEntity.AddComponent<MeshComponent>().SetMesh(m_ID);
		m_WorldRenderer =CreateSpecial<WorldRenderer>( m_World, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight() );
		*/
	}
	void SceneRendererUI::MeshSourceAssetUI() {
		/*
		float width = ImGui::GetWindowWidth();
		width *= 0.3;
		ImGui::BeginChild(m_ID, { width,ImGui::GetContentRegionAvail().y });
		{
			auto meshAsset = AssetManager::GetAsset<MeshSourceFileAsset>(m_ID);
			//ExternalAPI::ImGUIAPI::CheckBox("FaceCulling", &meshAsset->GetMesh()->m_FaceCulling);
			//SceneHierachyPanel::DrawVectorControl("Location",m_SceneEntity.GetComponent<TransformComponent>().Location,0,width * 0.25);
			//SceneHierachyPanel::DrawVectorControl("Rotation", m_SceneEntity.GetComponent<TransformComponent>().Rotation,0,width * 0.25);
			//SceneHierachyPanel::DrawVectorControl("Scale", m_SceneEntity.GetComponent<TransformComponent>().Scale,1,width * 0.25);
			//ExternalAPI::ImGUIAPI::CheckBox(meshAsset->GetName(), &meshAsset->GetMesh()->Enabled);
			ImGui::NewLine();
			for (SubMesh& subMesh : meshAsset->GetMesh()->meshes) {
				ExternalAPI::ImGUIAPI::CheckBox(subMesh.GetName(), &subMesh.Enabled);
			}
		}
		ImGui::EndChild();
		*/
	}
	
	void SceneRendererUI::RenderAsset(FrameTime deltaTime){
		/*
		Count<Asset> meshAsset; 
		switch (m_Type) {
			case SceneRendererType::MeshAsset:
				meshAsset = AssetManager::GetAsset<Mesh>(m_ID);
				break;
//			case SceneRendererType::MeshSourceFile:
//				meshAsset = AssetManager::GetAsset<MeshSourceFileAsset>(m_ID);
//				break;
		}
		ImGui::PushID(m_ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin(meshAsset->GetName().c_str(),&m_ShowWindow)) {

			switch (m_Type) {
				case SceneRendererType::MeshAsset:
					{

						MeshAssetUI();
						break;
					}
				case SceneRendererType::MeshSourceFile:
					{
						MeshSourceAssetUI();
						break;
					}
			}
			ImGui::SameLine();
			ImGui::BeginChild(meshAsset->GetName().c_str());
			m_WorldRenderer->Render(m_Camera);
			ScreenSize currentSize = { (uint32_t)ImGui::GetWindowSize().x,(uint32_t)ImGui::GetWindowSize().y };
			if (currentSize != m_ScreenSize) {
				m_WorldRenderer->Resize(currentSize);
				Application::Get()->GetWindow()->SetWindowInputEvent(false);
				m_Camera.OnUpdate(deltaTime, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
				Application::Get()->GetWindow()->SetWindowInputEvent(true);
			}

			m_ScreenSize = currentSize;
			
			const void* Text = m_WorldRenderer->GetImage().SourceImage;
			ImGui::Image((ImTextureID)Text,ImGui::GetWindowSize(), ImVec2{0,1}, ImVec2{1,0});
			if (ImGui::IsWindowFocused()) {
				Application::Get()->GetWindow()->SetWindowInputEvent(true);
				m_Camera.OnUpdate(deltaTime, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
				Application::Get()->GetWindow()->SetWindowInputEvent(false);
			}
			ImGui::EndChild();
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopID();
		*/
	};
}