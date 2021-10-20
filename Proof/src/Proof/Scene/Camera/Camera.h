#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Math/Math.h"

namespace Proof{
	class Proof_API Camera {
	public:
		enum class CameraType{Orthographic =0, Perspective=1};
		void CalculateProjection();
		void SetDimensions(uint32_t width,uint32_t Height){
			m_Width = width;
			m_Height = Height;
			CalculateProjection();
		}
		bool AutoSetDimension(bool value){
			value = m_AutoSetDimension;
		}
		Camera(Vector& Location);
		Camera()=default;
		
	private:
		bool m_AutoSetDimension =true;
		float m_NearPlane=0.1;
		float m_FarPlane=1000;
		float m_FovDeg=45;
		uint32_t m_Width=250,m_Height=250;

		Vector* m_Positon=nullptr;
		glm::vec3 m_Direction = {0,0,-1};
		glm::vec3 m_Up = {0,1,0};

		CameraType m_CameraType = CameraType::Perspective;
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_CameraMatrix = glm::mat4(1.0f);
		friend class World;
		friend class SceneHierachyPanel;
	};
}

