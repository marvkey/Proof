#include "Proofprch.h"
#include "Camera.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/KeyEvent.h"

namespace Proof {
	void Camera3D::OnUpdate(FrameTime DeltaTime) {
		MoveSpeed *= DeltaTime;
		KeyBoardInput(DeltaTime);
		MouseInput(DeltaTime);
		ScrollInput();
	}

	void Camera3D::BeginPlay() {
	}

	void Camera3D::KeyBoardInput(float DeltaTime){

		KeyClickedEvent KeyPressed;
		if (KeyPressed.GetKeyClicked() == KeyBoardKey::W)
			CameraPos += MoveSpeed * CameraFront;
		if (KeyPressed.GetKeyClicked() == KeyBoardKey::S)
			CameraPos -= MoveSpeed * CameraFront;
		if (KeyPressed.GetKeyClicked() == KeyBoardKey::A)
			CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * MoveSpeed;
		if (KeyPressed.GetKeyClicked() == KeyBoardKey::D)
			CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * MoveSpeed;
	}

	void Camera3D::MouseInput(float DeltaTime){
		
	}

	void Camera3D::ScrollInput(){
		MouseScrollEvent ScrollEvent;
		if (MoveSpeed > 0 && MoveSpeed < 100) {
			if (ScrollEvent.GetPosY() == 1) {
				MoveSpeed += 3;
			}
			else if (ScrollEvent.GetPosY() == -1) {
				MoveSpeed -= 3;
			}
		}
	}

}
