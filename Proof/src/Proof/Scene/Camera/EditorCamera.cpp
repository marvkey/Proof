#include "Proofprch.h"
#include "EditorCamera.h"
#include "Proof/Input/Input.h"
#include "Proof/Input/Mouse.h"
#include <Imgui/imgui.h>
#include <GLFW/glfw3.h>
namespace Proof{
	EditorCamera::EditorCamera(uint32_t width,uint32_t height,float fovDeg,float nearPlane,float farplane,glm::vec3 position)
		:
		m_Width(width),
		m_Height(height)
	{
		m_Positon =position;
		m_FovDeg = fovDeg;
		m_NearPlane =nearPlane;
		m_FarPlane =farplane;
	}
	void EditorCamera::OnUpdate(float DeltaTime,uint32_t width,uint32_t height) {
		m_Width=width;
		m_Height=height;
		if (Input::IsMouseButtonReleased(MouseButton::ButtonRight))
		{
			m_FirstClick = true;
			Mouse::CaptureMouse(false);

			PF_ENGINE_INFO("Mouse Speed {}", m_Speed);
		}

		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
		{
			Mouse::CaptureMouse(true);
			if (m_FirstClick == true)
			{
				m_FirstClick = false;
				m_MouseLastPosX = Input::GetMousePosX();
				m_MouseLastPosY = Input::GetMousePosY();
			}

			m_Xoffset = Mouse::GetPosX() - m_MouseLastPosX;
			m_Yoffset = m_MouseLastPosY - Mouse::GetPosY();

			m_Xoffset *= m_Sensitivity * DeltaTime;
			m_Yoffset *= m_Sensitivity * DeltaTime;
			m_Yaw += m_Xoffset;
			m_Pitch += m_Yoffset;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;
			glm::vec3 CameraDirection;
			CameraDirection.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			CameraDirection.y = sin(glm::radians(m_Pitch));
			CameraDirection.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Direction = glm::normalize(CameraDirection);
			m_MouseLastPosX = Input::GetMousePosX();
			m_MouseLastPosY = Input::GetMousePosY();


			if (Input::IsKeyPressed(KeyBoardKey::W))
			{
				m_Positon += m_Speed * DeltaTime * m_Direction;
			}
			if (Input::IsKeyPressed(KeyBoardKey::A))
			{
				m_Positon += m_Speed * DeltaTime * -glm::normalize(glm::cross(m_Direction, m_Up));
			}
			if (Input::IsKeyPressed(KeyBoardKey::S))
			{
				m_Positon += m_Speed * DeltaTime * -m_Direction;
			}
			if (Input::IsKeyPressed(KeyBoardKey::D))
			{
				m_Positon += m_Speed * DeltaTime * glm::normalize(glm::cross(m_Direction, m_Up));
			}
			if (Input::IsKeyPressed(KeyBoardKey::E))
				m_Positon += DeltaTime * m_Speed * m_Up;

			if (Input::IsKeyPressed(KeyBoardKey::Q))
				m_Positon -= DeltaTime * m_Speed * m_Up;

			if (Input::IsMouseScrolled())
			{
				if (Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl))
				{
					m_FovDeg -= (float)Input::GetScrollWheelY();
					return;
				}
				if (Input::IsMouseScrollUp())
				{
					if (m_Speed < 15)
						m_Speed += 5;
					else
					{
						m_Speed += 1.5;
					}
				}
				else if (Input::IsMouseScrollDown())
				{
					if (m_Speed < 3)
						m_Speed -= 0.25;
					else if (m_Speed < 15)
						m_Speed -= 1.5;
					else
						m_Speed -= 5;
				}
				PF_ENGINE_INFO("Mouse Speed {}", m_Speed);
			}
		}
		Recalculate();
	}
	void EditorCamera::Recalculate() {

		SetPerspectiveProjectionMatrix(m_FovDeg, m_Width, m_Height, m_NearPlane, m_FarPlane, m_Positon, m_Direction);
	}
	EditorCamera2D::EditorCamera2D(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_Projection(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateProjection();
	}

	void EditorCamera2D::UpdateProjection() 
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera2D::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera2D::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera2D::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera2D::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera2D::OnUpdate(float ts)
	{
		if (Input::IsKeyPressed(KeyBoardKey::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMousePosX(), Input::GetMousePosY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(MouseButton::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
		if (Input::IsMouseScrolled())
		{
			float delta = Input::GetMousePosY() * 0.1f;
			MouseZoom(delta);
			UpdateView();
		}
	}

	void EditorCamera2D::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera2D::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera2D::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera2D::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera2D::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera2D::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera2D::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat EditorCamera2D::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

}