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
	};


	struct DescriptrLayoutBinding {
		uint32_t binding;
		DescriptorType descriptorType;
		uint32_t descriptorCount;
		ShaderStage shaderStage ;
	};
	class DescriptorSet {
	public:
		virtual ~DescriptorSet()=default;
		struct Builder {
			Builder(DescriptorSets set);
			Builder& AddBinding(uint32_t binding, DescriptorType descriptorType, ShaderStage shaderStage, uint32_t count = 1);

			Count<DescriptorSet> Build();
		private:
			std::unordered_map<uint32_t, DescriptrLayoutBinding> Bindings{};
			DescriptorSets m_Set;
		};

		virtual DescriptorSet& WriteBuffer(uint32_t binding, Count<UniformBuffer> buffer) = 0;
		virtual DescriptorSet& WriteBuffer(uint32_t binding, Count<StorageBuffer> buffer) = 0;
		virtual DescriptorSet& WriteImage(uint32_t binding, Count<class TextureCube> image) = 0;
		virtual DescriptorSet& WriteImage(uint32_t binding, Count<class Texture2D> image) = 0;
		virtual DescriptorSet& WriteImage(uint32_t binding, std::vector<Count<class Texture2D>>& image) = 0;
		virtual void Bind(Count<class RenderCommandBuffer> commandBuffer, Count<class PipeLineLayout>piipeLineLayout) =0;
		static Count<DescriptorSet>Create(DescriptorSets set, std::unordered_map<uint32_t, DescriptrLayoutBinding> Bindings);
	};
}
