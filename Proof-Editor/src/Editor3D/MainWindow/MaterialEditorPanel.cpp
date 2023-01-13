#include "MaterialEditorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Editor3D/Editor3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
	MaterialEditorPanel::MaterialEditorPanel(MaterialAsset* material) {
		m_MaterialAsset = material;

	}

	void MaterialEditorPanel::ImGuiRender(FrameTime deltaTime) {
		if(m_ShowWindow==false)
			return;

		PF_PROFILE_FUNC();

		ImGui::Begin(m_MaterialAsset->GetName().c_str(),&m_ShowWindow);
		{
			ImGui::ColorEdit3("Colour",m_MaterialAsset->m_Material.Colour.GetValue_Ptr());
			ImGui::NewLine();
			ImGui::SliderFloat ("Metallnes", &m_MaterialAsset->m_Material.Metallness,0,1);
			ImGui::NewLine();
			ImGui::SliderFloat("Roughness",&m_MaterialAsset->m_Material.Roughness, 0, 1);

			m_MaterialAsset->SaveAsset();
		}
		ImGui::End();
	}
	PhysicsMaterialEditorPanel::PhysicsMaterialEditorPanel(PhysicsMaterialAsset* material) {
		m_MaterialAsset = material;
	}
	void PhysicsMaterialEditorPanel::ImGuiRender(FrameTime deltaTime) {
		if (m_ShowWindow == false)
			return;
		PF_PROFILE_FUNC();
		ImGui::Begin(m_MaterialAsset->GetName().c_str(), &m_ShowWindow);
		{
			ImGui::DragFloat("StaticFriction", &m_MaterialAsset->m_Material.StaticFriction, 0.05);
			ImGui::DragFloat("DynamicFriction", &m_MaterialAsset->m_Material.DynamicFriction, 0.05);
			ImGui::DragFloat("Bounciness", &m_MaterialAsset->m_Material.Bounciness, 0.05);

			ExternalAPI::ImGUIAPI::EnumCombo("FrictionCombine", m_MaterialAsset->m_Material.FrictionCombineMode);
			ExternalAPI::ImGUIAPI::EnumCombo("BouncinessCombine", m_MaterialAsset->m_Material.BouncinessCombineMode);
		}
		ImGui::End();
	}
}