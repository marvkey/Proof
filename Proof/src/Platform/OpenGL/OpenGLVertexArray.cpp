#include "Proofprch.h"
#include "OpenGLVertexArray.h"
namespace Proof {
	OpenGLVertexArray::OpenGLVertexArray(uint16_t Size) :
		VertexArraySize(Size)
	{
		glGenVertexArrays(VertexArraySize, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);
	}
	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(VertexArraySize, &VertexArrayObject);
	}
	void OpenGLVertexArray::BindVertexArray() {
		glBindVertexArray(VertexArrayObject);
	}
	void OpenGLVertexArray::AddAtributePointerInt(int Position, int Count, int SizeOfOneVertex, int Offset) {
		glVertexAttribPointer(Position, Count, GL_FLOAT, GL_FALSE, SizeOfOneVertex * sizeof(float), (void*)(Offset * sizeof(float)));
		glEnableVertexAttribArray(Position);
	}
	void OpenGLVertexArray::AddAtributePointervoid(int Position, int Count, int SizeOfOneVertex, const void* Offset) {
		glVertexAttribPointer(Position, Count, GL_FLOAT, GL_FALSE, SizeOfOneVertex * sizeof(float),Offset);
		glEnableVertexAttribArray(Position);
	}
	void OpenGLVertexArray::UnBind() {
		glDeleteVertexArrays(VertexArraySize, &VertexArrayObject);
	}

}
