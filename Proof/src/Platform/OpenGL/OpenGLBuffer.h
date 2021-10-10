#pragma once
#include "Proof/Renderer/Buffer.h"
namespace Proof {
	class Proof_API OpenGLVertexBuffer: public VertexBuffer {
	public:
		/** 
		* creates static vertex Buffer
		* @param Data, the data to be supplied to vertex buffer
		* @parm Size the size of the vertex Buffer
		*/
		OpenGLVertexBuffer(const void* Data,uint32_t Size);
		/**
		* creates Dynamic vertex Buffer
		* @parm Size the size of the vertex Buffer
		*/
		OpenGLVertexBuffer(uint32_t Size);

		/**
		* deletes the vertex buffer
		*/ 
		virtual ~OpenGLVertexBuffer()override;
		/**
		* sets as current vertex Buffer
		*/
		virtual void Bind()override;
		/**
		* sets the data for the vertex buffer
		* @param Data the location of the first element in the array or vector
		* @parm Size the size of element in vertex buffer 
		* @parm SizeOfVertexBuffer the size of the vertex buffer at given time
		*/
		virtual void AddData(const void* Data,uint32_t Size,uint32_t SizeOfVertexBuffer =0)override;
		/**
		*removes as Current vertex Buffer
		*/ 
		virtual void UnBind()override;
	private:
		uint32_t m_ID;
	};

	class Proof_API OpenGLIndexBuffer: public IndexBuffer {
	public:
		/**
		* creats Index Buffer
		* @parm Data, dat to be sent
		* @param Count in unsigned int,the vertecices to be drawn by index buffer
		*/
		OpenGLIndexBuffer(const void* Data,uint32_t Count);
		/**
		* deletes the Index Buffer
		*/ 
		virtual ~OpenGLIndexBuffer() override;
		
		/**
		*set as Current Index Buffer
		*/
		virtual void Bind();
		/**
		*removes as Current Index Buffer
		*/
		void UnBind();
		/*
		* returns the count of the IndexBuffer
		*/
		virtual uint32_t GetCount() {return m_Count;}
	private:
		uint32_t m_ID;
		uint32_t m_Count;
	};

	class Proof_API OpenGLRenderBuffer:public RenderBuffer
	{
	public:
		OpenGLRenderBuffer(RenderBufferAttachment type,uint32_t widht,uint32_t height);
		//OpenGLRenderBuffer(RenderBufferAttachment type,uint32t_t samples,uint32_t widht,uint32_t height);
		/**
		* set as the current RenderBuffer
		*/
		virtual void Bind();
		/**
		* unset as the current renderBuffer
		*/
		virtual void UnBind();

		/*
		* get the unique id of the render buffer
		*/
		virtual uint32_t GetID();

		void Remap(uint32_t width,uint32_t height,RenderBufferAttachment type);
	private:
		uint32_t m_ID;
		uint32_t m_Widht;
		uint32_t m_Height;
	};
}