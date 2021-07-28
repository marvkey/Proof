#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Proof/Core/Core.h"
#include "Proof/Scene/Camera/Camera.h"
#include "Proof/Core/FrameTime.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace Proof {
	class Proof_API EditorCamera3D {
	public:
		
		void OnUpdate(FrameTime DeltaTime);
		float GetFieldOfView() { return FieldOfView; }
		virtual glm::mat4 GetCameraView()const;
		glm::vec3 GetCameraPosition() {
			return CameraPos;
		}
		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat({0.0f,0.0f,0.0f}));

			return glm::translate(glm::mat4(1.0f),{CameraPos})
				* rotation
				* glm::scale(glm::mat4(1.0f),{1,1,1});
		}
		glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f); 
	protected:
		virtual void BeginPlay();
		virtual void KeyBoardInput(float DeltaTime);
		virtual void MouseInput(float DeltaTime);
		virtual void ScrollInput();
	private:
		glm::mat4 CameraView = glm::mat4(1.0f);
		glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 CameraSide = glm::vec3(-1.0f, 0.0f, 0.0f);
		float MouseLastPosX;
		float MouseLastPosY;
		float Xoffset;
		float Yoffset;
		float Sensitivity =0.1f;
		float Yaw = -90.f;
		float Pitch = 0.f;
		bool FirstMouseEnteredScreen;
		float FieldOfView = 45.f;
		void UpdateCameraVector();
		float MoveSpeed = 2.5f;
		float RotationSpeed = 3.5f;
		float PanSpeed = 2.0f;
	};
}


