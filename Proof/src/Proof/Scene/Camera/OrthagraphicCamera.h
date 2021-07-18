#pragma once
#include "Proof/Resources/Math/Math.h"
namespace Proof {
	class Proof_API OrthagraphicCamera {
	public:
		OrthagraphicCamera(float Left,float Right,float Bottom,float Top,const glm::vec3& Position ={0,0,0});

		void SetPosition(const glm::vec3& Position){m_Position = Position; RelcaluateViewMatrix();};
		void SetRotation(float Rotation){m_Rotation = Rotation; RelcaluateViewMatrix();};
		float GetRotation(){return m_Rotation;};
		glm::vec3 GetPosition(){return m_Position;};
		const glm::mat4& GetProjectionMatrix() const{return m_ProjectionMatrix;};
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; };
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position;
		float m_Rotation = 0.0f; // along z axis
		void RelcaluateViewMatrix();
	};
}

