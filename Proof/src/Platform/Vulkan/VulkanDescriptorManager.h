#pragma once
#include "Proof/Core/Core.h"
#include "Vulkan.h"
#include "VulkanShader.h"
#include "VulkanResourceBuffer.h"
#include "VulkanTexutre.h"

#include <map>

namespace Proof
{

	enum class RenderPassResourceType {
		None =0,
		UniformBuffer,
		StorageBuffer,
		UniformBufferSet,
		StorageBufferSet,
		Texture2D,
		TextureCube,
		Image2D,
		Image2DSet,
		Texture2DSet,
		ImageView,
		ImageViewSet,
		Image,		//temporyarr 
		ImageSet	//temporyarr 
	};
	/*
	enum class RenderPassInputType {
		None =0,
		UniformBuffer,
		Storagebuffer,
		UniformBufferSet,
		StorageBufferSet,
		ImageSampler1D,
		ImageSampler2D,
		ImageSampler3D,//(CUBE)
		StorageSampler1D,
		StorageSampler2D,
		StorageSampler3D
	};
	*/
	
	struct RenderPassInput
	{
		RenderPassResourceType Type = RenderPassResourceType::None;
		std::vector<Count<RefCounted>> Input;
		RenderPassInput() = default;
		RenderPassInput(Count<class UniformBuffer> resource):
			Type(RenderPassResourceType::UniformBuffer), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(Count<class UniformBufferSet> resource) :
			Type(RenderPassResourceType::UniformBufferSet), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(Count<class StorageBuffer> resource) :
			Type(RenderPassResourceType::StorageBuffer), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(Count<class StorageBufferSet> resource) :
			Type(RenderPassResourceType::StorageBufferSet), Input(std::vector<Count<RefCounted>>(1, resource))
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
		RenderPassInput(Count<class Image2D> resource) :
			Type(RenderPassResourceType::Image2D), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(const std::vector<Count<class Image2D>>& images) :
			Type(RenderPassResourceType::Image2DSet)
		{
			Input.reserve(images.size());

			for (auto& data : images)
			{
				Input.push_back(data);
			}
		}

		RenderPassInput(Count<class ImageView> resource) :
			Type(RenderPassResourceType::ImageView), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(const std::vector<Count<class ImageView>>& images) :
			Type(RenderPassResourceType::ImageViewSet)
		{
			Input.reserve(images.size());

			for (auto& data : images)
			{
				Input.push_back(data);
			}
		}
		RenderPassInput(Count<class Image> resource) :
			Type(RenderPassResourceType::Image), Input(std::vector<Count<RefCounted>>(1, resource))
		{

		}
		RenderPassInput(const std::vector<Count<class Image>>& images) :
			Type(RenderPassResourceType::ImageSet)
		{
			Input.reserve(images.size());

			for (auto& data : images)
			{
				Input.push_back(data);
			}
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
		void SetInput(std::string_view name, Count<class UniformBufferSet> buffer);
		void SetInput(std::string_view name, Count<class StorageBufferSet> buffer);
		void SetInput(std::string_view name, Count<class TextureCube> buffer);
		void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		void SetInput(std::string_view name, Count<class Texture2D> iamge);

		void SetInput(std::string_view name, Count<class ImageView> imageView);
		void SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews);

		void SetInput(std::string_view name, Count<class Image2D>image);
		void SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images );

		void SetInput(std::string_view name, Count<class Image> imageView);
		void SetInput(std::string_view name, const std::vector< Count<class Image>>& imageViews);

		void SetGoalballInputs(Count< GlobalBufferSet> uniformData);
		void Build();
		// does not clear the m_Inputs
		void Release();

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

		uint32_t m_ShaderReloadCallbackIndex;

		std::vector<std::map<uint32_t, DescriptorResource>>m_DescriptorSets;

		//set,binding,
		std::map<uint32_t, std::map<uint32_t, RenderPassInput>> m_Inputs ;
		VkDescriptorPool m_DescriptorPool;

		bool m_Build = false;
		//so we dont have to have every input in shader be bind if not needed 
		//(set, number of valid inputs)
		std::map<uint32_t,uint32_t> m_SizeInputsData ;
		// hasnt bind to a frame yet
		uint32_t m_LastFrameBinned = -1;

		std::vector<Count< GlobalBufferSet>> m_GlobalSets;

		void SetGlobalInput(Count<GlobalBufferSet> set);
	};
}
