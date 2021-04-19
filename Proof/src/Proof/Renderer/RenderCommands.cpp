#include "Proofprch.h"
#include "RenderCommands.h"

namespace Proof {
	void RenderCommands::PollEvents(){
		glfwPollEvents();
	}
	void RenderCommands::ClearBuffer(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void RenderCommands::SetColor(glm::vec4 Color){
		glClearColor(Color.r, Color.g, Color.b, Color.a);
	}
	void RenderCommands::SetColor(float R, float G, float B, float A){
		glClearColor(R, G, B, A);
	}
	void RenderCommands::SwapBuffer(GLFWwindow* Window){
		glfwSwapBuffers(Window);
	}
}