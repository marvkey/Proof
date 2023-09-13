#include "AssetManagerPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Imgui/ImguiLayer.h"
#include "../../ImGUIAPI.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Math/Math.h"
#include "Proof/Core/Profile.h"
#include <fmt/format.h>
namespace Proof {

	AssetManagerPanel::AssetManagerPanel()
	{
	}

	void AssetManagerPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen)
	{
		if (isOpen == false)
			return;
		PF_PROFILE_FUNC();
		UI::ScopedID customPushID(GetCustomPushID( ).Get());

		ImGui::Begin(dsiplayName, &isOpen);
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