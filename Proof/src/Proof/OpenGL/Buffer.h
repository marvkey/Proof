#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
namespace Proof {
	class Proof_API VertexBuffer{
	public:
		VertexBuffer(int Size = 1);

		void BindVertexBuffer();
		void AddVertexBufferData(void* Data, unsigned int Size);

	private:
		unsigned int VertexBufferObject;
	};

	class Proof_API IndexBuffer {
	public:
		IndexBuffer(int Size = 1);
		void AddIndexBufferData(void *Data,unsigned int Size);
		void BindIndexBuffer();
	private:
		unsigned int IndexBufferObject;
	};
}