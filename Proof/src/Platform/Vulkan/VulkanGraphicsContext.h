#pragma once
#include"Proof/Renderer/GraphicsContext.h"
#include "VulkanUtils/VulkanBufferBase.h"

#include <vulkan/vulkan.h>

namespace Proof
{

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class VulkanGraphicsContext : public GraphicsContext {
	public:
#ifdef  PF_ENABLE_DEBUG 
		const bool enableValidationLayers = true;
#else
		const bool enableValidationLayers = false;
#endif
		VkPhysicalDeviceProperties GetGPUProperties() {
			return m_GPUProperties;
		}
		virtual ~VulkanGraphicsContext();
		VulkanGraphicsContext(Window* windowHandle);
		Window* m_Window;
		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkDevice GetDevice() { return m_Device; }
		VkSurfaceKHR GetSurface() { return m_Surface; }
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() { return m_PresentQueue; }

		SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(m_PhysicalDevice); }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
		VkFormat FindSupportedFormat(
			const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		bool CreateVmaBuffer(VkBufferCreateInfo bufferInfo, VmaAllocationCreateInfo vmaInfo, VulkanBuffer& buffer);
		bool CreateVmaImage(VkImageCreateInfo bufferInfo, VmaAllocationCreateInfo vmaInfo, VulkanImage& image);
		Count<class SwapChain> GetSwapChain(){
			return CurrentWindow::GetWindow().GetSwapChain();
		}
		VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent) {
			VkImageCreateInfo info = { };
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.pNext = nullptr;

			info.imageType = VK_IMAGE_TYPE_2D;

			info.format = format;
			info.extent = extent;

			info.mipLevels = 1;
			info.arrayLayers = 1;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.usage = usageFlags;

			return info;
		}
		VkSampleCountFlagBits GetMaxSampleCount();
		static void CHECK_VULKAN_ERROR(VkResult err) {
			if (err == 0)
				return;
			PF_ENGINE_ERROR("[vulkan] Error: VkResult = {}", err);
			if (err < 0)
				abort();
		}
		VulkanBuffer CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage) {
	//allocate vertex buffer
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.pNext = nullptr;
			bufferInfo.size = allocSize;

			bufferInfo.usage = usage;


			//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = memoryUsage;

			VulkanBuffer newBuffer;

			//allocate the buffer
			vmaCreateBuffer(m_VMA_Allocator, &bufferInfo, &vmaallocInfo,
				&newBuffer.Buffer,
				&newBuffer.Allocation,
				nullptr);

			return newBuffer;
		}
		// Buffer Helper Functions
		void CreateBuffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		void CreateImageWithInfo(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory);

		
		VkPhysicalDeviceProperties m_GPUProperties;
		//void VMACreateBuffer(const VkBufferCreateInfo& bufferInfo, const VmaAllocationCreateInfo& VMAallocinfo, VkBuffer& buffer, VmaAllocation& vmaAlloc);
		//
		//const VmaAllocator& GetVMAAllocator() {
		//	return m_VMA_Allocator;
		//}

		uint32_t GetVulkanVersion() {
			return m_VulkanVersion;
		}
		size_t PadUniformBufferSize(size_t originalSize) {
		// Calculate required alignment based on minimum device offset alignment
			size_t minUboAlignment = m_GPUProperties.limits.minUniformBufferOffsetAlignment;
			size_t alignedSize = originalSize;
			if (minUboAlignment > 0) {
				alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
			}
			return alignedSize;
		}

		VmaAllocator GetVMA_Allocator() {
			return m_VMA_Allocator;
		}
		Count<class VulkanDescriptorPool> GetGlobalPool() {
			return m_GlobalPool;
		}
		const VkInstance GetInstance() {
			return m_Instance;
		}

		const VkPhysicalDevice GetGPU() {
			return m_PhysicalDevice;
		}
		VkAllocationCallbacks* GetAllocator() {
			return m_Allocator;
		}
		VkPipelineCache GetPipelineCache() {
			return m_PipelineCache;
		}


	private:
		uint32_t m_VulkanVersion;
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateCommandPool();
		void InitVMA();
		void InitDescriptors();
		// helper functions
		bool IsDeviceSuitable(VkPhysicalDevice device);
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void HasGflwRequiredInstanceExtensions();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkAllocationCallbacks* m_Allocator =nullptr;
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkCommandPool m_CommandPool;
		VkPipelineCache m_PipelineCache;
		VkDevice m_Device;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		//VmaAllocator m_VMA_Allocator; 
		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		Count<class VulkanDescriptorPool> m_GlobalPool = nullptr;
		VmaAllocator m_VMA_Allocator;
		friend class VulkanRenderer;
	};

}