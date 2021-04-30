#pragma once
#include "Proof3D/Renderer/Camera/EditorCamera.h"
namespace Proof {
	class Proof_API Renderer3D {
	public:
		Renderer3D();
		static void OnWindowResize(unsigned int Width, unsigned int Height);
		static void BeginScene(EditorCamera3D& Camera,Shader& Render_Shader,glm::mat4& Projection);
		static void BeginScene(EditorCamera3D& Camera);
		static void EndScene();
	private:
	};

}

