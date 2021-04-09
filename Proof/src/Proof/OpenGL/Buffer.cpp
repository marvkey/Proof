#include "Proofprch.h"
#include "Buffer.h"

namespace Proof {
	VertexBuffer::VertexBuffer(int Size) {
		glGenBuffers(Size, &VertexBufferObject);
	}

	void VertexBuffer::BindVertexBuffer(){
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	}
	void VertexBuffer::AddVertexBufferData(void* Data, unsigned int Size){
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(int Size){
		glGenBuffers(Size, &IndexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, IndexBufferObject);
	}
	void IndexBuffer::AddIndexBufferData(void* Data,unsigned int Size){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,Size, Data, GL_STATIC_DRAW);
	}
	void IndexBuffer::BindIndexBuffer(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBufferObject);
	}
}