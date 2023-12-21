#include "Proofprch.h"
#include "EditorCamera.h"
#include "Proof/Input/Input.h"
#include "Proof/Input/Mouse.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Events/Event.h"
#include "Proof/Events/MouseEvent.h"
#include <imgui.h>
#include <GLFW/glfw3.h>
namespace Proof
{
#if 0
	EditorCamera::EditorCamera(const float degFov, const float width, const float height, const float nearP, const float farP)
		: Camera(glm::perspectiveFov(glm::radians(degFov), width, height, farP, nearP), glm::mat4(1.0f),glm::perspectiveFov(glm::radians(degFov), width, height, nearP, farP)), 
		m_FocalPoint(0.0f), m_VerticalFOV(glm::radians(degFov)), m_NearClip(nearP), m_FarClip(farP)
	{
		Init();
	}

	void EditorCamera::Init()
	{
		constexpr glm::vec3 position = { -5, 5, 5 };
		m_Distance = glm::distance(position, m_FocalPoint);

		m_Yaw = 3.0f * glm::pi<float>() / 4.0f;
		m_Pitch = glm::pi<float>() / 4.0f;

		m_Position = CalculatePosition();
		const glm::quat orientation = GetOrientation();
		m_Direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	static void DisableMouse()
	{
		Mouse::SetCursorMode(CursorMode::Locked);
		UI::SetInputEnabled(false);
	}

	static void EnableMouse()
	{
		Mouse::SetCursorMode(CursorMode::Normal);
		UI::SetInputEnabled(true);
	}

	void EditorCamera::OnUpdate(FrameTime ts)
	{
		const glm::vec2& mouse{ Mouse::GetPosX(), Mouse::GetPosY() };
		const glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.002f;

		if (!m_IsActive)
		{
			if (!UI::IsInputEnabled())
				UI::SetInputEnabled(true);

			return;
		}

		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight) && !Input::IsKeyPressed(KeyBoardKey::LeftAlt))
		{
			m_CameraMode = CameraMode::FLYCAM;
			DisableMouse();
			const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

			const float speed = GetCameraSpeed();

			//*1000 for ms
			if (Input::IsKeyPressed(KeyBoardKey::Q))
				m_PositionDelta -= ts *1000 * speed * glm::vec3{ 0.f, yawSign, 0.f };
			if (Input::IsKeyPressed(KeyBoardKey::E))
				m_PositionDelta += ts * 1000 * speed * glm::vec3{ 0.f, yawSign, 0.f };
			if (Input::IsKeyPressed(KeyBoardKey::S))
				m_PositionDelta -= ts * 1000 * speed * m_Direction;
			if (Input::IsKeyPressed(KeyBoardKey::W))
				m_PositionDelta += ts* 1000 * speed * m_Direction;
			if (Input::IsKeyPressed(KeyBoardKey::A))
				m_PositionDelta -= ts* 1000 * speed * m_RightDirection;
			if (Input::IsKeyPressed(KeyBoardKey::D))
				m_PositionDelta += ts* 1000 * speed * m_RightDirection;

			constexpr float maxRate{ 0.12f };
			m_YawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
			m_PitchDelta += glm::clamp(delta.y * RotationSpeed(), -maxRate, maxRate);

			m_RightDirection = glm::cross(m_Direction, glm::vec3{ 0.f, yawSign, 0.f });

			m_Direction = glm::rotate(glm::normalize(glm::cross(glm::angleAxis(-m_PitchDelta, m_RightDirection),
				glm::angleAxis(-m_YawDelta, glm::vec3{ 0.f, yawSign, 0.f }))), m_Direction);

			const float distance = glm::distance(m_FocalPoint, m_Position);
			m_FocalPoint = m_Position + GetForwardDirection() * distance;
			m_Distance = distance;
		}
		else if (Input::IsKeyPressed(KeyBoardKey::LeftAlt))
		{
			m_CameraMode = CameraMode::ARCBALL;

			if (Input::IsMouseButtonPressed(MouseButton::ButtonMiddle))
			{
				DisableMouse();
				MousePan(delta);
			}
			else if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
			{
				DisableMouse();
				MouseRotate(delta);
			}
			else if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
			{
				DisableMouse();
				MouseZoom((delta.x + delta.y) * 0.1f);
			}
			else
				EnableMouse();
		}
		else
		{
			EnableMouse();
		}

