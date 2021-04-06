#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Proof/Core/Core.h"
#include "Proof/Renderer/Camera/Camera.h"
#include "Proof/OpenGL/Shader.h"
#include "Platform/CurrentWindow.h"
namespace Proof {
	class Proof_API EditorCamera3D : public Camera3D {
	public:
		EditorCamera3D() :
			EditorCameraShader("Proof/Core/vertex.vs", "Proof/Core/fragment.fs1")
		{

		}
		virtual void OnUpdate(FrameTime DeltaTime) override;
		float GetFieldOfView() { return FieldOfView; }
	protected:
		virtual void BeginPlay()override;
		virtual void KeyBoardInput(float DeltaTime);
		virtual void MouseInput(float DeltaTime);
		virtual void ScrollInput();
	private:
		Shader EditorCameraShader;
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
	};
}


