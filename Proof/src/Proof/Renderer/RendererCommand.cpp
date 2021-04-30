#include "Proofprch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace Proof {
	RendererAPI* RendererCommand::_RendererAPI = new OpenGLRendererAPI;
	void RendererCommand::PollEvents(){
		_RendererAPI->PollEvents();
	}
	void RendererCommand::Clear(){
		_RendererAPI->Clear();
	}
	void RendererCommand::SetClearColor(glm::vec4 Color){
		_RendererAPI->SetClearColor(Color);
	}
	void RendererCommand::SetClearColor(float R, float G, float B, float A){
		_RendererAPI->SetClearColor(R, G, B, A);
	}
	void RendererCommand::SwapBuffer(GLFWwindow* Window){
		_RendererAPI->SwapBuffer(Window);
	}
	void RendererCommand::EnableDepth(){
		_RendererAPI->EnableDepth();
	}
}