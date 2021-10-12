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
 
   void OpenGLRendererAPI::DrawArrays(uint32_t Count,DrawType drawType) {
	   glDrawArrays((uint32_t)drawType,0,Count);
   }
   void OpenGLRendererAPI::DrawIndexed(const Count<VertexArray>&ArrayObject,DrawType drawType) {
	   ArrayObject->Bind();
	   glDrawElements((uint32_t)drawType,ArrayObject->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,0);
	   glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::DrawIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,DrawType drawType){
	   ArrayObject->Bind();
	   glDrawElements((uint32_t)drawType,Count,GL_UNSIGNED_INT,0);
	   glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,uint32_t AmountElement,DrawType drawType) {
	    ArrayObject->Bind();
		glDrawElementsInstanced((uint32_t)drawType,Count,GL_UNSIGNED_INT,0,AmountElement);
		glBindTexture(GL_TEXTURE_2D,0);
   }

   void OpenGLRendererAPI::DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t AmountElement,DrawType drawType) {
	   ArrayObject->Bind();
	   ArrayObject->GetIndexBuffer()->Bind();
	   glDrawElementsInstanced((uint32_t)drawType,ArrayObject->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,0,AmountElement);
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