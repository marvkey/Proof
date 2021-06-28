#include "Proofprch.h"
#include "PerspectiveCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace Proof{
	PerspectiveCamera::PerspectiveCamera(float FOV_Degrees,float Aspect,float Near,float Far,const glm::vec3& Position)
		:m_ProjectionMatrix(glm::mat4(1.0f)),
		m_ViewMatrix(1.0f),m_Position(Position)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(FOV_Degrees),Aspect,Near,Far);
		RelcaluateViewMatrix();
	}
	void PerspectiveCamera::RelcaluateViewMatrix() {
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f),{m_Position})*
		Transform = glm::rotate(glm::mat4(1.0f),glm::radians(m_Position.x),{1,0,0})*Transform = glm::rotate(glm::mat4(1.0f),glm::radians(m_Position.y),{0,1,0})*
			Transform = glm::rotate(glm::mat4(1.0f),glm::radians(m_Position.z),{0,0,1});

		m_ViewMatrix = glm::inverse(Transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}