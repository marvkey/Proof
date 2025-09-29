#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/Curve.h"
#include <imgui.h>
#include "Proof/Renderer/Colors.h"
namespace Proof::UI
{
	bool InterpolationCurveEditor(const std::string& name, InterpolationCurve& curve, ImVec2 size = ImGui::GetContentRegionAvail());
	bool ColorGradientEditor(const std::string& name, ColorGradient& gradient, ImVec2 size = {ImGui::GetContentRegionAvail().x-10,22});
}