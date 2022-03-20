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
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)return;

		m_CheckeboardTexture = Texture2D::Create("Assets/Textures/CheckeboardTexture.jpg");
		whiteColourId  = m_CheckeboardTexture->GetID();
	}

	void MaterialEditorPanel::ImGuiRender(FrameTime deltaTime) {
		if(m_ShowWindow==false)
			return;
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
}