		m_InitialMousePosition = mouse;
		m_Position += m_PositionDelta;
		m_Yaw += m_YawDelta;
		m_Pitch += m_PitchDelta;

		if (m_CameraMode == CameraMode::ARCBALL)
			m_Position = CalculatePosition();

		UpdateCameraView();
}

	float EditorCamera::GetCameraSpeed() const
	{
		float speed = m_NormalSpeed;
		if (Input::IsKeyPressed(KeyBoardKey::LeftControl))
			speed /= 2 - glm::log(m_NormalSpeed);
		if (Input::IsKeyPressed(KeyBoardKey::LeftShift))
			speed *= 2 - glm::log(m_NormalSpeed);

		return glm::clamp(speed, MIN_SPEED, MAX_SPEED);
	}

	void EditorCamera::UpdateCameraView()
	{
		const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

		// Extra step to handle the problem when the camera direction is the same as the up vector
		const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
		if (cosAngle * yawSign > 0.99f)
			m_PitchDelta = 0.f;

		const glm::vec3 lookAt = m_Position + GetForwardDirection();

		m_Direction = glm::normalize(lookAt - m_Position);
		m_Distance = glm::distance(m_Position, m_FocalPoint);
		m_ViewMatrix = glm::lookAt(m_Position, lookAt, glm::vec3{ 0.f, yawSign, 0.f });
		//damping for smooth camera
		m_YawDelta *= 0.6f;
		m_PitchDelta *= 0.6f;
		m_PositionDelta *= 0.8f;
	}

	void EditorCamera::Focus(const glm::vec3& focusPoint)
	{
		m_FocalPoint = focusPoint;
		m_CameraMode = CameraMode::FLYCAM;
		if (m_Distance > m_MinFocusDistance)
		{
			m_Distance -= m_Distance - m_MinFocusDistance;
			m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
		}
		m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
		UpdateCameraView();
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		const float x = glm::min(float(m_ViewportWidth) / 1000.0f, 2.4f); // max = 2.4f
		const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		const float y = glm::min(float(m_ViewportHeight) / 1000.0f, 2.4f); // max = 2.4f
		const float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.3f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = glm::max(distance, 0.0f);
		float speed = distance * distance;
		speed = glm::min(speed, 50.0f); // max speed = 50
		return speed;
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrollEvent>([this](MouseScrollEvent& e) { return OnMouseScroll(e); });
	}

	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		if (!IsActive())
			return false;
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
		{
			
			m_NormalSpeed += e.GetScrollY() * 0.3f * m_NormalSpeed;
			m_NormalSpeed = std::clamp(m_NormalSpeed, MIN_SPEED, MAX_SPEED);
			PF_ENGINE_INFO("Mouse Speed {}", m_NormalSpeed);
		}
		else
		{
			MouseZoom(e.GetScrollY() * 0.1f);
			UpdateCameraView();
		}

		return true;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint -= GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		m_YawDelta += yawSign * delta.x * RotationSpeed();
		m_PitchDelta += delta.y * RotationSpeed();

		if (m_YawDelta > 89.0f)
			m_YawDelta = 89.0f;
		if (m_PitchDelta < -89.0f)
			m_PitchDelta = -89.0f;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		const glm::vec3 forwardDir = GetForwardDirection();
		m_Position = m_FocalPoint - forwardDir * m_Distance;
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += forwardDir * m_Distance;
			m_Distance = 1.0f;
		}
		m_PositionDelta += delta * ZoomSpeed() * forwardDir;
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance + m_PositionDelta;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch - m_PitchDelta, -m_Yaw - m_YawDelta, 0.0f));
	}
#endif
	static void DisableMouse()
	{
		Mouse::SetCursorMode(CursorMode::Locked);
		UI::SetInputEnabled(false);
	}

	static void EnableMouse()
	{
		Mouse::SetCursorMode(CursorMode::Normal);
		UI::SetInputEnabled(true);
	}
