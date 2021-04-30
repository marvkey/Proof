#pragma once
#include "Proof/Core/Core.h"
namespace Proof {
	class Proof_API VertexArray {
		public:
		virtual void BindVertexArray()=0;
		virtual void AddAtributePointerInt(int Position, int Count, int SizeOfOneVertex, int Offset)= 0; 
		virtual void AddAtributePointervoid(int Position, int Count, int SizeOfOneVertex, const void* Offset) = 0;
	};
}