#include "Proofprch.h"
#include "OpenGLBuffer.h"
#include "Glad/glad.h"

namespace Proof {
   OpenGLVertexBuffer::OpenGLVertexBuffer(const void* Data,uint32_t Size)
	{
	  glGenBuffers(1,&m_ID);
	  glBindBuffer(GL_ARRAY_BUFFER,m_ID);
	  glBufferData(GL_ARRAY_BUFFER,Size,Data,GL_STATIC_DRAW);
   }

   OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t Size) {
	   glGenBuffers(1,&m_ID);
	   glBindBuffer(GL_ARRAY_BUFFER,m_ID);
	   glBufferData(GL_ARRAY_BUFFER,Size,nullptr,GL_DYNAMIC_DRAW);
   }

   OpenGLVertexBuffer::~OpenGLVertexBuffer() {
	  glDeleteBuffers(1,&m_ID);
   }

   void OpenGLVertexBuffer::Bind() {
	  glBindBuffer(GL_ARRAY_BUFFER,m_ID);
   }
   void OpenGLVertexBuffer::AddData(const void* Data,uint32_t Size,uint32_t SizeOfVertexBuffer) {
	  glBindBuffer(GL_ARRAY_BUFFER,m_ID);
	  glBufferSubData(GL_ARRAY_BUFFER,SizeOfVertexBuffer,Size,Data);   
   }

   void OpenGLVertexBuffer::UnBind() {
	  glBindBuffer(GL_ARRAY_BUFFER,0);
   }

   OpenGLIndexBuffer::OpenGLIndexBuffer(const void* Data,uint32_t Count):
	  m_Count(Count)    
   {
	   // GL_ELEMENT_ARRAY_BUFFER not used in case a Vertex buffer is not set
	  glGenBuffers(1,&m_ID);
	  glBindBuffer(GL_ARRAY_BUFFER,m_ID);
	  glBufferData(GL_ARRAY_BUFFER,Count * sizeof(uint32_t),Data,GL_STATIC_DRAW);
   }
   OpenGLIndexBuffer::~OpenGLIndexBuffer() {
	  glDeleteBuffers(1,&m_ID);
   }
   void OpenGLIndexBuffer::Bind() {
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ID);
   }
   void OpenGLIndexBuffer::UnBind() {
	  glDeleteBuffers(1,&m_ID);
   }

  

}