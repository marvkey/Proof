#pragma once
#include "Proof/Renderer/Camera/EditorCamera.h"
namespace Proof {
	class Proof_API Renderer3D {
	public:
		static void OnWindowResize(unsigned int Width, unsigned int Height);
		static void BeginScene(EditorCamera3D& Camera);
		static void EndScene();
	};

}

