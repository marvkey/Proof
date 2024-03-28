 #pragma once
#include <iostream>

#include <imgui.h>
#include "UIColors.h"
#include "Proof/Renderer/Texture.h"
namespace Proof::UI
{
	namespace Draw {
		//=========================================================================================
		/// Lines
		void Underline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f);
	}
	void HandleModified(bool value);
	static void Separator()
	{
		ImGui::Separator();
	}
	struct ScopedStyleColor
	{
		ScopedStyleColor() = default;
		ScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopedStyleColor()
		{
			if (m_Set)
				ImGui::PopStyleColor();
		}
	private:
		bool m_Set = false;
	};

	struct ScopedStyleVar
	{
		ScopedStyleVar() = default;
		ScopedStyleVar(ImGuiStyleVar idx, float val, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleVar(idx, val);
		}

		ScopedStyleVar(ImGuiStyleVar idx, const ImVec2& val, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleVar(idx, val);
		}

		~ScopedStyleVar()
		{
			if (m_Set)
				ImGui::PopStyleVar();
		}
	private:
		bool m_Set = false;
	};

	struct ScopedID {
		ScopedID() = default;
		ScopedID(std::string str_id, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(str_id.c_str());
		}
		ScopedID(const char* str_id, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(str_id);
		}

		ScopedID(const char* str_id_begin, const char* str_id_end, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(str_id_begin, str_id_end);
		}
		ScopedID(int int_id, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(int_id);
		}

		ScopedID(const void* ptr_id, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushID(ptr_id);
		}
		~ScopedID()
		{
			if (m_Set)
				ImGui::PopID();
		}
	private:
		bool m_Set = false;
	};
	class ScopedColour
	{
	public:
		ScopedColour(const ScopedColour&) = delete;
		ScopedColour& operator=(const ScopedColour&) = delete;
		template<typename T>
		ScopedColour(ImGuiCol colourId, T colour) { ImGui::PushStyleColor(colourId, ImColor(colour).Value); }
		~ScopedColour() { ImGui::PopStyleColor(); }
	};
	class ScopedColourStack
	{
	public:
		ScopedColourStack(const ScopedColourStack&) = delete;
		ScopedColourStack& operator=(const ScopedColourStack&) = delete;

		template <typename ColourType, typename... OtherColours>
		ScopedColourStack(ImGuiCol firstColourID, ColourType firstColour, OtherColours&& ... otherColourPairs)
			: m_Count((sizeof... (otherColourPairs) / 2) + 1)
		{
			static_assert ((sizeof... (otherColourPairs) & 1u) == 0,
				"ScopedColourStack constructor expects a list of pairs of colour IDs and colours as its arguments");

			PushColour(firstColourID, firstColour, std::forward<OtherColours>(otherColourPairs)...);
		}

		~ScopedColourStack() { ImGui::PopStyleColor(m_Count); }

	private:
		int m_Count;

		template <typename ColourType, typename... OtherColours>
		void PushColour(ImGuiCol colourID, ColourType colour, OtherColours&& ... otherColourPairs)
		{
			if constexpr (sizeof... (otherColourPairs) == 0)
			{
				ImGui::PushStyleColor(colourID, ImColor(colour).Value);
			}
			else
			{
				ImGui::PushStyleColor(colourID, ImColor(colour).Value);
				PushColour(std::forward<OtherColours>(otherColourPairs)...);
			}
		}
	};

	
	// The delay won't work on texts, because the timer isn't tracked for them.
	static bool IsItemHovered(float delayInSeconds = 0.1f, ImGuiHoveredFlags flags = 0)
	{
		return ImGui::IsItemHovered() && GImGui->HoveredIdTimer > delayInSeconds; /*HoveredIdNotActiveTimer*/
	}
	static void SetTooltip(std::string_view text, float delayInSeconds = 0.1f, bool allowWhenDisabled = true, ImVec2 padding = ImVec2(5, 5))
	{
		if (IsItemHovered(delayInSeconds, allowWhenDisabled ? ImGuiHoveredFlags_AllowWhenDisabled : 0))
		{
			UI::ScopedStyleVar tooltipPadding(ImGuiStyleVar_WindowPadding, padding);
			UI::ScopedStyleColor textCol(ImGuiCol_Text, Colours::Theme::TextBrighter);
			ImGui::SetTooltip(text.data());
		}
	}
	ImTextureID GetTextureID(Count < class Image > texture);

	static void DrawButtonImage(const Count<Texture2D>& imageNormal, const Count<Texture2D>& imageHovered, const Count<Texture2D>& imagePressed,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImVec2 rectMin, ImVec2 rectMax)
	{
		auto* drawList = ImGui::GetWindowDrawList();
		if (ImGui::IsItemActive())
			drawList->AddImage(GetTextureID(imagePressed->GetImage()), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintPressed);
		else if (ImGui::IsItemHovered())
			drawList->AddImage(GetTextureID(imageHovered->GetImage()), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
		else
			drawList->AddImage(GetTextureID(imageNormal->GetImage()), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintNormal);
	};

	static void DrawButtonImage(const Count<Texture2D>& imageNormal, const Count<Texture2D>& imageHovered, const Count<Texture2D>& imagePressed,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImRect rectangle)
	{
		DrawButtonImage(imageNormal, imageHovered, imagePressed, tintNormal, tintHovered, tintPressed, rectangle.Min, rectangle.Max);
	};

	static void DrawButtonImage(const Count<Texture2D>& image,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImVec2 rectMin, ImVec2 rectMax)
	{
		DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed, rectMin, rectMax);
	};

	static void DrawButtonImage(const Count<Texture2D>& image,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImRect rectangle)
	{
		DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed, rectangle.Min, rectangle.Max);
	};


	static void DrawButtonImage(const Count<Texture2D>& imageNormal, const Count<Texture2D>& imageHovered, const Count<Texture2D>& imagePressed,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed)
	{
		DrawButtonImage(imageNormal, imageHovered, imagePressed, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	};

	static void DrawButtonImage(const Count<Texture2D>& image,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed)
	{
		DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	};


	//=========================================================================================
	/// Rectangle

	static inline ImRect GetItemRect()
	{
		return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	static inline ImRect RectExpanded(const ImRect& rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x -= x;
		result.Min.y -= y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	static inline ImRect RectOffset(const ImRect& rect, float x, float y)
	{
		ImRect result = rect;
		result.Min.x += x;
		result.Min.y += y;
		result.Max.x += x;
		result.Max.y += y;
		return result;
	}

	static inline ImRect RectOffset(const ImRect& rect, ImVec2 xy)
	{
		return RectOffset(rect, xy.x, xy.y);
	}
	bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);
	void EndPopup();

}