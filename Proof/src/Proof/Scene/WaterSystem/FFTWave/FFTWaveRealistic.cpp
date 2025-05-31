#include "Proofprch.h"
#include "FFTWaveRealistic.h"
#include "Proof/Math/Math.h"

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
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/SceneUtils.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include "Proof/Math/Random.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/MeshImpoter.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Asset/AssetManager.h"
#include "FFTClipMap.h"

#include "Proof/Platform/Vulkan/Vulkan.h"
#include "Proof/Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
#include "Proof/Scene/SceneUtils.h"
namespace Proof
{

#if 0
    Count<Image2D> CreateRenderImage(std::string debugName,uint32_t size, ImageFormat format = ImageFormat::RG32F, bool useMips = false)
    {
        // 1. Create the sampler config (equivalent to Unity settings)
        SamplerResourceConfig samplerConfig;
        samplerConfig.DebugName = "RenderTextureSampler";
        samplerConfig.Wrap = SamplerWrap::Repeat; // TextureWrapMode.Repeat
        samplerConfig.Filter = SamplerFilterMetaData(SamplerFilter::Linear, SamplerFilter::Linear); // Trilinear
        samplerConfig.MipMapMode = SamplerMipMapMode::Linear;
        samplerConfig.Anisotropy = 6.0f;

        // 2. Create the sampler object
        //Count<RenderSampler> sampler = RenderSampler::Create(samplerConfig);

        // 3. Create the render texture
        ImageConfiguration spec;
        spec.Width = size;
        spec.Height = size;
        spec.Format = format;
        if (useMips)
        {
            spec.Mips = Utils::GetMipLevelCount(size, size);
        }
        spec.Usage = ImageUsage::Storage;

        Count<Image2D> renderTexture = Image2D::Create(spec,SamplerFactory::GetTrilinear());

        return renderTexture;
    }

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

    Count<Image2D> CreateRenderImage(std::string debugName, uint32_t width, uint32_t height, ImageFormat  format, uint32_t numLayers = 1, SamplerWrap wrap = SamplerWrap::Repeat, SamplerFilter filter = SamplerFilter::Linear)
    {
        ImageConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;
        rt.Usage = ImageUsage::Storage;
        rt.Layers = numLayers;
        rt.Transfer = true;
        rt.Mips = Utils::GetMipLevelCount(width, height);

        auto tex = Image2D::Create(rt, SamplerFactory::GetTrilinear());
        return tex;
    }
#endif

    Count<Image2D> CreateStorageTexture(std::string debugName, uint32_t width, uint32_t height, ImageFormat  format, SamplerFilter filter = SamplerFilter::Nearest, bool generateMips = false, SamplerWrap wrap = SamplerWrap::Repeat)
    {

        ImageConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;
        rt.Usage = ImageUsage::Storage;
        rt.Transfer = true;
        if(generateMips)
            rt.Mips = Utils::GetMipLevelCount(width, height);

        auto tex = Image2D::Create(rt,wrap,filter);
        return tex;
    }


