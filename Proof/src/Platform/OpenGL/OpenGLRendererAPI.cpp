#include "Proofprch.h"
#include "OpenGLRendererAPI.h"

namespace Proof {
   void OpenGLRendererAPI::PollEvents() {
	  glfwPollEvents();
   }
   void OpenGLRendererAPI::Clear() {
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }
   void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray> VertexArray) {
   }
   void OpenGLRendererAPI::SetClearColor(glm::vec4 Color) {
	  glClearColor(Color.r,Color.g,Color.b,Color.a);
   }
   void OpenGLRendererAPI::SetClearColor(float R,float G,float B,float A) {
	  glClearColor(R,G,B,A);
   }
   void OpenGLRendererAPI::SwapBuffer(GLFWwindow* Window) {
	  glfwSwapBuffers(Window);
   }
   void OpenGLRendererAPI::EnableDepth() {
	  glEnable(GL_DEPTH_TEST);
   }
}