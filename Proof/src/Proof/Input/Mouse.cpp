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
	
	glm::vec2 Mouse::GetMouseViewportSpace(const glm::vec2& windowPos, const glm::vec2& windowSize)
	{
			auto mousePos = GetMousePos();

		// Calculate the offset of the mouse position within the window
		mousePos.x -= windowPos.x;
		mousePos.y -= windowPos.y;

		// Calculate the width and height of the viewport
		auto viewportWidth = windowSize.x;
		auto viewportHeight = windowSize.y;

		// Calculate and return the mouse position in normalized device coordinates (NDC)
		return { (mousePos.x / viewportWidth) * 2.0f - 1.0f, ((mousePos.y / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}

	bool Mouse::IsMouseMoved() {
		return Application::Get()->GetWindow()->IsMouseMoved();
	}

}
