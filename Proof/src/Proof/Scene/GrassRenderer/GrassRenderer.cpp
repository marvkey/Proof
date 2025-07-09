#include "Proofprch.h"
#include "GrassRenderer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Renderer/GraphicsPipeLine.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/ComputePass.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Platform/Vulkan/VulkanComputePass.h"
#include "Proof/Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Core/Application.h"
#include "Proof/Platform/Window/WindowsWindow.h"
#include "Proof/Platform/Vulkan/VulkanSwapChain.h"
#include "Proof/Platform/Vulkan/VulkanGraphicsContext.h"
#include "Proof/Platform/Vulkan/VulkanDevice.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Mesh.h"

namespace Proof
{

	float generateRandomFloat() { return rand() / (float)RAND_MAX; } //(0  and 1)

    Count<VertexArray> vertexArray;
	GrassRenderer::GrassRenderer(Count<WorldRenderer> renderer)
	{
        m_WorldRenderer = renderer.Get();


        if (vertexArray == nullptr)
        {
            vertexArray = VertexArray::Create({ sizeof(UBGrassBlade)});
            vertexArray->AddData(0, DataType::Vec4, offsetof(UBGrassBlade, UBGrassBlade::V0));
            vertexArray->AddData(1, DataType::Vec4, offsetof(UBGrassBlade, UBGrassBlade::V1));
            vertexArray->AddData(2, DataType::Vec4, offsetof(UBGrassBlade, UBGrassBlade::V2));
            vertexArray->AddData(3, DataType::Vec4, offsetof(UBGrassBlade, UBGrassBlade::Up));
        }
        GraphicsPipelineConfiguration pipelineConfig = m_WorldRenderer->GetGeometryPass()->GetPipeline()->GetConfig();

        // Give it a name for debugging / logging
        pipelineConfig.DebugName = "Grass";

        pipelineConfig.Shader = Renderer::GetShader("GrassRenderer");
        pipelineConfig.VertexArray = vertexArray;

        pipelineConfig.DrawMode = DrawType::PatchList;
        pipelineConfig.TessellationPatchControlPoints = 1;
        pipelineConfig.FillMode = PolygonFillMode::Fill;

        pipelineConfig.LineWidth = 1.0f;

        pipelineConfig.CullMode = CullMode::None;
        pipelineConfig.FrontFace = FrontFace::CounterClockWise;

        pipelineConfig.DepthTest =true;
        pipelineConfig.WriteDepth = false; // (TODO) maybe for now 
       // pipelineConfig.DepthCompareOperator = DepthCompareOperator::LessOrEqual;
        pipelineConfig.DepthCompareOperator = DepthCompareOperator::Less;

        pipelineConfig.Blend = true;


        RenderPassConfig renderPassConfig;
        renderPassConfig.DebugName = "Grass";
        renderPassConfig.Pipeline = GraphicsPipeline::Create(pipelineConfig);
        renderPassConfig.TargetFrameBuffer = nullptr;
        m_GrassRenderPass = RenderPass::Create(renderPassConfig);
        m_GrassGenerator = ComputePass::Create("Grass", Renderer::GetShader("GrassGenerator"));
	}
	void GrassRenderer::Update(float deltaTime)
	{

	}
	void GrassRenderer::Render(Count<class WorldRenderer> renderer)
	{
        const auto& planesMap = renderer->GetGrassPlanes().Get();
        if (planesMap.empty())
            return;

        PF_PROFILE_FUNC();

        auto plane = planesMap.begin()->second.first;
        glm::mat4 transform = planesMap.begin()->second.second;


        const uint32_t WORK_GROUP_SIZE = 32;

        m_GrassRenderPass->AddGlobalInput(renderer->GetGlobalInputs());
        m_GrassGenerator->AddGlobalInput(renderer->GetGlobalInputs());

       // Renderer::BeginCommandBuffer(m_RenderCommandBuffer);
        auto cmdBuffer = renderer->GetCommandBuffer();

        m_GrassRenderPass->SetTargetFrameBuffer(renderer->GetGeometryPass()->GetTargetFrameBuffer());
        {
            PF_PROFILE_SCOPE_DYNAMIC("Grass Compute");

            m_GrassGenerator->SetInput("InBlades", plane->BladesBuffer);
            m_GrassGenerator->SetInput("CulledBlades", plane->CulledBladesBuffer);
            m_GrassGenerator->SetInput("IndirectDrawBlades", plane->NumBladesBuffer);

            Renderer::BeginComputePass(cmdBuffer, m_GrassGenerator);

            //for (GrassBladePlane& plane : m_Planes)
            {
                GrassBladeDrawIndirect indirect = { .VertexCount = 0, .InstanceCount = 1, .FirstVertex = 0, .FirstInstance = 0 };
                plane->NumBladesBuffer->GetBuffer()->SetData(Buffer(&indirect, sizeof(GrassBladeDrawIndirect)));
                //remebr only workign cause on only 1 plane in sce
                m_GrassGenerator->Dispatch(static_cast<uint32_t>((plane->GetNumBlades() + WORK_GROUP_SIZE - 1) /
                    WORK_GROUP_SIZE), 1, 1);
            }
            Renderer::EndComputePass(m_GrassGenerator);

        }
#if 0
        std::vector<VkBufferMemoryBarrier> barriers(m_Planes.size());
        for (uint32_t j = 0; j < barriers.size(); ++j)
        {
            barriers[j].sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            barriers[j].srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
            barriers[j].dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
            //barriers[j].srcQueueFamilyIndex = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice()->GetPhysicalDevice()->GetQueueFamilyIndices().Compute;
            //barriers[j].dstQueueFamilyIndex = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice()->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
            barriers[j].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[j].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[j].buffer = m_Planes[j].NumBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer;
            //barriers[j].buffer = m_Planes[j].NumBladesBuffer.As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer;
            barriers[j].offset = 0;
            barriers[j].size = sizeof(GrassBladeDrawIndirect);

        }
#else
        std::vector<VkBufferMemoryBarrier> barriers;
        //for (uint32_t j = 0; j < m_Planes.size(); ++j)
        {
            // NumBladesBuffer: for vkCmdDrawIndirect
            VkBufferMemoryBarrier indirectBarrier = {};
            indirectBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            indirectBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
            indirectBarrier.dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
            indirectBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            indirectBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            //indirectBarrier.buffer = m_Planes[j].NumBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer;
           // indirectBarrier.buffer = m_Planes[j].NumBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer;
            indirectBarrier.buffer = plane->NumBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer;
            indirectBarrier.offset = 0;
            indirectBarrier.size = sizeof(GrassBladeDrawIndirect);
            barriers.push_back(indirectBarrier);

            // CulledBladesBuffer: for vertex fetch
            VkBufferMemoryBarrier vertexBarrier = {};
            vertexBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            vertexBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
            vertexBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            vertexBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vertexBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
           // vertexBarrier.buffer = m_Planes[j].CulledBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer;
            vertexBarrier.buffer = plane->CulledBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer;
            vertexBarrier.offset = 0;
           // vertexBarrier.size = m_Planes[j].CulledBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetSize();
            vertexBarrier.size = plane->CulledBladesBuffer->GetBuffer().As<VulkanStorageBuffer>()->GetSize();
            barriers.push_back(vertexBarrier);
        }
#endif

        Renderer::Submit([commandBuffer = cmdBuffer, grassCompute = m_GrassGenerator, barriers = barriers]()
            {
               
                vkCmdPipelineBarrier(
                    commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                    VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, // srcStageMask
                    VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT | VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, // dstStageMask 
                    0, 0, nullptr,
                    static_cast<uint32_t>(barriers.size()), barriers.data(),
                    0, nullptr);
            });

      
        Renderer::BeginRenderPass(cmdBuffer, m_GrassRenderPass);

        //for (GrassBladePlane& plane : m_Planes)
        {
            m_GrassRenderPass->PushData("u_PushData", &transform);
            Renderer::Submit([commandBuffer = cmdBuffer, culledBufferBlades = plane->BladesBuffer,numBladesBuffer = plane->NumBladesBuffer,transform = transform]() mutable
                {
                   VkDeviceSize instanceOffset[1] = { 0 };
                   vkCmdBindVertexBuffers(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), 0, 1, 
                       &culledBufferBlades->RT_GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer, instanceOffset);
                   //&culledBufferBlades.As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer, instanceOffset);
                   
                   vkCmdDrawIndirect(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                       numBladesBuffer->RT_GetBuffer().As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer, 0, 1,
                       //numBladesBuffer.As<VulkanStorageBuffer>()->GetDescriptorInfoVulkan().buffer, 0, 1,
                       sizeof(GrassBladeDrawIndirect));
                
                });
        }
        Renderer::EndRenderPass(m_GrassRenderPass);
	}
	GrassBladePlane::GrassBladePlane(float planeSize, uint32_t numBlades)
		:m_PlaneSize(planeSize), m_NumBlades(numBlades)
	{

		std::vector<UBGrassBlade> blades;
		blades.reserve(m_NumBlades);


        for (unsigned int i = 0; i < m_NumBlades; i++) {
            UBGrassBlade currentBlade = UBGrassBlade();

            glm::vec3 bladeUp(0.0f, 1.0f, 0.0f);

            // Generate positions and direction (v0)
            float x = (generateRandomFloat() - 0.5f) * m_PlaneSize;
            float y = 0.0f;
            float z = (generateRandomFloat() - 0.5f) * m_PlaneSize;
            float direction = generateRandomFloat() * 2.f * 3.14159265f;
            glm::vec3 bladePosition(x, y, z);
            currentBlade.V0 = glm::vec4(bladePosition, direction);

            // Bezier point and height (v1)
            float height =
                MIN_HEIGHT + (generateRandomFloat() * (MAX_HEIGHT - MIN_HEIGHT));
            currentBlade.V1 = glm::vec4(bladePosition + bladeUp * height, height);

            // Physical model guide and width (v2)
            float width = MIN_WIDTH + (generateRandomFloat() * (MAX_WIDTH - MIN_WIDTH));
            currentBlade.V2 = glm::vec4(bladePosition + bladeUp * height, width);

            // Up vector and stiffness coefficient (up)
            float stiffness =
                MIN_BEND + (generateRandomFloat() * (MAX_BEND - MIN_BEND));
            currentBlade.Up = glm::vec4(bladeUp, stiffness);

            blades.push_back(currentBlade);
        }

        GrassBladeDrawIndirect indirectDraw;
        indirectDraw.VertexCount = m_NumBlades;
        indirectDraw.InstanceCount = 1;
        indirectDraw.FirstVertex = 0;
        indirectDraw.FirstInstance = 0;

        BladesBuffer = StorageBufferSet::Create(Buffer(blades.data(), blades.size() * sizeof(UBGrassBlade)));
        CulledBladesBuffer = StorageBufferSet::Create(blades.size() * sizeof(UBGrassBlade));
        NumBladesBuffer = StorageBufferSet::Create(Buffer(&indirectDraw,sizeof(GrassBladeDrawIndirect)));
	}
    GrassBladePlane::GrassBladePlane(const std::vector<UBGrassBlade>& blades)
    {
        m_NumBlades = blades.size();
		m_PlaneSize = 15.0f; // Default size, can be adjusted based on your needs


        GrassBladeDrawIndirect indirectDraw;
        indirectDraw.VertexCount = m_NumBlades;
        indirectDraw.InstanceCount = 1;
        indirectDraw.FirstVertex = 0;
        indirectDraw.FirstInstance = 0;

        BladesBuffer = StorageBufferSet::Create(Buffer(blades.data(), blades.size() * sizeof(UBGrassBlade)));
        CulledBladesBuffer = StorageBufferSet::Create(blades.size() * sizeof(UBGrassBlade));
        NumBladesBuffer = StorageBufferSet::Create(Buffer(&indirectDraw, sizeof(GrassBladeDrawIndirect)));
    }
    UBGrassBlade::UBGrassBlade(glm::vec3 position)
    {
        glm::vec3 bladeUp(0.0f, 1.0f, 0.0f);

        float direction = generateRandomFloat() * 2.f * 3.14159265f;
        V0 = glm::vec4(position, direction);

        // Bezier point and height (v1)
        float height =
            MIN_HEIGHT + (generateRandomFloat() * (MAX_HEIGHT - MIN_HEIGHT));
        V1 = glm::vec4(position + bladeUp * height, height);

        // Physical model guide and width (v2)
        float width = MIN_WIDTH + (generateRandomFloat() * (MAX_WIDTH - MIN_WIDTH));
        V2 = glm::vec4(position + bladeUp * height, width);

        // Up vector and stiffness coefficient (up)
        float stiffness =
            MIN_BEND + (generateRandomFloat() * (MAX_BEND - MIN_BEND));
        Up = glm::vec4(bladeUp, stiffness);
    }
}
