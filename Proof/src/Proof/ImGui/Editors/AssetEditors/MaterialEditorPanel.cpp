#include "Proofprch.h"
#include "MaterialEditorPanel.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "../EditorResources.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Scene/Material.h"
#include "../Panels/SceneHierachyPanel.h"

#include "Proof/Scene/Material.h"
#include "Proof/Renderer/Shader.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Physics/PhysicsMaterial.h"
#include "Proof/ImGui/UI.h"

#include "Proof/Core/Profile.h"
#include "Proof/ImGui/Editors/Panels/DetailsPanel.h"
namespace Proof
{


	MaterialEditorPanel::MaterialEditorPanel()
		:
		AssetEditor("MaterialEditorPanel")
	{

	}
	void MaterialEditorPanel::OnImGuiRender()
	{
		// save if any data is changed
		PF_PROFILE_FUNC();
		if (!m_Material)return;
		m_DetailsPanel->OnImGuiRender(m_DetailsPanelName.c_str(), m_IsDetailsPanelOpen);
	}
	void MaterialEditorPanel::OnUpdate(FrameTime ts)
	{
		AssetEditor::OnUpdate(ts);
	}
	void MaterialEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::Material)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to MaterialEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_Material = asset.As<Material>();
		m_DetailsPanel = Count<DetailsPanel>::Create(std::bind(&MaterialEditorPanel::RenderDetailSettings,this));
		m_DetailsPanelName = fmt::format("Details##Material: {}", m_Material->GetID());
	}
	void MaterialEditorPanel::Save()
	{
		if (!m_Material)return;
		m_NeedsSaving = false;
		AssetManager::SaveAsset(m_Material->GetID());
	}
	void Proof::MaterialEditorPanel::RenderDetailSettings()
	{
		std::string name = m_Material->Name;

		Count<RenderMaterial> renderMaterial = m_Material->GetRenderMaterial().As<RenderMaterial>();
		auto shaderName = fmt::format("Shader: {}", renderMaterial->GetConfig().Shader->GetName());
		ImGui::Text(shaderName.c_str());

		UI::PushModified(m_NeedsSaving);
		//Albedo
		{
			bool renderToggle = true;
			Count<Texture2D> albedoMap = m_Material->GetAlbedoMap();
			if (albedoMap != nullptr && albedoMap != Renderer::GetWhiteTexture())
			{
				UI::Image(albedoMap, { ImGui::GetWindowWidth() / 4,ImGui::GetWindowHeight() / 6 });
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
						m_NeedsSaving = true;
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
				m_NeedsSaving = true;
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
						m_NeedsSaving = true;
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
						m_NeedsSaving = true;
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
			if (UI::AttributeSlider("Metallness", m_Material->GetMetalness(), 0, 1))
				m_NeedsSaving = true;
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
						m_NeedsSaving = true;
						m_Material->SetRoughnessMap(AssetManager::GetAsset<Texture2D>(Data));
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (renderToggle)
			{
				ImGui::SameLine();
				ImGui::Checkbox("Use", &m_Material->GetRoughnessTextureToggle());
				m_NeedsSaving = true;

			}
			ImGui::SameLine();
			if (UI::AttributeSlider("Roughness", m_Material->GetRoughness(), 0, 1))
				m_NeedsSaving = true;

		}
		ImGui::NewLine();
		//
		{
			if (UI::AttributeDrag("Emission", m_Material->GetEmission(), 0.25f))
				m_NeedsSaving = true;

		}
		UI::PopItemDisabled();
	}
	
	void MaterialEditorPanel::SetDefaultLayout()
	{
		ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());
		ImGuiWindow* window = ImGui::FindWindowByName(m_DetailsPanelName.c_str());
		if (m_DetailsPanel->GetImGuiWindow())
		{
			ImGui::SetWindowDock(m_DetailsPanel->GetImGuiWindow(), dockspace_id, 0);
		}
	}

	PhysicsMaterialEditorPanel::PhysicsMaterialEditorPanel()
		:
		AssetEditor("PhysicsMaterialEditorPanel")
	{
	}
	void PhysicsMaterialEditorPanel::OnImGuiRender() 
	{
		PF_PROFILE_FUNC();
		if (!m_Material)return;
		m_DetailsPanel->OnImGuiRender(m_DetailsPanelName.c_str(), m_IsDetailsPanelOpen);
	}
	void PhysicsMaterialEditorPanel::Save()
	{
		if (!m_Material)return;

		m_NeedsSaving = false;
		AssetManager::SaveAsset(m_Material->GetID());
	}
	void PhysicsMaterialEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::PhysicsMaterial)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to PhysicsMaterialEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_Material = asset.As<PhysicsMaterial>();
		m_DetailsPanelName = fmt::format("Details##PhysicsMaterial: {}", m_Material->GetID());
		m_DetailsPanel = Count<DetailsPanel>::Create(std::bind(&PhysicsMaterialEditorPanel::RenderDetailSettings, this));
	}

	void PhysicsMaterialEditorPanel::RenderDetailSettings()
	{
		// if any data is changed
		float staticFriction = m_Material->GetStaticFriction();
		float dynamicFrction = m_Material->GetDynamicFriction();
		float bounciness = m_Material->GetBounciness();

		UI::BeginPropertyGrid();
		UI::PushModified(m_NeedsSaving);
		if (UI::AttributeDrag("StaticFriction", staticFriction))
		{

			m_Material->SetStaticFriction(staticFriction);
		}
		if (UI::AttributeDrag("DynamicFriction", dynamicFrction))
		{
			m_Material->SetDynamicFriction(dynamicFrction);
		}
		if (UI::AttributeDrag("Bounciness", bounciness, 1, 0, 1))
		{
			m_Material->SetBounciness(bounciness);
		}
		CombineMode frictionMode = m_Material->GetFrictionCombineMode();
		CombineMode	bounceMode = m_Material->GetBouncinessCombineMode();
		if (UI::EnumCombo("FrictionCombine", frictionMode))
		{
			m_Material->SetFrictionCombineMode(frictionMode);
		}
		if (UI::EnumCombo("BouncinessCombine", bounceMode))
		{
			m_Material->SetBouncinessCombineMode(bounceMode);
		}
		UI::PopModified();
		UI::EndPropertyGrid();

	}

	void PhysicsMaterialEditorPanel::SetDefaultLayout()
	{
		ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());
		ImGuiWindow* window = ImGui::FindWindowByName(m_DetailsPanelName.c_str());
		if (m_DetailsPanel->GetImGuiWindow())
		{
			ImGui::SetWindowDock(m_DetailsPanel->GetImGuiWindow(), dockspace_id, 0);
		}
	}
	
}
