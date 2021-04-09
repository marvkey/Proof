#pragma once
#include<GL/glew.h>

namespace Proof {
	class Proof_API VertexArray{
	public:
		VertexArray(int Size =1);
		void BindVertexArray();
		void AddAtributePointer(int Position, int Size, int SizeOfOneVertex,int Offset);
	private:
		unsigned int VertexArrayObject;
	};
}

