#include "Proofprch.h"
#include "Mouse.h"
#include <Imgui/imgui.h>
#include <GLFW/glfw3.h>
#include "Platform/Window/WindowsWindow.h"
namespace Proof {
	static bool s_MouseCaptured = false;
	bool Mouse::IsMouseCaptured()
	{
		return s_MouseCaptured;
	}
	void Mouse::CaptureMouse(bool caputure)
	{
		s_MouseCaptured = caputure;
		if (caputure) {
			glfwSetInputMode((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; // no mouse capture
		}
		else {
			glfwSetInputMode((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // alllows mouse capture
		}
	}
	bool Mouse::IsMouseMoved() {
		return Application::Get()->GetWindow()->IsMouseMoved();
	}

}
