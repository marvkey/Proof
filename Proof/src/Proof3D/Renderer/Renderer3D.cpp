#include "Proofprch.h"
#include "Renderer3D.h"
#include "Platform/OpenGL/Shader.h"
namespace Proof {
	Renderer3D::Renderer3D(){
		Shader3D = std::make_unique<Shader>("Proof/Core/vertex.vs", "Proof/Core/fragment.fs1");
	}
	void Renderer3D::OnWindowResize(unsigned int Width, unsigned int Height) {
	}
	void Renderer3D::BeginScene(EditorCamera3D& Camera) {
		Shader3D->SetMat4("View", Camera.GetCameraView());
	}
	void Renderer3D::EndScene() {
	}
}