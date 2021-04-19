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
		glDeleteVertexArrays(VertexArraySize, &VertexArrayObject);
	}
	void VertexArray::BindVertexArray(){
		glBindVertexArray(VertexArrayObject);
	}
	void VertexArray::AddAtributePointer(int Position, int Count, int SizeOfOneVertex,int Offset){
		if (Offset == 0) {
			glVertexAttribPointer(Position, Count, GL_FLOAT, GL_FALSE, SizeOfOneVertex * sizeof(float), (void*)(Offset * sizeof(float)));
			glEnableVertexAttribArray(Position);
		}	
		else {
			glVertexAttribPointer(Position, Count, GL_FLOAT, GL_FALSE, SizeOfOneVertex * sizeof(float), (void*)(Offset * sizeof(float)));
			glEnableVertexAttribArray(Position);
		}
	}
	void VertexArray::UnBind(){
		glDeleteVertexArrays(VertexArraySize, &VertexArrayObject);
	}
}
