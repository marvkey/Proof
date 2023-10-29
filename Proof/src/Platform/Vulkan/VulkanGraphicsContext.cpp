#include "Proofprch.h"
#include "VulkanGraphicsContext.h"
#include "Vulkan.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include "Proof/Core/Application.h"

#include <GLFW/glfw3.h>
namespace Proof
{
	VkInstance VulkanGraphicsContext::m_VulkanInstance;
	#ifdef  PF_ENABLE_DEBUG 
	const bool s_Validation = true;
	#else
	const bool s_Validation = false;
	#endif
	static bool s_Initialized = false;
	constexpr const char* VkDebugUtilsMessageType(const VkDebugUtilsMessageTypeFlagsEXT type)
	{
		switch (type)
		{
			case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:		return "General";
			case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:	return "Validation";
			case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:	return "Performance";
			default:												return "Unknown";
		}
	}

	constexpr const char* VkDebugUtilsMessageSeverity(const VkDebugUtilsMessageSeverityFlagBitsEXT severity)
	{
		switch (severity)
		{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:		return "Info";
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:	return "Warning";
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:		return "Error";
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:	return "Critical";
			default:												return "Unknown";
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugUtilsMessengerCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, const VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		(void)pUserData; //Unused argument

		const bool performanceWarnings = false;
		if (!performanceWarnings)
		{
			if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
				return VK_FALSE;
		}

		std::string labels, objects;
		if (pCallbackData->cmdBufLabelCount)
		{
			labels = fmt::format("\tLabels({}): \n", pCallbackData->cmdBufLabelCount);
			for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; ++i)
			{
				const auto& label = pCallbackData->pCmdBufLabels[i];
				const std::string colorStr = fmt::format("[ {}, {}, {}, {} ]", label.color[0], label.color[1], label.color[2], label.color[3]);
				labels.append(fmt::format("\t\t- Command Buffer Label[{0}]: name: {1}, color: {2}\n", i, label.pLabelName ? label.pLabelName : "NULL", colorStr));
			}
		}

		if (pCallbackData->objectCount)
		{
			objects = fmt::format("\tObjects({}): \n", pCallbackData->objectCount);
			for (uint32_t i = 0; i < pCallbackData->objectCount; ++i)
			{
				const auto& object = pCallbackData->pObjects[i];
				objects.append(fmt::format("\t\t- Object[{0}] name: {1}, type: {2}, handle: {3:#x}\n", i, object.pObjectName ? object.pObjectName : "NULL", Utils::VkObjectTypeToString(object.objectType), object.objectHandle));
			}
		}

		const char* severity = VkDebugUtilsMessageSeverity(messageSeverity);
		if (severity == "Info")
		{
			PF_ENGINE_INFO("{0} {1} message: \n\t{2}\n {3} {4}", VkDebugUtilsMessageType(messageType),severity , pCallbackData->pMessage, labels, objects);
		}
		else  if (severity == "Warning")
		{
			PF_ENGINE_WARN("{0} {1} message: \n\t{2}\n {3} {4}", VkDebugUtilsMessageType(messageType), severity, pCallbackData->pMessage, labels, objects);
		}
		else  if (severity == "Error")
		{
			PF_ENGINE_ERROR("{0} {1} message: \n\t{2}\n {3} {4}", VkDebugUtilsMessageType(messageType), severity, pCallbackData->pMessage, labels, objects);
		}
		else
		{
			PF_ENGINE_CRITICAL("{0} {1} message: \n\t{2}\n {3} {4}", VkDebugUtilsMessageType(messageType), severity, pCallbackData->pMessage, labels, objects);
		}
		return VK_FALSE;
	}

	static bool CheckDriverAPIVersionSupport(uint32_t minimumSupportedVersion)
	{
		uint32_t instanceVersion;
		vkEnumerateInstanceVersion(&instanceVersion);

		if (instanceVersion < minimumSupportedVersion)
		{
			PF_ENGINE_CRITICAL("Incompatible Vulkan driver version!");
			PF_ENGINE_CRITICAL("  You have {}.{}.{}", VK_API_VERSION_MAJOR(instanceVersion), VK_API_VERSION_MINOR(instanceVersion), VK_API_VERSION_PATCH(instanceVersion));
			PF_ENGINE_CRITICAL("  You need at least {}.{}.{}", VK_API_VERSION_MAJOR(minimumSupportedVersion), VK_API_VERSION_MINOR(minimumSupportedVersion), VK_API_VERSION_PATCH(minimumSupportedVersion));

			return false;
		}

		return true;
	}

