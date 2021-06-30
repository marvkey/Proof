#include "Proofprch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
namespace Proof {
   void OpenGLRendererAPI::PollEvents() {
	  glfwPollEvents();
   }
   void OpenGLRendererAPI::Clear() {
	   if (IsDepthEnabled == true)
		   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   else
		   glClear(GL_COLOR_BUFFER_BIT);
   }
   void OpenGLRendererAPI::DrawIndexed(const Count<VertexArray>&ArrayObject) {
	   PF_CORE_ASSERT(ArrayObject->GetIndexBuffer(),"No Index Buffer attached to VertexAray")
	   ArrayObject->Bind();
	   glDrawElements(GL_TRIANGLES,ArrayObject->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,0);
	   glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::DrawIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count){ /* COuld Be Removed */
	   ArrayObject->Bind();
	   glDrawElements(GL_TRIANGLES,Count,GL_UNSIGNED_INT,0);
	   glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,uint32_t AmountElement) {
	    ArrayObject->Bind();
		glDrawElementsInstanced(GL_TRIANGLES,Count,GL_UNSIGNED_INT,0,AmountElement);
		glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t AmountElement) {
	   ArrayObject->Bind();
	   ArrayObject->GetIndexBuffer()->Bind();
	   glDrawElementsInstanced(GL_TRIANGLES,ArrayObject->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,0,AmountElement);
	   glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::SetViewPort(int num,int num2,int Width,int Height) {
	   glViewport(0,0,Width,Height);
   }
   void OpenGLRendererAPI::SetClearColor(const glm::vec4& Color) {
	  glClearColor(Color.r,Color.g,Color.b,Color.a);
   }
   void OpenGLRendererAPI::SetClearColor(float R,float G,float B,float A) {
	  glClearColor(R,G,B,A);
   }
   void OpenGLRendererAPI::SwapBuffer(GLFWwindow* Window) {
	  glfwSwapBuffers(Window);
   }
   void OpenGLRendererAPI::EnableDepth(bool Depth) {
	   if (Depth == true) {
		   IsDepthEnabled = true;
		   glEnable(GL_DEPTH_TEST);
	   }
	   else {
		   glDisable(GL_DEPTH_TEST);
			IsDepthEnabled = false;
	   }
   }
}