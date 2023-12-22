#include "Proofprch.h"
#include "UIHandlers.h"
#include "UI.h"
#include "Proof/Asset/AssetManager.h"
#include "UiUtilities.h"
#include "UIWidgets.h"
namespace Proof::UI
{
    bool AttributeAssetReference(const std::string& label, AssetType assetType, AssetID& outHandle, const PropertyAssetReferenceSettings& settings)
    {
#if 0
		bool modified = false;

        UI::ShiftCursor(10.0f, 9.0f);
        UI::AttributeLabel(label);
        ImGui::NextColumn();
        UI::ShiftCursorY(4.0f);

        ImGui::PushItemWidth(-1);

		ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		{
			ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
			float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
			float itemHeight = 28.0f;

			std::string buttonText = fmt::format("Null ({})",EnumReflection::EnumString(assetType));
			bool valid = true;
			if (AssetManager::HasAsset(outHandle))
			{
				const auto& assetInfo = AssetManager::GetAssetInfo(outHandle);
				if (settings.ShowFullFilePath)
					buttonText = assetInfo.Path.string();
				else
					buttonText = assetInfo.GetName();
			}

			if ((GImGui->CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0)
				buttonText = "---";

			// PropertyAssetReference could be called multiple times in same "context"
			// and so we need a unique id for the asset search popup each time.
			// notes
			// - don't use GenerateID(), that's inviting id clashes, which would be super confusing.
			// - don't store return from GenerateLabelId in a const char* here. Because its pointing to an internal
			//   buffer which may get overwritten by the time you want to use it later on.
			std::string assetSearchPopupID = UI::GenerateLabelID("ARSP");
			{
				UI::ScopedStyleColor buttonLabelColor(ImGuiCol_Text, valid ? settings.ButtonLabelColor : settings.ButtonLabelColorError);
				ImGui::Button(GenerateLabelID(buttonText), { width, itemHeight });

				const bool isHovered = ImGui::IsItemHovered();

				if (isHovered)
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						//AssetEditorPanelInterface::OpenEditor(AssetManager::GetAsset<Asset>(outHandle));
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						ImGui::OpenPopup(assetSearchPopupID.c_str());
					}
				}
			}

			ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;

			bool clear = false;
			if (Widgets::AssetSearchPopup(assetSearchPopupID.c_str(), assetType, outHandle, settings.AssetMemoryTypes))
			{
				modified = true;
			}
		}

		if (!IsItemDisabled())
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("asset_payload");

				if (data)
				{
					AssetID assetHandle = *(AssetID*)data->Data;
					if (AssetManager::HasAsset(assetHandle))
					{
						const auto& assetInfo = AssetManager::GetAssetInfo(assetHandle);
						if (assetInfo.Type == assetType)
						{
							outHandle = assetHandle;
							modified = true;
						}
					}
				}
			}
		}

		if (!IsItemDisabled())
			DrawItemActivityOutline(2.0f, true, Colours::Theme::Accent);

		ImGui::PopItemWidth();
		if (settings.AdvanceToNextColumn)
		{
			ImGui::NextColumn();
			Draw::Underline();
		}

		return modified;
#endif

		return AttributeMultiAssetReference(label, { assetType }, outHandle, settings);

    }
	bool AttributeMultiAssetReference(const std::string& label, std::initializer_list<AssetType> assetTypesList, AssetID& outHandle, const PropertyAssetReferenceSettings& settings)
	{
		bool modified = false;

		UI::ShiftCursor(0.f, 9.0f);
		UI::AttributeLabel(label);
		ImGui::NextColumn();
		UI::ShiftCursorY(4.0f);

		ImGui::PushItemWidth(-1);

		std::unordered_set<AssetType> assetTypesSet = assetTypesList;

		ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		{
			ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
			float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
			float itemHeight = 28.0f;

			std::string buttonText = "Null";

			for (auto& assetType : assetTypesSet)
			{
				buttonText += fmt::format(" ({})", EnumReflection::EnumString(assetType));
			}
			bool valid = true;
			if (AssetManager::HasAsset(outHandle))
			{
				const auto& assetInfo = AssetManager::GetAssetInfo(outHandle);
				if (settings.ShowFullFilePath)
					buttonText = assetInfo.Path.string();
				else
					buttonText = assetInfo.GetName();
			}

			if ((GImGui->CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0)
				buttonText = "---";

			// PropertyAssetReference could be called multiple times in same "context"
			// and so we need a unique id for the asset search popup each time.
			// notes
			// - don't use GenerateID(), that's inviting id clashes, which would be super confusing.
			// - don't store return from GenerateLabelId in a const char* here. Because its pointing to an internal
			//   buffer which may get overwritten by the time you want to use it later on.
			std::string assetSearchPopupID = UI::GenerateLabelID("ARSP");
			{
				UI::ScopedStyleColor buttonLabelColor(ImGuiCol_Text, valid ? settings.ButtonLabelColor : settings.ButtonLabelColorError);
				ImGui::Button(GenerateLabelID(buttonText), { width, itemHeight });

				const bool isHovered = ImGui::IsItemHovered();
				{
					UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f).Value);

					if (ImGui::BeginPopup((assetSearchPopupID + "RemovePopUp").c_str()))
					{
						if (ImGui::MenuItem("Remove"))
						{
							outHandle = 0;
							modified = true;

						}
						ImGui::EndPopup();
					}
				}
				if (isHovered)
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						//AssetEditorPanelInterface::OpenEditor(AssetManager::GetAsset<Asset>(outHandle));
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						ImGui::OpenPopup(assetSearchPopupID.c_str());
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup((assetSearchPopupID + "RemovePopUp").c_str());
					}
					
				}
			}

			ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;

			bool clear = false;
			if (Widgets::AssetSearchPopup(assetSearchPopupID.c_str(), assetTypesList, outHandle, settings.AssetMemoryTypes))
			{
				modified = true;
			}
		}

		if (!IsItemDisabled())
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("asset_payload");

				if (data)
				{
					AssetID assetHandle = *(AssetID*)data->Data;
					if (AssetManager::HasAsset(assetHandle))
					{
						const auto& assetInfo = AssetManager::GetAssetInfo(assetHandle);
						for (auto& assetType : assetTypesSet)
						{
							if (assetInfo.Type == assetType)
							{
								outHandle = assetHandle;
								modified = true;
							}
						}
					}
				}
			}
		}

		if (!IsItemDisabled())
			DrawItemActivityOutline(2.0f, true, Colours::Theme::Accent);

		ImGui::PopItemWidth();
		if (settings.AdvanceToNextColumn)
		{
			ImGui::NextColumn();
			Draw::Underline();
		}
		HandleModified(modified);
		return modified;
	}
}
