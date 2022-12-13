#pragma once
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include<vulkan/vulkan.h>
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "../VulkanPipeLineLayout.h"
#include "../VulkanPushConstant.h"
#include "../VulkanGraphicsContext.h"
#include "../VulkanDescriptorSet.h"
#include "../VulkanRenderPass.h"
#include "../VulkanFrameBuffer.h"
namespace Proof
{
	struct VulkanRendererVertex {

		VulkanRendererVertex(const glm::mat4& transform) :
			m_Transform(transform) {
		}
		glm::mat4 m_Transform;
	};
	struct PositionIndex {
		uint64_t Begin;
		uint64_t End;
	};
	struct DrawPipeline {
		Count<VulkanSwapChain> SwapChain = nullptr;
		std::vector<Count<VulkanCommandBuffer>> CommandBuffers;
		Count<VulkanCommandBuffer> CommandBuffer;
		Count<VulkanUniformBuffer> CameraBuffer = nullptr;
		Count<ScreenFrameBuffer> CurrentFrameBuffer = nullptr;
		//MESH ASSET ID, meshNumber
		std::unordered_map<AssetID, uint64_t> AmountMeshPerMeshAssetID;
		std::vector<VulkanRendererVertex>Transforms;
		std::unordered_map<AssetID, PositionIndex> MeshesPositionIndexTransforms;

		//we need this because when addign a id to a unordered map
		// treversing we could go in a different direction
		// therefore our transfomrs will be misplaced
		//with this we can add mesh id in tthe order there were added to the list 
		//and use the offset to treverse trhought them in the correct order the transforms were added
		std::vector<uint64_t> MeshesID;
		uint32_t NumberMeshes;
	};	
	struct RenderPassSubData {
		Vector4 Color = { 0,0,0,1.0f };
		float Depth = 1.0f;
		uint32_t Stencil = 0;
	};
	class VulkanRenderer {
	public:
		static void Init();

		static void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec);
		static void BeginContext(Count<ScreenFrameBuffer>& frameBuffer);
		static void EndContext();

		static void SubmitMesh(MeshComponent& mesh, const glm::mat4& transform);
		static void Destroy();
		static void BeginRenderPass(Count<VulkanCommandBuffer> commandBuffer, Count<VulkanRenderPass> renderPass, bool viewScreen = false, RenderPassSubData subData = RenderPassSubData());
		static void BeginRenderPass(Count<VulkanCommandBuffer> commandBuffer, Count<VulkanRenderPass> renderPass,Count<ScreenFrameBuffer> frameBuffer,bool viewScreen = false, RenderPassSubData subData = RenderPassSubData());
		static void EndRenderPass(Count<VulkanCommandBuffer> commandBuffer, Count<VulkanRenderPass> renderPass);
		static void Record(Count<VulkanCommandBuffer> commandBuffer,std::function<void(Count<VulkanCommandBuffer> commandBuffer)> data);
		static void SubmitCommandBuffer(Count<VulkanCommandBuffer> commandBuffer);
		static void SubmitDatafree(std::function<void()>&& func);
		template <class T>
		static void Submit(T func) {
			auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = graphicsContext->GetCommandPool();
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			func(commandBuffer);
			vkEndCommandBuffer(commandBuffer);
			
			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(graphicsContext->GetGraphicsQueue());

			vkFreeCommandBuffers(graphicsContext->GetDevice(), graphicsContext->GetCommandPool(), 1, &commandBuffer);
		}
		template <class T>
		static void SubmitControl(T func) {
			auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = graphicsContext->GetCommandPool();
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			func(commandBuffer);
		}
		static void BeginFrame();
		static void EndFrame();
		static DrawPipeline* s_Pipeline;
		static void Reset();
	private:
		static void OnWindowResize(WindowResizeEvent& e);
		static void DrawFrame();
		static void DrawContext();
		static void RecreateSwapChain();
		static void InitDescriptors();
		static void RecordBase(Count<VulkanCommandBuffer> commandBuffer);
		static bool s_InContext;
		static RendererConfig s_RendererConfig;
		static CurrentFrame s_CurrentFrame;
		friend class Renderer;
	};

	struct TrianglePipeLine {
		Count<VulkanGraphicsPipeline> GraphicsPipeline;
		Count<Shader> Shader;
		void Init();
	};
	struct MeshPipeLine {
		Count<VulkanGraphicsPipeline> GraphicsPipeline;
		Count<Shader> Shader;
		Count<VulkanPipeLineLayout> PipeLineLayout;
		Count<VulkanRenderPass> RenderPass;
		void Init();
	};

	
}