#include "Proofprch.h"
#include "VulkanImguiLayer.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGuizmo.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "VulkanDescriptorSet.h"
#include "VulkanFrameBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanImage.h"
#include "VulkanRenderer/VulkanRenderer.h"
namespace Proof {
	static ImGui_ImplVulkanH_Window g_MainWindowData;
	static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height) {
		const auto& graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();

		wd->Surface = surface;

		// Check for WSI support
		VkBool32 res;
		vkGetPhysicalDeviceSurfaceSupportKHR(graphicsContext->GetGPU(), graphicsContext->FindPhysicalQueueFamilies().graphicsFamily, wd->Surface, &res);
		if (res != VK_TRUE)
		{
			PF_CORE_ASSERT(false, "Error no WSI support on physical device 0\n");
		}

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(graphicsContext->GetGPU(), wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
		//wd->SurfaceFormat = graphicsContext->GetSwapChain().As<VulkanSwapChain>()->GetSurfaceFormat();
		// Select Present Mode
		#ifdef IMGUI_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
		#else
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
		#endif
		wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(graphicsContext->GetGPU(), wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		//wd->PresentMode = graphicsContext->GetSwapChain().As<VulkanSwapChain>()->GetPresentMode();
		//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

		// Create SwapChain, RenderPass, Framebuffer, etc.
		ImGui_ImplVulkanH_CreateOrResizeWindow(graphicsContext->GetInstance(),
			graphicsContext->GetGPU(),
			graphicsContext->GetDevice(), wd,
			graphicsContext->FindPhysicalQueueFamilies().graphicsFamily,
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
		#if 0 
		FrameBufferConfig frameBuffferconfig;
		frameBuffferconfig.DebugName = "Imgui-FrameBuffer";
		frameBuffferconfig.Attachments = { Application::Get()->GetWindow()->GetSwapChain()->GetImageFormat() };
		frameBuffferconfig.Attachments.Attachments[0].ExistingImage = Application::Get()->GetWindow()->GetSwapChain()->GetImageLayout();
		frameBuffferconfig.Size = { (float)Application::Get()->GetWindow()->GetWidth(), (float)Application::Get()->GetWindow()->GetHeight() };
		m_FrameBuffer = FrameBuffer::Create(frameBuffferconfig);
		#endif

		m_CommandBuffer = RenderCommandBuffer::Create();
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
		init_info.QueueFamily = graphicsContext->FindPhysicalQueueFamilies().graphicsFamily;
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
		ImGui_ImplVulkan_SetMinImageCount(graphicsContext->GetSwapChain()->GetImageCount());

		// Upload Fonts
		{
			Renderer::SubmitCommand([&](CommandBuffer* buffer) {
				ImGui_ImplVulkan_CreateFontsTexture(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer());
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
	}
	void VulkanImguiLayer::OnDetach()
	{
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
		});
	}
	void VulkanImguiLayer::End()
	{
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
		{
			//vkResetCommandPool(graphicsContext->GetDevice(), fd->CommandPool, 0);
			Renderer::BeginCommandBuffer(m_CommandBuffer);
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = wd->RenderPass;
			info.framebuffer = m_FrameBuffers[Renderer::GetCurrentFrame().ImageIndex];
			info.renderArea.extent.width = wd->Width;
			info.renderArea.extent.height = wd->Height;
			info.clearValueCount = 1;
			info.pClearValues = &wd->ClearValue;

			VkViewport vk_viewport;
			VkRect2D vk_scissor;
			vk_viewport.x = 0;
			vk_viewport.y = 0;
			vk_viewport.width = (float)wd->Width;
			vk_viewport.height = (float)wd->Height;
			vk_viewport.minDepth = 0;
			vk_viewport.maxDepth = 1;


			vk_scissor.offset = { (int)0, (int)0 };

			vk_scissor.extent = { (uint32_t)wd->Width, (uint32_t)wd->Height };
			vkCmdSetViewport(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_viewport);

			vkCmdSetScissor(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_scissor);

		
			vkCmdBeginRenderPass(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), &info, VK_SUBPASS_CONTENTS_INLINE);
			std::vector< VkClearAttachment> clears;
			std::vector< VkClearRect> reactClear;
			{
				clears.resize(1);
				clears[0].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				clears[0].clearValue = wd->ClearValue;
				clears[0].colorAttachment = 0;
				VkClearRect clearRect = {};
				clearRect.layerCount = 1;
				clearRect.rect = vk_scissor;
				reactClear.emplace_back(clearRect);
				vkCmdClearAttachments(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
					clears.size(),
					clears.data(),
					reactClear.size(),
					reactClear.data());
			}
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer());
			vkCmdEndRenderPass(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer());

			Renderer::EndCommandBuffer(m_CommandBuffer);
			Renderer::SubmitCommandBuffer(m_CommandBuffer);
		}

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			void* backup_current_context = Application::Get()->GetWindow()->GetWindow();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent((GLFWwindow*)backup_current_context);
		}
	}
	ImTextureID VulkanImguiLayer::ToImguiImage(Count<Texture2D> texture)
	{
		return ToImguiImage(texture->GetImage());
	}

	ImTextureID VulkanImguiLayer::ToImguiImage(Count<Image2D> image)
	{
		// passing this as a pointer so we can get hte location easier 
		const VkDescriptorImageInfo* imageDescriptorInfo = (VkDescriptorImageInfo*)image.As<VulkanImage2D>()->GetResourceDescriptorInfo();
		if (imageDescriptorInfo == nullptr || imageDescriptorInfo->imageView == nullptr || imageDescriptorInfo->sampler == nullptr)
			return ToImguiImage(Renderer::GetWhiteTexture());
		VkImage imageVK = image.As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image;

		uint64_t memLocation = (uint64_t)(image.Get());
		if (m_ImagesDescriptors.contains(memLocation))
			return (ImTextureID)m_ImagesDescriptors[memLocation];

		m_ImagesDescriptors[memLocation] = ImGui_ImplVulkan_AddTexture(imageDescriptorInfo->sampler, imageDescriptorInfo->imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		return (ImTextureID)m_ImagesDescriptors[memLocation];
	}
	void VulkanImguiLayer::UpdateImageDescriptor(const VulkanImage2D* image)
	{
		const VkDescriptorImageInfo* imageDescriptorInfo = (VkDescriptorImageInfo*)image->GetResourceDescriptorInfo();
		if (imageDescriptorInfo == nullptr || imageDescriptorInfo->imageView == nullptr || imageDescriptorInfo->sampler == nullptr)
			PF_CORE_ASSERT(false, "Updating image with null information");

		uint64_t memLocation = (uint64_t)(image);
		if (!m_ImagesDescriptors.contains(memLocation))
			return;
		ImGui_ImplVulkan_UpdateTextureProof(m_ImagesDescriptors[memLocation], imageDescriptorInfo->sampler, imageDescriptorInfo->imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
	void VulkanImguiLayer::RemoveImageDescriptor(const VulkanImage2D* image)
	{
		const VkDescriptorImageInfo* imageDescriptorInfo = (VkDescriptorImageInfo*)image->GetResourceDescriptorInfo();
		uint64_t memLocation = (uint64_t)(image);

		if (!m_ImagesDescriptors.contains(memLocation))
			return;

		VkDescriptorSet set = m_ImagesDescriptors[memLocation];
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
			graphicsContext->FindPhysicalQueueFamilies().graphicsFamily,
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

