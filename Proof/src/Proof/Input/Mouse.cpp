#include "Proofprch.h"
#include "Mouse.h"
#include <Imgui/imgui.h>
#include <GLFW/glfw3.h>
#include "Platform/Window/WindowsWindow.h"
#include "Proof/Core/Application.h"

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
			if(Application::Get()->GetImguiLayer() != nullptr)
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; // no mouse capture
		}
		else {
			glfwSetInputMode((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (Application::Get()->GetImguiLayer() != nullptr)
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // alllows mouse capture
		}
	}
	float Mouse::GetPosX()
	{
		return Application::Get()->GetWindow()->GetMousePosition().X;
	}
	float Mouse::GetPosY()
	{
		return Application::Get()->GetWindow()->GetMousePosition().Y;
	}
	float Mouse::GetScrollX()
	{
		return Application::Get()->GetWindow()->GetMouseScrollWheel().X;
	}
	float Mouse::GetScrollY()
	{
		return Application::Get()->GetWindow()->GetMouseScrollWheel().Y;
	}
	void Mouse::GetScreenSpace(glm::vec2 windwoPos, glm::vec2 windowSize)
	{
		float posX = GetPosX();
		float posY = GetPosY();
		float scaleFactorX, scaleFactorY;
		glfwGetWindowContentScale((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), &scaleFactorX, &scaleFactorY);  // Retrieve the DPI scaling factors

		// Calculate the screen space coordinates by applying the window position and DPI scaling
		posX = (posX * scaleFactorX) + windwoPos.x;
		posY = (posY * scaleFactorY) + windwoPos.y;
	}
	bool Mouse::IsMouseMoved() {
		return Application::Get()->GetWindow()->IsMouseMoved();
	}

}
