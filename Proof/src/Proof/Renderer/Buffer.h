#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"
namespace Proof {
	class Proof_API VertexBuffer {
	public:
		/*
		* sets as current vertex Buffer
		*/
		virtual void Bind(Count<class RenderCommandBuffer> commandBuffer, uint32_t binding = 0)const =0;
		/**
		* sets the data for the vertex buffer
		* @param Data the location of the first element in the array or vector
		* @parm Size the size of element in vertex buffer
		*/
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
	
		/**
		* creates static vertex Buffer
		* @param Data, the data to be supplied to vertex buffer
		* @parm Size the size of the vertex Buffer
		*/
		static Count<VertexBuffer>Create(const void* Data,uint32_t Size);

		virtual void Resize(uint32_t size) = 0;
		//virtual void Resize(const void* data, uint32_t size) = 0;
		/**
		* creates Dynamic vertex Buffer
		* @parm amount the size of the vertex Buffer
		*/
		static Count<VertexBuffer>Create(uint32_t Size);
		/**
		* deletes the vertex buffer
		*/
		virtual ~VertexBuffer() = default;

		template<typename T>
		std::vector<T> GetData()
		{
			std::vector<T> data;
			Buffer buffer = GetDataRaw();
			T* vertices = reinterpret_cast<T*>(buffer.Get());

			for (size_t i = 0; i < GetVertexSize() / sizeof(T); i++)
			{
				data.emplace_back(vertices[i]);
			}
			return data;
		}

		virtual Buffer GetDataRaw() = 0;
		virtual uint32_t GetVertexSize()const = 0;
	protected:
	};

	class Proof_API IndexBuffer {
	public:
		/**
		*set as Current Index Buffer
		*/
		virtual void Bind(Count<class RenderCommandBuffer> commandBuffer)const = 0;
		/**
		*removes as Current Index Buffer
		*/
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
		virtual uint32_t GetCount()const = 0;
		virtual uint32_t GetSize()const = 0;
		virtual std::vector<uint32_t> GetData()const = 0;


	};

}