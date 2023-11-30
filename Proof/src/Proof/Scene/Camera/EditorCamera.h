#pragma once
#include "Camera.h"
namespace Proof
{
	class EditorCamera : public Camera{
	public:
		EditorCamera(uint32_t width,uint32_t height,float fovDeg=45,float nearPlane = 0.1,float farplane = 2000,glm::vec3 position ={-5,5,5});
		void OnUpdate(float DeltaTime,uint32_t width,uint32_t height);
		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat({0.0f,0.0f,0.0f}));

			return glm::translate(glm::mat4(1.0f),{m_Positon})
				* rotation
				* glm::scale(glm::mat4(1.0f),{1,1,1});
		}

		void SetPosition(glm::vec3 position)
		{
			m_Positon = position;
		}

		glm::vec3 GetPosition()const
		{
			return m_Positon;
		}

		float GetNearPlane()const { return m_NearPlane; }
		float GetFarPlane()const { return m_FarPlane; }
	private:
		float m_FovDeg = 45.f;
		float m_NearPlane = 0.1;
		float m_FarPlane = 2000;
		float m_Speed = 5;
		float m_Sensitivity = 25;

		glm::vec3 m_Positon{ 0,0,0 };
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
	private:
		void Recalculate();

	};



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
