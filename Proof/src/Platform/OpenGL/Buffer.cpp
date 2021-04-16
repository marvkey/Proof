#include "Proofprch.h"
#include "Buffer.h"

namespace Proof {
	VertexBuffer::VertexBuffer(uint16_t Size) :
		VertexBufferSize(Size)
	{
		glGenBuffers(VertexBufferSize, &VertexBufferObject);
	}

	VertexBuffer::~VertexBuffer(){
		glDeleteBuffers(VertexBufferSize, &VertexBufferObject);
	}

	void VertexBuffer::BindVertexBuffer(){
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	}
	void VertexBuffer::AddVertexBufferData(void* Data, unsigned int Size){
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(uint16_t Size):
		IndexBufferSize(Size)
	{
		glGenBuffers(IndexBufferSize, &IndexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, IndexBufferObject);
	}
	IndexBuffer::~IndexBuffer(){
		glDeleteBuffers(IndexBufferSize, &IndexBufferObject);
	}
	void IndexBuffer::AddIndexBufferData(void* Data,unsigned int Size){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,Size, Data, GL_STATIC_DRAW);
	}
	void IndexBuffer::BindIndexBuffer(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBufferObject);
	}
}