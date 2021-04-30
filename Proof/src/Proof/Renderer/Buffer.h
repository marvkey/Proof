#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "Proof/Core/Core.h"
namespace Proof {
	class Proof_API VertexBuffer {
	public:
		virtual void BindVertexBuffer() = 0;
		virtual void AddVertexBufferData(void* Data, unsigned int Size)=0;
		virtual void UnBind() = 0;
	};

	class Proof_API IndexBuffer {
	public:
		virtual void AddIndexBufferData(void* Data, unsigned int Size) = 0;
		virtual void BindIndexBuffer()= 0;
		virtual void UnBind() = 0;
	};
}