#include "Proofprch.h"
#include "UIWidgets.h"
#include "Proof/Asset/AssetTypes.h"
#include "Proof/Asset/AssetManager.h"
#include "UiUtilities.h"
#include "UIColors.h"
#include "UI.h"
#include "Proof/Utils/StringUtils.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Input/ElevatedInputSystem/InputTypes.h"
#include "Proof/Project/Project.h"
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
	bool Widgets::EntitySearchPopup(const char* ID, Count<class World> scene, UUID& selected, bool* cleared, const char* hint, ImVec2 size)
	{
		UI::ScopedColour popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f));

		bool modified = false;

		auto entities = scene->GetAllEntitiesWith<IDComponent, TagComponent>();
		UUID current = selected;

		ImGui::SetNextWindowSize({ size.x, 0.0f });

		static bool s_GrabFocus = true;

		if (UI::BeginPopup(ID, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			static std::string searchString;

			if (ImGui::GetCurrentWindow()->Appearing)
			{
				s_GrabFocus = true;
				searchString.clear();
			}

			// Search widget
			UI::ShiftCursor(3.0f, 2.0f);
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetCursorPosX() * 2.0f);
			SearchWidget(searchString, hint, &s_GrabFocus);

			const bool searching = !searchString.empty();

			// Clear property button
			if (cleared != nullptr)
			{
				UI::ScopedColourStack buttonColours(
					ImGuiCol_Button, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.0f),
					ImGuiCol_ButtonHovered, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.2f),
					ImGuiCol_ButtonActive, UI::ColourWithMultipliedValue(Colours::Theme::Background, 0.9f));

				UI::ScopedStyleColor border(ImGuiStyleVar_FrameBorderSize, 0.0f);

				ImGui::SetCursorPosX(0);

				ImGui::PushItemFlag(ImGuiItemFlags_NoNav, searching);

				if (ImGui::Button("CLEAR", { ImGui::GetWindowWidth(), 0.0f }))
				{
					*cleared = true;
					modified = true;
				}

				ImGui::PopItemFlag();
			}

			// List of entities
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

					for (auto enttID : entities)
					{
						const auto& idComponent = entities.get<IDComponent>(enttID);
						const auto& tagComponent = entities.get<TagComponent>(enttID);

						if (!searchString.empty() && !UI::IsMatchingSearch(tagComponent.Tag, searchString))
							continue;

						bool is_selected = current == idComponent.GetID();
						if (ImGui::Selectable(tagComponent.Tag.c_str(), is_selected))
						{
							current = selected = idComponent.GetID();
							modified = true;
						}

						if (forwardFocus)
							forwardFocus = false;
						else if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndListBox();
				}
			}
			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}
		HandleModified(modified);
		return modified;
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
		HandleModified(modified);

		return modified;
	}

	bool Widgets::AssetSearchPopup(const char* ID, std::initializer_list<AssetType> assetTypes, AssetID& selected, UIMemoryAssetTypes memoryAssetTypes, bool allowClear, const char* hint, ImVec2 size)
	{
		std::unordered_set<AssetType> assetTypesSet = assetTypes;
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

					for (auto& assetType : assetTypesSet)
					{
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
					}
					ImGui::EndListBox();
				}
			}
			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}
		HandleModified(modified);
		return modified;
	}

	bool Widgets::SearchScriptsPopup(const char* ID, std::string& selected, bool allowClear, const char* hint, ImVec2 size)
	{
		UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f).Value);

		bool modified = false;

		auto current = selected;

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

					if (ScriptRegistry::IsInitialized())
					{
						for (auto& [scriptName, script] : ScriptEngine::GetEntityScripts())
						{


							if (!searchString.empty() && !UI::IsMatchingSearch(scriptName, searchString))
								continue;
							bool is_selected = (current == scriptName);
							if (ImGui::Selectable(scriptName.c_str(), is_selected))
							{
								current = scriptName;
								selected = scriptName;
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
					}

					ImGui::EndListBox();
				}
			}
			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}
		HandleModified(modified);

		return modified;
	}

	bool Widgets::ElevatedInputKeySearchPopup(const char* ID, ElevatedInputKey* selectedKey, ElevatedInputKeyFlags keyFlags, ElevatedInputKeyDeviceType device, bool analogKeys,bool digiitalKeys , const char* hint, bool allowClear, ImVec2 size)
	{
		UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f).Value);

		bool modified = false;

		auto current = *selectedKey;

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

			static auto deviceMatchesParam = [&](ElevatedInputKeyDeviceType deviceParam)
			{
				if (device == ElevatedInputKeyDeviceType::None)
					return true;
				return device == deviceParam;
			};

			static auto keyMatchesParam = [&](const ElevatedInputKey* paramKey)
			{
				if (keyFlags == ElevatedInputKeyFlags::NoFlags)
					return true;

				if (keyFlags == paramKey->GetKeyFlags())
					return true;

				return false;
			};

			static auto viewKeys = [&](ElevatedInputKeyDeviceType deviceType, bool& forwardFocus)
			{
				if (!ElevatedInputKeys::GetDeviceKeys().contains(deviceType))
					return;

				auto& list = ElevatedInputKeys::GetDeviceKeys().at(deviceType);

				for (const ElevatedInputKey* key : list)
				{
					if (!keyMatchesParam(key))
						continue;

					if (!searchString.empty() && !UI::IsMatchingSearch(key->GetKeyName(), searchString))
						continue;

					if (analogKeys == false)
					{
						if (key->IsAnalog())
							continue;
					}

					if (digiitalKeys == false)
					{
						if (key->IsDigital())
							continue;
					}
					bool is_selected = (current.GetKeyName() == key->GetKeyName());
					if (ImGui::Selectable(key->GetKeyName().c_str(), is_selected))
					{
						current = *key;
						*selectedKey = *key;
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

			};

			// List of assets
			{
				UI::ScopedStyleColor listBoxBg(ImGuiCol_FrameBg, IM_COL32_DISABLE);
				UI::ScopedStyleColor listBoxBorder(ImGuiCol_Border, IM_COL32_DISABLE);

				ImGuiID listID = ImGui::GetID("##SearchInputKey");
				if (ImGui::BeginListBox("##SearchInputKey", ImVec2(-FLT_MIN, 0.0f)))
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

					EnumReflection::ForEach< ElevatedInputKeyDeviceType>([&](ElevatedInputKeyDeviceType type)
						{
							if (type == ElevatedInputKeyDeviceType::None)
								return;

							if (searchString.empty())
							{
								if (UI::AttributeTreeNode(EnumReflection::EnumString(type), false, 6, 3))
								{
									viewKeys(type, forwardFocus);
									UI::EndTreeNode();
								}
							}
							else 
							{
								viewKeys(type, forwardFocus);
							}
						});
					ImGui::EndListBox();
				}
			}
			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}
		HandleModified(modified);

		return modified;
	}

}
