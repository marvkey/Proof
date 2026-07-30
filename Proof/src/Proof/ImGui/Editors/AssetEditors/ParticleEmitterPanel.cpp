#include "Proofprch.h"
#include "ParticleEmitterPanel.h"

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

#include <algorithm>

namespace Proof
{
	ParticleEmitterPanel::ParticleEmitterPanel()
		: AssetEditor("ParticleEmitterPanel")
	{

	}

	// does not work same as unity as unity has different variabel for 3d size and Normal Size
	static void DrawSizeControl(const std::string& label, glm::vec3& vecSize, float speed = 0.1f, bool use3D = false)
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

	Count<ParticleEmitterInstance> ParticleEmitterPanel::GetPreviewEmitterInstance()
	{
		if (!m_ParticleSystemInstance || !m_Emitter)
			return nullptr;

		auto timeline = m_ParticleSystemInstance->GetTimeline();
		if (!timeline)
			return nullptr;

		auto* track = timeline->GetTrack(m_Emitter->GetID());
		if (!track || track->EmitterClips.empty())
			return nullptr;

		m_EmitterInstance = track->EmitterClips[0].Emitter;
		return m_EmitterInstance;
	}

	void ParticleEmitterPanel::OnUpdate(FrameTime deltaTime)
	{
		if (!m_World || !m_WorldRenderer)
			return;

		m_Camera.SetViewportSize(std::max(m_ViewportSize.x, 1.0f), std::max(m_ViewportSize.y, 1.0f));
		m_Camera.SetActive(m_IsViewportFocused);
		m_Camera.OnUpdate(deltaTime);
		m_World->OnUpdateEditor(deltaTime);

		auto emitterInstance = GetPreviewEmitterInstance();
		if (emitterInstance)
		{
			m_World->OnRenderEditor(m_WorldRenderer, deltaTime, m_Camera, [emitterInstance](Count<WorldRenderer> renderer)
			{
				renderer->SubmitParticleEmitter(emitterInstance);
			});
		}

		m_SaveTimer -= deltaTime;
	}

