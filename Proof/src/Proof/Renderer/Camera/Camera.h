#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/OpenGL/Shader.h"
namespace Proof {
	class Proof_API Camera3D {
	public:
		virtual void OnUpdate(FrameTime DeltaTime);
	protected:
		virtual void BeginPlay();

		glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float MoveSpeed = 2.5f;
		float RotationSpeed = 3.5f;
		float PanSpeed = 2.0f;
	};
}


