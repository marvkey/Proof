#pragma once
#include <ImGui/imgui.h>

namespace Proof::UI::Colours
{
	static inline float Convert_sRGB_FromLinear(float theLinearValue);
	static inline float Convert_sRGB_ToLinear(float thesRGBValue);
	ImVec4 ConvertFromSRGB(ImVec4 colour);
	ImVec4 ConvertToSRGB(ImVec4 colour);
	namespace Theme
	{
		constexpr auto BackgroundPopup = IM_COL32(50, 50, 50, 255);
		constexpr auto BackgroundDark = IM_COL32(26, 26, 26, 255);
	}

}