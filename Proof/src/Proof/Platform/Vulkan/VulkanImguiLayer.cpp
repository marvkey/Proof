#include "Proofprch.h"
#include "VulkanImguiLayer.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Core/Application.h"
#include "VulkanSwapChain.h"
#include "VulkanFrameBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanImage.h"
#include "VulkanRenderer.h"
#include "VulkanImage.h"
#include "VulkanTexutre.h"
#include "Vulkan.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "Vulkan.h"

#include <GLFW/glfw3.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "ImGuizmo.h"
namespace Proof {
	static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;
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
		auto device = VulkanGraphicsContext::Get()->GetDevice()->GetVulkanDevice();

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

		Count<VulkanImguiLayer> instance = this;
		Renderer::Submit([instance]()
			{
				Application& app = *Application::Get();
				GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetWindow());

				auto vulkanContext = VulkanRenderer::GetGraphicsContext();
				auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

				VkDescriptorPool descriptorPool;

				// Create Descriptor Pool
				VkDescriptorPoolSize pool_sizes[] =
				{
					{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
					{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
					{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
					{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
				};
				VkDescriptorPoolCreateInfo pool_info = {};
				pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
				pool_info.maxSets = 100 * IM_ARRAYSIZE(pool_sizes);
				pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
				pool_info.pPoolSizes = pool_sizes;
				VK_CHECK_RESULT(vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptorPool));

				// Setup Platform/Renderer bindings
				ImGui_ImplGlfw_InitForVulkan(window, true);
				ImGui_ImplVulkan_InitInfo init_info = {};
				init_info.Instance = VulkanRenderer::GetGraphicsContext()->GetInstance();
				init_info.PhysicalDevice = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetPhysicalDevice()->GetVulkanPhysicalDevice();
				init_info.Device = device;
				init_info.QueueFamily = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
				init_info.Queue = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetGraphicsQueue();
				init_info.PipelineCache = nullptr;
				init_info.DescriptorPool = descriptorPool;
				init_info.Allocator = nullptr;
				init_info.MinImageCount = 2;
				Count<VulkanSwapChain> swapChain = Application::Get()->GetWindow()->GetSwapChain().As<VulkanSwapChain>();
				init_info.ImageCount = swapChain->GetImageCount();
				init_info.CheckVkResultFn = Utils::VulkanCheckResult;
				ImGui_ImplVulkan_Init(&init_info, swapChain->GetRenderPass());

				// Upload Fonts
				{
					// Use any command queue

					VkCommandBuffer commandBuffer = vulkanContext->GetDevice()->GetCommandBuffer(true);
					ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
					vulkanContext->GetDevice()->FlushCommandBuffer(commandBuffer);

					VK_CHECK_RESULT(vkDeviceWaitIdle(device));
					ImGui_ImplVulkan_DestroyFontUploadObjects();
				}

				uint32_t framesInFlight = Renderer::GetConfig().FramesFlight;
				s_ImGuiCommandBuffers.resize(framesInFlight);
				for (uint32_t i = 0; i < framesInFlight; i++)
					s_ImGuiCommandBuffers[i] = VulkanGraphicsContext::Get()->GetDevice()->CreateSecondaryCommandBuffer("ImGuiSecondaryCoommandBuffer");
			});
	}
	void VulkanImguiLayer::OnDetach()
	{
		PF_PROFILE_FUNC();

		Renderer::Submit([]()
			{
				auto device = VulkanGraphicsContext::Get()->GetDevice()->GetVulkanDevice();

				VK_CHECK_RESULT(vkDeviceWaitIdle(device));
				ImGui_ImplVulkan_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
			});
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
		
	}
	void VulkanImguiLayer::End()
	{
		PF_PROFILE_FUNC();
		ImGuiLayer::End();

		ImGui::Render();

		Count<VulkanSwapChain> swapChain = Application::Get()->GetWindow()->GetSwapChain().As<VulkanSwapChain>();

		VkClearValue clearValues[2];
		clearValues[0].color = { {0.1f, 0.1f,0.1f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		uint32_t width = swapChain->GetWidth();
		uint32_t height = swapChain->GetHeight();

		uint32_t commandBufferIndex = swapChain->GetFrameIndex();

		VkCommandBufferBeginInfo drawCmdBufInfo = {};
		drawCmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		drawCmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		drawCmdBufInfo.pNext = nullptr;

		VkCommandBuffer drawCommandBuffer = swapChain->GetCurrentDrawCommandBuffer();
		VK_CHECK_RESULT(vkBeginCommandBuffer(drawCommandBuffer, &drawCmdBufInfo));

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = swapChain->GetRenderPass();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = 2; // Color + depth
		renderPassBeginInfo.pClearValues = clearValues;
		renderPassBeginInfo.framebuffer = swapChain->GetCurrentFramebuffer();

		vkCmdBeginRenderPass(drawCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass = swapChain->GetRenderPass();
		inheritanceInfo.framebuffer = swapChain->GetCurrentFramebuffer();

		VkCommandBufferBeginInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

		VK_CHECK_RESULT(vkBeginCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex], &cmdBufInfo));

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = (float)height;
		viewport.height = -(float)height;
		viewport.width = (float)width;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.extent.width = width;
		scissor.extent.height = height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &scissor);

		ImDrawData* main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, s_ImGuiCommandBuffers[commandBufferIndex]);

		VK_CHECK_RESULT(vkEndCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex]));

		std::vector<VkCommandBuffer> commandBuffers;
		commandBuffers.push_back(s_ImGuiCommandBuffers[commandBufferIndex]);

		vkCmdExecuteCommands(drawCommandBuffer, uint32_t(commandBuffers.size()), commandBuffers.data());

		vkCmdEndRenderPass(drawCommandBuffer);

		VK_CHECK_RESULT(vkEndCommandBuffer(drawCommandBuffer));

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		
		std::vector<uint64_t> removeIndexes;
		for (auto& [Id, setInfo] : m_ImagesDescriptors)
		{
			auto [set, image] = setInfo;
			if (image.IsValid() == false)
			{
				if(set)
					ImGui_ImplVulkan_RemoveTexture(set);
				removeIndexes.push_back(Id);
			}
		}

		for (auto& index : removeIndexes)
			m_ImagesDescriptors.erase(index);
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
			ImGui_ImplVulkan_UpdateTextureProof(m_ImagesDescriptors[memLocation].first, imageDescriptorInfo->sampler, imageDescriptorInfo->imageView, imageDescriptorInfo->imageLayout);
			return (ImTextureID)m_ImagesDescriptors[memLocation].first;
		}

		m_ImagesDescriptors[memLocation].first = ImGui_ImplVulkan_AddTexture(imageDescriptorInfo->sampler, imageDescriptorInfo->imageView, imageDescriptorInfo->imageLayout);
		m_ImagesDescriptors[memLocation].second = image.Get();
		return (ImTextureID)m_ImagesDescriptors[memLocation].first;
	}
	
}

