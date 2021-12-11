#include "AssetManagerPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Resources/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
namespace Proof {
	void AssetManagerPanel::ImGuiRender(FrameTime deltaTime){
		if (m_ShowWindow == false)
			return;
		ImGui::Begin("Asset Manager", &m_ShowWindow);
		{
			for (auto it : AssetManager::s_AssetManager->m_AllAssets) {
				ImGui::BeginChildFrame((ImGuiID)it.first.Get(),{ImGui::GetContentRegionAvailWidth(),100},ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoScrollbar);
				ExternalAPI::ImGUIAPI::TextBar("Asset ID", std::to_string(it.first));
				if(it.second.second==nullptr)
					ExternalAPI::ImGUIAPI::TextBar("Loaded", "False");
				else
					ExternalAPI::ImGUIAPI::TextBar("Loaded", "True");

				ExternalAPI::ImGUIAPI::TextBar("Path", it.second.first.Path.string());

				if(it.second.first.Type == MeshAsset::GetAssetType())
					ExternalAPI::ImGUIAPI::TextBar("Type", "MeshAsset");
				else if(it.second.first.Type == Texture2DAsset::GetAssetType())
					ExternalAPI::ImGUIAPI::TextBar("Type", "Texture2DAsset");
				else
					ExternalAPI::ImGUIAPI::TextBar("Type", "MaterialAsset");

				ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}