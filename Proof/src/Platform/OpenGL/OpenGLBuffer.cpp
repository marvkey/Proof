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

   OpenGLRenderBuffer::OpenGLRenderBuffer(RenderBufferAttachment type,uint32_t widht,uint32_t height)
	   :m_Widht(widht),m_Height(height)
   {
	   glGenRenderbuffers(1,&m_ID);
	   glBindRenderbuffer(GL_RENDERBUFFER,m_ID);
	   glRenderbufferStorage(GL_RENDERBUFFER,(GLenum)type,widht,height);
   }

   void OpenGLRenderBuffer::Bind() {
	   glBindRenderbuffer(GL_RENDERBUFFER,m_ID);
   }

   void OpenGLRenderBuffer::UnBind() {
	   glBindRenderbuffer(GL_RENDERBUFFER,0);
   }

   uint32_t OpenGLRenderBuffer::GetID() {
	   return m_ID;
   }

   void OpenGLRenderBuffer::Remap(uint32_t width,uint32_t height,RenderBufferAttachment type) {
	   m_Widht=width;
	   m_Height=height;
	   glBindRenderbuffer(GL_RENDERBUFFER,m_ID);
	   glRenderbufferStorage(GL_RENDERBUFFER,(GLenum)type,width,height);
   }

}