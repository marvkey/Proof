#include "MaterialEditorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Editor3D/Editor3D.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Scene/Material.h"
#include "SceneHierachyPanel.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Core/Core.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/Application.h"
#include "../EditorResources.h"
#include "Proof/Renderer/RenderMaterial.h"

#include "Proof/Renderer/Shader.h"
namespace Proof
{


	MaterialEditorPanel::MaterialEditorPanel(Count<Material> material) :
		m_Material(material)
	{
	}
	float contdown = 10;
	void MaterialEditorPanel::ImGuiRender(FrameTime deltaTime) {
		if (m_ShowWindow == false)
			return;


		// save if any data is changed
		bool shouldSave = false;

		PF_PROFILE_FUNC();
		if (!m_Material)return;
		ImGui::PushID((MemoryAddress) & *m_Material.Get());
		std::string name = m_Material->Name;
		if (AssetManager::HasAsset(m_Material))
		{
			name = AssetManager::GetAssetInfo(m_Material).GetName();
		}
		Count<RenderMaterial> renderMaterial = m_Material->GetRenderMaterial();
		ImGui::Begin(name.c_str(), &m_ShowWindow);
		{
			auto shaderName = fmt::format("Shader: {}", renderMaterial->GetConfig().Shader->GetName());
			ImGui::Text(shaderName.c_str());
			//Albedo
			{
				bool renderToggle = true;
				Count<Texture2D> albedoMap = m_Material->GetAlbedoMap();
				if (albedoMap != nullptr && albedoMap != Renderer::GetWhiteTexture())
				{
					UI::Image(albedoMap , { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
					if (ImGui::BeginPopupContextItem("Albedo Texture Settings"))
					{
						ImGui::EndPopup();
					}

					if (ImGui::BeginPopup("Albedo Texture Settings"))
					{
						if (ImGui::MenuItem("Remove"))
						{
							m_Material->SetAlbedoMap(Renderer::GetWhiteTexture());
						}
						ImGui::EndPopup();
					}
				}
				else
				{
					UI::Image(EditorResources::CheckerBoardWhiteGrey, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
					
					renderToggle = false;
					
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							shouldSave = true;
							m_Material->SetAlbedoMap(AssetManager::GetAsset<Texture2D>(Data));
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (renderToggle)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Use", &m_Material->GetAlbedoTextureToggle());
				}
				ImGui::SameLine();
				if (ImGui::ColorEdit3("Albedo", m_Material->GetAlbedoColor().GetValue_Ptr()))
					shouldSave = true;
			}

			ImGui::NewLine();
			//Normal
			{
				bool renderToggle = true;
				Count<Texture2D> normalMap = m_Material->GetNormalMap();

				if (normalMap != nullptr && normalMap != Renderer::GetWhiteTexture())
				{

					UI::Image(normalMap, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
					if (ImGui::BeginPopupContextItem("Normal Texture Settings"))
					{
						ImGui::EndPopup();
					}

					if (ImGui::BeginPopup("Normal Texture Settings"))
					{
						if (ImGui::MenuItem("Remove"))
						{
							m_Material->SetNormalMap(Renderer::GetWhiteTexture());
						}
						ImGui::EndPopup();
					}
				}
				else
				{
					UI::Image(EditorResources::CheckerBoardWhiteGrey, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
					
					renderToggle = false;
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							shouldSave = true;
							m_Material->SetNormalMap(AssetManager::GetAsset<Texture2D>(Data));
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (renderToggle)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Use", &m_Material->GetNormalTextureToggle());
				}
			}
			ImGui::NewLine();
			//Metallic
			{
				bool renderToggle = true;
				Count<Texture2D> metallnesMap = m_Material->GetMetalnessMap();

				if (metallnesMap != nullptr && metallnesMap != Renderer::GetWhiteTexture())
				{
					UI::Image(metallnesMap, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
					if (ImGui::BeginPopupContextItem("Metallness Texture Settings"))
					{
						ImGui::EndPopup();
					}

					if (ImGui::BeginPopup("Metallness Texture Settings"))
					{
						if (ImGui::MenuItem("Remove"))
						{
							m_Material->SetMetalnessMap(Renderer::GetWhiteTexture());
						}
						ImGui::EndPopup();
					}
				}
				else
				{
					UI::Image(EditorResources::CheckerBoardWhiteGrey, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
					renderToggle = false;
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							shouldSave = true;
							m_Material->SetMetalnessMap(AssetManager::GetAsset<Texture2D>(Data));
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (renderToggle)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Use", &m_Material->GetMetalnessTextureToggle());
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("Metallness", &m_Material->GetMetalness(), 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp));
					shouldSave = true;
			}

			ImGui::NewLine();
			//Roughness
			{
				bool renderToggle = true;
				Count<Texture2D> roughnessMap = m_Material->GetRoughnessMap();

				if (roughnessMap != nullptr && roughnessMap != Renderer::GetWhiteTexture())
				{
					UI::Image(roughnessMap, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });

					if (ImGui::BeginPopupContextItem("Roughness Texture Settings"))
					{
						ImGui::EndPopup();
					}

					if (ImGui::BeginPopup("Roughness Texture Settings"))
					{
						if (ImGui::MenuItem("Remove"))
						{
							m_Material->SetRoughnessMap(Renderer::GetWhiteTexture());
						}
						ImGui::EndPopup();
					}

				}
				else
				{
					UI::Image(EditorResources::CheckerBoardWhiteGrey, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
					renderToggle = false;
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Texture).c_str()))
					{
						uint64_t Data = *(const uint64_t*)payload->Data;
						if (AssetManager::HasAsset(Data))
						{
							shouldSave = true;
							m_Material->SetRoughnessMap( AssetManager::GetAsset<Texture2D>(Data));
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (renderToggle)
				{
					ImGui::SameLine();
					ImGui::Checkbox("Use", &m_Material->GetRoughnessTextureToggle());
				}
				ImGui::SameLine();
				if (ImGui::SliderFloat("Roughness", &m_Material->GetRoughness(), 0, 1, "%.3f", ImGuiSliderFlags_AlwaysClamp));
				shouldSave = true;
			}
			
			//SceneHierachyPanel::DrawVector2Control("Tiling", m_Material->GetTiling());
			//SceneHierachyPanel::DrawVector2Control("Offset", m_Material->Offset);
			
			ImGui::End();
			ImGui::PopID();
			contdown -= deltaTime;
			if (shouldSave || contdown <= 0)
			{

				contdown = 10;
				AssetManager::SaveAsset(m_Material->GetID());
			}
		}
	}

	PhysicsMaterialEditorPanel::PhysicsMaterialEditorPanel(Count<PhysicsMaterial> material) :
		m_Material(material)
	{
	}
	void PhysicsMaterialEditorPanel::ImGuiRender(FrameTime deltaTime) {
		if (m_ShowWindow == false)
			return;
		PF_PROFILE_FUNC();
		if (!m_Material)return;

		// if any data is changed
		bool shouldSave = false;
		ImGui::PushID((MemoryAddress) & *m_Material.Get());
		std::string name = "No name";
		if (AssetManager::HasAsset(m_Material))
		{
			name = AssetManager::GetAssetInfo(m_Material).GetName();
		}
		ImGui::Begin(name.c_str(), &m_ShowWindow);
		{
			float staticFriction = m_Material->GetStaticFriction();
			float dynamicFrction = m_Material->GetDynamicFriction();
			float bounciness = m_Material->GetBounciness();

			if (ImGui::DragFloat("StaticFriction", &staticFriction, 0, FLT_MAX))
			{

				m_Material->SetStaticFriction(staticFriction);
				shouldSave = true;
			}
			if (ImGui::DragFloat("DynamicFriction", &dynamicFrction, 0, FLT_MAX))
			{
				shouldSave = true;
				m_Material->SetDynamicFriction(dynamicFrction);
			}
			if (ImGui::DragFloat("Bounciness", &bounciness, 0, 1))
			{
				shouldSave = true;
				m_Material->SetBounciness(bounciness);
			}
			CombineMode frictionMode = m_Material->GetFrictionCombineMode();
			CombineMode	bounceMode = m_Material->GetBouncinessCombineMode();
			if (ExternalAPI::ImGUIAPI::EnumCombo("FrictionCombine", frictionMode))
			{
				shouldSave = true;
				m_Material->SetFrictionCombineMode(frictionMode);
			}
			if (ExternalAPI::ImGUIAPI::EnumCombo("BouncinessCombine", bounceMode))
			{
				shouldSave = true;
				m_Material->SetBouncinessCombineMode(bounceMode);
			}
		}
		ImGui::End();
		ImGui::PopID();


		if (shouldSave)
		{
			AssetManager::SaveAsset(m_Material->GetID());
		}
	}

	ParticleSystemPanel::ParticleSystemPanel(Count<ParticleSystem> particle)
	{
		m_ParticleSystem = particle;
		m_World = Count<World>::Create();
		Entity entity = m_World->CreateEntity("particle");
		m_ParticleHandler = Count<ParticleHandler>::Create(m_ParticleSystem);
		entity.AddComponent<ParticleSystemComponent>().ParticleHandlerTable->SetHandler(0, m_ParticleHandler);
		entity.GetComponent<TransformComponent>().Location.Z -= 20.0f;
		//m_WorldRenderer = Count<WorldRenderer>::Create(m_World, 10, 10);
	}
	void ParticleSystemPanel::ImGuiRender(FrameTime deltaTime)
	{
		if (m_ShowWindow == false)
			return;
		m_Save -= deltaTime;
		if (m_Save <= 0.0f)
		{
			AssetManager::SaveAsset(m_ParticleSystem->GetID());
			m_Save = 10.0f;
		}
		m_World->OnUpdateEditor(deltaTime);
		UI::ScopedID  scopedID(std::to_string(m_ParticleSystem->GetID()));
		std::string particleSystemName = fmt::format("{} ParticleSystem Panel", AssetManager::GetAssetInfo(m_ParticleSystem).GetName());
		UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin(particleSystemName.c_str(), &m_ShowWindow);
		ImGui::BeginChild("Particle Data", { ImGui::GetContentRegionAvail().x / 3, ImGui::GetContentRegionAvail().y });
		{
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
			auto& location = m_World->FindEntityByTag("particle").GetComponent<TransformComponent>().Location;
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

			ExternalAPI::ImGUIAPI::CheckBox("Loop", &m_ParticleSystem->Loop);
			ExternalAPI::ImGUIAPI::CheckBox("PlayOnAwake", &m_ParticleSystem->PlayOnAwake);
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


		}
		ImGui::SameLine();
		{

			UI::ScopedStyleColor bgColor(ImGuiCol_ChildBg, { 0,0,0,1 });
			ImGui::BeginChild("Window", ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y });
			if (m_Size != Vector2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y })
			{
				m_Camera.OnUpdate(deltaTime, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
				m_Size = Vector2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y };
				//m_WorldRenderer->Resize(ScreenSize{ (uint32_t)m_Size.X,(uint32_t)m_Size.Y });
			}
			if (ImGui::IsWindowFocused())
			{
				Application::Get()->GetWindow()->SetWindowInputEvent(true);
				m_Camera.OnUpdate(deltaTime, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
				Application::Get()->GetWindow()->SetWindowInputEvent(false);
			}

			//m_WorldRenderer->Render(m_Camera, RenderSettings());
			//
			//UI::Image(m_WorldRenderer->GetImage(), ImVec2{ ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::EndChild();
		}
		ImGui::End();
	}
}
