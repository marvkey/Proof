#include "Proofprch.h"
#include "Glad/glad.h"
#include "OpenGLVertexArray.h"
namespace Proof {
   OpenGLVertexArray::OpenGLVertexArray(uint32_t Size):
	  m_Size(Size)
   {
	  glCreateVertexArrays(1,&m_ID);
   }
   OpenGLVertexArray::~OpenGLVertexArray() {
	  glDeleteVertexArrays(1,&m_ID);
   }
   void OpenGLVertexArray::Bind() {
	  glBindVertexArray(m_ID);
   }
   void OpenGLVertexArray::AddData(uint32_t Position,uint32_t Count,uint32_t SizeofVertex,const void* Offset) {
	   glBindVertexArray(m_ID);
	   glEnableVertexAttribArray(Position);
	   glVertexAttribPointer(Position,Count,GL_FLOAT,GL_FALSE,SizeofVertex,Offset);
   }
   void OpenGLVertexArray::UnBind() {
	   glBindVertexArray(0);
   }
   void OpenGLVertexArray::AttachIndexBuffer(Count<IndexBuffer>& indexBuffer) {
	   if (m_IndexBufferAttach == true) {
		   PF_ENGINE_WARN("Vertex Array already has a Index Buffer attached");
	   }
	   m_IndexBuffer = indexBuffer;
	   m_IndexBufferAttach = true;
   }
   void OpenGLVertexArray::AttributeDivisor(uint32_t Index,uint32_t Divisor) {
	   glBindVertexArray(m_ID);
	   glVertexAttribDivisor(Index,Divisor);
   }
}
