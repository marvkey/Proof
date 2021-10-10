#include "Proofprch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
namespace Proof {
	static void tempClear(uint32_t temp){
		glClear((uint32_t)temp);
	}
   void OpenGLRendererAPI::PollEvents() {
	  glfwPollEvents();
   }
   void OpenGLRendererAPI::Clear(uint32_t bitField) {
		glClear(bitField);
   }
 
   void OpenGLRendererAPI::DrawArrays(uint32_t Count) {
	   glDrawArrays(GL_TRIANGLES,0,Count);
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
	  // glDrawElementsInstanced(GL_LINE_STRIP,ArrayObject->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,0,AmountElement); // very cool effect to view all veritces
	   glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::SetViewPort(uint32_t width,uint32_t height) {
	   glViewport(0,0,width,height);
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
   void OpenGLRendererAPI::Enable(uint32_t bitField) {
		glEnable(bitField);
   }
   void OpenGLRendererAPI::Disable(uint32_t bitField) {
	   glDisable(bitField);
   }
   void OpenGLRendererAPI::DepthFunc(DepthType type) {
	   glDepthFunc((uint32_t)type);
   }
}