#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Renderer/Texture.h"
namespace Proof
{
	
	class VulkanFrameBuffer : public FrameBuffer {
	public:
		VulkanFrameBuffer(const FrameBufferConfig& attachments);
		virtual ~VulkanFrameBuffer();
		VkFramebuffer GetFrameBuffer();
		const FrameBufferConfig& GetConfig() const  
		{
			return m_Config;
		}
		const FrameBufferConfig& GetConfig() 
		{
			return m_Config;
		}
		virtual Count<Image> GetOutput(uint32_t imageIndex);
		virtual Count<Image> GetDepthOutput();

		virtual bool HasDepthImage();
		virtual void Resize(uint32_t width, uint32_t height);
		virtual void Resize(const glm::uvec2 size) { Resize(size.x, size.y); }

		virtual void Copy(Count<FrameBuffer> framebUFFer);

		VkRenderPass GetRenderPass() { return m_CompatibilityRenderPass; }
		ImageFormat GetDepthFormat() { return m_DepthFormat; }

		virtual uint32_t GetWidth() const { return m_Config.Width; }
		virtual uint32_t GetHeight() const {return m_Config.Height;}
	private:

		void RT_Build();
		void Build();
		void SetUpAttachments();

		void SetDepth(const FrameBufferImageConfig& imageAttach);
		void AddImage(const FrameBufferImageConfig& imageAttach);

		void CreateFramebuffer();
		void Release();
	private:
		FrameBufferConfig m_Config;

		//only one depth buffer allowed
		ImageFormat m_DepthFormat = ImageFormat::None;
		Count<Image> m_DepthImage = nullptr;
		std::vector<Count<Image>> m_Images;

		VkFramebuffer m_FrameBuffer;
		VkRenderPass m_CompatibilityRenderPass;
		
		friend class VulkanSwapChain;
	};
	
}
