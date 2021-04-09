#include "Proofprch.h"
#include "VertexArray.h"

namespace Proof {
	VertexArray::VertexArray(int Size){
		glGenVertexArrays(Size, &VertexArrayObject);
		glBindVertexArray(VertexArrayObject);
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
