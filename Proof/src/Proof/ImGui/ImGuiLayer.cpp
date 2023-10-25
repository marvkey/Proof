#include "Proofprch.h"
#include "ImGuiLayer.h"
#include "ImGui/imgui.h"
#include "Proof/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanImguiLayer.h"
namespace Proof
{
	#if 0
	struct ImguiRenderPass {
		Count<RenderCommandBuffer> CommandBuffer;
		Count<RenderPass> RenderPass;
		Count<FrameBuffer> FrameBuffer;
		ImguiRenderPass() {
			

			FrameBufferConfig frameBuffferconfig;
			frameBuffferconfig.DebugName = "Imgui-FrameBuffer";
			frameBuffferconfig.Attachments = { Application::Get()->GetWindow()->GetSwapChain()->GetImageFormat() };
			frameBuffferconfig.Attachments.Attachments[0].ExistingImage = Application::Get()->GetWindow()->GetSwapChain()->GetImageLayout();
			frameBuffferconfig.Size = { (float)Application::Get()->GetWindow()->GetWidth(), (float)Application::Get()->GetWindow()->GetHeight() };
			FrameBuffer = FrameBuffer::Create(frameBuffferconfig);
			RenderPassConfig config;
			config.DebugName = "Imgui RenderPass";
			config.Attachments = { Application::Get()->GetWindow()->GetSwapChain()->GetImageFormat() };
			config.Attachments.Attachments[0].PresentKHr = true;
			config.TargetBuffer = FrameBuffer;

			RenderPass = RenderPass::Create(config);
			CommandBuffer = RenderCommandBuffer::Create();
		}
	};
	static ImguiRenderPass* s_ImguiRenderPass;
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


	

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach() {
		Layer::OnAttach();
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
		if (Renderer::GetAPI() == Renderer::API::OpenGL)
		{
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 450");
		}

		if (Renderer::GetAPI() == Renderer::API::Vulkan)
		{
			s_ImguiRenderPass = new ImguiRenderPass();

			ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
			const auto& graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
			SetupVulkanWindow(wd, graphicsContext->GetSurface(), Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());

			ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), true);
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
			ImGui_ImplVulkan_Init(&init_info, s_ImguiRenderPass->RenderPass.As<VulkanRenderPass>()->GetRenderPass());
			ImGui_ImplVulkan_SetMinImageCount(Renderer::GetConfig().MaxImageCount);

			// Upload Fonts
			{
				Renderer::SubmitCommand([&](CommandBuffer* buffer) {
					ImGui_ImplVulkan_CreateFontsTexture(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer());
				});
				ImGui_ImplVulkan_DestroyFontUploadObjects();

			}
		}
	}
	void ImGuiLayer::OnDetach() {
		Layer::OnDetach();

		if (Renderer::GetAPI() == Renderer::API::OpenGL)
			ImGui_ImplOpenGL3_Shutdown();

		if (Renderer::GetAPI() == Renderer::API::Vulkan)
			ImGui_ImplVulkan_Shutdown();
		pdelete s_ImguiRenderPass;
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {

		if (Renderer::GetAPI() == Renderer::API::OpenGL)
		{
			ImGui_ImplOpenGL3_NewFrame();
		}

		if (Renderer::GetAPI() == Renderer::API::Vulkan)
		{
			ImGui_ImplVulkan_NewFrame();
		}
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) {
			if (e.GetWhidt() != 0 or e.GetHeight() != 0)
				m_WindoResize = true;
		});
	}
	void ImGuiLayer::End() {
		auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
		ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
		ImGuiIO& io = ImGui::GetIO();
		if (m_WindoResize == true)
		{

			FrameBufferConfig frameBuffferconfig;
			frameBuffferconfig.DebugName = "Imgui-FrameBuffer";
			frameBuffferconfig.Attachments = { Application::Get()->GetWindow()->GetSwapChain()->GetImageFormat() };
			frameBuffferconfig.Attachments.Attachments[0].ExistingImage = Application::Get()->GetWindow()->GetSwapChain()->GetImageLayout();
			frameBuffferconfig.Size = { (float)Application::Get()->GetWindow()->GetWidth(), (float)Application::Get()->GetWindow()->GetHeight() };
			
			s_ImguiRenderPass->FrameBuffer = FrameBuffer::Create(frameBuffferconfig); 
			ImGui_ImplVulkan_SetMinImageCount(graphicsContext->GetSwapChain()->GetImageCount());

			ImGui_ImplVulkanH_CreateOrResizeWindow(graphicsContext->GetInstance(),
				graphicsContext->GetGPU(),
				graphicsContext->GetDevice(), wd,
				graphicsContext->FindPhysicalQueueFamilies().graphicsFamily,
				nullptr, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight(),
				graphicsContext->GetSwapChain()->GetImageCount());
			m_WindoResize = false;
			io.DisplaySize = ImVec2((float)Application::Get()->GetWindow()->GetWidth(), (float)Application::Get()->GetWindow()->GetHeight());
		}
		wd->FrameIndex = Renderer::GetCurrentFrame().FrameinFlight;
		ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
		ImGui::Render();
		ImDrawData* main_draw_data = ImGui::GetDrawData();
		if (Renderer::GetAPI() == Renderer::API::OpenGL)
		{
			ImGui_ImplOpenGL3_RenderDrawData(main_draw_data);
		}
		{
			//https://github.com/1111mp/Vulkan/blob/master/src/Application.cpp
			Renderer::BeginCommandBuffer(s_ImguiRenderPass->CommandBuffer);
			// begin render pass

			s_ImguiRenderPass->RenderPass.As<VulkanRenderPass>()->BeginRenderPass(s_ImguiRenderPass->CommandBuffer);
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), s_ImguiRenderPass->CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer());
			Renderer::EndRenderPass(s_ImguiRenderPass->RenderPass);
			Renderer::EndCommandBuffer(s_ImguiRenderPass->CommandBuffer);
			Renderer::SubmitCommandBuffer(s_ImguiRenderPass->CommandBuffer);
		}
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			void* backup_current_context = Application::Get()->GetWindow()->GetWindow();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent((GLFWwindow*)backup_current_context);
		}
	}
	#endif
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGUI Layer") {
	}
	ImGuiLayer::ImGuiLayer(const std::string& name) :
		Layer(name)
	{

	}

	void ImGuiLayer::SetDarkTheme() {

		auto& style = ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f,0.1f,0.1f,1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.15f,0.15f,0.15f,1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };

		// Buttons Done
		colors[ImGuiCol_Button] = ImVec4{ 0.15f,0.15f,0.15f,1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f,0.2f,0.21f,1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.1f,0.1505f,0.151f,1.0f };


		// Tabs Done
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.2f,0.2f,0.2f,1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.1f,0.1f,0.1f,1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.1305f,0.12f,0.12f,1.0f };

		// Title Done
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.1f,0.1f,0.121f,1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.10f,0.12f,0.2f,1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.1095f,0.10f,0.1f,1.0f };
		//colors[ImGuiCol_TextDisabled]
		style.FrameRounding = 12;
		style.PopupRounding = 12;
		style.TabBorderSize = 1;
		style.GrabRounding = 12;
		style.PopupBorderSize = 0;
		style.ScrollbarSize = 15;
		style.WindowRounding = 0.0f;

	}
	Count<ImGuiLayer> ImGuiLayer::Create()
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImguiLayer>::Create();
		}
	}
}