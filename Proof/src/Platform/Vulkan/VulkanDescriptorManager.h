#pragma once
#include "Proof/Core/Core.h"
#include "Vulkan.h"
#include "VulkanShader.h"
#include "VulkanResourceBuffer.h"
#include "VulkanTexutre.h"
namespace Proof
{

	enum class RenderPassResourceType {
		None =0,
		UniformBuffer,
		StorageBuffer,
		Texture2D,
		TextureCube,
		Image2D,
		Texture2DSet,
	};

	enum class RenderPassInputType {
		None =0,
		UniformBuffer,
		Storagebuffer,
		ImageSampler1D,
		ImageSampler2D,
		ImageSampler3D,//(CUBE)
		StorageSampler1D,
		StorageSampler2D,
		StorageSampler3D
	};
	
	struct RenderPassInput
	{
		RenderPassResourceType Type = RenderPassResourceType::None;
		std::vector<Count<RendererResource>> Input;
		RenderPassInput() = default;
		RenderPassInput(Count<class UniformBuffer> resource):
			Type(RenderPassResourceType::UniformBuffer), Input(std::vector<Count<RendererResource>>(1, resource))
		{

		}
		RenderPassInput(Count<class StorageBuffer> resource) :
			Type(RenderPassResourceType::StorageBuffer), Input(std::vector<Count<RendererResource>>(1, resource))
		{

		}
		RenderPassInput(Count<class Texture2D> resource) :
			Type(RenderPassResourceType::Texture2D), Input(std::vector<Count<RendererResource>>(1, resource))
		{

		}

		RenderPassInput(Count<class TextureCube> resource) :
			Type(RenderPassResourceType::TextureCube), Input(std::vector<Count<RendererResource>>(1, resource))
		{

		}

		RenderPassInput(const std::initializer_list<Count<class Texture2D>>& images ) :
			Type(RenderPassResourceType::Texture2DSet)
		{
			Input.reserve(images.size());

			for (auto& data : images)
			{
				Input.push_back(data);
			}
		}

		RenderPassInput(const std::vector<Count<class Texture2D>>& images) :
			Type(RenderPassResourceType::Texture2DSet)
		{
			Input.reserve(images.size());

			for (auto& data : images)
			{
				Input.push_back(data);
			}
		}
		//RenderPassInput(Count<class Vulk> resource) :
		//	Type(RenderPassResourceType::UniformBuffer), Input(std::vector<Count<VulkanResource>>(1, resource))
		//{
		//
		//}
		/*
		void Set(Count< VulkanUniformBuffer> uniformBuffer, uint32_t index = 0)
		{
			Type = RenderPassResourceType::UniformBuffer;
			/Input = std::vector<Count<VulkanResource>>(1, uniformBuffer);
		}
		void Set(Count< VulkanStorageBuffer> buffer, uint32_t index = 0)
		{
			Type = RenderPassResourceType::StorageBuffer;
			Input = std::vector<Count<VulkanResource>>(1, buffer);
		}
		void Set(Count< VulkanTexture2D> texture, uint32_t index = 0)
		{
			Type = RenderPassResourceType::Texture2D;
			Input = std::vector<Count<VulkanResource>>(1, texture);
		}
		*/
	};
	struct VulkanDescriptorManagerConfig
	{
		std::string DebugName;
		Count<class VulkanShader> Shader;
	};


	class VulkanDescriptorManager
	{
	public:
		VulkanDescriptorManager(const VulkanDescriptorManagerConfig& config);
		void SetInput(std::string_view name, Count<class UniformBuffer> buffer);
		void SetInput(std::string_view name, Count<class StorageBuffer> buffer);
		void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		void SetInput(std::string_view name, Count<class Texture2D> iamge);

		void Bind();
		//void SetInput(std::string_view name, Count<class > buffer);
		struct DescriptorResource {
			VkDescriptorSet Set;
			VkDescriptorSetLayout Layout;
		};
		const std::vector<std::map<uint32_t, DescriptorResource>>& GetDescriptorSets()const { return m_DescriptorSets; }

		//bool IsBuild() { return m_Build; }
	private:
		VulkanDescriptorManagerConfig m_Config;
		// frames in flight, descriptor set(set number), 
		std::vector<std::map<uint32_t, std::map<uint32_t, VkWriteDescriptorSet>>> m_WriteDescriptorMap;
		//std::vector<std::map<uint32_t, std::vector<VkDescriptorSet>>>m_DescriptorSets;

		
		std::vector<std::map<uint32_t, DescriptorResource>>m_DescriptorSets;

		//set,binding,
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, RenderPassInput>> m_Inputs ;
		VkDescriptorPool m_DescriptorPool;

		void Init();
		bool m_Build = false;
	};
}