#if 0

	EditorCamera::EditorCamera(float fovDeg, const float width, const float height,float nearPlane,float farplane,glm::vec3 position)
	{
		m_Position = position;
		m_FovDeg = fovDeg;
		m_NearPlane =nearPlane;
		m_FarPlane = farplane;
		m_Width = width;
		m_Height = height;
		m_MouseSensitivity = 1.f;
		m_Yaw = -90.f;
		UpdateCameraVectors();
	}	
	void EditorCamera::OnUpdate(float deltaTime) 
	{
		UpdateMouseMovement(deltaTime);
	}

	void EditorCamera::UpdateKeyBoardMovement(float deltaTime)
	{
		if (!IsActive())
			return;

		bool changed = false;
		if (Input::IsKeyPressed(KeyBoardKey::W))
		{
			m_Position += m_MovementSpeed * deltaTime * m_Front;
			changed = true;
		}
		if (Input::IsKeyPressed(KeyBoardKey::S))
		{
			m_Position -= m_MovementSpeed * deltaTime * m_Front;
			changed = true;
		}
		if (Input::IsKeyPressed(KeyBoardKey::D))
		{
			m_Position += m_MovementSpeed * deltaTime * glm::normalize(glm::cross(m_Front, Math::GetUpVector()));
			changed = true;
		}
		if (Input::IsKeyPressed(KeyBoardKey::A))
		{
			m_Position -= m_MovementSpeed * deltaTime * glm::normalize(glm::cross(m_Front, Math::GetUpVector()));
			changed = true;
		}
		if (Input::IsKeyPressed(KeyBoardKey::E))
		{
			m_Position += m_MovementSpeed * deltaTime * Math::GetUpVector();
			changed = true;
		}
		if (Input::IsKeyPressed(KeyBoardKey::Q))
		{
			m_Position -= m_MovementSpeed * deltaTime * Math::GetUpVector();
			changed = true;
		}

		if(changed)
			UpdateCameraVectors();
	}
	void EditorCamera::UpdateMouseMovement(float deltaTime)
	{
		//if (!m_IsActive)
		//{
		//	if (!UI::IsInputEnabled())
		//		UI::SetInputEnabled(true);
		//
		//	return;
		//}

		if (Input::IsMouseButtonReleased(MouseButton::ButtonRight))
		{
			m_FirstMouse = true;
			EnableMouse();
		}
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
		{
			DisableMouse();

			if (m_FirstMouse)
			{
				m_LastMousePosX = Mouse::GetPosX();
				m_LastMousePosY = Mouse::GetPosY();
				m_FirstMouse = false;
			}
			float xOffset = Mouse::GetPosX() - m_LastMousePosX;
			float yOffset = m_LastMousePosY - Mouse::GetPosY();

			xOffset *= m_MouseSensitivity * deltaTime;
			yOffset *= m_MouseSensitivity * deltaTime;

			m_Yaw += xOffset;
			m_Pitch += yOffset;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;
			UpdateCameraVectors();

			UpdateKeyBoardMovement(deltaTime);
		}
		else
		{
			//DisableMouse();
		}
	}

	

	void EditorCamera::UpdateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		SetPerspectiveProjectionMatrix(m_FovDeg, m_Width, m_Height, m_NearPlane, m_FarPlane, m_Position, m_Front);
	}
	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrollEvent>([this](MouseScrollEvent& e) { return OnMouseScroll(e); });
	}
	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		if (!IsActive())
			return false;

		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
		{
			m_MovementSpeed += e.GetScrollY() * 0.3f * m_MovementSpeed;
			m_MovementSpeed = std::clamp(m_MovementSpeed, 2.0f, 100.0f);
		}
		else
		{
			MouseZoom(e.GetScrollY() * 0.1f);
			UpdateCameraVectors();
		}

		return true;
	}
	void EditorCamera::MouseZoom(float zoom)
	{
		m_FovDeg -= (float)zoom;
		if (m_FovDeg < 1.0f)
			m_FovDeg = 1.0f;
		if (m_FovDeg > 45.0f)
			m_FovDeg = 45.0f;
	}


	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		if (m_Width == width && m_Height == height)
			return;
		m_Width = width;
		m_Height = height;
		UpdateCameraVectors();
	}
