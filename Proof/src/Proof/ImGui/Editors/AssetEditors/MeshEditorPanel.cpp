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

namespace Proof
{

	MeshEditorPanel::MeshEditorPanel()
		:AssetEditor("MeshEditorPanel")
	{

	}
	void MeshEditorPanel::OnUpdate(FrameTime deltaTime)
	{
		if (!m_Mesh)return;
		if (m_IsViewportFocused)
		{
			Application::Get()->GetWindow()->SetWindowInputEvent(true);
			m_Camera.OnUpdate(deltaTime, GetWindowSize().x, GetWindowSize().y);
			Application::Get()->GetWindow()->SetWindowInputEvent(false);
		}
		m_WorldRenderer->SetViewportSize(GetWindowSize().x, GetWindowSize().y);
		m_World->OnUpdateEditor(deltaTime);
		m_World->OnRenderEditor(m_WorldRenderer, deltaTime, m_Camera);
		//m_SaveTimer -= deltaTime;
	}
	void MeshEditorPanel::OnImGuiRender()
	{

		if (!m_Mesh)return;
		float width = ImGui::GetWindowWidth();
		width *= 0.3;
		Count<MeshSource> meshSource = m_Mesh->GetMeshSource();
		ImGui::BeginChild("Mesh Editing", { ImGui::GetContentRegionAvail().x / 3,ImGui::GetContentRegionAvail().y });
		{
			// ediitng
			{
				std::string currentSelect = AssetManager::HasAsset(meshSource) ? AssetManager::GetAssetInfo(meshSource).GetName() : "Unnamed Source";
				ImGui::Text("Reinstate Mesh");
				ImGui::SameLine();
				if (ImGui::BeginCombo("##Reinstate Mesh", currentSelect.c_str()))
				{
					const std::unordered_set<AssetID>& meshSources = AssetManager::GetAllAssetType(AssetType::MeshSourceFile);
					for (auto& ID : meshSources)
					{
						auto& assetInfo = AssetManager::GetAssetInfo(ID);
						bool isSelected = (currentSelect == assetInfo.GetName());
						if (ImGui::Selectable(assetInfo.GetName().c_str(), isSelected))
						{
							// mesh source has been changed
							meshSource = AssetManager::GetAsset<MeshSource>(assetInfo.ID);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (m_Mesh->GetMeshSource() != meshSource)
				{
					m_Mesh->Reset(meshSource);
				}
			}
			/// more settings for futre we dont need to disable submeshes from here it will be done on the mesh source and can create a mesh from that
		}
		ImGui::EndChild();
		ImGui::SameLine();
		//viewport
		ImGui::BeginChild("Mesh Viewport", { ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y });
		{
			UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });

			if (ImGui::IsWindowFocused())
			{
				m_IsViewportFocused = true;
			}
			else
			{
				m_IsViewportFocused = false;
			}
			UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		}
		ImGui::EndChild();

	}
	void MeshEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::Mesh)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to MeshEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_Mesh = asset.As<Mesh>();
		m_World = Count<World>::Create();
		Entity entity = m_World->CreateEntity("Mesh");
		entity.AddComponent<MeshComponent>().SetMesh(m_Mesh->GetID());
		//entity.AddComponent<SkyLightComponent>().Turbidity = 2.0f;
		//entity.AddComponent<SkyLightComponent>().Azimuth = 0.1;
		entity.GetComponent<TransformComponent>().Location.z -= 20.0f;
		m_WorldRenderer = Count<WorldRenderer>::Create();
	}
}

