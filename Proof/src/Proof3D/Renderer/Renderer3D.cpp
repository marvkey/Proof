#include "Proofprch.h"
#include "Renderer3D.h"
#include "Platform/OpenGL/Shader.h"
namespace Proof {
	Renderer3D::Renderer3D(){
	}
	void Renderer3D::OnWindowResize(unsigned int Width, unsigned int Height) {
	}
	void Renderer3D::BeginScene(EditorCamera3D& Camera, Shader& Render_Shader, glm::mat4& Projection){
		Projection = glm::perspective(glm::radians(Camera.GetFieldOfView()), (float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(), 0.1f, 100.0f);
		Render_Shader.SetMat4("Projection", Projection);
	}
	void Renderer3D::BeginScene(EditorCamera3D& Camera) {
	}
	void Renderer3D::EndScene() {
	}
}