#endif
	EditorCamera::EditorCamera(uint32_t width, uint32_t height, float fovDeg, float nearPlane, float farplane, glm::vec3 position)
		:
		m_Width(width),
		m_Height(height)
	{
		m_Position = position;
		m_FovDeg = fovDeg;
		m_NearPlane = nearPlane;
		m_FarPlane = farplane;
	}
	void EditorCamera::OnUpdate(float deltaTime)
	{
		UpdateMovement(deltaTime);
	}
	void EditorCamera::UpdateMovement(float deltaTime)
	{
		if (UpdateMouseMovement(deltaTime))
		{
			UpdateKeyBoardMovement(deltaTime);
			Recalculate();
		}
	}
	bool EditorCamera::UpdateMouseMovement(float deltaTime) // returns if active
	{
		bool active = false;
		if (Input::IsMouseButtonReleased(MouseButton::ButtonRight))
		{
			m_FirstClick = true;
			EnableMouse();
		}

		if (!IsActive())
		{
			EnableMouse();
			return false;
		}

		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
		{
			active = true;
			DisableMouse();
			if (m_FirstClick == true)
			{
				m_FirstClick = false;
				m_MouseLastPosX = Input::GetMousePosX();
				m_MouseLastPosY = Input::GetMousePosY();
			}

			m_Xoffset = Mouse::GetPosX() - m_MouseLastPosX;
			m_Yoffset = m_MouseLastPosY - Mouse::GetPosY();

			m_Xoffset *= m_Sensitivity * deltaTime;
			m_Yoffset *= m_Sensitivity * deltaTime;
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

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;
		}
		return active;
	}
	void EditorCamera::UpdateKeyBoardMovement(float deltaTime)
	{
		

		if (Input::IsKeyPressed(KeyBoardKey::W))
			m_Position += m_Speed * deltaTime * m_Direction;
		if (Input::IsKeyPressed(KeyBoardKey::A))
			m_Position += m_Speed * deltaTime * -glm::normalize(glm::cross(m_Direction, m_Up));
		if (Input::IsKeyPressed(KeyBoardKey::S))
			m_Position += m_Speed * deltaTime * -m_Direction;
		if (Input::IsKeyPressed(KeyBoardKey::D))
			m_Position += m_Speed * deltaTime * glm::normalize(glm::cross(m_Direction, m_Up));
		if (Input::IsKeyPressed(KeyBoardKey::E))
			m_Position += deltaTime * m_Speed * m_Up;
		if (Input::IsKeyPressed(KeyBoardKey::Q))
			m_Position -= deltaTime * m_Speed * m_Up;

	}
	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrollEvent>([this](MouseScrollEvent& e) { return OnMouseScroll(e); });
	}
	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		if (!IsActive())
			return false;
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
		{
			m_Speed += e.GetScrollY() * 0.3f * m_Speed;
			m_Speed = std::clamp(m_Speed,MIN_SPEED, MAX_SPEED);
		}
		else
		{
			MouseZoom(e.GetScrollY());
			Recalculate();
		}

		return true;
	}
	void EditorCamera::MouseZoom(float zoom)
	{
		m_FovDeg -= (float)zoom;
		if (m_FovDeg < MIN_FOV)
			m_FovDeg = MIN_FOV;
		if (m_FovDeg > MAX_FOV)
			m_FovDeg = MAX_FOV;

	}
	void EditorCamera::SetSpeed(float speed)
	{
		m_Speed = speed;
		m_Speed = std::clamp(m_Speed, MIN_SPEED, MAX_SPEED);
	}
	void EditorCamera::SetFOV(float fov)
	{
		m_FovDeg = fov;
		if (m_FovDeg < MIN_FOV)
			m_FovDeg = MIN_FOV;
		if (m_FovDeg > MAX_FOV)
			m_FovDeg = MAX_FOV;
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		if (m_Width == width && m_Height == height)
			return;
		m_Width = width;
		m_Height = height;
		Recalculate();
	}
	void EditorCamera::Recalculate() 
	{
		SetPerspectiveProjectionMatrix(m_FovDeg, m_Width, m_Height, m_NearPlane, m_FarPlane, m_Position, m_Direction);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="fov"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
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