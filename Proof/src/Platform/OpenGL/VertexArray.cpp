#include "Proofprch.h"
#include "VertexArray.h"

namespace Proof {
	VertexArray::VertexArray(uint16_t Size):
		VertexArraySize(Size)
	{
		glGenVertexArrays(VertexArraySize, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);
	}
	VertexArray::~VertexArray(){
		glDeleteVertexArrays(1, &VertexArrayObject);
	}
	void VertexArray::BindVertexArray(){
		glBindVertexArray(VertexArrayObject);
	}
	void VertexArray::AddAtributePointer(int Position, int Size, int SizeOfOneVertex,int Offset){
		if (Offset == 0) {
			glVertexAttribPointer(Position, Size, GL_FLOAT, GL_FALSE, SizeOfOneVertex * sizeof(float), (void*)(Offset * sizeof(float)));
			glEnableVertexAttribArray(Position);
		}	
		else {
			glVertexAttribPointer(Position, Size, GL_FLOAT, GL_FALSE, SizeOfOneVertex * sizeof(float), (void*)(Offset * sizeof(float)));
			glEnableVertexAttribArray(Position);
		}
	}
}
