#include "Proofprch.h"
#include "OrthagraphicCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace Proof {
	OrthagraphicCamera::OrthagraphicCamera(float Left,float Right,float Bottom,float Top,const glm::vec3& Position)
		:m_ProjectionMatrix(glm::ortho(Left,Right,Bottom,Top,-1.0f,1.0f)),
		m_ViewMatrix(1.0f),m_Position(Position)
	{
		RelcaluateViewMatrix();
	}
	void OrthagraphicCamera::RelcaluateViewMatrix() {
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f),m_Position)* 
			glm::rotate(glm::mat4(1.0f),glm::radians(m_Rotation),glm::vec3(0,0,1));

		m_ViewMatrix = glm::inverse(Transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix*m_ViewMatrix;
	}
}