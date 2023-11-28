#include "Proofprch.h"
#include "UIWidgets.h"
#include "Proof/Asset/AssetTypes.h"
#include "Proof/Asset/AssetManager.h"
#include "UiUtilities.h"
#include "UIColors.h"
#include "UI.h"
#include "Proof/Utils/StringUtils.h"
namespace Proof::UI
{
	bool IsMatchingSearch(const std::string& item, std::string_view searchQuery, bool caseSensitive, bool stripWhiteSpaces, bool stripUnderscores)
	{

		if (searchQuery.empty())
			return true;

		if (item.empty())
			return false;

		// Replace underscores with spaces if required
		std::string itemSanitized = stripUnderscores ? Utils::String::ReplaceUnderscoresWithSpaces(item) : item;

		// Strip white spaces if required
		if (stripWhiteSpaces)
			itemSanitized = Utils::String::RemoveWhiteSpaces(itemSanitized);

		// Strip white spaces from the search query if required
		std::string searchString = stripWhiteSpaces ? Utils::String::RemoveWhiteSpaces(searchQuery.data()) : std::string(searchQuery);

		// Convert to lowercase if not case-sensitive
		if (!caseSensitive)
		{
			itemSanitized = Utils::String::ToLower(itemSanitized);
			searchString = Utils::String::ToLower(searchString);
		}

		bool result = false;

		// Check for multiple search terms
		if (Utils::String::ContainsWhitespace(searchString))
		{
			std::vector<std::string> searchTerms = Utils::String::SplitAtWhitespace(searchString);
			result = std::all_of(searchTerms.begin(), searchTerms.end(),
				[&itemSanitized](const std::string& searchTerm)
				{
					return !searchTerm.empty() && Utils::String::ContainsSubstring(itemSanitized, searchTerm);
				});
		}
		else
		{
			// Single search term
			result = Utils::String::ContainsSubstring(itemSanitized, searchString);
		}

		return result;
	}
	bool Widgets::AssetSearchPopup(const char* ID, AssetType assetType, AssetID& selected, UIMemoryAssetTypes memoryAssetTypes, bool allowClear, const char* hint, ImVec2 size)
	{
		UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f).Value);

		bool modified = false;

		AssetID current = selected;

		ImGui::SetNextWindowSize({ size.x, 0.0f });

		static bool grabFocus = true;

		if (UI::BeginPopup(ID, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			static std::string searchString;

			if (ImGui::GetCurrentWindow()->Appearing)
			{
				grabFocus = true;
				searchString.clear();
			}

			// Search widget
			UI::ShiftCursor(3.0f, 2.0f);
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetCursorPosX() * 2.0f);
			SearchWidget(searchString, hint, &grabFocus);

			const bool searching = !searchString.empty();

			// Clear property button
			if (allowClear)
			{
				UI::ScopedColourStack buttonColours(
					ImGuiCol_Button, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.0f),
					ImGuiCol_ButtonHovered, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.2f),
					ImGuiCol_ButtonActive, UI::ColourWithMultipliedValue(Colours::Theme::Background, 0.9f));

				UI::ScopedStyleVar border(ImGuiStyleVar_FrameBorderSize, 0.0f);

				ImGui::SetCursorPosX(0);

				ImGui::PushItemFlag(ImGuiItemFlags_NoNav, searching);

				if (ImGui::Button("CLEAR", { ImGui::GetWindowWidth(), 0.0f }))
				{
					allowClear = true;
					modified = true;
				}

				ImGui::PopItemFlag();
			}

			// List of assets
			{
				UI::ScopedStyleColor listBoxBg(ImGuiCol_FrameBg, IM_COL32_DISABLE);
				UI::ScopedStyleColor listBoxBorder(ImGuiCol_Border, IM_COL32_DISABLE);

				ImGuiID listID = ImGui::GetID("##SearchListBox");
				if (ImGui::BeginListBox("##SearchListBox", ImVec2(-FLT_MIN, 0.0f)))
				{
					bool forwardFocus = false;

					ImGuiContext& g = *GImGui;
					if (g.NavJustMovedToId != 0)
					{
						if (g.NavJustMovedToId == listID)
						{
							forwardFocus = true;
							// ActivateItem moves keyboard navigation focuse inside of the window
							ImGui::ActivateItem(listID);
							ImGui::SetKeyboardFocusHere(1);
						}
					}

					const auto& assetsRegistry = AssetManager::GetAllAssetType(assetType);

					for (auto& assetId : assetsRegistry)
					{
						const auto& assetInfo = AssetManager::GetAssetInfo(assetId);

						if (assetInfo.RuntimeAsset && memoryAssetTypes == UIMemoryAssetTypes::None)
							continue;

						if (assetInfo.RuntimeAsset && memoryAssetTypes == UIMemoryAssetTypes::Default)
						{
							if (!AssetManager::IsDefaultAsset(assetInfo.ID))
								continue;
						}

						const std::string assetName = assetInfo.GetName();
						if (!searchString.empty() && !UI::IsMatchingSearch(assetName, searchString))
							continue;
						bool is_selected = (current == assetInfo.ID);
						if (ImGui::Selectable(assetName.c_str(), is_selected))
						{
							current = assetInfo.ID;
							selected = assetInfo.ID;
							modified = true;
						}

						if (forwardFocus)
						{
							forwardFocus = false;
						}
						else if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndListBox();
				}
			}
			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}

		return modified;
	}
	
}
