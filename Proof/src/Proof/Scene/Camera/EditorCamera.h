#pragma once
#include "Camera.h"
namespace Proof
{
	//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/e7c349b70bd95af3ab673556cdb56cb2cc40b48e/Engine/X2/Editor/EditorCamera.cpp
	enum class CameraMode
	{
		NONE, FLYCAM, ARCBALL
	};
#if 0
	class EditorCamera : public Camera
	{
	public:
		EditorCamera(const float degFov, const float width, const float height, float nearPlane = 0.1, float farplane = 2000);
		void Init();

		void Focus(const glm::vec3& focusPoint);
		void OnUpdate(FrameTime ts);
		void OnEvent(class Event& e);

		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }

		CameraMode GetCurrentMode() const { return m_CameraMode; }

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		const glm::vec3& GetFocalPoint() const { return m_FocalPoint; }

		inline void SetViewportSize(uint32_t width, uint32_t height)
		{
			if (m_ViewportWidth == width && m_ViewportHeight == height)
				return;
			SetPerspectiveProjectionMatrix(m_VerticalFOV, (float)width, (float)height, m_NearClip, m_FarClip,m_Position,m_Direction);
			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjection() const { return GetProjectionMatrix() * m_ViewMatrix; }
		const glm::mat4& GetUnReversedViewProjection() const { return GetUnReversedProjectionMatrix() * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		const glm::vec3& GetPosition() const { return m_Position; }

		glm::quat GetOrientation() const;

		[[nodiscard]] float GetVerticalFOV() const { return m_VerticalFOV; }
		[[nodiscard]] float GetAspectRatio() const { return m_AspectRatio; }
		[[nodiscard]] float GetNearClip() const { return m_NearClip; }
		[[nodiscard]] float GetFarClip() const { return m_FarClip; }
		[[nodiscard]] float GetPitch() const { return m_Pitch; }
		[[nodiscard]] float GetYaw() const { return m_Yaw; }
		[[nodiscard]] float GetCameraSpeed() const;
	private:
		void UpdateCameraView();

		bool OnMouseScroll(class MouseScrollEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		glm::vec3 m_Position, m_Direction, m_FocalPoint;

		// Perspective projection params
		float m_VerticalFOV, m_AspectRatio, m_NearClip, m_FarClip;

		bool m_IsActive = false;
		bool m_Panning, m_Rotating;
		glm::vec2 m_InitialMousePosition{};
		glm::vec3 m_InitialFocalPoint, m_InitialRotation;

		float m_Distance;
		float m_NormalSpeed{ 0.002f };

		float m_Pitch, m_Yaw;
		float m_PitchDelta{}, m_YawDelta{};
		glm::vec3 m_PositionDelta{};
		glm::vec3 m_RightDirection{};

		CameraMode m_CameraMode{ CameraMode::ARCBALL };

		float m_MinFocusDistance{ 100.0f };

		uint32_t m_ViewportWidth{ 1280 }, m_ViewportHeight{ 720 };

		constexpr static float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 2.0f };
		friend class EditorLayer;
	};
#endif
#if 1

	class EditorCamera : public Camera 
	{
	public:
		EditorCamera(uint32_t width =200, uint32_t height =200, float fovDeg = 45, float nearPlane = 0.1, float farplane = 2000, glm::vec3 position = { -5,5,5 });
		void OnUpdate(float deltaTime);
		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat({ 0.0f,0.0f,0.0f }));

			return glm::translate(glm::mat4(1.0f), { m_Position })
				* rotation
				* glm::scale(glm::mat4(1.0f), { 1,1,1 });
		}

		void SetPosition(glm::vec3 position)
		{
			m_Position = position;
		}

		glm::vec3 GetPosition()const
		{
			return m_Position;
		}

		float GetNearPlane()const { return m_NearPlane; }
		float GetFarPlane()const { return m_FarPlane; }
		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
		void SetViewportSize(uint32_t width, uint32_t height);
		void OnEvent(class Event& event);

		float GetSpeed() { return m_Speed; };
		void SetSpeed(float speed);

		float GetFOV() { return m_FovDeg; }
		void SetFOV(float fov); 

		constexpr static float MIN_SPEED{ 2.f }, MAX_SPEED{ 100.0f };
		constexpr static float MIN_FOV{ 5.f }, MAX_FOV{ 170.0f };
		void Recalculate();
	private:
		float m_FovDeg = 45.f;
		float m_NearPlane = 0.1;
		float m_FarPlane = 2000;
		float m_Speed = 5;
		float m_Sensitivity = 25;

		glm::vec3 m_Position{ 0,0,0 };
		glm::vec3 m_Direction{ 0,0,-1 };
		glm::vec3 m_Up{ 0,1,0 };

		float m_MouseLastPosX;
		float m_MouseLastPosY;
		float m_Xoffset;
		float m_Yoffset;
		float m_Yaw = -90.f;
		float m_Pitch = 0.f;
		bool m_FirstClick = true;
		uint32_t m_Width, m_Height;
		bool m_IsActive = false;
	private:
		bool OnMouseScroll(class MouseScrollEvent& e);
		void MouseZoom(float zoom);
		void UpdateKeyBoardMovement(float deltaTime);
		bool UpdateMouseMovement(float deltaTime); // returns if active
		void UpdateMovement(float deltaTime);
	};
#if 0
	class EditorCamera : public Camera{
	public:
		EditorCamera(float fovDeg=45, const float width =200, const float height = 200,float nearPlane = 0.1,float farplane = 2000,glm::vec3 position ={-5,5,5});
		void OnUpdate(float DeltaTime);

		void SetPosition(glm::vec3 position) { m_Position = position; }
		glm::vec3 GetPosition()const { return m_Position; }

		float GetNearPlane()const { return m_NearPlane; }
		float GetFarPlane()const { return m_FarPlane; }
		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active) { m_IsActive = active; }
		void SetViewportSize(uint32_t width, uint32_t height);
		void OnEvent(class Event& event);

	private:
		// camera Attributes
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		// euler Angles
		float m_Yaw;
		float m_Pitch;
		// camera options
		float m_MovementSpeed = 0.002f;
		float m_MouseSensitivity = 0.1f;
		float m_FovDeg;
		float m_NearPlane;
		float m_FarPlane;
		uint32_t m_Width =200, m_Height = 200;

		const float YAW = -90.0f;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVITY = 0.1f;
		const float ZOOM = 45.0f;
		bool m_FirstMouse = true;
		bool m_IsActive = false;

		float m_LastMousePosX;
		float m_LastMousePosY;
	private:
		void UpdateCameraVectors();
		void UpdateKeyBoardMovement(float deltaTime);
		void UpdateMouseMovement(float deltaTime);
		bool OnMouseScroll(class MouseScrollEvent& e);
		void MouseZoom(float zoom);

		constexpr static float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 5.0f };

	};
#endif
#endif


	class EditorCamera2D {
	public:
		EditorCamera2D() = default;
		EditorCamera2D(float fov=30.0f, float aspectRatio = 1.778f, float nearClip = 0.1, float farClip=2000);

		void OnUpdate(float ts);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();


		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}
