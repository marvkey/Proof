#pragma once
#include <imgui.h>
#include "Proof/Asset/AssetTypes.h"
#include "UITypes.h"
#include "UiUtilities.h"
#include "UI.h"
namespace Proof::UI
{
	
	bool IsMatchingSearch(const std::string& item, std::string_view searchQuery, bool caseSensitive = false, bool stripWhiteSpaces = false, bool stripUnderscores = false);
	class Widgets
	{
	public:
		template<uint32_t BuffSize = 256, typename StringType>
		static bool SearchWidget(StringType& searchString, const char* hint = "Search...", bool* grabFocus = nullptr)
		{
			PushID();

			ShiftCursorY(1.0f);

			const bool layoutSuspended = []
			{
				ImGuiWindow* window = ImGui::GetCurrentWindow();
				if (window->DC.CurrentLayout)
				{
					ImGui::SuspendLayout();
					return true;
				}
				return false;
			}();

			bool modified = false;
			bool searching = false;

			const float areaPosX = ImGui::GetCursorPosX();
			const float framePaddingY = ImGui::GetStyle().FramePadding.y;

			UI::ScopedStyleVar rounding(ImGuiStyleVar_FrameRounding, 3.0f);
			UI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

			if constexpr (std::is_same<StringType, std::string>::value)
			{
				char searchBuffer[BuffSize]{};
				strcpy_s<BuffSize>(searchBuffer, searchString.c_str());
				if (ImGui::InputText(GenerateID(), searchBuffer, BuffSize))
				{
					searchString = searchBuffer;
					modified = true;
				}
				else if (ImGui::IsItemDeactivatedAfterEdit())
				{
					searchString = searchBuffer;
					modified = true;
				}

				searching = searchBuffer[0] != 0;
			}
			else
			{
				static_assert(std::is_same<decltype(&searchString[0]), char*>::value,
					"searchString paramenter must be std::string& or char*");

				if (ImGui::InputText(GenerateID(), searchString, BuffSize))
				{
					modified = true;
				}
				else if (ImGui::IsItemDeactivatedAfterEdit())
				{
					modified = true;
				}

				searching = searchString[0] != 0;
			}

			if (grabFocus && *grabFocus)
			{
				if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)
					&& !ImGui::IsAnyItemActive()
					&& !ImGui::IsMouseClicked(0))
				{
					ImGui::SetKeyboardFocusHere(-1);
				}

				if (ImGui::IsItemFocused())
					*grabFocus = false;
			}

			UI::DrawItemActivityOutline(3.0f, true, Colours::Theme::Accent);
			ImGui::SetItemAllowOverlap();

			ImGui::SameLine(areaPosX + 5.0f);

			if (layoutSuspended)
				ImGui::ResumeLayout();

			ImGui::BeginHorizontal(GenerateID(), ImGui::GetItemRectSize());
			const ImVec2 iconSize(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight());

			// Search icon
			{
				const float iconYOffset = framePaddingY - 3.0f;
				UI::ShiftCursorY(iconYOffset);
				//UI::Image(EditorResources::SearchIcon, iconSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
				UI::ShiftCursorY(-iconYOffset);

				// Hint
				if (!searching)
				{
					UI::ShiftCursorY(-framePaddingY + 1.0f);
					UI::ScopedStyleColor text(ImGuiCol_Text, Colours::Theme::TextDarker);
					UI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, framePaddingY));
					ImGui::TextUnformatted(hint);
					UI::ShiftCursorY(-1.0f);
				}
			}

			ImGui::Spring();

			// Clear icon
			if (searching)
			{
				const float spacingX = 4.0f;
				const float lineHeight = ImGui::GetItemRectSize().y - framePaddingY / 2.0f;

				if (ImGui::InvisibleButton(GenerateID(), ImVec2{ lineHeight, lineHeight }))
				{
					if constexpr (std::is_same<StringType, std::string>::value)
						searchString.clear();
					else
						memset(searchString, 0, BuffSize);

					modified = true;
				}

				if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
					ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

				if (ImGui::Button("Clear"))
				{
					if constexpr (std::is_same<StringType, std::string>::value)
						searchString.clear();
					else
						memset(searchString, 0, BuffSize);

					modified = true;
				}
				//UI::DrawButtonImage(EditorResources::ClearIcon, IM_COL32(160, 160, 160, 200),
				//	IM_COL32(170, 170, 170, 255),
				//	IM_COL32(160, 160, 160, 150),
				//	UI::RectExpanded(UI::GetItemRect(), -2.0f, -2.0f));

				ImGui::Spring(-1.0f, spacingX * 2.0f);
			}

			ImGui::EndHorizontal();
			UI::ShiftCursorY(-1.0f);
			UI::PopID();
			return modified;
		}

		static bool AssetSearchPopup(const char* ID, AssetType assetType, AssetID& selected, UIMemoryAssetTypes types, bool allowClear = true, const char* hint = "Search Assets", ImVec2 size = ImVec2{ 250.0f, 350.0f });
		static bool AssetSearchPopup(const char* ID, std::initializer_list<AssetType> assetTypes, AssetID& selected, UIMemoryAssetTypes types, bool allowClear = true, const char* hint = "Search Assets", ImVec2 size = ImVec2{ 250.0f, 350.0f });
	};
}