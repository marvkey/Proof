#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/Curve.h"
#include <imgui.h>
namespace Proof::UI
{
	bool InterpolationCurveEditor(const std::string& name, InterpolationCurve& curve, ImVec2 size = ImGui::GetContentRegionAvail());
}