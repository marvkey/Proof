#include "Proofprch.h"
#include "PerformancePanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Core/Core.h"
namespace Proof
{
	void PerformancePanel::ImGuiRender(FrameTime deltaTime) {
		if (m_ShowWindow == false)
			return;
		if (ImGui::Begin("Performance ", &m_ShowWindow)) {
			for (auto& [name, timeManage] : FrameTimersControll::s_FrameTimers) {
				ImGui::Text("%s %f", name.c_str(), timeManage.Time);
			}
		}
		ImGui::End();
	}
}