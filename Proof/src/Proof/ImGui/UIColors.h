#pragma once
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"


namespace Proof::UI
{
	namespace Colours
	{
		static inline float Convert_sRGB_FromLinear(float theLinearValue);
		static inline float Convert_sRGB_ToLinear(float thesRGBValue);
		ImVec4 ConvertFromSRGB(ImVec4 colour);
		ImVec4 ConvertToSRGB(ImVec4 colour);
		static inline uint32_t ConvertToColorRGBAuint(ImVec4 colour)
		{
			return IM_COL32(colour.x, colour.y, colour.z, colour.w);
		}
		namespace Theme
		{
			constexpr auto Accent = IM_COL32(236, 158, 36, 255);
			constexpr auto BackgroundPopup = IM_COL32(50, 50, 50, 255);
			constexpr auto Background = IM_COL32(36, 36, 36, 255);
			constexpr auto BackgroundDark = IM_COL32(26, 26, 26, 255);
			constexpr auto TextDarker = IM_COL32(128, 128, 128, 255);
			constexpr auto TextBrighter = IM_COL32(210, 210, 210, 255);
			constexpr auto Text = IM_COL32(192, 192, 192, 255);
			constexpr auto TextError = IM_COL32(230, 51, 51, 255);
			constexpr auto TextHighlight = IM_COL32(255, 255, 102, 1);
			constexpr auto Selection = IM_COL32(237, 192, 119, 255);
			constexpr auto SelectionMuted = IM_COL32(237, 201, 142, 23);
			constexpr auto Muted = IM_COL32(77, 77, 77, 255);

		}
	}
	static inline ImColor ColourWithMultipliedValue(const ImColor& color, float multiplier)
	{
		const ImVec4& colRaw = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colRaw.x, colRaw.y, colRaw.z, hue, sat, val);
		return ImColor::HSV(hue, sat, std::min(val * multiplier, 1.0f));
	}
	static inline ImColor ColourWithMultipliedSaturation(const ImColor& color, float multiplier)
	{
		const ImVec4& colRaw = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colRaw.x, colRaw.y, colRaw.z, hue, sat, val);
		return ImColor::HSV(hue, std::min(sat * multiplier, 1.0f), val);
	}

	static inline ImColor ColourWithMultipliedHue(const ImColor& color, float multiplier)
	{
		const ImVec4& colRaw = color.Value;
		float hue, sat, val;
		ImGui::ColorConvertRGBtoHSV(colRaw.x, colRaw.y, colRaw.z, hue, sat, val);
		return ImColor::HSV(std::min(hue * multiplier, 1.0f), sat, val);
	}

	static inline ImColor ColourWithMultipliedAlpha(const ImColor& color, float multiplier)
	{
		ImVec4 colRaw = color.Value;
		colRaw.w *= multiplier;
		return colRaw;
	}

}