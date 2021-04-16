#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
namespace Proof {
	class Proof_API VertexBuffer{
	public:
		VertexBuffer(uint16_t Size = 1);
		~VertexBuffer();
		void BindVertexBuffer();
		void AddVertexBufferData(void* Data, unsigned int Size);

	private:
		unsigned int VertexBufferObject;
		uint16_t VertexBufferSize;
	};

	class Proof_API IndexBuffer {
	public:
		IndexBuffer(uint16_t Size = 1);
		~IndexBuffer();
		void AddIndexBufferData(void *Data,unsigned int Size);
		void BindIndexBuffer();
	private:
		unsigned int IndexBufferObject;
		uint16_t IndexBufferSize;
	};
}