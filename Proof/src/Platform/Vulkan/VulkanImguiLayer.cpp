#include "Proofprch.h"
#include "VulkanImguiLayer.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGuizmo.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Core/Application.h"

#include <GLFW/glfw3.h>
#include "VulkanFrameBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanImage.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanImage.h"
#include "VulkanTexutre.h"
#include "Vulkan.h"
#include "VulkanCommandBuffer.h"

#include "VulkanCommandBuffer.h"
namespace Proof {
	static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;
	static ImGui_ImplVulkanH_Window g_MainWindowData;
	static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height) {
		const auto& graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();

		wd->Surface = surface;

		// Check for WSI support
		VkBool32 res;
		vkGetPhysicalDeviceSurfaceSupportKHR(graphicsContext->GetGPU(), graphicsContext->FindPhysicalQueueFamilies().graphicsAndComputeFamily.value(), wd->Surface, &res);
		if (res != VK_TRUE)
		{
			PF_CORE_ASSERT(false, "Error no WSI support on physical device 0\n");
		}

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		//wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(graphicsContext->GetGPU(), wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
		wd->SurfaceFormat = graphicsContext->GetSwapChain().As<VulkanSwapChain>()->GetSurfaceFormat();
		// Select Present Mode
		#ifdef IMGUI_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
		#else
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
		#endif
		//wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(graphicsContext->GetGPU(), wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		wd->PresentMode = graphicsContext->GetSwapChain().As<VulkanSwapChain>()->GetPresentMode();
		//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

		// Create SwapChain, RenderPass, Framebuffer, etc.
		ImGui_ImplVulkanH_CreateOrResizeWindow(graphicsContext->GetInstance(),
			graphicsContext->GetGPU(),
			graphicsContext->GetDevice(), wd,
			graphicsContext->FindPhysicalQueueFamilies().graphicsAndComputeFamily.value(),
			nullptr, width, height,
			graphicsContext->GetSwapChain()->GetImageCount());

	}
	void check_vk_result(VkResult err)
	{
		VK_CHECK_RESULT(err);
	}
	VulkanImguiLayer::VulkanImguiLayer():
		ImGuiLayer("VulkanImguiLayer") 
	{

	}
	VulkanImguiLayer::~VulkanImguiLayer()
	{

	}
	void VulkanImguiLayer::OnAttach()
	{
	
		PF_PROFILE_FUNC();
		m_CommandBuffer = Count<VulkanRenderCommandBuffer>::Create("Imgui Command Buffer",true);
		const auto& graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// for assetVIewe we can use imguidoc node no split
		ImGuiIO& io = ImGui::GetIO();;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		//io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

		/* for fonts got to cherno video from 8 mINute on making editor look GOOd*/
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Regular.ttf", 17.0f);
		SetDarkTheme();
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		auto err = vkCreateDescriptorPool(graphicsContext->GetDevice(), &pool_info, nullptr, &m_DescriptorPool);
		check_vk_result(err);
		ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
		
		SetupVulkanWindow(wd, graphicsContext->GetSurface(), Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());

		ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = graphicsContext->GetInstance();
		init_info.PhysicalDevice = graphicsContext->GetGPU();
		init_info.Device = graphicsContext->GetDevice();
		init_info.QueueFamily = graphicsContext->FindPhysicalQueueFamilies().graphicsAndComputeFamily.value();
		init_info.Queue = graphicsContext->GetGraphicsQueue();
		init_info.PipelineCache = nullptr;
		init_info.DescriptorPool = m_DescriptorPool;
		init_info.Subpass = 0;
		init_info.MinImageCount = graphicsContext->GetSwapChain()->GetImageCount();
		init_info.ImageCount = graphicsContext->GetSwapChain()->GetImageCount();
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = nullptr;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, wd->RenderPass );
	//	ImGui_ImplVulkan_SetMinImageCount(graphicsContext->GetSwapChain()->GetImageCount());

		// Upload Fonts
		{
			Renderer::SubmitCommand([&](CommandBuffer* buffer) {
				ImGui_ImplVulkan_CreateFontsTexture(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight));
			});
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}

		m_FrameBuffers.resize(Renderer::GetConfig().MaxImageCount);
		for(int i= 0; i <Renderer::GetConfig().MaxImageCount; i++)
		{
			VkFramebufferCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.renderPass = wd->RenderPass;
			info.attachmentCount = 1;
			info.pAttachments = &Application::Get()->GetWindow()->GetSwapChain()->GetImageLayout().GetImageIndex(i).As<VulkanImage2D>()->GetDescriptorInfoVulkan().imageView;
			info.width = wd->Width;
			info.height = wd->Height;
			info.layers = 1;
			vkCreateFramebuffer(graphicsContext->GetDevice(), &info, nullptr, &m_FrameBuffers[i]);
		}
		uint32_t framesInFlight = Renderer::GetConfig().FramesFlight;
		s_ImGuiCommandBuffers.resize(framesInFlight);
		for (uint32_t i = 0; i < framesInFlight; i++)
		{
			VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
			cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmdBufAllocateInfo.commandPool = graphicsContext->GetCommandPool();
			cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			cmdBufAllocateInfo.commandBufferCount = 1;
			VK_CHECK_RESULT(vkAllocateCommandBuffers(graphicsContext->GetDevice(), &cmdBufAllocateInfo, &s_ImGuiCommandBuffers[i]));
			graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_COMMAND_BUFFER, "ImGuiSecondaryCommandBuffer", s_ImGuiCommandBuffers[i]);

		}
	}
	void VulkanImguiLayer::OnDetach()
	{
		PF_PROFILE_FUNC();

		const auto& graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		for (int i = 0; i < m_FrameBuffers.size(); i++)
		{
			Renderer::SubmitDatafree([buffer = m_FrameBuffers[i]]() {
				const auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
				vkDestroyFramebuffer(device, buffer, nullptr);
			});
		}
		ImGui_ImplVulkanH_DestroyWindow(graphicsContext->GetInstance(), graphicsContext->GetDevice(), &g_MainWindowData, nullptr);
		vkDestroyDescriptorPool(graphicsContext->GetDevice(), m_DescriptorPool, nullptr);
	}
	void VulkanImguiLayer::Begin()
	{
		PF_PROFILE_FUNC();

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}
	void VulkanImguiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) {
			if (e.GetWhidt() != 0 or e.GetHeight() != 0)
				m_Resize = true;
			return false;
		});
	}
	void VulkanImguiLayer::End()
	{
		PF_PROFILE_FUNC();

		uint32_t frameinFlight = Renderer::GetCurrentFrame().FrameinFlight;
		uint32_t currentImageIndex = Renderer::GetCurrentFrame().ImageIndex;
		auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
		ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
		ImGuiIO& io = ImGui::GetIO();
		if (m_Resize == true)
			OnResize();
		
		io.DisplaySize = ImVec2((float)Application::Get()->GetWindow()->GetWidth(), (float)Application::Get()->GetWindow()->GetHeight());
		wd->FrameIndex = Renderer::GetCurrentFrame().FrameinFlight;

		ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();
		VkCommandBufferBeginInfo drawCmdBufInfo = {};
		drawCmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		drawCmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		drawCmdBufInfo.pNext = nullptr;

		VkCommandBuffer drawCommandBuffer = graphicsContext->GetSwapChain().As<VulkanSwapChain>()->GetCommandBuffer(frameinFlight);
		//VK_CHECK_RESULT(vkBeginCommandBuffer(drawCommandBuffer, &drawCmdBufInfo));
		VkClearValue clearValues[2];
		clearValues[0].color = { {0.1f, 0.1f,0.1f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };
		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = wd->RenderPass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = io.DisplaySize.x;
		renderPassBeginInfo.renderArea.extent.height = io.DisplaySize.y;
		renderPassBeginInfo.clearValueCount = 2; // Color + depth
		renderPassBeginInfo.pClearValues = clearValues;
		renderPassBeginInfo.framebuffer = m_FrameBuffers[currentImageIndex];

		vkCmdBeginRenderPass(drawCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass = wd->RenderPass;
		inheritanceInfo.framebuffer = m_FrameBuffers[currentImageIndex];

		VkCommandBufferBeginInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

		VK_CHECK_RESULT(vkBeginCommandBuffer(s_ImGuiCommandBuffers[frameinFlight], &cmdBufInfo));

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = (float)wd->Height;
		viewport.height = -(float)wd->Height;
		viewport.width = (float)wd->Width;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(s_ImGuiCommandBuffers[frameinFlight], 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.extent.width = wd->Height;
		scissor.extent.height = wd->Height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(s_ImGuiCommandBuffers[frameinFlight], 0, 1, &scissor);

		ImDrawData* main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, s_ImGuiCommandBuffers[frameinFlight]);

		VK_CHECK_RESULT(vkEndCommandBuffer(s_ImGuiCommandBuffers[frameinFlight]));

		std::vector<VkCommandBuffer> commandBuffers;
		commandBuffers.push_back(s_ImGuiCommandBuffers[frameinFlight]);

		vkCmdExecuteCommands(drawCommandBuffer, uint32_t(commandBuffers.size()), commandBuffers.data());

		vkCmdEndRenderPass(drawCommandBuffer);

		//VK_CHECK_RESULT(vkEndCommandBuffer(drawCommandBuffer));

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		
	}
	ImTextureID VulkanImguiLayer::ToImguiImage(Count<Texture2D> texture)
	{
		return ToImguiImage(texture->GetImage());
	}

	ImTextureID VulkanImguiLayer::ToImguiImage(Count<Image> image)
	{
		// passing this as a pointer so we can get hte location easier 
		const VkDescriptorImageInfo* imageDescriptorInfo = (VkDescriptorImageInfo*)image->GetResourceDescriptorInfo();
		if (imageDescriptorInfo == nullptr || imageDescriptorInfo->imageView == nullptr || imageDescriptorInfo->sampler == nullptr)
			return ToImguiImage(Renderer::GetWhiteTexture());

		uint64_t memLocation = (uint64_t)(image.Get());

		VkDescriptorImageInfo* infoRef = (VkDescriptorImageInfo*)image->GetResourceDescriptorInfo();
		if (m_ImagesDescriptors.contains(memLocation))
		{
			// in case maybe the image holding it wa deleted but another image is holding the data 
			m_ImagesDescriptors[memLocation].second = image.Get();
			
			return (ImTextureID)m_ImagesDescriptors[memLocation].first;
		}

		m_ImagesDescriptors[memLocation].first = ImGui_ImplVulkan_AddTexture(imageDescriptorInfo->sampler, imageDescriptorInfo->imageView, imageDescriptorInfo->imageLayout);
		m_ImagesDescriptors[memLocation].second = image.Get();
		return (ImTextureID)m_ImagesDescriptors[memLocation].first;
	}
	void VulkanImguiLayer::UpdateImageDescriptor(const Image* image)
	{
		const VkDescriptorImageInfo* imageDescriptorInfo = (VkDescriptorImageInfo*)image->GetResourceDescriptorInfo();
		if (imageDescriptorInfo == nullptr || imageDescriptorInfo->imageView == nullptr || imageDescriptorInfo->sampler == nullptr)
			PF_CORE_ASSERT(false, "Updating image with null information");

		uint64_t memLocation = (uint64_t)(image);
		if (!m_ImagesDescriptors.contains(memLocation))
			return;
		ImGui_ImplVulkan_UpdateTextureProof(m_ImagesDescriptors[memLocation].first, imageDescriptorInfo->sampler, imageDescriptorInfo->imageView, imageDescriptorInfo->imageLayout);
	}
	void VulkanImguiLayer::RemoveImageDescriptor(const Image* image)
	{
		const VkDescriptorImageInfo* imageDescriptorInfo = (VkDescriptorImageInfo*)image->GetResourceDescriptorInfo();
		uint64_t memLocation = (uint64_t)(image);

		if (!m_ImagesDescriptors.contains(memLocation))
			return;

		VkDescriptorSet set = m_ImagesDescriptors[memLocation].first;

		ImGui_ImplVulkan_RemoveTexture(set);
		m_ImagesDescriptors.erase(memLocation);
	}
	void VulkanImguiLayer::OnResize()
	{
		auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
		ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
		ImGuiIO& io = ImGui::GetIO();
		for (int i = 0; i < m_FrameBuffers.size(); i++)
		{
			Renderer::SubmitDatafree([buffer = m_FrameBuffers[i]]() {
				const auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
				vkDestroyFramebuffer(device, buffer, nullptr);
			});
		}
		ImGui_ImplVulkan_SetMinImageCount(graphicsContext->GetSwapChain()->GetImageCount());
		ImGui_ImplVulkanH_CreateOrResizeWindow(graphicsContext->GetInstance(),
			graphicsContext->GetGPU(),
			graphicsContext->GetDevice(), wd,
			graphicsContext->FindPhysicalQueueFamilies().graphicsAndComputeFamily.value(),
			nullptr, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight(),
			graphicsContext->GetSwapChain()->GetImageCount());

		m_FrameBuffers.resize(Renderer::GetConfig().MaxImageCount);
		for (int i = 0; i < Renderer::GetConfig().MaxImageCount; i++)
		{
			VkFramebufferCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.renderPass = wd->RenderPass;
			info.attachmentCount = 1;
			info.pAttachments = &Application::Get()->GetWindow()->GetSwapChain()->GetImageLayout().GetImageIndex(i).As<VulkanImage2D>()->GetDescriptorInfoVulkan().imageView;
			info.width = wd->Width;
			info.height = wd->Height;
			info.layers = 1;
			vkCreateFramebuffer(graphicsContext->GetDevice(), &info, nullptr, &m_FrameBuffers[i]);
		}
		m_Resize = false;
	}
}