	void ParticleEmitterPanel::DrawPropertiesPanel()
	{
		ImGui::BeginChild("Particle Data", ImVec2(0.0f, 0.0f), true);

		if (!m_Emitter)
		{
			ImGui::TextDisabled("No particle emitter loaded.");
			ImGui::EndChild();
			return;
		}

		Count<ParticleEmitter> emitter = m_Emitter;
		SBParticleInitialSettings& state = emitter->ParticleInitialState;
		SBParticleEmitterSettings& settings = emitter->ParticleEmitterSettings;

		// ───────────── Core ─────────────
		{
			UI::AttributeTextureAssetReference("Texture", emitter->Texture);
			UI::BeginPropertyGrid();

			UI::AttributeDrag("Duration", state.Duration, 0.25);
			UI::AttributeDrag("FadeOut Speed", state.FadeOutSpeed, 0.01);
			UI::AttributeDrag("Start Lifetime", state.StartLifetime, 0.01);
			UI::AttributeDrag("Start Speed", state.StartSpeed, 0.01);
			UI::AttributeDrag("Gravity Modifier", state.GravityModifier, 0.01f);
			UI::AttributeColor("Start Color", state.StartColor);

			DrawSizeControl("Start Size", state.StartSize, 0.1f);

			uint32_t particles = emitter->GetParticleCount();
			UI::AttributeDrag("MaxParticles", particles, 10, 1);

			if (particles != emitter->GetParticleCount())
				emitter->m_MaxParticles = particles;

			UI::EndPropertyGrid();
		}

		// ───────────── Emission ─────────────
		if (UI::AttributeTreeNode("Emission", true, 1, 1))
		{
			UI::BeginPropertyGrid();

			UI::AttributeDrag("Particles Per Second", settings.Emission.ParticlesPerSecond, 2);
			UI::AttributeDrag("Particles Per Distance", settings.Emission.ParticlesPerDistance, 1);

			UI::EndPropertyGrid();

			if (UI::AttributeButton("Add Burst"))
				emitter->Bursts.emplace_back(ParticleBurst());

			for (uint32_t i = 0; i < emitter->Bursts.size(); i++)
			{
				UI::ScopedID burstID(fmt::format("Burtst index {}, particle id{}", i, m_Emitter->GetID()).c_str());
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
		if (UI::AttributeTreeNode("Shape", true, 1, 1))
		{
			UI::BeginPropertyGrid();

			UI::EnumCombo("Shape", (ParticleEmitterShape&)settings.Shape.Shape);

			bool enabled = settings.Shape.bEnabled == 1;
			if (UI::AttributeBool("Enable Shape", enabled))
				settings.Shape.bEnabled = enabled ? 1 : 0;

			UI::AttributeSlider("Randomize Direction", settings.Shape.RandomizeDirection, 0.0f, 1.0f);
			UI::AttributeSlider("Spherize Direction", settings.Shape.SpherizeDirection, 0.0f, 1.0f);
			UI::AttributeSlider("Randomize Position", settings.Shape.RandomizePosition, 0.0f, 10.0f);

			switch (ParticleEmitterShape(settings.Shape.Shape))
			{
			case ParticleEmitterShape::Cone:
				{
					UI::AttributeSlider("Cone Radius", settings.Shape.ConeRadius, 0.0f, 50.0f);
					UI::AttributeSlider("Cone Angle", settings.Shape.ConeAngleDegrees, 0.0f, 360.0f);
					break;
				}
			case ParticleEmitterShape::Sphere:
				{
					UI::AttributeSlider("Sphere Radius", settings.Shape.SphereRadius, 0.0f, 50.0f);
					break;
				}
			default:
				break;
			}

			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}

		// ───────────── Velocity Over Lifetime ─────────────
		if (UI::AttributeTreeNode("Velocity Over LifeTime", false, 1, 1))
		{
			UI::BeginPropertyGrid();

			bool enabled = settings.VelocityOverLifeTime.bEnabled == 1;
			if (UI::AttributeBool("Enabled Velocity Over LifeTime", enabled))
				settings.VelocityOverLifeTime.bEnabled = enabled ? 1 : 0;

			UI::AttributeDrag("Linear", settings.VelocityOverLifeTime.Linear, 0.01);
			UI::AttributeDrag("Speed Modifier", settings.VelocityOverLifeTime.SpeedModifier, 0.1);
			UI::AttributeDrag("Orbital", settings.VelocityOverLifeTime.Orbital, 0.01);
			UI::AttributeSlider("Radial", settings.VelocityOverLifeTime.Radial, 0.0f, 10.0f);
			UI::AttributeDrag("Offset", settings.VelocityOverLifeTime.Offset, 0.01);

			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}

		// ───────────── Color Over Lifetime ─────────────
		if (UI::AttributeTreeNode("Color Over LifeTime", false, 1, 1))
		{
			UI::BeginPropertyGrid();

			bool enabled = settings.ColorOverLifeTime.bEnabled == 1;
			if (UI::AttributeBool("Enabled Color Over LifeTime", enabled))
				settings.ColorOverLifeTime.bEnabled = enabled ? 1 : 0;

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
				settings.SizeOverlifeTime.bEnabled = enabled ? 1 : 0;

			DrawSizeControl("Final Size", settings.SizeOverlifeTime.FinalSize, 0.1);

			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}

		ImGui::EndChild();
	}

	void ParticleEmitterPanel::DrawViewportPanel()
	{
		UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });
		ImGui::BeginChild("Particle Viewport", ImVec2(0.0f, 0.0f), true);

		const ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = glm::vec2(viewportSize.x, viewportSize.y);
		m_WorldRenderer->SetViewportSize(viewportSize.x, viewportSize.y);
		m_IsViewportFocused = ImGui::IsWindowFocused();

		UI::Image(m_WorldRenderer->GetFinalPassImage(), viewportSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::EndChild();
	}

	void ParticleEmitterPanel::DrawPlaybackPanel()
	{
		ImGui::BeginChild("Particle Playback", ImVec2(0.0f, 0.0f), true);

		if (!m_ParticleSystemInstance)
		{
			ImGui::TextDisabled("No particle preview available.");
			ImGui::EndChild();
			return;
		}

		auto emitterInstance = GetPreviewEmitterInstance();
		ParticleSystemState state = m_ParticleSystemInstance->GetState();
		bool isPlaying = state == ParticleSystemState::Play;
		bool canStop = state == ParticleSystemState::Play || state == ParticleSystemState::Pause;

		const float spacing = ImGui::GetStyle().ItemSpacing.x;
		const float buttonWidth = (ImGui::GetContentRegionAvail().x - spacing * 2.0f) / 3.0f;

		if (ImGui::Button(isPlaying ? "Pause" : "Play", ImVec2(buttonWidth, 0.0f)))
		{
			if (isPlaying)
				m_ParticleSystemInstance->Pause();
			else
			{
				m_ParticleSystemInstance->SetLooping(m_Resimulate);
				m_ParticleSystemInstance->Play();
			}
		}

		ImGui::SameLine();

		ImGui::BeginDisabled(!emitterInstance);

		if (ImGui::Button("Restart", ImVec2(buttonWidth, 0.0f)))
		{
			m_ParticleSystemInstance->SetLooping(m_Resimulate);
			m_ParticleSystemInstance->Reset();

			if (state == ParticleSystemState::Play)
				m_ParticleSystemInstance->Play();
		}                                             

		ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::BeginDisabled(!canStop);

		if (ImGui::Button("Stop", ImVec2(buttonWidth, 0.0f)))
			m_ParticleSystemInstance->Stop(false);

		ImGui::EndDisabled();

		ImGui::Separator();
		UI::BeginPropertyGrid();

		float playbackSpeed = m_ParticleSystemInstance->SimulationSpeed;
		if (UI::AttributeDrag("Playback Speed", playbackSpeed, 0.01f, 0.0f))
			m_ParticleSystemInstance->SimulationSpeed = std::max(playbackSpeed, 0.0f);

		if (UI::AttributeBool("Resimulate", m_Resimulate))
			m_ParticleSystemInstance->SetLooping(m_Resimulate);

		UI::EndPropertyGrid();
		ImGui::Separator();

		if (emitterInstance)
		{
			const SBParticleTrackableData trackableData = emitterInstance->GetTrackableData();

			ImGui::Text("Playback Time");
			ImGui::SameLine(150.0f);
			ImGui::Text("%.2f", trackableData.TimeElapsed);

			ImGui::Text("Particles");
			ImGui::SameLine(150.0f);
			ImGui::Text("%d / %d", trackableData.ActiveParticles, trackableData.MaxParticles);

			//ImGui::Text("State");
			//ImGui::SameLine(150.0f);
			//ImGui::TextUnformatted(trackableData.State == 2 ? "Done" : "Playing");
		}

		ImGui::EndChild();
	}

	void ParticleEmitterPanel::OnImGuiRender()
	{
		UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2{ 6,6 });

		ImGui::Begin("Main Winodw");

		if (!m_Emitter || !m_WorldRenderer)
		{
			ImGui::TextDisabled("No particle emitter loaded.");
			ImGui::End();
			return;
		}

		const ImVec2 available = ImGui::GetContentRegionAvail();
		const float spacing = ImGui::GetStyle().ItemSpacing.x;
		const float propertiesWidth = std::max(available.x * 0.32f, 320.0f);
		const float playbackWidth = std::max(available.x * 0.20f, 280.0f);
		const float viewportWidth = std::max(available.x - propertiesWidth - playbackWidth - spacing * 2.0f, 200.0f);

		ImGui::BeginChild("Particle Properties Panel", ImVec2(propertiesWidth, available.y), false);
		DrawPropertiesPanel();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Particle View Panel", ImVec2(viewportWidth, available.y), false);
		DrawViewportPanel();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Particle Playback Panel", ImVec2(0.0f, available.y), false);
		DrawPlaybackPanel();
		ImGui::EndChild();

		ImGui::End();
	}

