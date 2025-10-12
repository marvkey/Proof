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
#include "Proof/Renderer/Shader.h"

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

		{

			std::vector<std::string> selecteableShaders;
			for(auto& [name, shader] : Renderer::GetShaderLibrary()->GetShaderMap())
			{
				if (shader->GetAllShaderMacroDefines().contains("MATERIAL_SURFACE") || shader->GetAllShaderMacroDefines().contains("MATERIAL_POST_PROCESS"))
				{
					selecteableShaders.push_back(name);
				}
			}

			auto [changed, outSelectionIndex, outSelectiongString]
				= UI::Combo("Sahder", selecteableShaders, renderMaterial->GetConfig().Shader->GetName());

			if (changed)
			{
				m_Material->SetMaterialShader(m_Material->Name, Renderer::GetShader(outSelectiongString));
				// dont render this frame so the render material can be set ready in the next frame
				return;
			}
		}
		
		if (renderMaterial->GetConfig().Shader->GetName() == "ProofPBR_Static" || renderMaterial->GetConfig().Shader->GetName() == "ProofPBRTransparent_Static")
			RenderDefautlPBRShader();
		else
			RenderMaterials();
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
	void MaterialEditorPanel::RenderDefautlPBRShader()
	{
		Count<RenderMaterial> renderMaterial = m_Material->GetRenderMaterial().As<RenderMaterial>();

		PbrSurfaceMaterial surfaceMat(m_Material);

		bool transparentShader = renderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBRTransparent_Static");


		UI::PushModified(m_NeedsSaving);
		//Albedo
		if (UI::AttributeTreeNode("Albedo"))
		{
			AssetID outHandle;
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
			if (!transparentShader)
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
			UI::AttributeSlider("", surfaceMat.GetMetalness(), 0, 1);
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

		UI::PopModified();
	}
	void MaterialEditorPanel::RenderMaterials()
	{
		Count<RenderMaterial> renderMaterial = m_Material->GetRenderMaterial().As<RenderMaterial>();

		auto variables = m_Material->GetMaterialVariables();

		UI::BeginPropertyGrid();
		for (auto& [name, varType] : variables)
		{
			switch (varType)
			{
				case VariableTypes::Bool:
				{
					bool value = renderMaterial->GetBool(name);
					if (UI::AttributeBool(name.c_str(), value))
					{
						renderMaterial->Set(name, value);
						m_NeedsSaving = true;
					}
				}
				break;
				case VariableTypes::Int:
				{
					int value = renderMaterial->GetInt(name);
					if (UI::AttributeDrag(name.c_str(), value))
					{
						renderMaterial->Set(name, value);
						m_NeedsSaving = true;
					}
				}
				break;
				case VariableTypes::Float:
				{
					float value = renderMaterial->GetFloat(name);
					if (UI::AttributeDrag(name.c_str(), value))
					{
						renderMaterial->Set(name, value);
						m_NeedsSaving = true;
					}
				}
				break;
				case VariableTypes::Vec2:
				{
					glm::vec2 value = renderMaterial->GetVector2(name);
					if (UI::AttributeDrag(name.c_str(), value))
					{
						renderMaterial->Set(name, value);
						m_NeedsSaving = true;
					}
				}
				break;
				case VariableTypes::Vec3:
				{
					glm::vec3 value = renderMaterial->GetVector(name);
					if (UI::AttributeDrag(name.c_str(), value))
					{
						renderMaterial->Set(name, value);
						m_NeedsSaving = true;
					}
				}
				break;
				case VariableTypes::Vec4:
				{
					glm::vec4 value = renderMaterial->GetVector4(name);
					if (UI::AttributeDrag(name.c_str(), value))
					{
						renderMaterial->Set(name, value);
						m_NeedsSaving = true;
					}
				}
				break;
			}
		}

		UI::EndPropertyGrid();
		auto editableTextures = m_Material->GetEditableTextures();

		ImGui::Separator();

		UI::BeginPropertyGrid();
		for(auto textureName : editableTextures)
		{
			AssetID outHandle;
			if (renderMaterial->TryGetTexture2D(textureName) == nullptr)
			{
				outHandle = 0;
			}
			else
				outHandle = renderMaterial->TryGetTexture2D(textureName)->GetID();

			if (UI::AttributeTextureAssetReference(textureName.c_str(), outHandle))
			{
				if (AssetManager::HasAsset(outHandle))
					renderMaterial->Set(textureName, AssetManager::GetAsset<Texture2D>(outHandle));
				else
					renderMaterial->Set(textureName, Renderer::GetWhiteTexture());
				m_NeedsSaving = true;
			}
		}

		UI::EndPropertyGrid();
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
