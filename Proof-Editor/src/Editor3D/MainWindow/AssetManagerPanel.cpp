#include "AssetManagerPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Imgui/ImguiLayer.h"
#include "../ImGUIAPI.h"
#include "Proof/Asset/AssetManager.h"
#include <fmt/format.h>
namespace Proof {
	void AssetManagerPanel::ImGuiRender(FrameTime deltaTime){
		if (m_ShowWindow == false)
			return;
		PF_PROFILE_FUNC();

		ImGui::Begin("Asset Manager", &m_ShowWindow);
		{
			for (auto [ID,assetContainer] : AssetManager::GetAssets()) {
				ImGui::BeginChildFrame((ImGuiID)ID,{ImGui::GetContentRegionAvail().x,100}, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
				ExternalAPI::ImGUIAPI::TextBar("Asset ID", std::to_string(assetContainer.Info.ID));
				if(assetContainer.Info.State== AssetState::Ready)
					ExternalAPI::ImGUIAPI::TextBar("Loaded", "False");
				else
					ExternalAPI::ImGUIAPI::TextBar("Loaded", "True");

				ExternalAPI::ImGUIAPI::TextBar("Path", assetContainer.Info.Path.string());

				ExternalAPI::ImGUIAPI::TextBar("Type", fmt::format("{}{}",EnumReflection::EnumString(assetContainer.Info.Type),"Asset"));
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}