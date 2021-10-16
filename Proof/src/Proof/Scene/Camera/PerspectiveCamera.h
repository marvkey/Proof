#pragma once
#include "Proof/Resources/Math/Math.h"

namespace Proof{
	class Proof_API PerspectiveCamera {
	public:
		PerspectiveCamera(float FOV_Degrees,float Aspect,float Near, float Far,const glm::vec3& Position={0.f,0.f,0.f});

		void SetPosition(const Vector& Position) { m_Position = Position; RelcaluateViewMatrix(); };
		void SetRotation(const Vector& Position) { m_Rotation = m_Rotation; RelcaluateViewMatrix(); };
		glm::vec3 GetPosition(){return m_Position;}
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; };
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; };
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_LookAt;
		glm::vec3 m_up;
		//void SetCameraLookAt();
		void RelcaluateViewMatrix();
	};
}

