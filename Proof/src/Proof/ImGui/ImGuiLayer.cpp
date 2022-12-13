#include "Proofprch.h"
#include "ImGuiLayer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "Proof/Core/CurrentWindow.h"
#include <GLFW/glfw3.h>
#include "ImGuizmo.h"
#include "Proof/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanDescriptorSet.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanRenderer/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Platform/Vulkan/VulkanVertexArray.h"
namespace Proof
{
	/*
		VkAttachmentDescription attachment = {};
				attachment.format = graphicsContext->GetSwapChain()->GetImageFormat();
				//attachment.format = wd->SurfaceFormat.format;
				attachment.samples = VK_SAMPLE_COUNT_1_BIT;
				//attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				//attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
				attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				VkAttachmentReference color_attachment = {};
				color_attachment.attachment = 0;
				color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &color_attachment;
				VkSubpassDependency dependency = {};
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = 0;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				VkRenderPassCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				info.attachmentCount = 1;
				info.pAttachments = &attachment;
				info.subpassCount = 1;
				info.pSubpasses = &subpass;
				info.dependencyCount = 1;
				info.pDependencies = &dependency;
	*/
	struct ImguiRenderPass {
		Count<VulkanCommandBuffer> CommandBuffer;
		Count<VulkanRenderPass> RenderPass;
		Count<VulkanScreenFrameBuffer> FrameBuffer;
		ImguiRenderPass() {
		
			RenderPass = CreateCount<VulkanRenderPass>(VulkanRenderPassDefaultType::Other);
			CommandBuffer = CreateCount<VulkanCommandBuffer>();
			FrameBuffer = CreateCount<VulkanScreenFrameBuffer>(Vector2{ (float)CurrentWindow::GetWindow().GetWidth(),(float)CurrentWindow::GetWindow().GetHeight() }
			, RenderPass, false);
			
		}
	};
	static ImguiRenderPass* s_ImguiRenderPass;
	static ImGui_ImplVulkanH_Window g_MainWindowData;
	void check_vk_result(VkResult err) {
		PF_ENGINE_ERROR("[vulkan] Error: VkResult = {}", err);
	}
	static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height) {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		wd->Surface = surface;

		// Check for WSI support
		VkBool32 res;
		vkGetPhysicalDeviceSurfaceSupportKHR(graphicsContext->GetGPU(), graphicsContext->FindPhysicalQueueFamilies().graphicsFamily, wd->Surface, &res);
		if (res != VK_TRUE) {
			PF_CORE_ASSERT(false, "Error no WSI support on physical device 0\n");
		}

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(graphicsContext->GetGPU(), wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);
		//wd->SurfaceFormat = graphicsContext->GetSwapChain()->GetSurfaceFormat();
		// Select Present Mode
		#ifdef IMGUI_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
		#else
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
		#endif
		wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(graphicsContext->GetGPU(), wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		//wd->PresentMode = graphicsContext->GetSwapChain()->GetPresentMode();
		//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

		// Create SwapChain, RenderPass, Framebuffer, etc.
		ImGui_ImplVulkanH_CreateOrResizeWindow(graphicsContext->GetInstance(),
			graphicsContext->GetGPU(),
			graphicsContext->GetDevice(), wd,
			graphicsContext->FindPhysicalQueueFamilies().graphicsFamily,
			nullptr, width, height,
			graphicsContext->GetSwapChain()->GetImageCount());

	}
	static void NewSetupVulkanWindow(ImGui_ImplVulkanH_Window* wd) {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		wd->Surface = graphicsContext->GetSurface();

		// Check for WSI support
		VkBool32 res;
		vkGetPhysicalDeviceSurfaceSupportKHR(graphicsContext->GetGPU(), graphicsContext->FindPhysicalQueueFamilies().graphicsFamily, wd->Surface, &res);
		if (res != VK_TRUE) {
			PF_CORE_ASSERT(false, "Error no WSI support on physical device 0\n");
		}

		// Select Surface Format
		wd->SurfaceFormat = graphicsContext->GetSwapChain()->GetSurfaceFormat();
		wd->PresentMode = graphicsContext->GetSwapChain()->GetPresentMode();

		ImGui_ImplVulkanH_CreateOrResizeWindow(graphicsContext->GetInstance(),
			graphicsContext->GetGPU(),
			graphicsContext->GetDevice(), wd,
			graphicsContext->FindPhysicalQueueFamilies().graphicsFamily,
			nullptr, CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight(),
			graphicsContext->GetSwapChain()->GetImageCount());
	}
	static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data) {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		wd->FrameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		wd->SemaphoreIndex = Renderer::GetCurrentFrame().ImageIndex;
		VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
		VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
		vkAcquireNextImageKHR(graphicsContext->GetDevice(), wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
		//if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
		//	g_SwapChainRebuild = true;
		//	return;
		//}


		ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
		{
			vkWaitForFences(graphicsContext->GetDevice(), 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
			//
			vkResetFences(graphicsContext->GetDevice(), 1, &fd->Fence);
		}
		{
			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkBeginCommandBuffer(fd->CommandBuffer, &info);
			{
				{
					VkRenderPassBeginInfo info = {};
					info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					info.renderPass = wd->RenderPass;
					info.framebuffer = fd->Framebuffer;
					info.renderArea.extent.width = wd->Width;
					info.renderArea.extent.height = wd->Height;
					info.clearValueCount = 1;
					info.pClearValues = &wd->ClearValue;
					vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
				}
				ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);
				vkCmdEndRenderPass(fd->CommandBuffer);
				{
					VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					VkSubmitInfo info = {};
					info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
					info.waitSemaphoreCount = 1;
					info.pWaitSemaphores = &image_acquired_semaphore;
					info.pWaitDstStageMask = &wait_stage;
					info.commandBufferCount = 1;
					info.pCommandBuffers = &fd->CommandBuffer;
					info.signalSemaphoreCount = 1;
					info.pSignalSemaphores = &render_complete_semaphore;

					vkEndCommandBuffer(fd->CommandBuffer);

					vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &info, fd->Fence);
				}
			}
		}
		
	}


	static void NewFrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data) {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		wd->FrameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		wd->SemaphoreIndex = Renderer::GetCurrentFrame().ImageIndex;

		ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];

		{
			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkBeginCommandBuffer(s_ImguiRenderPass->CommandBuffer->GetCommandBuffer(), &info);
			{
				{
					VkRenderPassBeginInfo info = {};
					info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					info.renderPass = wd->RenderPass;
					info.framebuffer = fd->Framebuffer;
					info.renderArea.extent.width = wd->Width;
					info.renderArea.extent.height = wd->Height;
					info.clearValueCount = 1;
					info.pClearValues = &wd->ClearValue;
					vkCmdBeginRenderPass(s_ImguiRenderPass->CommandBuffer->GetCommandBuffer(), &info, VK_SUBPASS_CONTENTS_INLINE);
				}
				ImGui_ImplVulkan_RenderDrawData(draw_data, s_ImguiRenderPass->CommandBuffer->GetCommandBuffer());
				vkCmdEndRenderPass(s_ImguiRenderPass->CommandBuffer->GetCommandBuffer());
				vkEndCommandBuffer(s_ImguiRenderPass->CommandBuffer->GetCommandBuffer());
			}
		}

	}


	static void FramePresent(ImGui_ImplVulkanH_Window* wd) {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		//if (g_SwapChainRebuild)
		//	return;
		VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
		VkPresentInfoKHR info = {};
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &render_complete_semaphore;
		info.swapchainCount = 1;
		info.pSwapchains = &wd->Swapchain;
		info.pImageIndices = &wd->FrameIndex;
		vkQueuePresentKHR(graphicsContext->GetGraphicsQueue(), &info);
		//if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
		//	g_SwapChainRebuild = true;
		//	return;
		//}
		//check_vk_result(err);
		wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
	}

	ImGuiLayer::ImGuiLayer() :
		Layer("ImGUI Layer")
	{
	}
	VkRenderPass m_ImGuiRenderPass;
	std::vector<VkFramebuffer> m_ImGuiFramebuffers;
	std::vector < VkCommandBuffer > m_ImGuiCommandBuffers;
	std::vector < VkImageView> m_ImguiViewbackbuffers;

	void ImGuiLayer::OnAttach() {
		Layer::OnAttach();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		//io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
		
		/* for fonts got to cherno video from 8 mINute on making editor look GOOd*/

		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Bold.ttf",18.0f);
		io.FontDefault =io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Regular.ttf",17.0f);
		SetDarkTheme();
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {

			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)CurrentWindow::GetWindow().GetWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 450");
		} 
		{

		}
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
			s_ImguiRenderPass = new ImguiRenderPass();

			ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
			const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
			SetupVulkanWindow(wd, graphicsContext->GetSurface(), CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight());

			{
				VkImageViewCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				info.format = graphicsContext->GetSwapChain()->GetImageFormat();
				//info.format = wd->SurfaceFormat.format;
				info.components.r = VK_COMPONENT_SWIZZLE_R;
				info.components.g = VK_COMPONENT_SWIZZLE_G;
				info.components.b = VK_COMPONENT_SWIZZLE_B;
				info.components.a = VK_COMPONENT_SWIZZLE_A;
				m_ImguiViewbackbuffers.resize(graphicsContext->GetSwapChain()->GetImageCount());
				VkImageSubresourceRange image_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
				info.subresourceRange = image_range;
				for (uint32_t i = 0; i < graphicsContext->GetSwapChain()->GetImageCount(); i++) {
					//ImGui_ImplVulkanH_Frame* fd = &wd->Frames[i];
					info.image = graphicsContext->GetSwapChain()->m_SwapChainImages[i];
					vkCreateImageView(graphicsContext->GetDevice(), &info, nullptr, &m_ImguiViewbackbuffers[i]);
					//check_vk_result(err);
				}
			}
			{
				VkAttachmentDescription attachment = {};
				attachment.format = graphicsContext->GetSwapChain()->GetImageFormat();
				//attachment.format = wd->SurfaceFormat.format;
				attachment.samples = VK_SAMPLE_COUNT_1_BIT;
				attachment.loadOp = wd->ClearEnable ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				VkAttachmentReference color_attachment = {};
				color_attachment.attachment = 0;
				color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				VkSubpassDescription subpass = {};
				subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpass.colorAttachmentCount = 1;
				subpass.pColorAttachments = &color_attachment;
				VkSubpassDependency dependency = {};
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = 0;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				VkRenderPassCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				info.attachmentCount = 1;
				info.pAttachments = &attachment;
				info.subpassCount = 1;
				info.pSubpasses = &subpass;
				info.dependencyCount = 1;
				info.pDependencies = &dependency;

				if (vkCreateRenderPass(graphicsContext->GetDevice(), &info, nullptr, &m_ImGuiRenderPass) != VK_SUCCESS)
					throw std::runtime_error("failed to create render pass!");
			}
			{
				m_ImGuiFramebuffers.resize(graphicsContext->GetSwapChain()->GetImageCount());

				VkImageView attachment[1];
				VkFramebufferCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				info.renderPass = m_ImGuiRenderPass;
				info.attachmentCount = 1;
				info.pAttachments = attachment;
				info.width = CurrentWindow::GetWindow().GetWidth();
				info.height = CurrentWindow::GetWindow().GetHeight();
				info.layers = 1;
				for (uint32_t i = 0; i < graphicsContext->GetSwapChain()->GetImageCount(); i++) {
					//attachment[0] = m_ImguiViewbackbuffers[i];
					attachment[0] = graphicsContext->GetSwapChain()->m_SwapChainImageViews[i];

					if (vkCreateFramebuffer(graphicsContext->GetDevice(), &info, nullptr, &m_ImGuiFramebuffers[i]) != VK_SUCCESS) {
						PF_CORE_ASSERT(false, "failed to create framebuffer!");
					}
				}
			}
			{

				m_ImGuiCommandBuffers.resize(Renderer::GetConfig().FramesFlight);

				VkCommandBufferAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool = graphicsContext->GetCommandPool();
				allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				allocInfo.commandBufferCount = (uint32_t)m_ImGuiCommandBuffers.size();

				if (vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, m_ImGuiCommandBuffers.data()) != VK_SUCCESS) {
					throw std::runtime_error("failed to allocate command buffers!");
				}
			}
			ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)CurrentWindow::GetWindow().GetWindow(), true);
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = graphicsContext->GetInstance();
			init_info.PhysicalDevice = graphicsContext->GetGPU();
			init_info.Device = graphicsContext->GetDevice();
			init_info.QueueFamily = graphicsContext->FindPhysicalQueueFamilies().graphicsFamily;
			init_info.Queue = graphicsContext->GetGraphicsQueue();
			init_info.PipelineCache = nullptr;
			init_info.DescriptorPool = graphicsContext->GetGlobalPool()->GetPool();
			init_info.Subpass = 0;
			init_info.MinImageCount = graphicsContext->GetSwapChain()->GetImageCount();
			init_info.ImageCount = graphicsContext->GetSwapChain()->GetImageCount();
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			init_info.Allocator = nullptr;
			init_info.CheckVkResultFn = nullptr;
			s_ImguiRenderPass->RenderPass->m_RenderPass = m_ImGuiRenderPass;
			//s_ImguiRenderPass->RenderPass->m_RenderPass = wd->RenderPass;
			ImGui_ImplVulkan_Init(&init_info, s_ImguiRenderPass->RenderPass->m_RenderPass);
			ImGui_ImplVulkan_SetMinImageCount(Renderer::GetConfig().ImageSize);


			// Upload Fonts
			{
				VulkanRenderer::Submit([&](VkCommandBuffer buffer) {
					ImGui_ImplVulkan_CreateFontsTexture(buffer);
				});
				ImGui_ImplVulkan_DestroyFontUploadObjects();

			}



		}
	}
	void ImGuiLayer::OnDetach() {
		Layer::OnDetach();
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {

		}
	}

	void ImGuiLayer::Begin() {

		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {
			ImGui_ImplOpenGL3_NewFrame();
		}

		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
			ImGui_ImplVulkan_NewFrame();
		}
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		//ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) {
			
		});
	}
	
	void ImGuiLayer::End() {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)CurrentWindow::GetWindow().GetWidth(), (float)CurrentWindow::GetWindow().GetHeight());
		//wd->FrameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		//ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
		//
		//wd->ClearValue.color.float32[0] = ImGui::GetStyle().Colors[ImGuiCol_WindowBg].x * ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w;
		//wd->ClearValue.color.float32[1] = ImGui::GetStyle().Colors[ImGuiCol_WindowBg].y * ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w;
		//wd->ClearValue.color.float32[2] = ImGui::GetStyle().Colors[ImGuiCol_WindowBg].z * ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w;
		//wd->ClearValue.color.float32[3] = ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w;
		ImGui::Render();
		//Vector4 color{ wd->ClearValue.color.float32[0],wd->ClearValue.color.float32[1],wd->ClearValue.color.float32[2],wd->ClearValue.color.float32[3] };
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		///s_ImguiRenderPass->CommandBuffer->m_CommandBuffer[Renderer::GetCurrentFrame().FrameinFlight] = fd->CommandBuffer;
		s_ImguiRenderPass->FrameBuffer->m_Framebuffers[Renderer::GetCurrentFrame().ImageIndex] = m_ImGuiFramebuffers[Renderer::GetCurrentFrame().ImageIndex];
		//s_ImguiRenderPass->FrameBuffer->m_Framebuffers[Renderer::GetCurrentFrame().ImageIndex] = wd->Frames[Renderer::GetCurrentFrame().ImageIndex].Framebuffer;
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {
			ImGui_ImplOpenGL3_RenderDrawData(main_draw_data);
		}
		{
			//https://github.com/1111mp/Vulkan/blob/master/src/Application.cpp
			//graphicsContext->GetSwapChain()->WaitFences();
//			graphicsContext->GetSwapChain()->WaitFences();
			//graphicsContext->GetSwapChain()->AcquireNextImage(&Renderer::GetCurrentFrame().ImageIndex);

			//graphicsContext->GetSwapChain()->ResetFences();
			VulkanRenderer::BeginRenderPass(s_ImguiRenderPass->CommandBuffer, s_ImguiRenderPass->RenderPass, s_ImguiRenderPass->FrameBuffer, true);
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), s_ImguiRenderPass->CommandBuffer->GetCommandBuffer());
			VulkanRenderer::EndRenderPass(s_ImguiRenderPass->CommandBuffer, s_ImguiRenderPass->RenderPass);
			VulkanRenderer::SubmitCommandBuffer(s_ImguiRenderPass->CommandBuffer);
			//graphicsContext->GetSwapChain()->SubmitCommandBuffers({ s_ImguiRenderPass->CommandBuffer }, &Renderer::GetCurrentFrame().ImageIndex);
			//NewFrameRender(wd, main_draw_data);
			//FrameRender(wd, main_draw_data);;
		}
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			void* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent((GLFWwindow*)backup_current_context);
		}
		//FramePresent(wd);
	
		//if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
		//	NewFrameRender(wd, main_draw_data);
		//	graphicsContext->GetSwapChain()->SubmitCommandBuffers({ s_ImguiRenderPass->CommandBuffer },&Renderer::GetCurrentFrame().ImageIndex);
		//	//VulkanRenderer::SubmitCommandBuffer(s_ImguiRenderPass->CommandBuffer);
		//}
		//FramePresent(wd);
	}
	void ImGuiLayer::SetDarkTheme() {

		auto& style = ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{0.1f,0.1f,0.1f,1.0f};

		// Headers
		colors[ImGuiCol_Header] = ImVec4{0.15f,0.15f,0.15f,1.0f};
		colors[ImGuiCol_HeaderHovered] = ImVec4{0.2f,0.2f,0.5f,1.0f};
		colors[ImGuiCol_HeaderActive] = ImVec4{0.2f,0.2f,0.5f,1.0f};

		// Buttons Done
		colors[ImGuiCol_Button] = ImVec4{0.15f,0.15f,0.15f,1.0f};
		colors[ImGuiCol_ButtonHovered] = ImVec4{0.2f,0.2f,0.5f,1.0f};
		colors[ImGuiCol_ButtonActive] = ImVec4{0.2f,0.2f,0.5f,1.0f};

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{0.2f,0.2f,0.21f,1.0f};
		colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f,0.305f,0.31f,1.0f};
		colors[ImGuiCol_FrameBgActive] = ImVec4{0.1f,0.1505f,0.151f,1.0f};

			
		// Tabs Done
		colors[ImGuiCol_Tab] = ImVec4{0.15f,0.1505f,0.151f,1.0f};
		colors[ImGuiCol_TabHovered] = ImVec4{0.2f,0.2f,0.5f,1.0f};
		colors[ImGuiCol_TabActive] = ImVec4{0.2f,0.2f,0.2f,1.0f};
		colors[ImGuiCol_TabUnfocused] = ImVec4{0.1f,0.1f,0.1f,1.0f};
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.1305f,0.12f,0.12f,1.0f};

		// Title Done
		colors[ImGuiCol_TitleBg] = ImVec4{0.1f,0.1f,0.121f,1.0f};
		colors[ImGuiCol_TitleBgActive] = ImVec4{0.10f,0.12f,0.2f,1.0f};
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.1095f,0.10f,0.1f,1.0f};
		style.FrameRounding = 12;
		style.PopupRounding =12;
		style.TabBorderSize =1;
		style.PopupRounding =12;
		style.GrabRounding =12;
		style.PopupBorderSize =0;
		style.ScrollbarSize =15;
		style.WindowRounding = 0.0f;

	}
}