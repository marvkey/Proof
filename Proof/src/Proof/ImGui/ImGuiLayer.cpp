#include "Proofprch.h"
#include "ImGuiLayer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "Proof/Core/CurrentWindow.h"
#include<GLFW/glfw3.h>
#include "ImGuizmo.h"
#include "Proof/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
#include "Platform/Vulkan/VulkanDescriptorSet.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Platform/Vulkan/VulkanRenderer/VulkanRenderer.h"
namespace Proof {
	static ImGui_ImplVulkanH_Window g_MainWindowData;
	struct VulkanImgui {
		VkRenderPass RenderPass;
		std::vector<VkFramebuffer> frameBuffer;
		Count<VulkanGraphicsPipeline> GraphicsPipeline;

	};
	static VulkanImgui* s_VulkanImgui;
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGUI Layer")
	{
		delete s_VulkanImgui;
		s_VulkanImgui = new VulkanImgui();
	}

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
		io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
		
		/* for fonts got to cherno video from 8 mINute on making editor look GOOd*/

		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Bold.ttf",18.0f);
		io.FontDefault =io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Regular.ttf",17.0f);
		SetDarkTheme();
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {

			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)CurrentWindow::GetWindow().GetWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 450");
		} 
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
			auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
			ImGui_ImplVulkan_InitInfo init_info = {};

			//ImGui_ImplVulkanH_CreateOrResizeWindow(graphicsContext->GetInstance(), 
			//	graphicsContext->GetGPU(), graphicsContext->GetDevice(), &g_MainWindowData, 
			//	graphicsContext->FindPhysicalQueueFamilies().graphicsFamily, nullptr, CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight(), CurrentWindow::GetWindow().GetSwapChain()->GetImageCount());
			//ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

			{
				VkAttachmentDescription attachment = {};
				attachment.format = CurrentWindow::GetWindow().GetSwapChain()->GetSwapChainImageFormat();
				attachment.samples = VK_SAMPLE_COUNT_1_BIT;
				attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
				attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
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
				dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				VkRenderPassCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
				info.attachmentCount = 1;
				info.pAttachments = &attachment;
				info.subpassCount = 1;
				info.pSubpasses = &subpass;
				info.dependencyCount = 1;
				info.pDependencies = &dependency;
				if (vkCreateRenderPass(graphicsContext->GetDevice(), &info, nullptr, &s_VulkanImgui->RenderPass) != VK_SUCCESS) {
					PF_CORE_ASSERT(false,"Could not create Dear ImGui's render pass");
				}

				{
					VkImageView attachment[1];
					VkFramebufferCreateInfo info = {};
					info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					info.renderPass = s_VulkanImgui->RenderPass;
					info.attachmentCount = 1;
					info.pAttachments = attachment;
					info.width = CurrentWindow::GetWindow().GetWidth();
					info.height = CurrentWindow::GetWindow().GetHeight();
					info.layers = 1;
					s_VulkanImgui->frameBuffer.resize(CurrentWindow::GetWindow().GetSwapChain()->GetImageCount());
					for (uint32_t i = 0; i < CurrentWindow::GetWindow().GetSwapChain()->GetImageCount(); i++) {
						//ImGui_ImplVulkanH_Frame* fd = &wd->Frames[i];
						attachment[0] = CurrentWindow::GetWindow().GetSwapChain()->GetImageView(i);
						vkCreateFramebuffer(graphicsContext->GetDevice(), &info, nullptr, &s_VulkanImgui->frameBuffer[i]);
						//check_vk_result(err);
					}
				}
			}
			ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)CurrentWindow::GetWindow().GetWindow(), true);
			init_info.Instance = graphicsContext->GetInstance();
			init_info.PhysicalDevice = graphicsContext->GetGPU();
			init_info.Device = graphicsContext->GetDevice();
			init_info.QueueFamily = graphicsContext->FindPhysicalQueueFamilies().graphicsFamily;
			init_info.Queue = graphicsContext->GetGraphicsQueue();
			//init_info.PipelineCache = graphicsContext->GetPipelineCache();
			init_info.DescriptorPool = graphicsContext->GetGlobalPool()->GetPool();
			init_info.Subpass = 0;
			init_info.MinImageCount = 2;
			init_info.ImageCount = CurrentWindow::GetWindow().GetSwapChain()->GetImageCount();
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			init_info.Allocator = graphicsContext->GetAllocator();
			init_info.CheckVkResultFn = VulkanGraphicsContext::CHECK_VULKAN_ERROR;
			ImGui_ImplVulkan_Init(&init_info, s_VulkanImgui->RenderPass);
			VulkanRenderer::Submit([&](VkCommandBuffer cmd) {
				ImGui_ImplVulkan_CreateFontsTexture(cmd);
			});

			//clear font textures from cpu data
			ImGui_ImplVulkan_DestroyFontUploadObjects();

			//_mainDeletionQueue.push_function([=]() {
			//
			//	ImGui_ImplVulkan_Shutdown();
			//});
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
		ImGuizmo::BeginFrame();
	}
	
	void ImGuiLayer::End() {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		//vkWaitForFences(graphicsContext->GetDevice(), 1, &CurrentWindow::GetWindow().GetSwapChain()->m_InFlightFences[Renderer::GetCurrentFrame()], VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking

		//vkResetFences(graphicsContext->GetDevice(), 1, &CurrentWindow::GetWindow().GetSwapChain()->m_InFlightFences[Renderer::GetCurrentFrame()]);
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)CurrentWindow::GetWindow().GetWidth(), (float)CurrentWindow::GetWindow().GetHeight());
		ImGui::Render();
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL) {
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
			CurrentWindow::GetWindow().GetSwapChain()->WaitAndResetFences();
			VulkanRenderer::s_Pipeline->SwapChain->AcquireNextImage(&VulkanRenderer::swapchainImageIndex);

			VulkanRenderer::BeginRenderPass(nullptr,s_VulkanImgui->RenderPass,s_VulkanImgui->frameBuffer[VulkanRenderer::swapchainImageIndex]);
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), VulkanRenderer::s_Pipeline->CommandBuffer->GetCommandBuffer());
				// Submit command buffer
			vkCmdEndRenderPass(VulkanRenderer::s_Pipeline->CommandBuffer->GetCommandBuffer());
			{
				{
					VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					VkSubmitInfo info = {};
					info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
					info.waitSemaphoreCount = 1;
					info.pWaitSemaphores = &CurrentWindow::GetWindow().GetSwapChain()->m_ImageAvailableSemaphores[Renderer::GetCurrentFrame()];
					info.pWaitDstStageMask = &wait_stage;
					info.commandBufferCount = 1;
					info.pCommandBuffers = &VulkanRenderer::s_Pipeline->CommandBuffer->GetCommandBuffer();
					info.signalSemaphoreCount = 1;
					info.pSignalSemaphores = &CurrentWindow::GetWindow().GetSwapChain()->m_RenderFinishedSemaphores[Renderer::GetCurrentFrame()];

					vkEndCommandBuffer(VulkanRenderer::s_Pipeline->CommandBuffer->GetCommandBuffer());
					//check_vk_result(err);
					vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &info, CurrentWindow::GetWindow().GetSwapChain()->m_InFlightFences[Renderer::GetCurrentFrame()]);
					//check_vk_result(err);
				}
				VkSemaphore render_complete_semaphore = CurrentWindow::GetWindow().GetSwapChain()->m_RenderFinishedSemaphores[Renderer::GetCurrentFrame()];
				VkPresentInfoKHR info = {};
				info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				info.waitSemaphoreCount = 1;
				info.pWaitSemaphores = &render_complete_semaphore;
				info.swapchainCount = 1;
				info.pSwapchains = &CurrentWindow::GetWindow().GetSwapChain()->m_SwapChain;
				info.pImageIndices = &VulkanRenderer::swapchainImageIndex;
				VkResult err = vkQueuePresentKHR(graphicsContext->GetGraphicsQueue(), &info);
				if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
					//g_SwapChainRebuild = true;
					//return;
				}
				//check_vk_result(err);
			}
		}
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}		
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
	}
}