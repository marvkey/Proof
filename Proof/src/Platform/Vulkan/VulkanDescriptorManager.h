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
		std::vector<Count<RefCounted>> Input;
		RenderPassInput() = default;
		RenderPassInput(Count<class UniformBuffer> resource):
			Type(RenderPassResourceType::UniformBuffer), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(Count<class StorageBuffer> resource) :
			Type(RenderPassResourceType::StorageBuffer), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(Count<Texture2D> resource) :
			Type(RenderPassResourceType::Texture2D), Input(std::vector<Count<RefCounted>>(1, resource))
		{
		}

		RenderPassInput(Count<TextureCube> resource) :
			Type(RenderPassResourceType::TextureCube), Input(std::vector<Count<RefCounted>>(1, resource))
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
		uint32_t LastSet = 3; //(set0,set1,set2,set3)  those are 3 sets
	};


	class VulkanDescriptorManager : public RefCounted
	{
	public:
		VulkanDescriptorManager(const VulkanDescriptorManagerConfig& config);
		~VulkanDescriptorManager();
		void SetInput(std::string_view name, Count<class UniformBuffer> buffer);
		void SetInput(std::string_view name, Count<class StorageBuffer> buffer);
		void SetInput(std::string_view name, Count<class TextureCube> buffer);
		void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		void SetInput(std::string_view name, Count<class Texture2D> iamge);

		void Bind();
		//void SetInput(std::string_view name, Count<class > buffer);
		struct DescriptorResource {
			VkDescriptorSet Set  = nullptr;
			VkDescriptorSetLayout Layout = nullptr;
		};
		const std::vector<std::map<uint32_t, DescriptorResource>>& GetDescriptorSets()const { return m_DescriptorSets; }
		const std::map<uint32_t, std::map<uint32_t, RenderPassInput>>& GetInputs() { return m_Inputs; }
		//bool IsBuild() { return m_Build; }
	private:
		VulkanDescriptorManagerConfig m_Config;
		// frames in flight, descriptor set(set number), 
		std::vector<std::map<uint32_t, std::map<uint32_t, VkWriteDescriptorSet>>> m_WriteDescriptorMap;
		//std::vector<std::map<uint32_t, std::vector<VkDescriptorSet>>>m_DescriptorSets;

		
		std::vector<std::map<uint32_t, DescriptorResource>>m_DescriptorSets;

		//set,binding,
		std::map<uint32_t, std::map<uint32_t, RenderPassInput>> m_Inputs ;
		VkDescriptorPool m_DescriptorPool;

		void Init();
		bool m_Build = false;
		// hasnt bind to a frame yet
		uint32_t m_LastFrameBinned = -1;
		void Release();
	};
}
