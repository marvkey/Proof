#include "Proofprch.h"
#include "PerformancePanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "Proof/Core/Core.h"
namespace Proof
{

	void PerformancePanel::ImGuiRender(FrameTime deltaTime) {
		#if 0
		if (m_ShowWindow == false)
			return;
		PF_PROFILE_FUNC();

		std::vector<Compare> frameTimersCPU;
		std::vector<Compare> frameTimersRenderer;
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (ImGui::Begin("Performance ", &m_ShowWindow)) {
			
			bool cpuOpen = ImGui::TreeNodeEx("CPUTREENODE", treeNodeFlags, "CPU");

			if (cpuOpen) {
				for (uint32_t i = 0; i < FrameTimersControll::s_FrameTimers.size(); i++) {
					if (FrameTimersControll::s_FrameTimers[i].TimerManage.TimerType == TimerTypes::CPUTimer)
						frameTimersCPU.push_back({ FrameTimersControll::s_FrameTimers[i].TimerManage.Time,i });
				}
				std::sort(frameTimersCPU.begin(), frameTimersCPU.end(), [](const Compare& lhs, const Compare& rhs) {
					return lhs.Time > rhs.Time;
					});
				for (Compare& comp : frameTimersCPU)
					PerformanceData(comp);
				ImGui::TreePop();
			}
			bool rendererOpen = ImGui::TreeNodeEx("RendererNODE", treeNodeFlags, "Renderer");
			if (rendererOpen) {
				for (uint32_t i = 0; i < FrameTimersControll::s_FrameTimers.size(); i++) {
					if (FrameTimersControll::s_FrameTimers[i].TimerManage.TimerType == TimerTypes::Renderer)
						frameTimersRenderer.push_back({ FrameTimersControll::s_FrameTimers[i].TimerManage.Time,i });
				}
				std::sort(frameTimersRenderer.begin(), frameTimersRenderer.end(), [](const Compare& lhs, const Compare& rhs) {
					return lhs.Time > rhs.Time;
					});
				for (Compare& comp : frameTimersRenderer)
					PerformanceData(comp);
				ImGui::TreePop();
			}
		}
		ImGui::End();
		#endif
	}
	void PerformancePanel::PerformanceData(Compare& comp) {
		auto& Time = FrameTimersControll::s_FrameTimers[comp.Pos];
		std::string form = fmt::format("{:.3f}ms {}", Time.TimerManage.Time, Time.FunctionName);
		if (Time.Times.size() > 1) {
			bool biggerTime = (Time.TimerManage.Time > Time.TimerManage.MaxTime) && Time.TimerManage.MaxTime > 0;
			if (biggerTime)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
			bool open = ImGui::TreeNodeEx((void*)comp.Pos, ImGuiTreeNodeFlags_SpanFullWidth, form.c_str());
			if (biggerTime)
				ImGui::PopStyleColor();
			if (open) {
				for (float frameTime : Time.Times) {
					if (frameTime > Time.TimerManage.MaxTime && Time.TimerManage.MaxTime > 0)
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
					ImGui::Text("%.3fms", frameTime);
					if (frameTime > Time.TimerManage.MaxTime && Time.TimerManage.MaxTime > 0)
						ImGui::PopStyleColor();
				}
				ImGui::TreePop();
			}
			return;
		}
		ImGui::Text("%.3fms %s", Time.TimerManage.Time, Time.FunctionName.c_str());
	}
}