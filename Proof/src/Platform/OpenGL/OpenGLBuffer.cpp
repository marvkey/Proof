#include "Proofprch.h"
#include "OpenGLBuffer.h"

namespace Proof {
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint16_t Size) :
		VertexBufferSize(Size)
	{
		glGenBuffers(VertexBufferSize, &VertexBufferObject);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer(){
		glDeleteBuffers(VertexBufferSize, &VertexBufferObject);
	}

	void OpenGLVertexBuffer::BindVertexBuffer(){
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	}
	void OpenGLVertexBuffer::AddVertexBufferData(void* Data, unsigned int Size){
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::UnBind(){
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint16_t Size):
		IndexBufferSize(Size)
	{
		glGenBuffers(IndexBufferSize, &IndexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, IndexBufferObject);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer(){
		glDeleteBuffers(IndexBufferSize, &IndexBufferObject);
	}
	void OpenGLIndexBuffer::AddIndexBufferData(void* Data,unsigned int Size){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,Size, Data, GL_STATIC_DRAW);
	}
	void OpenGLIndexBuffer::BindIndexBuffer(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBufferObject);
	}
	void OpenGLIndexBuffer::UnBind(){
		glDeleteBuffers(IndexBufferSize, &IndexBufferObject);
	}
}