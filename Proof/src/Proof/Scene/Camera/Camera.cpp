#include "Proofprch.h"
#include "Camera.h"

namespace Proof
{
	Camera::Camera(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& unReversedProjection)
		:
		m_ProjectionMatrix(projection), m_ViewMatrix(view), m_UnReversedProjectionMatrix(unReversedProjection)
	{

	}
	Camera::Camera(const float degFov, const float width, const float height, const float nearP, const float farP, const glm::vec3& position, const glm::vec3& direction)
		:
		m_ProjectionMatrix(glm::perspectiveFov(glm::radians(degFov), width, height, farP, nearP)),
		m_ViewMatrix (glm::lookAt(position, position + direction, m_Up)),
		m_UnReversedProjectionMatrix(glm::perspectiveFov(glm::radians(degFov), width, height, nearP, farP))
	{

	}
}

