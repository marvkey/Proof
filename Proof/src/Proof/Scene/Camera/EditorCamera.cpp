#include "Proofprch.h"
#include "EditorCamera.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Core/CurrentWindow.h"
#include <ImGui/imgui.h>
namespace Proof {

	void EditorCamera3D::BeginPlay() {

	}
	void EditorCamera3D::OnUpdate(FrameTime DeltaTime){
		//glm::vec3 temp{CameraPos.x*3,CameraPos.y * 3,CameraPos.z * 3};
		CameraView = glm::lookAt(CameraPos,CameraPos + CameraFront, CameraUp);
		
		if (Input::IsMouseButtonReleased(MouseButton::ButtonRight)){
			glfwSetInputMode(CurrentWindow::GetWindow(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // alllows mouse capture

		}
			
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight)) {
			glfwSetInputMode(CurrentWindow::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; // no mouse capture

			MouseInput(DeltaTime);
			KeyBoardInput(DeltaTime);
			ScrollInput();
			MouseLastPosX = Input::GetMousePosX();
			MouseLastPosY = Input::GetMousePosY();
		}
		
		if (MoveSpeed < 0 || MoveSpeed == 0.25)
			MoveSpeed = 0.5;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = 89.0f;

		if (FieldOfView < 1.0f)
			FieldOfView = 1.0f;
		if (FieldOfView > 45.0f)
			FieldOfView = 45.0f;
	}

	void EditorCamera3D::KeyBoardInput(float DeltaTime) {
		if (Input::IsKeyPressed(KeyBoardKey::W)) 
			CameraPos += MoveSpeed * CameraFront * DeltaTime;
		if (Input::IsKeyPressed(KeyBoardKey::S)) 
			CameraPos -= MoveSpeed * CameraFront * DeltaTime;
		if (Input::IsKeyPressed(KeyBoardKey::A)) 
			CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * MoveSpeed * DeltaTime;
		if (Input::IsKeyPressed(KeyBoardKey::D)) 
			CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * MoveSpeed * DeltaTime;
		if (Input::IsKeyPressed(KeyBoardKey::E))
			CameraPos += CameraUp*DeltaTime*MoveSpeed;
		if (Input::IsKeyPressed(KeyBoardKey::Q))
			CameraPos -= CameraUp * DeltaTime*MoveSpeed;

	}

	void EditorCamera3D::MouseInput(float DeltaTime) {
		bool ButttonPressed = Input::IsMouseButtonPressed(MouseButton::ButtonRight);
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight)) {
			if (FirstMouseEnteredScreen) {
				MouseLastPosX =Mouse::GetPosX();
				MouseLastPosY = Mouse::GetPosY();
				FirstMouseEnteredScreen = false;
			}
			
			Xoffset = Mouse::GetPosX() - MouseLastPosX;
			Yoffset = MouseLastPosY - Mouse::GetPosY();
			MouseLastPosX = Mouse::GetPosX();
			MouseLastPosY = Mouse::GetPosY();

			Xoffset *= Sensitivity;
			Yoffset *= Sensitivity;

			Yaw += Xoffset;
			Pitch += Yoffset;
			UpdateCameraVector();
		}
	}

	void  EditorCamera3D::ScrollInput() {
		if (Input::IsMouseScrolled()) {
			if (Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl)) {
				FieldOfView -= (float)Input::GetScrollWheelY();
				return;
			}
			if (Input::IsMouseScrollUp())
				MoveSpeed += 1.5;
			else if (Input::IsMouseScrollDown()) {
				if (MoveSpeed < 3) 
					MoveSpeed -= 0.25;
				else 
					MoveSpeed -= 1.5;
			}
			PF_ENGINE_INFO("Mouse Speed %f",MoveSpeed);
		}
	}
	glm::mat4 EditorCamera3D::GetCameraView()const{
		return CameraView;
	}
	void EditorCamera3D::UpdateCameraVector(){
		glm::vec3 CameraDirection;
		CameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		CameraDirection.y = sin(glm::radians(Pitch));
		CameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		CameraFront = glm::normalize(CameraDirection);
		CameraFront = glm::normalize(CameraFront);
	}
}