    const uint32_t Size = 256;
    static float g = 9.81f;
    void FFTWaveRealisticCascade::FillSettingsStruct(const DisplaySpectrumSettings& display, UBSpectrumSettings& settings)
    {
        settings.Scale = display.Scale;
        settings.Angle = static_cast<float>(display.WindDirection) / 180.0f * Math::PIE();
        settings.SpreadBlend = display.SpreadBlend;
        settings.Swell = std::max(0.01f, static_cast<float>(display.Swell));
        settings.Alpha = JonswapAlpha(g, display.Fetch, display.WindSpeed);
        settings.PeakOmega = JonswapPeakFrequency(g, display.Fetch, display.WindSpeed);
        settings.Gamma = display.PeakEnhancement;
        settings.ShortWavesFade = display.ShortWavesFade;
    }
    FFTWaveRealisticCascade::FFTWaveRealisticCascade(uint32_t cascadeIndex, uint32_t size, Count<Texture2D> noiseTexture)
        :
        m_InitialSpectrumContainer(noiseTexture,cascadeIndex)
    {
        m_CascadeIndex = cascadeIndex;
        
        m_TimeDependentSpectrum = ComputePass::Create(fmt::format("FFT time dpendent cascade {}",cascadeIndex),Renderer::GetShader("FFTTimeDependentSpectrum"));

        m_CascadeBufferMap = CreateStorageTexture(fmt::format("FFT Buffer map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RG32F);
        
        m_DxDz = CreateStorageTexture(fmt::format("Cascade{} DxDz", cascadeIndex), size, size, ImageFormat::RG32F);
        m_DyDxz = CreateStorageTexture(fmt::format("Cascade{} DyDxz", cascadeIndex), size, size, ImageFormat::RG32F);
        m_DyxDyz = CreateStorageTexture(fmt::format("Cascade{} DyxDyz", cascadeIndex), size, size, ImageFormat::RG32F);
        m_DxxDzz  = CreateStorageTexture(fmt::format("Cascade{} DxxDzz ", cascadeIndex), size, size, ImageFormat::RG32F);


        m_TimeDependentSpectrum->SetInput("H0", m_InitialSpectrumContainer.m_InitialSpectrumMap);
        m_TimeDependentSpectrum->SetInput("WavesData", m_InitialSpectrumContainer.m_PrecomputedData);

        m_TimeDependentSpectrum->SetInput("DxDz", m_DxDz);
		m_TimeDependentSpectrum->SetInput("DyDxz", m_DyDxz);
		m_TimeDependentSpectrum->SetInput("DyxDyz", m_DyxDyz);
		m_TimeDependentSpectrum->SetInput("DxxDzz", m_DxxDzz);


        m_DisplacementMap = CreateStorageTexture(fmt::format("FFT Displacement map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F, SamplerFilter::Linear);
		m_DerivativesMap = CreateStorageTexture(fmt::format("FFT Derivatives map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F, SamplerFilter::Linear,true);
		m_TurbulenceMap = CreateStorageTexture(fmt::format("FFT Turbulence map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F, SamplerFilter::Linear, true);
		m_Turbulence2Map = CreateStorageTexture(fmt::format("FFT Turbulence2 map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F, SamplerFilter::Linear, true);

		m_TextureMergePass = ComputePass::Create(fmt::format("FFT Texture merge cascade {}",cascadeIndex),Renderer::GetShader("FFTTextureMerger"));
        m_TextureMergePass->SetInput("Displacement", m_DisplacementMap);
        m_TextureMergePass->SetInput("Derivatives", m_DerivativesMap);

        m_TextureMergePass->SetInput("Dx_Dz", m_DxDz);
        m_TextureMergePass->SetInput("Dy_Dxz", m_DyDxz);
        m_TextureMergePass->SetInput("Dyx_Dyz", m_DyxDyz);
        m_TextureMergePass->SetInput("Dxx_Dzz", m_DxxDzz);


    }
   
    void ReesourceWait(Count<Image> imageToWait, Count<RenderCommandBuffer> cmdBuffer)
    {

        Renderer::Submit([image = imageToWait.As<VulkanImage2D>(), commandBuffer = cmdBuffer]
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
    }
    
    void FFTWaveRealisticCascade::CalculateWavesAtTime(Count<RenderCommandBuffer> cmdBuffer, Count<FFTWaveRealistic> wave,float time)
    {
        const uint32_t WorkGroup = 8;
        //time dependent
        {
            Renderer::BeginComputePass(cmdBuffer, m_TimeDependentSpectrum);
            m_TimeDependentSpectrum->PushData(Buffer(&time, sizeof(float)));
            m_TimeDependentSpectrum->Dispatch({ Size/ WorkGroup, Size / WorkGroup,1 });
            Renderer::EndComputePass(m_TimeDependentSpectrum);
        }

        // calculate IFFTs of complex apmpatudes
        {
            wave->IFFT2D(m_DxDz, m_CascadeBufferMap);
			wave->IFFT2D(m_DyDxz, m_CascadeBufferMap);  
			wave->IFFT2D(m_DyxDyz, m_CascadeBufferMap);
			wave->IFFT2D(m_DxxDzz, m_CascadeBufferMap);
        }

        float deltaTime = FrameTime::GetWorldDeltaTime() / 1000;

        if (deltaTime > 0.5) 
        {
            // avoid too big delta time
            deltaTime = 0.5;
        }

        {
			m_PingPongTurbulence = !m_PingPongTurbulence;

            m_TextureMergePass->SetInput("TurbulenceRead", m_PingPongTurbulence ? m_TurbulenceMap : m_Turbulence2Map);
            m_TextureMergePass->SetInput("TurbulenceWrite", m_PingPongTurbulence ? m_TurbulenceMap : m_Turbulence2Map);


            Renderer::BeginComputePass(cmdBuffer, m_TextureMergePass);
            glm::vec2 data = {wave->Settings.lambda,deltaTime };
            m_TextureMergePass->PushData(Buffer(&data, sizeof(data)));
            m_TextureMergePass->Dispatch({ Size/ WorkGroup ,Size / WorkGroup,1 });
            Renderer::EndComputePass(m_TextureMergePass);
        }

        m_DerivativesMap->GenerateMips();

        {
			Count<Image2D> pingPongTurbulence = m_PingPongTurbulence ? m_TurbulenceMap : m_Turbulence2Map;
            pingPongTurbulence->GenerateMips();
        }
    }


    struct UBCascadeSettings
    {
        uint32_t Size;
        float LengthScale;
        float CutoffHigh;
        float CutoffLow;
        float GravityAcceleration;
        float Depth;
    };

    FFTWaveRealisticCascade::InitialSpectrumContainer::InitialSpectrumContainer(Count<Texture2D> noiseTexture, uint32_t cascadeIndex)
    {
        m_InitialSpectrumPass = ComputePass::Create(fmt::format("FFT Initial specturm cascade {}",cascadeIndex),Renderer::GetShader("FFTInitialSpectrum"));

        m_InitialSpectrumMap = CreateStorageTexture(fmt::format("FFT initial spectrum Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F);
        m_PrecomputedData = CreateStorageTexture(fmt::format("FFT Precompute Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F);
        m_BufferMap = CreateStorageTexture(fmt::format("FFT Buffer map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RG32F);

        m_SpectrumParameters = UniformBufferSet::Create(sizeof(UBSpectrumSettings) * 2); // 2 of these
        m_ParamBuffer = UniformBufferSet::Create(sizeof(UBCascadeSettings));
        
        m_InitialSpectrumPass->SetInput("WavesData", m_PrecomputedData);
        m_InitialSpectrumPass->SetInput("H0K", m_BufferMap);
        m_InitialSpectrumPass->SetInput("Noise", noiseTexture);
        m_InitialSpectrumPass->SetInput("SpectrumParams", m_SpectrumParameters);
        m_InitialSpectrumPass->SetInput("Params", m_ParamBuffer);

        m_ConjuagatedSpectrumPass = ComputePass::Create(fmt::format("FFT Conjugated spectrum cascade {}",cascadeIndex),Renderer::GetShader("FFTConjugatedSpectrum"));

        m_ConjuagatedSpectrumPass->SetInput("H0", m_InitialSpectrumMap);
        m_ConjuagatedSpectrumPass->SetInput("Params", m_ParamBuffer);
        m_ConjuagatedSpectrumPass->SetInput("H0K", m_BufferMap);

    }


    void FFTWaveRealisticCascade::InitialSpectrumContainer::Generate(uint32_t lengthScale, uint32_t cutOfflow, uint32_t CutOffHigh, Count<RenderCommandBuffer> cmdBuffer)
    {
        const uint32_t WorkGroup = 8;

        UBCascadeSettings settings;
        settings.Size = Size;
        settings.LengthScale = lengthScale;
        settings.CutoffLow = cutOfflow;
        settings.CutoffHigh = CutOffHigh;
        settings.GravityAcceleration = g;
        settings.Depth = 3;

        m_ParamBuffer->SetData(Renderer::GetCurrentFrameInFlight(), Buffer(&settings, sizeof(settings)));


        Renderer::BeginComputePass(cmdBuffer, m_InitialSpectrumPass);
        m_InitialSpectrumPass->Dispatch({ Size / WorkGroup ,Size / WorkGroup,1 });
        Renderer::EndComputePass(m_InitialSpectrumPass);

        Renderer::BeginComputePass(cmdBuffer, m_ConjuagatedSpectrumPass);
        m_InitialSpectrumPass->Dispatch({ Size / WorkGroup ,Size / WorkGroup,1 });
        Renderer::EndComputePass(m_ConjuagatedSpectrumPass);

    }
    FFTWaveRealistic::FFTWaveRealistic(Water* water)
        : Wave(water, WaveType::RealisticFastFourierTransformWave)
    {
        Init();

    }
    FFTWaveRealistic::FFTWaveRealistic(Water* water, Count<FFTWaveRealistic> other)
        : Wave(water, WaveType::RealisticFastFourierTransformWave)
    {
        Init();
    }
    FFTWaveRealistic::~FFTWaveRealistic()
    {
    }
    void FFTWaveRealistic::Render(Count<class WorldRenderer> renderer)
    {
        Renderer::BeginCommandBuffer(m_CommandBuffer);
        if (m_InitilizedPrecompute == false)
        {
            Renderer::BeginComputePass(Renderer::GetRendererCommandBuffer(), m_TwiddleFacorsPass);
            {
                glm::uvec2 data{ 1,Size };
                m_TwiddleFacorsPass->PushData("params", &data);
            }
            const int logSize = static_cast<int>(glm::log2((float)Size));
            const uint32_t WorkGroup = 8;

            m_TwiddleFacorsPass->Dispatch({ logSize / WorkGroup,Size / 2 / WorkGroup,1 });

            Renderer::EndComputePass(m_TwiddleFacorsPass);
            m_InitilizedPrecompute = true;
        }
        for(auto cascade : m_Cascades)
          cascade->CalculateWavesAtTime(m_CommandBuffer,Count<FFTWaveRealistic>(this), FrameTime::GetTime());

        Renderer::EndCommandBuffer(m_CommandBuffer);
        Renderer::SubmitCommandBuffer(m_CommandBuffer);

    }
    void FFTWaveRealistic::IFFT2D(Count<Image2D> inputImage, Count<Image2D> bufferImage)
    {
        const uint32_t WorkGroup = 8;

        const int logSize = static_cast<int>(glm::log2((float)Size));

        bool pingPong = false;
		// horizontal pass
        {
            Renderer::BeginComputePass(m_CommandBuffer, m_HorizontalStep[0]);
            for (int i = 0; i < logSize; i++)
            {
                pingPong = !pingPong;
                glm::uvec2 data{ i,Size };

                m_HorizontalStep[0]->PushData("params", &data);
				m_HorizontalStep[0]->SetInput("InputBuffer", pingPong ? inputImage : bufferImage);
				m_HorizontalStep[0]->SetInput("OutputBuffer", pingPong ? bufferImage : inputImage);

				m_HorizontalStep[0]->Dispatch({ Size / WorkGroup ,Size / WorkGroup,1 });

				ReesourceWait(pingPong ? bufferImage : inputImage, m_CommandBuffer);    
            }

            Renderer::EndComputePass(m_HorizontalStep[0]);
        }

        // vertical pass
        {
            Renderer::BeginComputePass(m_CommandBuffer, m_VerticalStep[0]);
            for (int i = 0; i < logSize; i++)
            {
                pingPong = !pingPong;
                glm::uvec2 data{ i,Size };

                m_VerticalStep[0]->PushData("params", &data);
                m_VerticalStep[0]->SetInput("InputBuffer", pingPong ? inputImage : bufferImage);
                m_VerticalStep[0]->SetInput("OutputBuffer", pingPong ? bufferImage : inputImage);

                m_VerticalStep[0]->Dispatch({ Size / WorkGroup ,Size / WorkGroup,1 });

                ReesourceWait(pingPong ? bufferImage : inputImage, m_CommandBuffer);
            }

            Renderer::EndComputePass(m_VerticalStep[0]);
        }

        if(pingPong)
			Renderer::CopyImage(m_CommandBuffer, bufferImage,inputImage);

		m_PermuteStep->SetInput("InputBuffer", inputImage);
		m_PermuteStep->SetInput("OutputBuffer", bufferImage);   
        Renderer::BeginComputePass(m_CommandBuffer, m_PermuteStep);

		m_PermuteStep->Dispatch({ Size / WorkGroup ,Size / WorkGroup,1 });
        Renderer::EndComputePass(m_PermuteStep);

		Renderer::CopyImage(m_CommandBuffer, bufferImage, inputImage);
    }

    float NormalRandom()
    {
        return glm::cos(2 * glm::pi<float>() * Random::Real<float>(0,1)) * glm::sqrt(-2 * glm::log(Random::Real<float>(0, 1)));
    }
    void FFTWaveRealistic::Init()
    {
        m_InitilizedPrecompute = false;
        // noise Texture
        {
			TextureConfiguration noiseTexConfig;
            noiseTexConfig.DebugName = "FFTWave NoiseTextureConfig";
            noiseTexConfig.Width = Size;
			noiseTexConfig.Height = Size;
			noiseTexConfig.Format = ImageFormat::RG32F;
            noiseTexConfig.Storage = true;

			Buffer noiseData(Utils::GetImageMemorySize(noiseTexConfig.Format,Size,Size));
            float* floatData = reinterpret_cast<float*>(noiseData.Data);
            for (int j = 0; j < Size; ++j) {
                for (int i = 0; i < Size; ++i) {
                    int index = (j * Size + i) * 2;
                    floatData[index + 0] = NormalRandom(); // R
                    floatData[index + 1] = NormalRandom(); // G
                }
            }

			m_NoiseTexture = Texture2D::Create(noiseTexConfig, noiseData,SamplerFactory::GetPoint());
            noiseData.Release();

        }
		m_TwiddleFacorsPass = ComputePass::Create("FFT Twiddle Factors Pass", Renderer::GetShader("FFTPrecomputeTwiddleFactorsAndInputIndices"));


       const int logSize = static_cast<int>(glm::log2((float)Size));
       const uint32_t WorkGroup = 8;
		m_CommandBuffer = RenderCommandBuffer::Create("FFT Wave Realistic Command Buffer");
#if 0
        Renderer::BeginComputePass(Renderer::GetRendererCommandBuffer(), m_TwiddleFacorsPass);
        {
            glm::uvec2 data{ 1,Size };
		    m_TwiddleFacorsPass->PushData("params", &data);
        }

        m_TwiddleFacorsPass->Dispatch({ logSize/ WorkGroup,Size/ 2 / WorkGroup,1 });

        Renderer::EndComputePass(m_TwiddleFacorsPass);
#endif

        m_PrcomuteData = CreateStorageTexture("FFT Precompute Data", logSize, Size, ImageFormat::RGBA32F);

        m_TwiddleFacorsPass->SetInput("PrecomputeBuffer", m_PrcomuteData);


        for (int i = 0; i < 2; ++i)
        {
            m_HorizontalStep[i] = ComputePass::Create("FFT Horizontal Step", Renderer::GetShader("FFTHorizontalStepinverse"));
            m_HorizontalStep[i]->SetInput("PrecomputedData", m_PrcomuteData);

            m_VerticalStep[i] = ComputePass::Create("FFT Vertical Step", Renderer::GetShader("FFTVerticalStepInverse"));
            m_VerticalStep[i]->SetInput("PrecomputedData", m_PrcomuteData);

        }

        m_PermuteStep = ComputePass::Create("FFT Permute Pass", Renderer::GetShader("FFTPermute"));


        m_Cascades.resize(3);

        for (int i = 0; i < m_Cascades.size(); i++)
        {
            if(m_Cascades[i] == nullptr)
				m_Cascades[i] = Count<FFTWaveRealisticCascade>::Create(i, Size, m_NoiseTexture);
        }

    }
}
