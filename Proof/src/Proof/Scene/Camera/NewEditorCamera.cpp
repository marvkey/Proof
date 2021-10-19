#include "Proofprch.h"
#include "NewEditorCamera.h"
#include "Proof/Input/Input.h"
#include <ImGui/imgui.h>
namespace Proof{
	NewEditorCamera::NewEditorCamera(uint32_t width,uint32_t height,float fovDeg,float nearPlane,float farplane,glm::uvec3 position) {
		m_Height = height;
		m_Width = width;
		m_Positon =position;
		m_FovDeg = fovDeg;
		m_NearPlane =nearPlane;
		m_FarPlane =farplane;
		m_View = glm::lookAt(m_Positon,m_Positon+m_Direction,m_Up);
		m_Projection = glm::perspective(glm::radians(fovDeg),(float)(m_Width/m_Height),m_NearPlane,m_FarPlane);
		m_CameraMatrix = m_View*m_Projection;
	}
	void NewEditorCamera::OnUpdate(FrameTime DeltaTime,uint32_t width,uint32_t height) {
		m_Width=width;
		m_Height=height;
		Recalculate();	
		OnUpdate(DeltaTime);
	}
	void NewEditorCamera::OnUpdate(FrameTime DeltaTime) {
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight)) {
			glfwSetInputMode(CurrentWindow::GetWindow(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; // no mouse capture
			if(m_FirstClick == true){
				m_FirstClick=false;
				MouseLastPosX = Input::GetMousePosX();
				MouseLastPosY = Input::GetMousePosY();
			}

			Xoffset = Mouse::GetPosX() - MouseLastPosX;
			Yoffset = MouseLastPosY - Mouse::GetPosY();

			Xoffset *= m_Sensitivity*DeltaTime;
			Yoffset *= m_Sensitivity*DeltaTime;
			Yaw+=Xoffset;
			Pitch+=Yoffset;
			
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
			glm::vec3 CameraDirection;
			CameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			CameraDirection.y = sin(glm::radians(Pitch));
			CameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			m_Direction = glm::normalize(CameraDirection);
			MouseLastPosX = Input::GetMousePosX();
			MouseLastPosY = Input::GetMousePosY();
		}

		if (Input::IsMouseButtonReleased(MouseButton::ButtonRight)) {
			m_FirstClick =false;
			glfwSetInputMode(CurrentWindow::GetWindow(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // alllows mouse capture
		}

		if(Input::IsKeyPressed(KeyBoardKey::W)){
			m_Positon+=m_Speed * DeltaTime *m_Direction;
		}
		if (Input::IsKeyPressed(KeyBoardKey::A)) {
			m_Positon += m_Speed*DeltaTime * -glm::normalize(glm::cross(m_Direction,m_Up));
		}
		if (Input::IsKeyPressed(KeyBoardKey::S)) {
			m_Positon += m_Speed * DeltaTime * -m_Direction;
		}
		if (Input::IsKeyPressed(KeyBoardKey::D)) {
			m_Positon += m_Speed * DeltaTime * glm::normalize(glm::cross(m_Direction,m_Up));
		}
		if (Input::IsKeyPressed(KeyBoardKey::E))
			m_Positon += DeltaTime * m_Speed *m_Up;
		if (Input::IsKeyPressed(KeyBoardKey::Q))
			m_Positon -=DeltaTime * m_Speed* m_Up;
		if (Input::IsMouseScrolled()) {
			if (Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl)) {
				m_FovDeg -= (float)Input::GetScrollWheelY();
				return;
			}
			if (Input::IsMouseScrollUp()){
				if(m_Speed<15)
					m_Speed+=5;
				else{
					m_Speed+= 1.5;
				}
			}
			else if (Input::IsMouseScrollDown()) {
				if (m_Speed < 3)
					m_Speed -= 0.25;
				else if(m_Speed <15)
					m_Speed -= 1.5;
				else
					m_Speed -= 5;
			}
			PF_ENGINE_INFO("Mouse Speed %f",m_Speed);
		}
	}
	void NewEditorCamera::Recalculate() {
		m_CameraMatrix = m_View * m_Projection;
		m_View = glm::lookAt(m_Positon,m_Positon + m_Direction,m_Up);
		m_Projection = glm::perspective(glm::radians(m_FovDeg),(float)(m_Width/m_Height),m_NearPlane,m_FarPlane);
	}
}