#include "MaterialEditorPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "../Editor3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
	MaterialEditorPanel::MaterialEditorPanel(MaterialAsset* material) {
		m_MaterialAsset = material;

		m_CheckeboardTexture = Texture2D::Create("Assets/Textures/CheckeboardTexture.jpg");
		whiteColourId  = m_CheckeboardTexture->GetID();
	}

	void MaterialEditorPanel::ImGuiRender(FrameTime deltaTime) {
		if(m_ShowWindow==false)
			return;

		PF_PROFILE_FUNC();

		ImGui::Begin(m_MaterialAsset->GetName().c_str(),&m_ShowWindow);
		{
			ImGui::ColorEdit3("Colour",glm::value_ptr(m_MaterialAsset->m_Material.m_Colour));
			ImGui::Image((ImTextureID)(m_MaterialAsset->m_Material.AlbedoTexture == nullptr ? whiteColourId : m_MaterialAsset->m_Material.AlbedoTexture->GetID()),{50,50});
			if (ImGui::Button("Albedo",{100,50})) {
				std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
				if (file.empty() == false) {
					m_MaterialAsset->m_Material.AlbedoTexture = Texture2D::Create(file);
				}
			}
			ImGui::NewLine();


			ImGui::Image((ImTextureID)(m_MaterialAsset->m_Material.MetallicTexture == nullptr ? whiteColourId : m_MaterialAsset->m_Material.MetallicTexture->GetID()),{50,50});
			if (ImGui::Button("Metallness",{100,50})) {
				std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
				if (file.empty() == false) {
					m_MaterialAsset->m_Material.MetallicTexture = Texture2D::Create(file);
				}
			}
			ImGui::NewLine();
			ImGui::DragFloat("Metallnes",&m_MaterialAsset->m_Material.m_Metallness,0.001);

			ImGui::Image((ImTextureID)(m_MaterialAsset->m_Material.RoughnessTexture == nullptr ? whiteColourId : m_MaterialAsset->m_Material.RoughnessTexture->GetID()),{50,50});
			if (ImGui::Button("RoughnessTexture",{100,50})) {
				std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
				if (file.empty() == false) {
					m_MaterialAsset->m_Material.RoughnessTexture = Texture2D::Create(file);
				}
			}
			ImGui::NewLine();
			ImGui::DragFloat("Roughness",&m_MaterialAsset->m_Material.m_Roughness,0.001);


			if (ImGui::Button("Normal",{100,50})) {
				std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
				if (file.empty() == false) {
					m_MaterialAsset->m_Material.NormalTexture = Texture2D::Create(file);
				}
			}
			ImGui::Image((ImTextureID)(m_MaterialAsset->m_Material.NormalTexture == nullptr ? whiteColourId : m_MaterialAsset->m_Material.NormalTexture->GetID()),{50,50});

			ImGui::NewLine();
			ImGui::DragFloat("AO",&m_MaterialAsset->m_Material.m_AO,0.001);

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