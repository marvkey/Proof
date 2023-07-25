#pragma once
#include "Proof/Core/Core.h"
#include "RendererResouce.h"
#include "Renderer.h"
#include "Shader.h"
namespace Proof
{
	enum class DescriptorSets {
		Zero = 0,
		One,
		Two,
		Three
	};
	enum class DescriptorType{
		ImageSampler,
		UniformBuffer,
		StorageBuffer
	} ;
	class StorageBuffer : public RendererBufferResource {
	public:
		virtual ~StorageBuffer() = default;
		//static Count<StorageBuffer>Create(const void* data, uint32_t size, uint32_t offset = 0, uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
		static Count<StorageBuffer>Create(uint32_t size);
		static Count<StorageBuffer>Create(const void* data, uint32_t size);

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual void Resize(uint32_t size) = 0;
		virtual void Resize(const void* data, uint32_t size) = 0;
		virtual uint32_t GetSize() = 0;
	};
	class UniformBuffer : public RendererBufferResource {
	public:
		virtual ~UniformBuffer() = default;
		/*
		* Createas a uniform buffer
		* @param name= the identifier of the uniform buffer
		* @param size = the size of the uniform buffer in bytes
		* @param biningPoint = the binding point of the uniform buffer
		*/
		static Count<UniformBuffer>Create(uint32_t binding);
		static Count<UniformBuffer>Create(const void* data,uint32_t size) ;

		/**
		* changes the data of a set data in the uniform buffer
		* @param Data the location of the variable or variables given an array
		* @param Size of element in bytes
		* @param the offset in the uniform buffer from the stating point in bytes if it is the last element just use the current size of the uniform buffer
		*/
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		virtual void Resize(uint32_t size) = 0;
		virtual void Resize(const void* data, uint32_t size) = 0;
		virtual uint32_t GetSize() = 0;
	};
}
