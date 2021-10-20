#include "Proofprch.h"
#include "Camera.h"

namespace Proof
{
	void Camera::CalculateProjection() {
		if (m_Positon == nullptr)
			return;
		m_View = glm::lookAt(glm::vec3{*m_Positon},glm::vec3{*m_Positon} + m_Direction,m_Up);
		m_Projection = glm::perspective(glm::radians(m_FovDeg),(float)m_Width / (float)m_Height,m_NearPlane,m_FarPlane);
		m_CameraMatrix = m_View * m_Projection;
	}
	Camera::Camera(Vector& Location):
		m_Positon(&Location)
	{
		if(m_Positon !=nullptr)
			CalculateProjection();
	}
}