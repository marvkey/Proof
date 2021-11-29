#pragma once
#include "Proof/Resources/Math/Math.h"
namespace Proof
{
	class EditorCamera {
	public:
		glm::vec3 m_Positon {0,0,0};
		glm::vec3 m_Direction{0,0,-1};
		glm::vec3 m_Up {0,1,0};

		uint32_t m_Width, m_Height;
		float m_Speed =5;
		float m_Sensitivity =0.5;
		EditorCamera(uint32_t width,uint32_t height,float fovDeg=45,float nearPlane = 0.1,float farplane = 200,glm::uvec3 position ={0,0,0});

		glm::mat4 m_View =glm::mat4(1.0f);
		glm::mat4 m_Projection= glm::mat4(1.0f);
		glm::mat4 m_CameraMatrix= glm::mat4(1.0f);
		float m_FovDeg =45.f;
		float m_NearPlane =0.1;
		float m_FarPlane =200;

		void OnUpdate(FrameTime DeltaTime,uint32_t width,uint32_t height);
		void OnUpdate(FrameTime DeltaTime);
		void Recalculate();
		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat({0.0f,0.0f,0.0f}));

			return glm::translate(glm::mat4(1.0f),{m_Positon})
				* rotation
				* glm::scale(glm::mat4(1.0f),{1,1,1});
		}
		float MouseLastPosX;
		float MouseLastPosY;
		float Xoffset;
		float Yoffset;
		float Yaw = -90.f;
		float Pitch = 0.f;
		bool m_FirstClick = true;
	};
}
