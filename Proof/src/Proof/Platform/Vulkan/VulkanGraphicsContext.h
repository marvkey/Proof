#pragma once
#include "Proof/Renderer/GraphicsContext.h"
#include <vulkan/vulkan.h>
namespace Proof
{
	#if 0
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsAndComputeFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
		}
	};
	#endif


	class VulkanGraphicsContext : public GraphicsContext
	{
	public:
		VulkanGraphicsContext();
		virtual ~VulkanGraphicsContext();
		Count<class VulkanDevice> GetDevice() { return m_Device; }
		static VkInstance GetInstance() { return m_VulkanInstance; }
		static Count<VulkanGraphicsContext> Get();
		std::pair<VkSampler, uint64_t> GetOrCreateSampler(VkSamplerCreateInfo samplerInfo);
		void DeleteSampler(uint64_t samplerHash);
		uint32_t GetVulkanVersion() { return m_VulkanVersion; }
	private:
		std::unordered_map<uint64_t, std::pair<uint32_t, VkSampler>> m_Samplers;

		static VkInstance m_VulkanInstance;
		VkPipelineCache m_PipelineCache = nullptr;

		Count<class VulkanPhysicalDevice > m_PhysicalDevice;
		Count<VulkanDevice> m_Device;
		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger = VK_NULL_HANDLE;

		uint32_t m_VulkanVersion;
	};
	#if 0

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
		void SetDebugUtilsObjectName(VkObjectType objectType, const std::string& name, void* objectHandle);
		virtual ~VulkanGraphicsContext();
		VulkanGraphicsContext(Window* windowHandle);
		Window* m_Window;
		VkDevice GetDevice() { return m_Device; }
		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkSurfaceKHR GetSurface() { return m_Surface; }
		// graphics and compute queue
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() { return m_PresentQueue; }



		SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(m_PhysicalDevice); }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
		VkFormat FindSupportedFormat(
			const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		Count<class SwapChain> GetSwapChain();
		VkSampleCountFlagBits GetMaxSampleCount();
		VkSampleCountFlagBits GetSampleCount();
		uint32_t GetUniformPadSize(uint32_t originalSize)
		{
			// Calculate required alignment based on minimum device offset alignment
			size_t minUboAlignment = m_GPUProperties.limits.minUniformBufferOffsetAlignment;
			size_t alignedSize = originalSize;
			if (minUboAlignment > 0)
			{
				alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
			}
			return alignedSize;
		}


		uint32_t GetStoragePadSize(uint32_t originalSize)
		{
			// Calculate required alignment based on minimum device offset alignment
			size_t minUboAlignment = m_GPUProperties.limits.minStorageBufferOffsetAlignment;
			size_t alignedSize = originalSize;
			if (minUboAlignment > 0)
			{
				alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
			}
			return alignedSize;
		}

		VkPhysicalDeviceProperties m_GPUProperties;
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
		//Count<class VulkanDescriptorPool> GetGlobalPool() {
		//	return m_GlobalPool;
		//}
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

		
		//returns sampler and its hash
		std::pair<VkSampler, uint64_t> GetOrCreateSampler(VkSamplerCreateInfo samplerInfo);
		// (DO NOT PUT THIS INOT A RENDDER::SUBMIT FREE BECAUSE IT SENDS THE SMAPLER THERE ALREADY
		void DeleteSampler(uint64_t samplerHash);
	private:
		VkCommandPool m_CommandPool;
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
		VkPipelineCache m_PipelineCache;
		VkDevice m_Device;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		//VmaAllocator m_VMA_Allocator; 

		friend class VulkanRenderer;
		// sampler has, sampler ref count, sampler
		std::unordered_map<uint64_t, std::pair<uint32_t,VkSampler>> m_Samplers;
	};
	#endif
}