	void ParticleEmitterPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (!asset || asset->GetAssetType() != AssetType::ParticleEmitter)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to ParticleEmitterPanel {}", asset ? EnumReflection::EnumString(asset->GetAssetType()) : "Invalid", m_TitleAndId);
			return;
		}

		m_Emitter = asset.As<ParticleEmitter>();
		m_World = Count<World>::Create();

		Entity entity = m_World->CreateEntity("Particle");

		Count<ParticleSystem> particleSystem = Count<ParticleSystem>::Create(m_Emitter->GetID());

		AssetManager::CreateRuntimeAsset(particleSystem, fmt::format("ParticleEmmiterPanel {} Particlesystem", AssetManager::GetAssetInfo(m_Emitter->GetID()).GetName()));
		m_ParticleSystemInstance = Count<ParticleEffect>::Create(particleSystem);

		entity.AddComponent<ParticleEffectComponent>().ParticleEffect = m_ParticleSystemInstance;

		Entity light = m_World->CreateEntity("light");
		light.AddComponent<SkyLightComponent>();
		light.AddComponent<DirectionalLightComponent>();
		light.GetComponent<TransformComponent>().SetRotationEuler(glm::vec3(80, 10, 0));

		m_Camera.SetPosition(glm::vec3(0, 2.5, 13.5));
		//m_Camera.SetDirection(glm::vec3(-0.9, -0.3, -0.7));
		// -0.9,-0.3,-0.07
		m_WorldRenderer = Count<WorldRenderer>::Create();

		m_Resimulate = true;
		m_ParticleSystemInstance->SetLooping(m_Resimulate);
		m_ParticleSystemInstance->Play();
		m_EmitterInstance = GetPreviewEmitterInstance();
	}

	void ParticleEmitterPanel::Save()
	{
		if (m_Emitter == nullptr)
			return;

		m_NeedsSaving = false;
		AssetManager::SaveAsset(m_Emitter->GetID());
		//Compile();
		for (auto worldWeak : World::GetAllActiveWorlds())
		{
			if (!worldWeak.IsValid())
				continue;

			auto world = worldWeak.Lock();

			if (world->GetState() != WorldState::Edit)
				continue;

			world->ForEachEnitityWith<ParticleEffectComponent>([&](Entity e)
			{
				auto particleEffect = e.GetComponent<ParticleEffectComponent>().ParticleEffect;
				if (!particleEffect)
					return;

				auto timeline = particleEffect->GetTimeline();
				if (!timeline)
					return;

				for (auto& [id, track] : timeline->m_Tracks)
				{
					AssetKey<AssetType::ParticleEmitter> emitterHandle = id;
					if (emitterHandle != m_Emitter->GetID())
						continue;

					track.SyncAllClipInstances();
				}
			});
		}
	}
}