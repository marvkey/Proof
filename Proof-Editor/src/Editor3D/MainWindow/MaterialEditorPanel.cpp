#include "MaterialEditorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Editor3D/Editor3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer.h"
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
			ImGui::ColorEdit3("Colour", m_Material->Colour.GetValue_Ptr());
			ImGui::NewLine();
			ImGui::SliderFloat ("Metallnes", &m_Material->Metallness,0,1);
			ImGui::NewLine();
			ImGui::SliderFloat("Roughness",&m_Material->Roughness, 0, 1);
		}
		ImGui::End();
		ImGui::PopID();
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
		ImGui::PushID((MemoryAddress) & *m_Material.Get());
		std::string name = "No name";
		if (AssetManager::HasAsset(m_Material))
		{
			name = AssetManager::GetAssetInfo(m_Material).GetName();
		}
		ImGui::Begin(name.c_str(), &m_ShowWindow);
		{
			ImGui::DragFloat("StaticFriction", &m_Material->StaticFriction, 0.05);
			ImGui::DragFloat("DynamicFriction", &m_Material->DynamicFriction, 0.05);
			ImGui::DragFloat("Bounciness", &m_Material->Bounciness, 0.05);

			ExternalAPI::ImGUIAPI::EnumCombo("FrictionCombine", m_Material->FrictionCombineMode);
			ExternalAPI::ImGUIAPI::EnumCombo("BouncinessCombine", m_Material->BouncinessCombineMode);
		}
		ImGui::End();
		ImGui::PopID();
	}
}