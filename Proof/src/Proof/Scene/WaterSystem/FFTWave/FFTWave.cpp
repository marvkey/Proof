#include "Proofprch.h"
#include "FFTWave.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RendererSampler.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/ComputePass.h"
#include "Proof/Renderer/ComputePipeline.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/SceneUtils.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include "Proof/Math/Random.h"
#include "Proof/Math/Math.h"

#include "Proof/Platform/Vulkan/Vulkan.h"
#include "Proof/Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
namespace Proof
{

#define GRID_DIM 1024
#define RESOLUTION 512
#define WORK_GROUP_DIM 32

    Count<Texture2D> CreateRenderTex(std::string debugName, uint32_t width, uint32_t height, uint32_t depth, ImageFormat  format, bool useMips) {

        TextureConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;
        rt.Storage = true;
        rt.GenerateMips = useMips;
        rt.Depth = depth;

        auto tex = Texture2D::Create(rt, SamplerWrap::ClampBorder, SamplerFilter::Linear);
        return tex;
    }

    Count<Texture2D> CreateRenderTex(std::string debugName, uint32_t width, uint32_t height, ImageFormat  format, bool useMips, SamplerWrap wrap = SamplerWrap::ClampBorder, SamplerFilter filter = SamplerFilter::Linear)
    {
        TextureConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;
        rt.Storage = true;
        rt.GenerateMips = useMips;


        auto tex = Texture2D::Create(rt, wrap, filter);
        return tex;
    }
    FFTWave::FFTWave(Count<FFTWave> other)
        : Wave(other->m_Water, WaveType::FastFourierTransformWave)
    {
        InitTextures();
        InitPasses();
    }
    FFTWave::FFTWave(Count<class Water> water)
        : Wave(water, WaveType::FastFourierTransformWave)
    {
        InitTextures();
        InitPasses();
    }
    void FFTWave::Update(float deltaTime)
    {
        PF_PROFILE_FUNC();

    }

