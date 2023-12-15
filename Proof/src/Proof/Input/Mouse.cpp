#include "Proofprch.h"
#include "Mouse.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "Proof/Platform/Window/WindowsWindow.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/UI.h"

namespace Proof 
{
	static bool s_MouseCaptured = false;
	bool Mouse::IsMouseCaptured()
	{
		return s_MouseCaptured;
	}
	void Mouse::SetCursorMode(CursorMode mode)
	{
		auto window = Application::Get()->GetWindow();
		glfwSetInputMode(static_cast<GLFWwindow*>(window->GetWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);

		if (Application::Get()->GetConfig().EnableImgui)
			UI::SetInputEnabled(mode == CursorMode::Normal);
	}
	CursorMode Mouse::GetCursorMode()
	{
		auto window = Application::Get()->GetWindow();
		return (CursorMode)(glfwGetInputMode(static_cast<GLFWwindow*>(window->GetWindow()), GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
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
