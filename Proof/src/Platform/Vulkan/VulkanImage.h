#pragma once
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Image.h"

namespace Proof
{
	

	/*
	class VulkanTexture2D : public Texture2D{
		VulkanTexture2D(const TextureConfiguration& config, const std::filesystem::path& path);
	private:
		TextureConfiguration m_Config;
		std::filesystem::path m_Path;
	};
	*/
	struct VulkanImageInfo {
		VulkanImageAlloc ImageAlloc;
		VkImageView ImageView = nullptr;
		VkSampler Sampler = nullptr;
	};

	class VulkanImage2D : public Image2D
	{
	public:
		VulkanImage2D(const ImageConfiguration& imageSpec);

		//TOdo remove this, sttrictly for the swapchain
		VulkanImage2D(const ImageConfiguration& imageSpec, VulkanImageInfo imageinfo, uint64_t samplerHash);
		virtual ~VulkanImage2D();

		virtual void Resize(uint32_t width, uint32_t height);
		virtual void Resize(Vector2U size) { Resize(size.X, size.Y); };

		virtual Vector2U GetSize()
		{
			return { m_Specification.Width, m_Specification.Height };
		}
		float GetAspectRatio() { return (float)GetWidth() / (float)GetHeight(); };

		const ImageConfiguration& GetSpecification()const { return m_Specification; };
		ImageConfiguration& GetSpecificationRef(){ return m_Specification; };
		virtual uint32_t GetWidth() { return m_Specification.Width; };
		virtual uint32_t GetHeight() { return m_Specification.Height; };

		// subnit to render thread when use
		void CopyToHost(Buffer& data);

		const Buffer& GetBuffer()const { return m_ImageData; }

		// do on the render thread
		Count<ImageView> GetImageView(Count<Image2D> image,uint32_t layer=0, uint32_t mip=0);

		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return *(VkDescriptorImageInfo*)GetResourceDescriptorInfo(); };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const { return (ResourceDescriptorInfo)&m_DescriptorImageInfo; }
		void UpdateDescriptor();

		const VulkanImageInfo& Getinfo()const
		{
			return m_Info;
		}
		const VulkanImageInfo& Getinfo()
		{
			return m_Info;
		}
		VulkanImageInfo& GetinfoRef()
		{
			return m_Info;
		}
		//virtual uint64_t GetHash() const override{ return (uint64_t)m_Info.ImageAlloc.Image; }
		void Release();
		void SetSamplerHash(uint64_t hash)
		{		
			m_SamplerHash = hash;
		}

		uint32_t GetSamplerHash() { return m_SamplerHash; }
		//virtual RawImage GetRawImage() { return m_Info.ImageView; };
		void Build();

	private:
		ImageConfiguration m_Specification;
		Buffer m_ImageData;
		VulkanImageInfo m_Info;
		VkDeviceSize m_GPUAllocationSize;
		std::map<uint32_t, std::map<uint32_t, Count<ImageView>>> m_ImageViews;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
		uint64_t m_SamplerHash;
		bool m_SwapchainImage = false;

	};

	class VulkanImageView : public ImageView {
	public:
		VulkanImageView(const ImageViewConfiguration& spec);
	private:
		void Init();
		void Release();
		void UpdataDescriptor();
		ImageViewConfiguration m_Specification;
		VkImageView m_ImageView;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
		friend class VulkanImage2D;
	};
}