	VulkanGraphicsContext::VulkanGraphicsContext()
	{
		PF_ENGINE_INFO("Renderer {}", "VulkanGraphicsContext::Create");

		//PF_CORE_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

		m_VulkanVersion = VK_API_VERSION_1_2;

		if (!CheckDriverAPIVersionSupport(m_VulkanVersion))
		{
			MessageBox(nullptr, L"Incompatible Vulkan driver version.\nUpdate your GPU drivers!", L"Proof Error", MB_OK | MB_ICONERROR);
			PF_CORE_ASSERT(false);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Application Info
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Proof";
		appInfo.pEngineName = "Proof";
		appInfo.apiVersion = m_VulkanVersion;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Extensions and Validation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Very little performance hit, can be used in Release.
		if (s_Validation)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		}

		VkValidationFeatureEnableEXT enables[] = { VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT };
		VkValidationFeaturesEXT features = {};
		features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		features.enabledValidationFeatureCount = 1;
		features.pEnabledValidationFeatures = enables;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = nullptr; // &features;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

		// TODO: Extract all validation into separate class
		if (s_Validation)
		{
			const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
			// Check if this layer is available at instance level
			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
			bool validationLayerPresent = false;
			PF_ENGINE_TRACE("Renderer {}", "Vulkan Instance Layers:");
			for (const VkLayerProperties& layer : instanceLayerProperties)
			{
				PF_ENGINE_TRACE("Renderer  {0}", layer.layerName);
				if (strcmp(layer.layerName, validationLayerName) == 0)
				{
					validationLayerPresent = true;
					break;
				}
			}
			if (validationLayerPresent)
			{
				instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
				instanceCreateInfo.enabledLayerCount = 1;
			}
			else
			{
				PF_ENGINE_TRACE("Renderer {}", "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Instance and Surface Creation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_VulkanInstance));
		Utils::VulkanLoadDebugUtilsExtensions(m_VulkanInstance);

		if (s_Validation)
		{

			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanInstance, "vkCreateDebugUtilsMessengerEXT");
			PF_CORE_ASSERT(vkCreateDebugUtilsMessengerEXT != NULL, "");
			VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo{};
			debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugUtilsCreateInfo.pfnUserCallback = VulkanDebugUtilsMessengerCallback;
			debugUtilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT /*  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT*/;

			VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(m_VulkanInstance, &debugUtilsCreateInfo, nullptr, &m_DebugUtilsMessenger));

		}

		m_PhysicalDevice = Count<VulkanPhysicalDevice>::Create();

		VkPhysicalDeviceFeatures enabledFeatures;
		memset(&enabledFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
		enabledFeatures.samplerAnisotropy = true;
		enabledFeatures.wideLines = true;
		enabledFeatures.fillModeNonSolid = true;
		enabledFeatures.independentBlend = true;
		enabledFeatures.pipelineStatisticsQuery = true;
		m_Device = Count<VulkanDevice>::Create(m_PhysicalDevice, enabledFeatures);

		VulkanAllocator::Init(m_Device);

		// Pipeline Cache
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreatePipelineCache(m_Device->GetVulkanDevice(), &pipelineCacheCreateInfo, nullptr, &m_PipelineCache));
	}

	VulkanGraphicsContext::~VulkanGraphicsContext()
	{
		for (auto [hash, hashData] : m_Samplers)
		{
			auto [refCount, sampler] = hashData;
			auto device = GetDevice()->GetVulkanDevice();
			vkDestroySampler(device, sampler, nullptr);
		}
		m_Samplers.clear();
		vkDestroyPipelineCache(m_Device->GetVulkanDevice(), m_PipelineCache, nullptr);

		m_Device = nullptr;
		vkDestroyInstance(m_VulkanInstance, nullptr);
		m_VulkanInstance = nullptr;
	}

	Count<VulkanGraphicsContext> VulkanGraphicsContext::Get()
	{
		return Application::Get()->GetGraphicsContext().As<VulkanGraphicsContext>();
	}
	std::pair<VkSampler, uint64_t> VulkanGraphicsContext::GetOrCreateSampler(VkSamplerCreateInfo samplerInfo)
	{
		auto device = GetDevice()->GetVulkanDevice();

		auto hash = Utils::GetHashSamplerInfo(samplerInfo);
		if (m_Samplers.contains(hash))
		{
			m_Samplers[hash].first++;
			return { m_Samplers[hash].second, hash };
		}
		VkSampler sampler;
		vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
		VulkanUtils::SetDebugUtilsObjectName(device,VK_OBJECT_TYPE_SAMPLER, std::format("{} Sampler", hash), sampler);
		m_Samplers[hash] = { 1,sampler };
		return { sampler,hash };
	}

	void VulkanGraphicsContext::DeleteSampler(uint64_t samplerHash)
	{
		if (m_Samplers.contains(samplerHash) == false)return;
		auto [refCount, sampler] = m_Samplers[samplerHash];
		refCount--;
		if (refCount == 0)
		{
			auto device = GetDevice()->GetVulkanDevice();
			vkDestroySampler(device, sampler, nullptr);
			m_Samplers.erase(samplerHash);
		}
	}
	
	#if 0 
	static const std::vector<const char*> s_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
	static const std::vector<const char*> s_DeviceExtensions = { VK_KHR_MULTIVIEW_EXTENSION_NAME,VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	static const std::vector<const char*> s_InstanceExtension = { VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME };
	// local callback functions
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageType,const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,void* pUserData) {
		if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
			PF_ENGINE_CRITICAL("VULKAN: {}", pCallbackData->pMessage);
		else if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			PF_ENGINE_ERROR("VULKAN: {}", pCallbackData->pMessage);
		else if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			PF_ENGINE_TRACE("VULKAN: {}", pCallbackData->pMessage);
		else if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			PF_ENGINE_WARN("VULKAN: {}", pCallbackData->pMessage);
		else
			PF_ENGINE_TRACE("VULKAN: {}", pCallbackData->pMessage);

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,const VkAllocationCallbacks* pAllocator,VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,"vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance,
			"vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
	void VulkanGraphicsContext::SetDebugUtilsObjectName(VkObjectType objectType, const std::string& name, void* objectHandle) {

		static PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXTFuncPtr = nullptr;

		if (vkSetDebugUtilsObjectNameEXTFuncPtr == nullptr)
		{
			vkSetDebugUtilsObjectNameEXTFuncPtr = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(
				vkGetDeviceProcAddr(m_Device, "vkSetDebugUtilsObjectNameEXT")
				);
		}
		VkDebugUtilsObjectNameInfoEXT nameInfo{};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.pNext = nullptr;
		nameInfo.objectType = objectType;
		nameInfo.objectHandle = reinterpret_cast<uint64_t>(objectHandle);;
		nameInfo.pObjectName = name.c_str();

		VK_CHECK_RESULT(vkSetDebugUtilsObjectNameEXTFuncPtr(m_Device, &nameInfo));
	}
	VulkanGraphicsContext::VulkanGraphicsContext(Window* window) : m_Window{ window } {
		CreateInstance();
		SetupDebugMessenger();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateCommandPool();
		InitDescriptors();
		InitVMA();
		VulkanAllocator::Init(this);
	}

	VulkanGraphicsContext::~VulkanGraphicsContext() {
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
		}
		for (auto [hash, hashData] : m_Samplers)
		{
			auto [refCount, sampler] = hashData;
			Renderer::SubmitDatafree([sampler = sampler, device = m_Device] {
				vkDestroySampler(device, sampler, nullptr);
			});
		}
		m_Samplers.clear();
		Renderer::SubmitDatafree([device = m_Device,instance = m_Instance, surface = m_Surface,commandPool = m_CommandPool]()mutable {
			vkDestroyCommandPool(device, commandPool, nullptr);
			vkDestroyDevice(device, nullptr);

			vkDestroySurfaceKHR(instance, surface, nullptr);
			vkDestroyInstance(instance, nullptr);
		});
		VulkanAllocator::ShutDown();
	}

	std::pair<VkSampler, uint64_t> VulkanGraphicsContext::GetOrCreateSampler(VkSamplerCreateInfo samplerInfo)
	{
		auto hash=Utils::GetHashSamplerInfo(samplerInfo);
		if (m_Samplers.contains(hash))
		{
			m_Samplers[hash].first++;
			return { m_Samplers[hash].second, hash};
		}

		VkSampler sampler;
		vkCreateSampler(m_Device, &samplerInfo, nullptr, &sampler);
		SetDebugUtilsObjectName(VK_OBJECT_TYPE_SAMPLER, std::format("{} Sampler", hash), sampler);
		m_Samplers[hash] = { 1,sampler };
		return { sampler,hash };
	}

	void VulkanGraphicsContext::DeleteSampler(uint64_t samplerHash)
	{
		if (m_Samplers.contains(samplerHash) == false)return;
		auto [refCount,sampler] = m_Samplers[samplerHash];
		refCount--;
		if (refCount == 0)
		{
			Renderer::SubmitDatafree([sampler = sampler, device = m_Device] {
				vkDestroySampler(device, sampler, nullptr);
			});
			m_Samplers.erase(samplerHash);
		}
	}

	void VulkanGraphicsContext::CreateInstance() {
		if (enableValidationLayers && !CheckValidationLayerSupport()) {
			PF_CORE_ASSERT(false, "validation layers requested, but not available!");
		}
		m_VulkanVersion = VK_API_VERSION_1_2;
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Proof";
		appInfo.applicationVersion = m_VulkanVersion;
		appInfo.pEngineName = "Proof";
		appInfo.engineVersion = m_VulkanVersion;
		appInfo.apiVersion = m_VulkanVersion;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(s_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = s_ValidationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, m_Allocator, &m_Instance) != VK_SUCCESS) {
			PF_CORE_ASSERT(false, "failed to create instance!");
		}

		HasGflwRequiredInstanceExtensions();
	}

