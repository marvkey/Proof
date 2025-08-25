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
#include "Proof/ImGui/UIHandlers.h"

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

		Entity particleEntity = m_World->TryGetEntityByTag("Particle");
		if(particleEntity.IsValid())
		{
			if(particleEntity.GetComponent<ParticleSystemComponent>().ParticleSytemInstance->m_Emmiters.empty())
				particleEntity.GetComponent<ParticleSystemComponent>().ParticleSytemInstance->m_Emmiters.push_back(m_ParticleSystem->GetEmitter(0));
			else
				particleEntity.GetComponent<ParticleSystemComponent>().ParticleSytemInstance->m_Emmiters[0] = m_ParticleSystem->GetEmitter(0);
		}
		m_Camera.SetViewportSize(GetWindowSize().x, GetWindowSize().y);
		m_Camera.SetActive(m_IsViewportFocused);
		m_Camera.OnUpdate(deltaTime);
		m_World->OnUpdateEditor(deltaTime);
		m_World->OnRenderEditor(m_WorldRenderer, deltaTime, m_Camera, [&](Count<WorldRenderer> renderer) {
			renderer->SubmitParticleEmitter(m_ParticleSystem->GetEmitter(0));
			});

		//m_ParticleSystem->GetEmitter(0)->OnUpdate((float)deltaTime, Transform());
		m_SaveTimer -= deltaTime;

		
	}
	// does not work same as unity as unity has different variabel for 3d size and Normal Size
	void DrawSizeControl(const std::string& label, glm::vec3& vecSize, float speed = 0.1f,bool use3D = false )
	{
		if (use3D)
		{
			UI::AttributeDrag(label, vecSize, speed);
		}
		else
		{
			float magnitude = glm::length(vecSize);
			if (UI::AttributeDrag(label, magnitude, speed, 0.0f))
			{
				// Avoid division by zero
				glm::vec3 direction = vecSize == glm::vec3(0.0f) ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::normalize(vecSize);
				vecSize = direction * magnitude;
			}
		}
	}

	void ParticleSystemEditorPanel::OnImGuiRender()
	{
		UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		ImGui::Begin("Main Winodw");


		if (m_ParticleSystem->GetEmitterCount() == 0)
		{
			m_ParticleSystem->CreateEmitter();
		}

		Count<ParticleEmitter> emitter = m_ParticleSystem->GetEmitter(0);

		Entity entity = m_World->TryGetEntityByTag("particle");

		//entity.GetComponent<ParticleSystemComponent>().emitter = emitter;

		SBParticleInitalState& state = emitter->ParticleInitialState;
		SBParticleEmitterSettings& settings = emitter->ParticleEmitterSettings;
		ImGui::BeginChild("Particle Data", { ImGui::GetContentRegionAvail().x / 3, ImGui::GetContentRegionAvail().y });

		// ───────────── Core ─────────────
		{
			UI::AttributeTextureAssetReference("Texture", emitter->Texture);
			UI::BeginPropertyGrid();

			
			
			UI::AttributeDrag("Duration", state.Duration, 0.25);
			UI::AttributeBool("Looping", (bool&)state.bLooping);
			UI::AttributeDrag("FadeOut Speed", state.FadeOutSpeed, 0.01);
			UI::AttributeDrag("Start Lifetime", state.StartLifetime, 0.01);
			UI::AttributeDrag("Start Speed", state.StartSpeed,0.01);
			UI::AttributeDrag("Gravity Modifier", state.GravityModifier,0.01f);
			UI::AttributeColor("Start Color", state.StartColor);


			DrawSizeControl("Start Size", state.StartSize,0.1f);

			uint32_t particles = emitter->GetParticleCount();
			if (UI::AttributeDrag("MaxParticles", particles, 10, 1));

			if(particles != emitter->GetParticleCount())
				emitter->ResetMaxParticles(particles);

			UI::EndPropertyGrid();
		}

		// ───────────── Emission ─────────────
		if (UI::AttributeTreeNode("Emission",1,1))
		{
			UI::BeginPropertyGrid();
			
			UI::AttributeDrag("Particles Per Second", settings.Emission.ParticlesPerSecond,2);
			UI::AttributeDrag("Particles Per Distance", settings.Emission.ParticlesPerDistance,1);
			UI::EndPropertyGrid();

			if(UI::AttributeButton("Add Burst"))
				emitter->Bursts.emplace_back(ParticleBurst());

			for (uint32_t i = 0; i < emitter->Bursts.size(); i++)
			{
				UI::ScopedID burstID(fmt::format("Burtst index {}, particle id{}", i, m_ParticleSystem->GetID()).c_str());
				if (UI::AttributeTreeNode(fmt::format("Burst {}", i), true, 0.25f, 0.25f))
				{
					ParticleBurst& burst = emitter->Bursts[i];
					UI::BeginPropertyGrid();

					UI::AttributeDrag("StartTime", burst.StartTime, 0.01);
					UI::AttributeDrag("Count", burst.Count, 5);
					UI::AttributeDrag("Cycles", burst.Cycles, 1);
					UI::AttributeDrag("Interval", burst.Interval, 0.01);
					UI::AttributeDrag("Probability", burst.Probability, 0.01, 0, 1);

					UI::EndPropertyGrid();

					UI::EndTreeNode();
				}
			}

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
			if (UI::AttributeBool("Enabled Velocity Over LifeTime", enabled))
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
			if (UI::AttributeBool("Enabled Color Over LifeTime", enabled))
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
			if (UI::AttributeBool("Enabled Over LifeTime", enabled))
			{
				if (enabled)
					settings.SizeOverlifeTime.bEnabled = 1;
				else
					settings.SizeOverlifeTime.bEnabled = 0;
			}

			DrawSizeControl("Final Size", settings.SizeOverlifeTime.FinalSize,0.1);

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
		Entity entity = m_World->CreateEntity("Particle");
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
		if (m_ParticleSystem == nullptr)
			return;
	
		m_NeedsSaving = false;
		AssetManager::SaveAsset(m_ParticleSystem->GetID());
		//Compile();
		for (auto worldWeak : World::GetAllActiveWorlds())
		{
			if (!worldWeak.IsValid())
				continue;

			auto world = worldWeak.Lock();

			if (world->GetState() != WorldState::Edit)
				continue;

			world->ForEachEnitityWith<ParticleSystemComponent>([&](Entity e)
			{
				if(e.GetComponent<ParticleSystemComponent>().ParticleSytemInstance->GetParticleSystem() == m_ParticleSystem)
					e.GetComponent<ParticleSystemComponent>().ParticleSytemInstance->SyncWithParicleSystem();
			});

		}
		
	}
}