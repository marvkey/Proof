#include "Proofprch.h"
#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>

namespace Proof {
	void Mouse::CaptureMouse(bool caputure)
	{
		if (caputure) {
			glfwSetInputMode((GLFWwindow*)CurrentWindow::GetWindowAPI(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; // no mouse capture
		}
		else {
			glfwSetInputMode((GLFWwindow*)CurrentWindow::GetWindowAPI(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // alllows mouse capture
		}
	}
}
