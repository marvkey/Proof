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
		auto material = m_MaterialAsset->GetMaterial();

		ImGui::Begin(m_MaterialAsset->GetName().c_str(),&m_ShowWindow);
		{
			ImGui::ColorEdit3("Colour", material->Colour.GetValue_Ptr());
			ImGui::NewLine();
			ImGui::SliderFloat ("Metallnes", &material->Metallness,0,1);
			ImGui::NewLine();
			ImGui::SliderFloat("Roughness",&material->Roughness, 0, 1);

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
		auto material = m_MaterialAsset->GetMaterial();
		ImGui::Begin(m_MaterialAsset->GetName().c_str(), &m_ShowWindow);
		{
			ImGui::DragFloat("StaticFriction", &material->StaticFriction, 0.05);
			ImGui::DragFloat("DynamicFriction", &material->DynamicFriction, 0.05);
			ImGui::DragFloat("Bounciness", &material->Bounciness, 0.05);

			ExternalAPI::ImGUIAPI::EnumCombo("FrictionCombine", material->FrictionCombineMode);
			ExternalAPI::ImGUIAPI::EnumCombo("BouncinessCombine", material->BouncinessCombineMode);
		}
		ImGui::End();
	}
}