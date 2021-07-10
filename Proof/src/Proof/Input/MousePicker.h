#pragma once
#include "Proof/Core/Core.h"
#include "Proof3D/Math/Math.h"
#include "Mouse.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof3D/Renderer/Camera/EditorCamera.h"
namespace Proof{
	class Proof_API MousePicker {
	public:
		MousePicker(glm::mat4 ProjectionMatrix,class EditorCamera3D& camera);
		Vector GetCurrentRay(){
			return m_CurrentRay;	
		}
		void Update(){
			m_ViewMatrix = m_Camera->GetCameraView();
			m_CurrentRay = CalculateMouseArray();
		}
	private:
		EditorCamera3D* m_Camera;
		Vector m_CurrentRay;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::vec3 CalculateMouseArray(){
			glm::vec2 NormalizedDeviceCoords = GetNormalizedDeviceCoords(Mouse::GetMouseX(),Mouse::GetMouseY());
			glm::vec4 RayClip(NormalizedDeviceCoords.x,NormalizedDeviceCoords.y,-1.0f,1.0f);
			glm::vec4 RayEye = glm::inverse(m_ProjectionMatrix)* RayClip;
			RayEye ={RayEye.x,RayEye.y,-1.0f,0.0f};
			glm::vec3 RayWorld = glm::inverse(m_ViewMatrix) * RayEye;
			RayWorld = glm::normalize(RayWorld);
			return RayWorld;
		}
		
		glm::vec2 GetNormalizedDeviceCoords(float MouseX, float MouseY){
			float x = (2.0f * MouseX) / CurrentWindow::GetWindowWidth() - 1.0f;
			float y = 1.0f - (2.0f * MouseY) / CurrentWindow::GetWindowHeight();
			return{x,y};
		}
	};
}
