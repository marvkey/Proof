#pragma once
#include "Proof/Renderer/VertexArray.h"
namespace Proof {
	class Proof_API OpenGLVertexArray:public VertexArray {
	public:
		/**
		* create a vertex array
		* @param Size, the size of the vertex array
		*/
		OpenGLVertexArray(uint32_t Size = 1);
		 /**
		 * Delete the vertex array
		 */
		virtual ~OpenGLVertexArray();
		/**
		* set as the current Vertex Array
		*/
		virtual void Bind();
		/**
		* sets the data of the vertex array
		* @param Positon, the position we want to send to the shader
		* @param the size of component, E.G. Vector will be 3, Float will be 1
		* @param the size of one full vertex
		* @param offset of the first component
		*/
		virtual void AddData(uint32_t Position,uint32_t Count,uint32_t SizeofVertex, size_t Offset);
		/**
		* remove as the current Vertex Array
		*/
		virtual void UnBind();
		/**
		* attaches a index buffer for drawing
		* @param indexBuffer, the created index buffer to attach
		*/
		virtual void AttachIndexBuffer(Count<IndexBuffer>& indexBuffer);
		/**
		* returns the index buffer
		*/
		virtual Count<IndexBuffer>GetIndexBuffer() { return m_IndexBuffer; };
		/*
		* tells the currently bind buffer how to read data when using instance rendering
		* @param Index, the index we want to modify
		* @param Divisor, do we want to read data every new isntance rendered which will be set to one or every 2 instnaces whihc will be set to 2 and so on
		*/
		virtual void AttributeDivisor(uint32_t Index,uint32_t Divisor);

	private:
		uint32_t m_ID;
		uint32_t m_Size;
		Count<IndexBuffer>m_IndexBuffer;
		bool m_IndexBufferAttach = false;
	};
}

