#include "Proofprch.h"
#include "ParticleSystemEditorPanel.h"
#include "Proof/Renderer/Renderer.h"
#include "../Panels/SceneHierachyPanel.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UiUtilities.h"
namespace Proof
{
	ParticleSystemEditorPanel::ParticleSystemEditorPanel()
		:
		AssetEditor("ParticleSystemEditorPanel")
	{
		
	}
	void ParticleSystemEditorPanel::OnUpdate(FrameTime deltaTime)
	{
		if (!m_World)return;


		if (m_IsViewportFocused)
		{
			Application::Get()->GetWindow()->SetWindowInputEvent(true);
			m_Camera.OnUpdate(deltaTime, GetWindowSize().x, GetWindowSize().y);
			Application::Get()->GetWindow()->SetWindowInputEvent(false);
		}
		m_World->OnUpdateEditor(deltaTime);
		m_World->OnRenderEditor(m_WorldRenderer, deltaTime, m_Camera);
		m_SaveTimer -= deltaTime;
	}
	void ParticleSystemEditorPanel::OnImGuiRender()
	{
		if (!m_ParticleSystem)return;
		if (m_SaveTimer <= 0.0f)
		{
			AssetManager::SaveAsset(m_ParticleSystem->GetID());
			m_SaveTimer = 100.0f;
		}
		UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::BeginChild("Particle Data", { ImGui::GetContentRegionAvail().x / 3, ImGui::GetContentRegionAvail().y });
		if (m_ParticleHandler->GetState() == ParticleHandlerState::Play)
		{
			if (ImGui::Button("Pause"))
				m_ParticleHandler->Pause();
			ImGui::SameLine();
			if (ImGui::Button("Restart"))
				m_ParticleHandler->Restart();
			ImGui::SameLine();
			if (ImGui::Button("End"))
				m_ParticleHandler->End();

		}
		else
		{
			if (ImGui::Button("Play"))
			{
				m_ParticleHandler->Play();
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				m_ParticleHandler->End();
			}
		}
		auto& location = m_World->TryGetEntityByTag("particle").GetComponent<TransformComponent>().Location;
		SceneHierachyPanel::DrawVectorControl("Location", location);
		SceneHierachyPanel::DrawVectorControl("Velocity", m_ParticleSystem->Velocity);
		SceneHierachyPanel::DrawVectorControl("VelocityVariation", m_ParticleSystem->VelocityVariation);
		ImGui::Separator();

		ImGui::ColorEdit4("ColorBegin", glm::value_ptr(m_ParticleSystem->ColorBegin));
		ImGui::ColorEdit4("ColorEnd", glm::value_ptr(m_ParticleSystem->ColorEnd));

		ImGui::Separator();
		ImGui::Checkbox("Use3D", &m_ParticleSystem->Use3D);
		if (m_ParticleSystem->Use3D)
		{
			SceneHierachyPanel::DrawVectorControl("Rotaiton3D", m_ParticleSystem->Rotation3D);

			SceneHierachyPanel::DrawVectorControl("BeginSize3D", m_ParticleSystem->SizeBegin3D);
			SceneHierachyPanel::DrawVectorControl("EndSize3D", m_ParticleSystem->SizeEnd3D);
			SceneHierachyPanel::DrawVectorControl("SizeVariation3D", m_ParticleSystem->SizeVariation3D);
		}
		else
		{
			ImGui::DragFloat("BeginSize", &m_ParticleSystem->SizeBegin, 0.5, 0);
			ImGui::DragFloat("EndSize", &m_ParticleSystem->SizeEnd, 0.5, 0);
			ImGui::DragFloat("SizeVariation", &m_ParticleSystem->SizeVariation, 0.5, 0);
		}
		ImGui::Separator();

		UI::AttributeBool("Loop", m_ParticleSystem->Loop);
		UI::AttributeBool("PlayOnAwake", m_ParticleSystem->PlayOnAwake);
		ImGui::DragFloat("Life", &m_ParticleSystem->LifeTime, 0.5, 0);
		ImGui::DragScalar("MaxParticles", ImGuiDataType_U32, &m_ParticleSystem->MaxParticles, 4, 0);
		{
			if (m_ParticleSystem->Texture != nullptr)
			{
				bool fdasf = true;
				ImGui::Checkbox("##x", &fdasf);
				ImGui::SameLine();
				UI::Image(Renderer::GetWhiteTexture(), { 30,30 });
			}
			else
			{
				UI::Image(Renderer::GetWhiteTexture(), { 30,30 });
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
				{
					uint64_t Data = *(const uint64_t*)payload->Data;
					if (AssetManager::HasAsset(Data))
					{
						m_ParticleSystem->Texture = AssetManager::GetAsset<Texture2D>(Data);
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::Separator();
		ImGui::DragScalar("ParticleOverTime", ImGuiDataType_U32, &m_ParticleSystem->Emision.ParticleOverTime, 3);
		ImGui::DragFloat("SpawnRateDistance", &m_ParticleSystem->Emision.SpawnRateDistance, 0.5, 0);
		ImGui::EndChild();

		ImGui::SameLine();
		{

			UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("Window", ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y });

			if (ImGui::IsWindowFocused())
			{
				m_IsViewportFocused = true;
			}
			else
			{
				m_IsViewportFocused = false;
			}

			UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::EndChild();
		}

	}
	
	
	void ParticleSystemEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::ParticleSystem)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to ParticleSystemEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_ParticleSystem = asset.As<ParticleSystem>();
		m_World = Count<World>::Create();
		Entity entity = m_World->CreateEntity("particle");
		m_ParticleHandler = Count<ParticleHandler>::Create(m_ParticleSystem);
		entity.AddComponent<ParticleSystemComponent>().ParticleHandlerTable->SetHandler(0, m_ParticleHandler);
		entity.GetComponent<TransformComponent>().Location.z -= 20.0f;
		m_WorldRenderer = Count<WorldRenderer>::Create();
	}
}