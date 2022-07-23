#include "AssetManagerPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../ImGUIAPI.h"
#include "Proof/Resources/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Resources/Asset/TextureAsset/TextureAsset.h"
#include <fmt/format.h>
namespace Proof {
	void AssetManagerPanel::ImGuiRender(FrameTime deltaTime){
		if (m_ShowWindow == false)
			return;
		PF_PROFILE_FUNC();

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

				ExternalAPI::ImGUIAPI::TextBar("Type", fmt::format("{}{}",EnumReflection::EnumString(it.second.first.Type),"Asset"));
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}