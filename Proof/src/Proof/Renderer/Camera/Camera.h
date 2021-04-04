#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Proof {
	class Proof_API Camera3D {
	protected:
		virtual void OnUpdate(FrameTime DeltaTime);
		virtual void BeginPlay();
	private:
		float MoveSpeed;
		glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		
		void KeyBoardInput(float DeltaTime);
		void MouseInput(float DeltaTime);
		void ScrollInput();
	};
}


