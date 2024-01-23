#pragma once
#include "Proof/Core/Core.h"
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Core/Buffer.h"
#include <vulkan/vulkan.h>
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
	struct VulkanImageInfo 
	{
		VulkanImageAlloc ImageAlloc;
		VkImageView ImageView = nullptr;
		VkSampler Sampler = nullptr;
	};
	using Image2DResizeCallback = std::function<void(Count< Image2D>image)>;
	class VulkanImage2D : public Image2D
	{
	public:
		VulkanImage2D(const ImageConfiguration& imageSpec, VkSampleCountFlagBits sampleFlags = VK_SAMPLE_COUNT_1_BIT);

		//TOdo remove this, sttrictly for the swapchain
		VulkanImage2D(const ImageConfiguration& imageSpec, VulkanImageInfo imageinfo, uint64_t samplerHash);
		virtual ~VulkanImage2D();
		virtual void AddResizeCallback(const Image2DResizeCallback& func);
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
		std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const;

		const Buffer& GetBuffer()const { return m_ImageData; }

		// do on the render thread
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
		void RT_Build();

		void CopyFromHostBuffer(Buffer& buffer);

		void CallOnResizeFunctions();

		void ClearImageViews()
		{
			m_ImageViews.clear();
		}
		Count<ImageView> CreateOrGetImageMip(uint32_t mip, uint32_t layer = 0);
		virtual void CreateMipAndLayerViews();

	private:
		std::vector<Image2DResizeCallback > m_ResizeCallbacks;

		VkSampleCountFlagBits m_SampleFlags;
		ImageConfiguration m_Specification;
		Buffer m_ImageData;
		VulkanImageInfo m_Info;
		VkDeviceSize m_GPUAllocationSize;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
		uint64_t m_SamplerHash;
		bool m_SwapchainImage = false;

		//layer, mip
		std::map<uint32_t, std::map<uint32_t, Count<ImageView>>> m_ImageViews;
	};

	class VulkanImageView : public ImageView 
	{
	public:
		VulkanImageView(const ImageViewConfiguration& spec);
		~VulkanImageView();
		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return *(VkDescriptorImageInfo*)GetResourceDescriptorInfo(); };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const { return (ResourceDescriptorInfo)&m_DescriptorImageInfo; }
		void UpdateDescriptor();
		virtual const ImageViewConfiguration& GetSpecification()const { return m_Specification; }
		virtual Count<Image2D> GetImage()const { return m_Specification.Image; };

		virtual Vector2U GetSize() { return GetImage()->GetSize(); }
		virtual float GetAspectRatio() { return GetImage()->GetAspectRatio(); }
		virtual uint32_t GetWidth() {return GetImage()->GetWidth();};
		virtual uint32_t GetHeight() { return GetImage()->GetHeight(); };
		virtual glm::uvec2 GetMipSize();

		const VulkanImageInfo& GetVulkanImageInfo() const { return m_Info; }
		const VulkanImageInfo& GetVulkanImageInfo() { return m_Info; }
		VulkanImageInfo& GetVulkanImageInfoRef() { return m_Info; }
	private:
		VulkanImageInfo m_Info;
		void Build();
		void RT_Build();
		void Release();
		ImageViewConfiguration m_Specification;
		VkImageView m_ImageView;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
		friend class VulkanImage2D;
	};
}