    void FFTWave::Render(Count<class WorldRenderer> renderer)
    {
        PF_PROFILE_FUNC();

        Renderer::BeginCommandBuffer(m_CommandBuffer);
        // Initial spectrum

        {
            PF_PROFILE_SCOPE_DYNAMIC("Initial Spectrum");

            Renderer::BeginComputePass(m_CommandBuffer, m_InitialSpectrumPass);

            struct InitialSpectrumPushData
            {
                int Resolution;
                int OceanSize;
                glm::vec2 wind;
            } Data;

            Data.Resolution = RESOLUTION;
            Data.OceanSize = OceanSize;
            Data.wind = glm::vec2(WaveInfo.WindMagnitude * glm::cos(WaveInfo.windAngle), WaveInfo.WindMagnitude* glm::sin(WaveInfo.windAngle));

            m_InitialSpectrumPass->PushData("u_PC", &Data);
            m_InitialSpectrumPass->Dispatch(RESOLUTION / WORK_GROUP_DIM, RESOLUTION / WORK_GROUP_DIM, 1);
            Renderer::EndComputePass(m_InitialSpectrumPass);

        }

        //phase
        {
            PF_PROFILE_SCOPE_DYNAMIC("Phase");

            Renderer::BeginComputePass(m_CommandBuffer, m_PhasePass);

            struct PhasePushData
            {
                float DeltaTime;  // Time step for phase update
                int OceanSize;    // Physical size of ocean grid
                int Resolution;   // Grid resolution (number of points per axis)
            } Data;
            Data.Resolution = RESOLUTION;
            Data.OceanSize = OceanSize;
            Data.DeltaTime = FrameTime::GetWorldDeltaTime();

            if (m_IsInPingPhase)
            {
                m_PhasePass->SetInput("u_Phases", m_PingPhaseTexture);
                m_PhasePass->SetInput("o_DeltaPhases", m_PongPhaseTexture);

            }
            else
            {
                m_PhasePass->SetInput("u_Phases", m_PongPhaseTexture);
                m_PhasePass->SetInput("o_DeltaPhases", m_PingPhaseTexture);
            }

            m_PhasePass->PushData("u_PC", &Data);
            m_PhasePass->Dispatch(RESOLUTION / WORK_GROUP_DIM, RESOLUTION / WORK_GROUP_DIM, 1);

            Renderer::EndComputePass(m_PhasePass);
        }

        // spectrum
        {
            PF_PROFILE_SCOPE_DYNAMIC("Spectrum");

            Renderer::BeginComputePass(m_CommandBuffer, m_SpectrumPass);
            struct PhasePushData
            {
                int OceanSize;    // Physical size of the ocean grid
                float Choppiness; // Controls wave steepness and horizontal displacement
            } Data;
            Data.OceanSize = OceanSize;
            Data.Choppiness  = WaveInfo.Choppiness;


            if (m_IsInPingPhase)
                m_SpectrumPass->SetInput("u_Phases", m_PongPhaseTexture);
            else
                m_SpectrumPass->SetInput("u_Phases", m_PingPhaseTexture);

            m_SpectrumPass->PushData("u_PC", &Data);
            m_SpectrumPass->Dispatch(RESOLUTION / WORK_GROUP_DIM, RESOLUTION / WORK_GROUP_DIM, 1);

            Renderer::EndComputePass(m_SpectrumPass);

        }

        // Horizontal and vertical

        {
            PF_PROFILE_SCOPE_DYNAMIC("HorizontalAndVertical");

            bool tempAsInput = false; // Should you use temp_texture as input to the FFT shader?
            // horizontal 
            {
                PF_PROFILE_SCOPE_DYNAMIC("Horizontal");

                Renderer::BeginComputePass(m_CommandBuffer, m_HorizontalPass);

                struct HorizontalPushData
                {
                    int TotalCount;  // Total number of elements in FFT
                    int SubseqCount; // Sub-sequence count for butterfly operation
                } Data;

                Data.TotalCount = RESOLUTION;
                for (int p = 1; p < RESOLUTION; p <<= 1)
                {

                    if (tempAsInput)
                    {
                        m_HorizontalPass->SetInput("u_Input", m_TempTexture);
                        m_HorizontalPass->SetInput("u_Output", m_SpectrumTexture);
                    }
                    else
                    {
                        m_HorizontalPass->SetInput("u_Input", m_SpectrumTexture);
                        m_HorizontalPass->SetInput("u_Output", m_TempTexture);
                    }
                    Data.SubseqCount = p;
                    m_HorizontalPass->PushData("u_PC", &Data);
                    m_HorizontalPass->Dispatch(RESOLUTION, 1, 1);


                    Renderer::Submit([image = tempAsInput ? m_SpectrumTexture->GetImage().As<VulkanImage2D>() : m_TempTexture->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer]
                        {
                            VkImageMemoryBarrier imageMemoryBarrier = {};
                            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
                            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
                            imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
                            imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1 };
                            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
                            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                            vkCmdPipelineBarrier(
                                commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0,
                                0, nullptr,
                                0, nullptr,
                                1, &imageMemoryBarrier);
                        });

                    tempAsInput = !tempAsInput;

                }

                Renderer::EndComputePass(m_HorizontalPass);

            }

            //vertical
            {
                PF_PROFILE_SCOPE_DYNAMIC("Vertical");


                Renderer::BeginComputePass(m_CommandBuffer, m_VerticalPass);

                struct VerticalPushData
                {
                    int TotalCount;  // Total number of elements in FFT
                    int SubseqCount; // Sub-sequence count for butterfly operation
                } Data;

                Data.TotalCount = RESOLUTION;
                for (int p = 1; p < RESOLUTION; p <<= 1)
                {

                    if (tempAsInput)
                    {
                        m_VerticalPass->SetInput("u_Input", m_TempTexture);
                        m_VerticalPass->SetInput("u_Output", m_SpectrumTexture);
                    }
                    else
                    {
                        m_VerticalPass->SetInput("u_Input", m_SpectrumTexture);
                        m_VerticalPass->SetInput("u_Output", m_TempTexture);
                    }
                    Data.SubseqCount = p;
                    m_VerticalPass->PushData("u_PC", &Data);
                    m_VerticalPass->Dispatch(RESOLUTION, 1, 1);

                    Renderer::Submit([image = tempAsInput ? m_SpectrumTexture->GetImage().As<VulkanImage2D>() : m_TempTexture->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer]
                        {
                            VkImageMemoryBarrier imageMemoryBarrier = {};
                            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
                            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
                            imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
                            imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1 };
                            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
                            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

                            vkCmdPipelineBarrier(
                                commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0,
                                0, nullptr,
                                0, nullptr,
                                1, &imageMemoryBarrier);
                        });

                    tempAsInput = !tempAsInput;
                }

                Renderer::EndComputePass(m_VerticalPass);

            }
   
        }

        // Normal Map
        {
            PF_PROFILE_SCOPE_DYNAMIC("NormalMap");

            Renderer::BeginComputePass(m_CommandBuffer, m_NormalMapPass);
            struct PhasePushData
            {
                int Resolution;  // Grid resolution (number of points per axis)
                int OceanSize;   // Physical size of the ocean grid
            } Data;
            Data.OceanSize = OceanSize;
            Data.Resolution = RESOLUTION;


            m_NormalMapPass->PushData("u_PC", &Data);
            m_NormalMapPass->Dispatch(RESOLUTION / WORK_GROUP_DIM, RESOLUTION / WORK_GROUP_DIM, 1);

            Renderer::EndComputePass(m_NormalMapPass);
        }


        m_IsInPingPhase = !m_IsInPingPhase;

        Renderer::EndCommandBuffer(m_CommandBuffer);
        Renderer::SubmitCommandBuffer(m_CommandBuffer);

    }

    void FFTWave::InitPasses()
	{
        PF_PROFILE_FUNC();
        m_CommandBuffer = RenderCommandBuffer::Create("FFTCommandBuffer");

      //  m_Material = Count<Material>::Create("FFTWave",Renderer::GetShader("FFTWave"));

        // Initial Spectrum
        m_InitialSpectrumPass = ComputePass::Create({ "FFTInitialSpectrum",ComputePipeline::Create({"FFTInitialSpectrum",Renderer::GetShader("FFTInitialSpectrum")})});
       
        m_InitialSpectrumPass->SetInput("o_InitialSpectrum", m_InitialSpectrumTexture);
      
        // phase
        m_PhasePass = ComputePass::Create({ "FFTPhase",ComputePipeline::Create({"FFTPhase",Renderer::GetShader("FFTPhase")}) });
       
        // time varying spectrum
        m_SpectrumPass = ComputePass::Create({ "FFTSpectrum",ComputePipeline::Create({"FFTSpectrum",Renderer::GetShader("FFTSpectrum")}) });

        m_SpectrumPass->SetInput("u_InitialSpectrum", m_InitialSpectrumTexture);
        m_SpectrumPass->SetInput("o_Spectrum", m_SpectrumTexture);

        // horizontal and vertical
        m_HorizontalPass = ComputePass::Create({ "FFTHorizontal",ComputePipeline::Create({"FFTHorizontal",Renderer::GetShader("FFTHorizontal")}) });
        m_VerticalPass = ComputePass::Create({ "FFTVertical",ComputePipeline::Create({"FFTVertical",Renderer::GetShader("FFTVertical")}) });

        //normal
        m_NormalMapPass = ComputePass::Create({ "FFTNormalMap",ComputePipeline::Create({"FFTNormalMap",Renderer::GetShader("FFTNormalMap")}) });

        m_NormalMapPass->SetInput("u_DisplacementMap", m_SpectrumTexture);
        m_NormalMapPass->SetInput("o_NormalMap", m_NormalTexture);
	}
    void FFTWave::InitTextures()
    {
        m_InitialSpectrumTexture = CreateRenderTex("FFTInitialSpectrum", RESOLUTION, RESOLUTION, ImageFormat::R32F, false);
        // phase
        std::vector<float> ping_phase_array(RESOLUTION * RESOLUTION);

        // lop over the 2D grid and fills it with random values based on [0,2pi]
        for (size_t i = 0; i < RESOLUTION * RESOLUTION; ++i)
            ping_phase_array[i] = Random::Real<float>(0, 1) * 2.f * Math::PIE();

        {
            TextureConfiguration config;
            config.DebugName = "FFTPing";
            config.Format = ImageFormat::R32F;
            config.Width = RESOLUTION;
            config.Height = RESOLUTION;
            config.Storage = true;


            m_PingPhaseTexture = Texture2D::Create(config, SamplerWrap::ClampBorder, SamplerFilter::Nearest);
        }

        m_PongPhaseTexture = CreateRenderTex("FFTPong", RESOLUTION, RESOLUTION, ImageFormat::R32F, false);
        // time varying spectrum 
        m_SpectrumTexture = CreateRenderTex("FFTSpectrum", RESOLUTION, RESOLUTION, ImageFormat::RGBA32F, false, SamplerWrap::Repeat, SamplerFilter::Linear);

        // horizontal and vertical 
        m_TempTexture = CreateRenderTex("FFTTemp", RESOLUTION, RESOLUTION, ImageFormat::RGBA32F, false);

        // normal map
        m_NormalTexture = CreateRenderTex("FFTNormalMap", RESOLUTION, RESOLUTION, ImageFormat::RGBA32F, false, SamplerWrap::Repeat, SamplerFilter::Linear);

    }

    Count<class RenderMaterial> FFTWave::GetRenderMaterial()
    {
        return NULL;
        //return m_Material->GetRenderMaterial();
    }
}
