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
#include "Proof/Renderer/Texture.h"

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

	enum class ShaderType
	{
		Standard,
		Transparent,
		Glitchy
	};
	void Proof::MaterialEditorPanel::RenderDetailSettings()
	{
		std::string name = m_Material->Name;

		Count<RenderMaterial> renderMaterial = m_Material->GetRenderMaterial().As<RenderMaterial>();
		auto shaderName = fmt::format("Shader: {}", renderMaterial->GetConfig().Shader->GetName());
		ImGui::Text(shaderName.c_str());

		PbrSurfaceMaterial surfaceMat(m_Material);

		ShaderType type;

		if(renderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static"))
			type = ShaderType::Standard;
		else if (renderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBRTransparent_Static"))
			type = ShaderType::Transparent;
		else
			type = ShaderType::Glitchy;

		if(UI::EnumCombo("ShaderType",type))
		{
			switch (type)
			{
			case ShaderType::Standard:
				m_Material->SetMaterialShader(m_Material->Name, Renderer::GetShader("ProofPBR_Static"));
				break;
			case ShaderType::Transparent:
				m_Material->SetMaterialShader(m_Material->Name, Renderer::GetShader("ProofPBRTransparent_Static"));
				break;
			case ShaderType::Glitchy:
				m_Material->SetMaterialShader(m_Material->Name, Renderer::GetShader("ProofPBR_Glitchy"));
				break;
			default:
				break;
			}
			// dont render this frame so the render material can be set ready in the next frame
			return;
		}

		bool transparentShader = renderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBRTransparent_Static");

	
		UI::PushModified(m_NeedsSaving);
		//Albedo
		if(UI::AttributeTreeNode("Albedo"))
		{
			AssetID outHandle ;
			if (surfaceMat.GetAlbedoMap() == nullptr)
			{
				surfaceMat.SetAlbedoMap(Renderer::GetWhiteTexture());
				outHandle = 0;
			}
			else
				outHandle = surfaceMat.GetAlbedoMap()->GetID();

			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					surfaceMat.SetAlbedoMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					surfaceMat.SetAlbedoMap(Renderer::GetWhiteTexture());
			}
			
			ImGui::SameLine();
			if(!transparentShader)
				UI::AttributeColor("", surfaceMat.GetAlbedoColor());
			else
				UI::AttributeColor("", renderMaterial->GetVector4("u_MaterialUniform.Albedo"));
			UI::EndTreeNode();
		}

		//Normal
		if (UI::AttributeTreeNode("Normal"))
		{
			AssetID outHandle;
			if (surfaceMat.GetNormalMap() == nullptr)
			{
				surfaceMat.SetNormalMap(Renderer::GetWhiteTexture());
				outHandle = 0;
			}
			else
				outHandle = surfaceMat.GetNormalMap()->GetID();
			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					surfaceMat.SetNormalMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					surfaceMat.SetNormalMap(Renderer::GetWhiteTexture());
			}

			ImGui::SameLine();
			UI::AttributeBool("", surfaceMat.GetNormalTextureToggle());
			UI::EndTreeNode();
		}
		
		//Metalness
		if (UI::AttributeTreeNode("Metalness"))
		{
			AssetID outHandle;
			if (surfaceMat.GetMetalnessMap() == nullptr)
			{
				surfaceMat.SetMetalnessMap(Renderer::GetWhiteTexture());
				outHandle = 0;
			}
			else
				outHandle = surfaceMat.GetMetalnessMap()->GetID();

			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					surfaceMat.SetMetalnessMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					surfaceMat.SetMetalnessMap(Renderer::GetWhiteTexture());
			}
			ImGui::SameLine();
			UI::AttributeSlider("", surfaceMat.GetMetalness(),0,1);
			UI::EndTreeNode();
		}

		//roughness
		if (UI::AttributeTreeNode("Roughness"))
		{
			AssetID outHandle;
			if (surfaceMat.GetRoughnessMap() == nullptr)
			{
				surfaceMat.SetRoughnessMap(Renderer::GetWhiteTexture());
				outHandle = 0;
			}
			else
				outHandle = surfaceMat.GetRoughnessMap()->GetID();

			if (UI::AttributeTextureAssetReference("", outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					surfaceMat.SetRoughnessMap(AssetManager::GetAsset<Texture2D>(outHandle));
				else
					surfaceMat.SetRoughnessMap(Renderer::GetWhiteTexture());
			}

			ImGui::SameLine();
			UI::AttributeSlider("", surfaceMat.GetRoughness(), 0, 1);
			UI::EndTreeNode();
		}

		if (UI::AttributeTreeNode("Emission"))
		{
			UI::AttributeDrag("Emission", surfaceMat.GetEmission(), 0.15, 0);

			bool& ovverideToggle = surfaceMat.GetEmissionOverrideColorToggle();

			UI::AttributeBool("OverrideEmissionColor", ovverideToggle);


			UI::PushItemDisabled(!ovverideToggle);
			UI::AttributeColor("EmissionColor", surfaceMat.GetEmissionOverrideColor());
			UI::PopItemDisabled();

			UI::EndTreeNode();
		}
		
		UI::AttributeDrag("Tiling", surfaceMat.GetTiling());
		UI::AttributeDrag("Offset", surfaceMat.GetOffset());

		if(type ==ShaderType::Glitchy)
		{
			float glitchRate = renderMaterial->GetFloat("GlitchRate");
			if (UI::AttributeDrag("GlitchRate", glitchRate,0.1f,0.f,10.f))
			{
				renderMaterial->Set("GlitchRate", glitchRate);
			}
			float glitchScale = renderMaterial->GetFloat("GlitchScale");
			if (UI::AttributeDrag("GlitchScale", glitchScale, 0.1f, 0.f, 10.f))
			{
				renderMaterial->Set("GlitchScale", glitchScale);
			}
		}

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
