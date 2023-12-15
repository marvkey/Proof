#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"

namespace Proof
{

	class Camera 
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection, const glm::mat4& view,const glm::mat4& unReversedProjection);
		Camera(const float degFov, const float width, const float height, const float nearP, const float farP,const glm::vec3& position, const glm::vec3&direction );
		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetUnReversedProjectionMatrix() const { return m_UnReversedProjectionMatrix; }

		void SetProjectionMatrix(const glm::mat4 projection, const glm::mat4& view, const glm::mat4 unReversedProjection)
		{
			m_ProjectionMatrix = projection;
			m_UnReversedProjectionMatrix = unReversedProjection;
			m_ViewMatrix = view;
		}
		void SetPerspectiveProjectionMatrix(const float degFov, const float width, const float height, const float nearP, const float farP, const glm::vec3& position, const glm::vec3& direction)
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(degFov), width / height, nearP, farP);
			m_ViewMatrix = glm::lookAt(position, position + direction, m_Up);
			m_UnReversedProjectionMatrix = glm::perspectiveFov(glm::radians(degFov), width, height, nearP, farP);
		}

		void SetOrthoProjectionMatrix(const float width, const float height, const float nearP, const float farP)
		{
			m_ProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, farP, nearP);
			m_UnReversedProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, nearP, farP);
		}

		float GetExposure() const { return m_Exposure; }
		float& GetExposure() { return m_Exposure; }
	protected:
		float m_Exposure = 0.8f;
		glm::vec3 m_Up{ 0,1,0 };
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_UnReversedProjectionMatrix = glm::mat4(1.0f);
	};
}