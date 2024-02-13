#pragma once
#include "Proof/Core/Core.h"
#include "RendererResouce.h"
#include "Buffer.h"
#include <unordered_map>
namespace Proof
{
	class StorageBuffer : public RendererResource {
	public:
		virtual ~StorageBuffer() = default;
		static Count<StorageBuffer>Create(uint64_t size);
		static Count<StorageBuffer>Create(Buffer data);
		virtual void SetData(Buffer data, uint64_t offset = 0) = 0;

		virtual void Resize(uint64_t size) = 0;
		virtual void Resize(Buffer data) = 0;
		virtual uint64_t GetSize() = 0;
		virtual Buffer GetDataRaw() = 0;

		RENDER_VIEW_RESOURCE_CLASS_TYPE(StorageBuffer);

	};
	class UniformBuffer : public RendererResource {
	public:
		virtual ~UniformBuffer() = default;
		/*
		* Createas a uniform buffer
		* @param name= the identifier of the uniform buffer
		* @param size = the size of the uniform buffer in bytes
		* @param biningPoint = the binding point of the uniform buffer
		*/
		static Count<UniformBuffer>Create(uint64_t size);
		static Count<UniformBuffer>Create(Buffer data) ;

		/**
		* changes the data of a set data in the uniform buffer
		* @param Data the location of the variable or variables given an array
		* @param Size of element in bytes
		* @param the offset in the uniform buffer from the stating point in bytes if it is the last element just use the current size of the uniform buffer
		*/
		virtual void SetData(Buffer data,uint64_t offset = 0) = 0;

		//virtual void Resize(uint64_t size) = 0;
		//virtual void Resize(Buffer data) = 0;
		virtual uint64_t GetSize() = 0;
		RENDER_VIEW_RESOURCE_CLASS_TYPE(UniformBuffer);
	};


	class UniformBufferSet : public RefCounted
	{
	public:

		static Count<UniformBufferSet>Create(uint64_t size);
		static Count<UniformBufferSet>Create(Buffer data);
		virtual Count<UniformBuffer> GetBuffer(uint32_t index) = 0;

		//virtual void Resize(uint32_t index, uint64_t size) = 0;
		//virtual void Resize(uint32_t index, Buffer data) = 0;
		virtual void SetData(uint32_t index, Buffer data, uint64_t offset = 0) = 0;
	};

	class StorageBufferSet : public RefCounted
	{
	public:

		static Count<StorageBufferSet>Create(uint64_t size);
		static Count<StorageBufferSet>Create(Buffer data);
		virtual Count<StorageBuffer> GetBuffer(uint32_t index) = 0;

		virtual void Resize(uint32_t index, uint64_t size) = 0;
		virtual void Resize(uint32_t index, Buffer data) = 0;
		virtual void SetData(uint32_t index, Buffer data, uint64_t offset = 0) = 0;
	};


	class GlobalBufferSet : public RefCounted
	{
	public:
		void SetData(const std::string& name, Count<StorageBufferSet> set);
		void SetData(const std::string& name, Count<UniformBufferSet> set);
		void SetData(const std::string& name, Count<StorageBuffer> set);
		void SetData(const std::string& name, Count<UniformBuffer> set);
		void SetData(const std::string& name, Count<class Image> set);
		void SetData(const std::string& name, Count<class Texture> set);
		void SetData(const std::string& name, Count<class RenderSampler> set);
		const std::unordered_map<std::string, std::pair<RendererResourceType, Count<RefCounted>>>& GetBuffers() { return m_Buffers; }
	private:
		std::unordered_map<std::string, std::pair<RendererResourceType, Count<RefCounted>>> m_Buffers;
	};
}
