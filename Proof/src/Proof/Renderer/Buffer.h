#pragma once
namespace Proof {
	class Proof_API VertexBuffer {
	public:
		/*
		* sets as current vertex Buffer
		*/
		virtual void Bind() = 0;
		/**
		* sets the data for the vertex buffer
		* @param Data the location of the first element in the array or vector
		* @parm Size the size of element in vertex buffer
		* @parm SizeOfVertexBuffer the size of the vertex buffer at given time
		*/
		virtual void AddData(const void* Data,uint32_t Size,uint32_t SizeOfVertexBuffer=0) = 0;
		/**
		*removes as Current vertex Buffer
		*/
		virtual void UnBind() = 0;
		/**
		* creates static vertex Buffer
		* @param Data, the data to be supplied to vertex buffer
		* @parm Size the size of the vertex Buffer
		*/
		static Count<VertexBuffer>Create(const void* Data,uint32_t Size);
		/**
		* creates Dynamic vertex Buffer
		* @parm amount the size of the vertex Buffer
		*/
		static Count<VertexBuffer>Create(uint32_t Size);
		/**
		* deletes the vertex buffer
		*/
		virtual ~VertexBuffer() = default;
	};

	class Proof_API IndexBuffer {
	public:
		/**
		*set as Current Index Buffer
		*/
		virtual void Bind() = 0;
		/**
		*removes as Current Index Buffer
		*/
		virtual void UnBind() = 0;
		/**
		* creats Index Buffer
		* @parm Data, dat to be sent
		* @param Count in uint32_t,the vertecices to be drawn by index buffer
		*/
		static Count<IndexBuffer>Create(const void* Data,uint32_t Count);
		/**
		* deletes the Index Buffer
		*/
		virtual ~IndexBuffer() = default;
		/*
		* returns the count of the IndexBuffer
		*/
		virtual uint32_t GetCount() = 0;
	};

	enum class RenderBufferAttachment:uint32_t {
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthComponent16 = GL_DEPTH_COMPONENT16,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthComponent32F = GL_DEPTH_COMPONENT32F,
		Depth24Stencil8 = GL_DEPTH24_STENCIL8,
		Depth32FStencil8 = GL_DEPTH32F_STENCIL8,
		DepthStencil8 = GL_STENCIL_INDEX8
	};
	class Proof_API RenderBuffer
	{
	public:
		/**
		* set as the current RenderBuffer
		*/
		virtual void Bind() =0;
		/**
		* unset as the current renderBuffer
		*/
		virtual void UnBind()=0;

		/*
		* get the unique id of the render buffer
		*/
		virtual uint32_t GetID()=0;

		virtual void Remap(uint32_t width,uint32_t height,RenderBufferAttachment type)=0;
		static Count<RenderBuffer>Create(RenderBufferAttachment type,uint32_t width,uint32_t height);
		//static Count<RenderBuffer>Create(RenderBufferAttachment type,uint32_t samples,uint32_t widht,uint32_t height);

	};
}