	void VulkanGraphicsContext::PickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			PF_CORE_ASSERT(false, "failed to find GPUs with Vulkan support!");
		}
		PF_ENGINE_INFO("Device Count {}", deviceCount);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (IsDeviceSuitable(device)) {
				m_PhysicalDevice = device;
				break;
			}
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE) {
			PF_CORE_ASSERT(false, "failed to find a suitable GPU!");
		}

		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_GPUProperties);
		PF_ENGINE_INFO("Gpu Name: {}", m_GPUProperties.deviceName);
		PF_ENGINE_INFO("	Driver Version: {}", m_GPUProperties.driverVersion);
		PF_ENGINE_INFO("	Vendor ID: {}", m_GPUProperties.vendorID);
		PF_ENGINE_INFO("	Device ID: {}", m_GPUProperties.deviceID);
		PF_ENGINE_INFO("	Type: {}", m_GPUProperties.deviceType);
		PF_ENGINE_INFO("	API Version: {}", m_GPUProperties.apiVersion);
		PF_ENGINE_INFO("	max bound descriptor sets: {}", m_GPUProperties.limits.maxBoundDescriptorSets);
		PF_ENGINE_INFO("	minimum buffer alignment : {}", m_GPUProperties.limits.minUniformBufferOffsetAlignment);
	}

	void VulkanGraphicsContext::CreateLogicalDevice() {
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsAndComputeFamily.value(), indices.presentFamily.value()};

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}
		VkPhysicalDeviceMultiviewFeatures multiviewFeatures = {};
		multiviewFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES;
		multiviewFeatures.multiview = VK_TRUE; // Enable the multiview feature


		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.depthClamp = VK_TRUE;
		deviceFeatures.geometryShader = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.independentBlend = VK_TRUE;
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount  = s_DeviceExtensions.size();
		createInfo.ppEnabledExtensionNames = s_DeviceExtensions.data();
		// might not really be necessary anymore because device specific validation layers
		// have been deprecated
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(s_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = s_ValidationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		//explains this
		//https://www.reddit.com/r/vulkan/comments/or8e8u/comment/h6hdwyr/
		VkPhysicalDeviceVulkan11Features features;
		features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
		features.pNext = nullptr;
		features.storageBuffer16BitAccess = VK_FALSE;
		features.uniformAndStorageBuffer16BitAccess = VK_FALSE;
		features.storagePushConstant16 = VK_FALSE;
		features.storageInputOutput16 = VK_FALSE;
		features.multiview = VK_TRUE;
		features.multiviewGeometryShader = VK_TRUE;
		features.multiviewTessellationShader = VK_TRUE;
		features.variablePointersStorageBuffer = VK_FALSE	;
		features.variablePointers = VK_FALSE;
		features.protectedMemory = VK_FALSE;
		features.samplerYcbcrConversion = VK_FALSE;
		features.shaderDrawParameters = VK_TRUE;

		createInfo.pNext = &features;
		//createInfo.pNext = nullptr;
		
		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
			PF_CORE_ASSERT(false, "failed to create logical device!");
		}
		vkGetDeviceQueue(m_Device, indices.graphicsAndComputeFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);

		uint32_t deviceExtenison = 0;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice,nullptr, &deviceExtenison, nullptr);
		std::vector<VkExtensionProperties> deviceExtensionsList(deviceExtenison);
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &deviceExtenison, deviceExtensionsList.data());

		PF_ENGINE_TRACE("avalaible Device extensions:");
		std::unordered_set<std::string> availableDeviceExtension;
		for (const auto& extension : deviceExtensionsList)
		{
			PF_ENGINE_TRACE("\t {}", extension.extensionName);
			availableDeviceExtension.insert(extension.extensionName);
		}
	}

	void VulkanGraphicsContext::CreateCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = FindPhysicalQueueFamilies();

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.pNext = nullptr;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsAndComputeFamily.value();

		//we also want the pool to allow for resetting of individual command buffers
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		{
			PF_CORE_ASSERT(false, "failed to create command pool!");
		}
	}


	void VulkanGraphicsContext::InitVMA() {
	}

	void VulkanGraphicsContext::InitDescriptors() {
	}

	void VulkanGraphicsContext::CreateSurface() {
		VkResult result = glfwCreateWindowSurface(m_Instance, static_cast<::GLFWwindow*>(m_Window->GetWindow()), nullptr, &m_Surface);
		if (result != VK_SUCCESS) {
			PF_CORE_ASSERT(false, "failed to craete window surface");
		}
	}

	bool VulkanGraphicsContext::IsDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = FindQueueFamilies(device);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.isComplete() && extensionsSupported && swapChainAdequate &&
			supportedFeatures.samplerAnisotropy;
	}

	void VulkanGraphicsContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;  // Optional
	}

	void VulkanGraphicsContext::SetupDebugMessenger() {
		if (!enableValidationLayers) return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
			PF_CORE_ASSERT(false, "failed to set up debug messenger!");
		}
	}

	bool VulkanGraphicsContext::CheckValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : s_ValidationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> VulkanGraphicsContext::GetRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		for (auto extensina : s_InstanceExtension)
			extensions.push_back(extensina);

		return extensions;
	}

	void VulkanGraphicsContext::HasGflwRequiredInstanceExtensions() {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		

		PF_ENGINE_TRACE("avalaible Instance extensions:");
		std::unordered_set<std::string> available;
		for (const auto& extension : extensions) {
			PF_ENGINE_TRACE("\t {}", extension.extensionName);
			available.insert(extension.extensionName);
		}

		PF_ENGINE_TRACE("required extensions:");
		auto requiredExtensions = GetRequiredExtensions();
		for (const auto& required : requiredExtensions) {
			PF_ENGINE_TRACE("\t {}", required);
			if (available.find(required) == available.end()) {
				PF_CORE_ASSERT(false, "Missing required glfw extension");
			}
		}
 	}

	bool VulkanGraphicsContext::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(
			device,
			nullptr,
			&extensionCount,
			availableExtensions.data());

		std::set<std::string> requiredExtensions(s_DeviceExtensions.begin(), s_DeviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices VulkanGraphicsContext::FindQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)&& !indices.graphicsAndComputeFamily.has_value())
			{
				indices.graphicsAndComputeFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.isComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	SwapChainSupportDetails VulkanGraphicsContext::QuerySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkFormat VulkanGraphicsContext::FindSupportedFormat(
		const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (
				tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		PF_CORE_ASSERT(false, "failed to find supported format!");
	}
	Count<class SwapChain> VulkanGraphicsContext::GetSwapChain()
	{
		return Application::Get()->GetWindow()->GetSwapChain();
	}

	VkSampleCountFlagBits VulkanGraphicsContext::GetMaxSampleCount()
	{
		return VK_SAMPLE_COUNT_1_BIT;
	}

	VkSampleCountFlagBits VulkanGraphicsContext::GetSampleCount()
	{
		return VK_SAMPLE_COUNT_1_BIT;
	}

	uint32_t VulkanGraphicsContext::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		PF_CORE_ASSERT(false, "failed to find suitable memory type!");
	}
	#endif
}