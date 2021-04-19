#pragma once
#include<GL/glew.h>

namespace Proof {
	class Proof_API VertexArray{
	public:
		VertexArray(uint16_t Size =1);
		~VertexArray();
		void BindVertexArray();
		void AddAtributePointer(int Position, int Count, int SizeOfOneVertex,int Offset);
		void UnBind(); // Not Nedable gets called by destroctor;
	private:
		unsigned int VertexArrayObject;
		uint16_t VertexArraySize;
	};
}

