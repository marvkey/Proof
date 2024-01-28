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
#include "Proof/ImGui/UIHandlers.h"

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
		if(UI::AttributeTreeNode("Albedo"))
		{
			auto outHandle = m_Material->GetAlbedoMap()->GetID();
			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					m_Material->SetAlbedoMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					m_Material->SetAlbedoMap(Renderer::GetWhiteTexture());
			}
			
			ImGui::SameLine();
			UI::AttributeColor("", m_Material->GetAlbedoColor());
			UI::EndTreeNode();
		}

		//Normal
		if (UI::AttributeTreeNode("Normal"))
		{
			auto outHandle = m_Material->GetNormalMap()->GetID();
			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					m_Material->SetNormalMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					m_Material->SetNormalMap(Renderer::GetWhiteTexture());
			}

			ImGui::SameLine();
			UI::AttributeBool("", m_Material->GetNormalTextureToggle());
			UI::EndTreeNode();
		}
		
		//Metalness
		if (UI::AttributeTreeNode("Metalness"))
		{
			auto outHandle = m_Material->GetMetalnessMap()->GetID();
			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					m_Material->SetMetalnessMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					m_Material->SetMetalnessMap(Renderer::GetWhiteTexture());
			}
			ImGui::SameLine();
			UI::AttributeSlider("", m_Material->GetMetalness(),0,1);
			UI::EndTreeNode();
		}

		//roughness
		if (UI::AttributeTreeNode("Roughness"))
		{
			auto outHandle = m_Material->GetRoughnessMap()->GetID();
			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					m_Material->SetRoughnessMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					m_Material->SetRoughnessMap(Renderer::GetWhiteTexture());
			}

			ImGui::SameLine();
			UI::AttributeSlider("", m_Material->GetRoughness(), 0, 1);
			UI::EndTreeNode();
		}

		if (UI::AttributeTreeNode("Emission"))
		{
			UI::AttributeDrag("Emission", m_Material->GetEmission(), 0.15, 0);

			bool& ovverideToggle = m_Material->GetEmissionOverrideColorToggle();

			UI::AttributeBool("OverrideEmissionColor", ovverideToggle);


			UI::PushItemDisabled(!ovverideToggle);
			UI::AttributeColor("EmissionColor", m_Material->GetEmissionOverrideColor());
			UI::PopItemDisabled();

			UI::EndTreeNode();
		}
		
		UI::AttributeDrag("Tiling", m_Material->GetTiling());
		UI::AttributeDrag("Offset", m_Material->GetOffset());

		UI::PopModified();
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
	bool MaterialEditorPanel::IsSubWindowsHovered()
	{
		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
	}
	bool MaterialEditorPanel::IsSubWindowsFocused()
	{
		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
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
	bool PhysicsMaterialEditorPanel::IsSubWindowsHovered()
	{
		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
	}
	bool PhysicsMaterialEditorPanel::IsSubWindowsFocused()
	{

		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
	}
}
