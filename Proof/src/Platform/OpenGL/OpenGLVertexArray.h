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
		virtual ~OpenGLVertexArray()override;
		/**
		* set as the current Vertex Array
		*/
		virtual void Bind()override;
		/**
		* sets the data of the vertex array
		* @param Positon, the position we want to send to the shader
		* @param the size of component, E.G. Vector will be 3, Float will be 1
		* @param the size of one full vertex
		* @param offset of the first component
		*/
		virtual void AddData(uint32_t Position,uint32_t Count,uint32_t SizeofVertex,const void* Offset);
		/**
		* remove as the current Vertex Array
		*/
		virtual void UnBind()override;
		/**
		* attaches a index buffer for drawing
		* @param indexBuffer, the created index buffer to attach
		*/
		virtual void AttachIndexBuffer(Count<IndexBuffer>& indexBuffer)override;
		/**
		* returns the index buffer
		*/
		virtual Count<IndexBuffer>GetIndexBuffer() { return m_IndexBuffer; };
	private:
		uint32_t m_ID;
		uint32_t m_Size;
		Count<IndexBuffer>m_IndexBuffer;
		bool m_IndexBufferAttach = false;
	};
}

