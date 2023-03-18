#include "MaterialEditorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Editor3D/Editor3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Scene/Material.h"
#include "Proof/Core/Core.h"
namespace Proof
{
	

	MaterialEditorPanel::MaterialEditorPanel(Count<Material> material):
		m_Material(material)
	{
	}

	void MaterialEditorPanel::ImGuiRender(FrameTime deltaTime) {
		if(m_ShowWindow==false)
			return;

		// save if any data is changed
		bool shouldSave = false;

		PF_PROFILE_FUNC();
		if (!m_Material)return;
		ImGui::PushID((MemoryAddress)&*m_Material.Get());
		std::string name = "No name";
		if (AssetManager::HasAsset(m_Material))
		{
			name = AssetManager::GetAssetInfo(m_Material).GetName();
		}
		ImGui::Begin(name.c_str(),&m_ShowWindow);
		{
			if (ImGui::ColorEdit3("Colour", m_Material->Colour.GetValue_Ptr()))
			{
				shouldSave = true;
			}
			ImGui::NewLine();
			
			if (ImGui::SliderFloat("Metallnes", &m_Material->Metallness, 0, 1))
			{
				shouldSave = true;
			}
			ImGui::NewLine();

			if (ImGui::SliderFloat("Roughness", &m_Material->Roughness, 0, 1))
			{
				shouldSave = true;
			}
		}
		ImGui::End();
		ImGui::PopID();

		if (shouldSave)
		{
			AssetManager::SaveAsset(m_Material->GetID());
		}
	}

	PhysicsMaterialEditorPanel::PhysicsMaterialEditorPanel(Count<PhysicsMaterial> material):
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

			if (ImGui::SliderFloat("StaticFriction", &staticFriction, 0, FLT_MAX, "0.05", ImGuiSliderFlags_AlwaysClamp));
			{
				m_Material->SetStaticFriction(staticFriction);
				shouldSave = true;
			}
			if (ImGui::SliderFloat("DynamicFriction", &dynamicFrction, 0, FLT_MAX, "0.05", ImGuiSliderFlags_AlwaysClamp));
			{
				shouldSave = true;
				m_Material->SetDynamicFriction(dynamicFrction);
			}
			if (ImGui::SliderFloat("Bounciness", &bounciness, 0, 1, "0.01", ImGuiSliderFlags_AlwaysClamp));
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
}