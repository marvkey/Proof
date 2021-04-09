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
		float MoveSpeed = 2.5f;
		float RotationSpeed = 3.5f;
		float PanSpeed = 2.0f;
	};
}


