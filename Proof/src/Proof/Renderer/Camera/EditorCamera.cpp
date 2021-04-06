#include "Proofprch.h"
#include "EditorCamera.h"
#include "Proof/Events/MouseEvent.h"
#include "Platform/CurrentWindow.h"
namespace Proof {

	void EditorCamera3D::BeginPlay() {

	}
	void EditorCamera3D::OnUpdate(FrameTime DeltaTime){
		glm::mat4 CameraView = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		float radius = 5.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		CameraView = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
		EditorCameraShader.SetMat4("View", CameraView);
		KeyBoardInput(DeltaTime);
		MouseInput(DeltaTime);
		ScrollInput();
		if (MoveSpeed < 0 || MoveSpeed==0.25)
			MoveSpeed = 0.5;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = 89.0f;

		if (FieldOfView < 1.0f)
			FieldOfView = 1.0f;
		if (FieldOfView > 45.0f)
			FieldOfView = 45.0f;

		if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft) || Input::IsMouseButtonPressed(MouseButton::ButtonRight)) 
			glfwSetInputMode(CurrentWindow::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else {
			glfwSetInputMode(CurrentWindow::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			MouseMoveEvent Event;
			MouseLastPosX = Event.GetPosX();
			MouseLastPosY = Event.GetPosY();
		}
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

	void  EditorCamera3D::MouseInput(float DeltaTime) {
		MouseMoveEvent Event;
		bool ButttonPressed = Input::IsMouseButtonPressed(MouseButton::ButtonLeft) || Input::IsMouseButtonPressed(MouseButton::ButtonRight);
		if (Event.OnEvent()&&ButttonPressed) {
			if (FirstMouseEnteredScreen) {
				MouseLastPosX =Event.GetPosX();
				MouseLastPosY = Event.GetPosY();
				FirstMouseEnteredScreen = false;
			}
			
			Xoffset = Event.GetPosX() - MouseLastPosX;
			Yoffset = MouseLastPosY - Event.GetPosY();
			MouseLastPosX = Event.GetPosX();
			MouseLastPosY = Event.GetPosY();

			Xoffset *= Sensitivity;
			Yoffset *= Sensitivity;

			Yaw += Xoffset;
			Pitch += Yoffset;
			UpdateCameraVector();
		}
		
	}

	void  EditorCamera3D::ScrollInput() {
		MouseScrollEvent ScrollEvent;
		if (ScrollEvent.OnEvent()) {

			if (Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl)) {
				FieldOfView -= (float)ScrollEvent.GetPosY();
				return;
			}
			if (ScrollEvent.GetPosY() == 1) {
				MoveSpeed += 1.5;
			}
			else if (ScrollEvent.GetPosY() == -1) {
				if (MoveSpeed < 3) {
					MoveSpeed -= 0.25;
				}
				else {
					MoveSpeed -= 1.5;
				}
			}
		}
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
