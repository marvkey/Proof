#pragma once
namespace Proof {
	class Proof_API VertexArray {
	public:
		/**
		* create a vertex array
		* @param Size, the size of the vertex array
		*/
		static Count<VertexArray> Create(uint32_t Size = 1);
		/**
		* Delete the vertex array
		*/
		virtual ~VertexArray() = default;
		/**
		* set as the current Vertex Array
		*/
		virtual void Bind()=0;
		/**
		* sets the data of the vertex array
		* @param Positon, the position we want to send to the shader
		* @param the size of component, E.G. Vector will be 3, Float will be 1
		* @param the size of one full vertex
		* @param offset of the first component
		*/
		virtual void AddData(uint32_t Position,uint32_t Count,uint32_t SizeofVertex,const void* Offset) = 0;
		/**
		* remove as the current Vertex Array
		*/
		virtual void UnBind() = 0;
		/**
		* attaches a index buffer for drawing
		* @param indexBuffer, the created index buffer to attach
		*/
		virtual void AttachIndexBuffer(Count<IndexBuffer>& indexBuffer)=0;
		/**
		* returns the index buffer
		*/ 
		virtual Count<IndexBuffer>GetIndexBuffer() = 0;

		/*
		* tells the currently bind buffer how to read data when using instance rendering
		* @param Index, the index we want to modify
		* @param Divisor, do we want to read data every new isntance rendered which will be set to one or every 2 instnaces whihc will be set to 2 and so on
		*/
		virtual void AttributeDivisor(uint32_t Index,uint32_t Divisor) =0;
	};
}