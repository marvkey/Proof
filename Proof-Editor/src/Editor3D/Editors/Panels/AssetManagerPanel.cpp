#include "AssetManagerPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Imgui/ImguiLayer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Math/Math.h"
#include "Proof/Core/Profile.h"
#include "Proof/ImGui/UI.h"

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
			UI::BeginPropertyGrid("AssetManagerPanelGrid");
			
			for (auto [ID,assetContainer] : AssetManager::GetAssets()) {
				ImGui::BeginChildFrame((ImGuiID)ID,{ImGui::GetContentRegionAvail().x,100}, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

				UI::BeginPropertyGrid(fmt::format("{}Grid",assetContainer.Info.GetName()));
				UI::AttributeTextBar("Asset ID", std::to_string(assetContainer.Info.ID));
				if(assetContainer.Info.State== AssetState::Ready)
					UI::AttributeTextBar("Loaded", "False");
				else
					UI::AttributeTextBar("Loaded", "True");

				UI::AttributeTextBar("Path", AssetManager::GetAssetFileSystemPath(assetContainer.Info.Path).string());

				UI::AttributeTextBar("Type", fmt::format("{}{}",EnumReflection::EnumString(assetContainer.Info.Type),"Asset"));
				ImGui::EndChild();
				UI::EndPropertyGrid();
			}
			UI::EndPropertyGrid();
		}
		ImGui::End();
	}
}