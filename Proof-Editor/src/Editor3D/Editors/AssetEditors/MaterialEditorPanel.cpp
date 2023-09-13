#include "MaterialEditorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../../ImGUIAPI.h"
#include "../../EditorResources.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Scene/Material.h"
#include "../Panels/SceneHierachyPanel.h"

#include "Proof/Scene/Material.h"
#include "Proof/Renderer/Shader.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Renderer.h"

#include "Proof/Core/Profile.h"
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
		bool shouldSave = false;

		PF_PROFILE_FUNC();
		if (!m_Material)return;
		std::string name = m_Material->Name;

		Count<RenderMaterial> renderMaterial = m_Material->GetRenderMaterial().As<RenderMaterial>();
		auto shaderName = fmt::format("Shader: {}", renderMaterial->GetConfig().Shader->GetName());
		ImGui::Text(shaderName.c_str());
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
						m_Material->SetRoughnessMap(AssetManager::GetAsset<Texture2D>(Data));
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

		if (shouldSave || m_SaveCountDown <= 0)
		{

			m_SaveCountDown = 100;
			AssetManager::SaveAsset(m_Material->GetID());
		}
	}
	void MaterialEditorPanel::OnUpdate(FrameTime ts)
	{
		m_SaveCountDown -= ts;
	}
	void MaterialEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::Material)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to MaterialEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_Material = asset.As<Material>(); 
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

		// if any data is changed
		bool shouldSave = false;
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

		if (shouldSave)
		{
			AssetManager::SaveAsset(m_Material->GetID());
		}
	}
	void PhysicsMaterialEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::PhysicsMaterial)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to PhysicsMaterialEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_Material = asset.As<PhysicsMaterial>();
	}

	
}
