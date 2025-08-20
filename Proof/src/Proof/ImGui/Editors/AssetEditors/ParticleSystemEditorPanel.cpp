#include "Proofprch.h"
#include "ParticleSystemEditorPanel.h"
#include "Proof/Renderer/Renderer.h"
#include "../Panels/SceneHierachyPanel.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/ParticleSystem/ParticleSystem.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Renderer/Texture.h"
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


		m_Camera.SetViewportSize(GetWindowSize().x, GetWindowSize().y);
		m_Camera.SetActive(m_IsViewportFocused);
		m_Camera.OnUpdate(deltaTime);
		m_World->OnUpdateEditor(deltaTime);
		m_World->OnRenderEditor(m_WorldRenderer, deltaTime, m_Camera);
		m_SaveTimer -= deltaTime;

		
	}
	
	void ParticleSystemEditorPanel::OnImGuiRender()
	{
		UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		ImGui::Begin("Main Winodw");


		static Count<ParticleEmitter> emitter = Count<ParticleEmitter>::Create();



		Entity entity = m_World->TryGetEntityByTag("particle");

		entity.GetComponent<ParticleSystemComponent>().emitter = emitter;

		SBParticleInitalState& state = emitter->ParticleInitialState;
		SBParticleEmitterSettings& settings = emitter->ParticleEmitterSettings;
		ImGui::BeginChild("Particle Data", { ImGui::GetContentRegionAvail().x / 3, ImGui::GetContentRegionAvail().y });

		// ───────────── Core ─────────────
		{
			UI::BeginPropertyGrid();

			UI::AttributeDrag("Duration", state.Duration, 0.25);
			UI::AttributeBool("Looping", (bool&)state.bLooping);
			UI::AttributeDrag("FadeOut Speed", state.FadeOutSpeed, 0.01);
			UI::AttributeDrag("Start Lifetime", state.StartLifetime, 0.01);
			UI::AttributeDrag("Start Speed", state.StartSpeed,0.01);
			UI::AttributeDrag("Gravity Modifier", state.GravityModifier,0.01f);
			UI::AttributeColor("Start Color", state.StartColor);
			UI::AttributeDrag("Start Size", state.StartSize,0.1);

			UI::EndPropertyGrid();
		}

		// ───────────── Emission ─────────────
		if (UI::AttributeTreeNode("Emission",1,1))
		{
			UI::BeginPropertyGrid();
			
			UI::AttributeSlider("Particles Per Second", settings.Emission.ParticlesPerSecond, 0.0f, 5000.0f);
			UI::AttributeSlider("Particles Per Distance", settings.Emission.ParticlesPerDistance, 0.0f, 100.0f);

			UI::EndPropertyGrid();

			UI::EndTreeNode();
		}

		// ───────────── Shape ─────────────
		if (UI::AttributeTreeNode("Shape",1,1))
		{
			UI::BeginPropertyGrid();

			UI::EnumCombo("Shape", (ParticleEmitterShape&)settings.Shape.Shape);

			bool enabled = settings.Shape.bEnabled == 1;
			if (UI::AttributeBool("Enable Shape", enabled))
			{
				if (enabled)
					settings.Shape.bEnabled = 1;
				else
					settings.Shape.bEnabled = 0;
			}
			UI::AttributeSlider("Randomize Direction", settings.Shape.RandomizeDirection, 0.0f, 1.0f);
			UI::AttributeSlider("Spherize Direction", settings.Shape.SpherizeDirection, 0.0f, 1.0f);
			UI::AttributeSlider("Randomize Position", settings.Shape.RandomizePosition, 0.0f, 10.0f);

			switch (ParticleEmitterShape (settings.Shape.Shape))
			{
			case Proof::ParticleEmitterShape::Cone:
				{
				UI::AttributeSlider("Cone Radius", settings.Shape.ConeRadius, 0.0f, 50.0f);
				UI::AttributeSlider("Cone Angle", settings.Shape.ConeAngleDegrees, 0.0f, 360.0f);
				break;
				}
			case Proof::ParticleEmitterShape::Sphere:
				{
					UI::AttributeSlider("Sphere Radius", settings.Shape.SphereRadius, 0.0f, 50.0f);
				}
					break;
			default:
				break;
			}

			UI::EndPropertyGrid();

			UI::EndTreeNode();
		}

		// ───────────── Velocity Over Lifetime ─────────────
		if (UI::AttributeTreeNode("Velocity Over LifeTime",false,1,1))
		{
			UI::BeginPropertyGrid();

			bool enabled = settings.VelocityOverLifeTime.bEnabled == 1;
			if (UI::AttributeBool("Enabled", enabled))
			{
				if (enabled)
					settings.VelocityOverLifeTime.bEnabled = 1;
				else
					settings.VelocityOverLifeTime.bEnabled = 0;
			}

			UI::AttributeDrag("Linear", settings.VelocityOverLifeTime.Linear,0.01);
			UI::AttributeDrag("Speed Modifier", settings.VelocityOverLifeTime.SpeedModifier,0.1);
			UI::AttributeDrag("Orbital", settings.VelocityOverLifeTime.Orbital,0.01);
			UI::AttributeSlider("Radial", settings.VelocityOverLifeTime.Radial, 0.0f, 10.0f);
			UI::AttributeDrag("Offset", settings.VelocityOverLifeTime.Offset,0.01);

			UI::EndPropertyGrid();

			UI::EndTreeNode();
		}

		// ───────────── Color Over Lifetime ─────────────
		if (UI::AttributeTreeNode("Color Over LifeTime", false, 1, 1))
		{
			UI::BeginPropertyGrid();

			bool enabled = settings.ColorOverLifeTime.bEnabled == 1;
			if (UI::AttributeBool("Enabled", enabled))
			{
				if (enabled)
					settings.ColorOverLifeTime.bEnabled = 1;
				else
					settings.ColorOverLifeTime.bEnabled = 0;
			}

			UI::AttributeColor("Final Color", settings.ColorOverLifeTime.FinalColor);

			UI::EndPropertyGrid();

			UI::EndTreeNode();
		}

		// ───────────── Size Over Lifetime ─────────────
		if (UI::AttributeTreeNode("Size Over LifeTime", false, 1, 1))
		{
			UI::BeginPropertyGrid();

			bool enabled = settings.SizeOverlifeTime.bEnabled == 1;
			if (UI::AttributeBool("Enabled", enabled))
			{
				if (enabled)
					settings.SizeOverlifeTime.bEnabled = 1;
				else
					settings.SizeOverlifeTime.bEnabled = 0;
			}

			UI::AttributeDrag("Final Size", settings.SizeOverlifeTime.FinalSize);

			UI::EndPropertyGrid();

			UI::EndTreeNode();
		}
		ImGui::EndChild();

		ImGui::SameLine();
		{

			UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("Window", ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y });

			m_WorldRenderer->SetViewportSize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

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
		ImGui::End();


#if 0
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
#endif
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
		//m_ParticleHandler = Count<ParticleHandler>::Create(m_ParticleSystem);
		//entity.AddComponent<ParticleSystemComponent>().ParticleHandlerTable->SetHandler(0, m_ParticleHandler);
		//entity.GetComponent<TransformComponent>().Location.z -= 20.0f;

		entity.AddComponent<ParticleSystemComponent>();

		Entity light = m_World->CreateEntity("light");
		light.AddComponent<SkyLightComponent>();
		light.AddComponent<DirectionalLightComponent>();
		light.GetComponent<TransformComponent>().SetRotationEuler(glm::vec3(80, 10, 0));

		m_Camera.SetPosition(glm::vec3(0, 2.5, 13.5));
		//m_Camera.SetDirection(glm::vec3(-0.9, -0.3, -0.7));
		// -0.9,-0.3,-0.07
		m_WorldRenderer = Count<WorldRenderer>::Create();
	}
	void ParticleSystemEditorPanel::Save()
	{
	}
}