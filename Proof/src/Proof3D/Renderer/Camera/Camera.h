#pragma once
#include "glm/glm.hpp"
namespace Proof {
	class Proof_API Camera {
	public:
		virtual void OnUpdate(FrameTime DeltaTime);
		virtual glm::mat4 GetCameraView()const {
			return glm::mat4(1.0f);
		}
	protected:
		virtual void BeginPlay();
		float MoveSpeed = 2.5f;
		float RotationSpeed = 3.5f;
		float PanSpeed = 2.0f;
	};
}


