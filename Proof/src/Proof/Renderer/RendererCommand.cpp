#include "Proofprch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace Proof {
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
	void RendererCommand::PollEvents(){
		s_RendererAPI->PollEvents();
	}
	void RendererCommand::Clear(){
		s_RendererAPI->Clear();
	}
	void RendererCommand::SetClearColor(glm::vec4 Color){
		s_RendererAPI->SetClearColor(Color);
	}
	void RendererCommand::SetClearColor(float R, float G, float B, float A){
		s_RendererAPI->SetClearColor(R, G, B, A);
	}
	void RendererCommand::SwapBuffer(GLFWwindow* Window){
		s_RendererAPI->SwapBuffer(Window);
	}
	void RendererCommand::EnableDepth(bool TrueOrFalse){
		s_RendererAPI->EnableDepth(TrueOrFalse);
	}
	void RendererCommand::SetViewPort(int Width,int Height) {
		s_RendererAPI->SetViewPort(0,0,Width,Height);
	}
}