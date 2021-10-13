#include "Proofprch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace Proof {
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;

	void RendererCommand::SetClearColor(glm::vec4 Color){
		s_RendererAPI->SetClearColor(Color);
	}
	void RendererCommand::SetClearColor(float R, float G, float B, float A){
		s_RendererAPI->SetClearColor(R, G, B, A);
	}
	
	void RendererCommand::SetViewPort(uint32_t width,uint32_t height) {
		s_RendererAPI->SetViewPort(width,height);
	}
}