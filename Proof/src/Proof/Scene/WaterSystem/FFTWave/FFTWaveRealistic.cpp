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
#include "Proof/Renderer/RenderPass.h"
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
#include "Proof/Renderer/SamplerFactory.h"
#include "Proof/Renderer/RendererSampler.h"
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
    Count<Image2D> CreateStorageTextureSampler(std::string debugName, uint32_t width, uint32_t height, ImageFormat  format, Count<RenderSampler> sampler,bool generateMips = false)
    {

        ImageConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;
        rt.Usage = ImageUsage::Storage;
        rt.Transfer = true;
        if (generateMips)
            rt.Mips = Utils::GetMipLevelCount(width, height);

        auto tex = Image2D::Create(rt, sampler);
        return tex;
    }

    const uint32_t Size = 256;
    static float G = 9.81f;

    static float JonswapAlpha(float g, float fetch, float windSpeed) 
    {
        return 0.076f * std::pow(g * fetch / windSpeed / windSpeed, -0.22f);
    }

    static float JonswapPeakFrequency(float g, float fetch, float windSpeed) 
    {
        return 22.0f * std::pow(windSpeed * fetch / g / g, -0.33f);
    }

    void FFTWaveRealisticCascade::FillSettingsStruct(const DisplaySpectrumSettings& display, UBSpectrumSettings& settings)
    {
        settings.Scale = display.Scale;
        settings.Angle = glm::radians((float)display.WindDirection);
        settings.SpreadBlend = display.SpreadBlend;
        settings.Swell = std::max(0.01f, static_cast<float>(display.Swell));
        settings.Alpha = JonswapAlpha(G, display.Fetch, display.WindSpeed);
        settings.PeakOmega = JonswapPeakFrequency(G, display.Fetch, display.WindSpeed);
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
        m_TimeDependentSpectrum->SetInput("WavesData", m_InitialSpectrumContainer.m_WavesData);

        m_TimeDependentSpectrum->SetInput("DxDz", m_DxDz);
		m_TimeDependentSpectrum->SetInput("DyDxz", m_DyDxz);
		m_TimeDependentSpectrum->SetInput("DyxDyz", m_DyxDyz);
		m_TimeDependentSpectrum->SetInput("DxxDzz", m_DxxDzz);

#if 0
        m_DisplacementMap = CreateStorageTexture(fmt::format("FFT Displacement map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFilter::Linear);
		m_DerivativesMap = CreateStorageTexture(fmt::format("FFT Derivatives map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFilter::Linear,true);
		m_TurbulenceMap = CreateStorageTexture(fmt::format("FFT Turbulence map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFilter::Linear, true);
		m_Turbulence2Map = CreateStorageTexture(fmt::format("FFT Turbulence2 map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFilter::Linear, true);
#else
        m_DisplacementMap = CreateStorageTextureSampler(fmt::format("FFT Displacement map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFactory::GetBilinear());
        m_DerivativesMap = CreateStorageTextureSampler(fmt::format("FFT Derivatives map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFactory::GetTrilinear(), true);
        m_TurbulenceMap = CreateStorageTextureSampler(fmt::format("FFT Turbulence map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFactory::GetTrilinear(), true);
        m_Turbulence2Map = CreateStorageTextureSampler(fmt::format("FFT Turbulence2 map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA16F, SamplerFactory::GetTrilinear(), true);
#endif
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
        // TODO PROBLEMIS IN ONE OF THESE FUNCITON
        const uint32_t WorkGroup = 8;
        //time dependent
        {
            time = FrameTime::GetTime();
            Renderer::BeginComputePass(cmdBuffer, m_TimeDependentSpectrum);
            m_TimeDependentSpectrum->PushData("u_PC", &time);
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

        float deltaTime = FrameTime::GetWorldDeltaTime();

        if (deltaTime > 0.5) 
        {
            // avoid too big delta time
           deltaTime = 0.5;
        }

        {
			m_PingPongTurbulence = !m_PingPongTurbulence;

            m_TextureMergePass->SetInput("TurbulenceRead", m_PingPongTurbulence ? m_TurbulenceMap : m_Turbulence2Map);
            m_TextureMergePass->SetInput("TurbulenceWrite", m_PingPongTurbulence ? m_Turbulence2Map : m_TurbulenceMap);


            Renderer::BeginComputePass(cmdBuffer, m_TextureMergePass);
            glm::vec2 data = {wave->Settings.lambda,deltaTime };
            m_TextureMergePass->PushData("u_PC",&data);
            m_TextureMergePass->Dispatch({ Size/ WorkGroup ,Size / WorkGroup,1 });
            Renderer::EndComputePass(m_TextureMergePass);
        }

        m_DerivativesMap->GenerateMips();

        {
			Count<Image2D> pingPongTurbulence = m_PingPongTurbulence ? m_TurbulenceMap : m_Turbulence2Map;
            pingPongTurbulence->GenerateMips();
        }
    }


    struct alignas(16) UBCascadeSettings
    {
        uint32_t Size;
        float LengthScale;
        float CutoffHigh;
        float CutoffLow;
        float GravityAcceleration;
        float Depth;
    };
    struct alignas(16) UBOceanSettings {
        glm::vec4 Color;         // _Color
        glm::vec4 FoamColor;     // _FoamColor
        glm::vec4 SSSColor;      // _SSSColor

        float SSSStrength;       // _SSSStrength
        float Roughness;         // _Roughness
        float RoughnessScale;    // _RoughnessScale
        float MaxGloss;          // _MaxGloss

        float FoamBiasLOD0;      // _FoamBiasLOD0
        float FoamBiasLOD1;      // _FoamBiasLOD1
        float FoamBiasLOD2;      // _FoamBiasLOD2
        float FoamScale;         // _FoamScale

        float ContactFoam;       // _ContactFoam
        float padding[3];        // pad to 16-byte alignment (std140 rules)
    };

    struct alignas(16) UBOceanParams {
        float LengthScale0;      // float
        float LengthScale1;
        float LengthScale2;
        float LOD_scale;

        float SSSBase;
        float SSSScale;
        float padding[2];        // pad to 16 bytes (std140)
    };
    FFTWaveRealisticCascade::InitialSpectrumContainer::InitialSpectrumContainer(Count<Texture2D> noiseTexture, uint32_t cascadeIndex)
    {
        m_InitialSpectrumPass = ComputePass::Create(fmt::format("FFT Initial specturm cascade {}",cascadeIndex),Renderer::GetShader("FFTInitialSpectrum"));

        m_InitialSpectrumMap = CreateStorageTexture(fmt::format("FFT initial spectrum Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F);
        m_WavesData = CreateStorageTexture(fmt::format("FFT Precompute Cascade {}", cascadeIndex), Size, Size, ImageFormat::RGBA32F);
        m_BufferMap = CreateStorageTexture(fmt::format("FFT Buffer map Cascade {}", cascadeIndex), Size, Size, ImageFormat::RG32F);

        m_SpectrumParameters = StorageBufferSet::Create(sizeof(UBSpectrumSettings) * 2); // 2 of these
        m_ParamBuffer = UniformBufferSet::Create(sizeof(UBCascadeSettings));
        
        m_InitialSpectrumPass->SetInput("WavesData", m_WavesData);
        m_InitialSpectrumPass->SetInput("H0K", m_BufferMap);
        m_InitialSpectrumPass->SetInput("Noise", noiseTexture);
        m_InitialSpectrumPass->SetInput("SpectrumParams", m_SpectrumParameters);
        m_InitialSpectrumPass->SetInput("Params", m_ParamBuffer);

        m_ConjuagatedSpectrumPass = ComputePass::Create(fmt::format("FFT Conjugated spectrum cascade {}",cascadeIndex),Renderer::GetShader("FFTConjugatedSpectrum"));

        m_ConjuagatedSpectrumPass->SetInput("H0", m_InitialSpectrumMap);
        m_ConjuagatedSpectrumPass->SetInput("Params", m_ParamBuffer);
        m_ConjuagatedSpectrumPass->SetInput("H0K", m_BufferMap);

    }

    std::vector<UBSpectrumSettings> contain(2);

    void FFTWaveRealisticCascade::InitialSpectrumContainer::Generate(Count<class FFTWaveRealistic> waveRealistic,uint32_t lengthScale, float cutOfflow, float CutOffHigh, Count<RenderCommandBuffer> cmdBuffer)
    {
        const uint32_t WorkGroup = 8;

        UBCascadeSettings settings;
        settings.Size = Size;
        settings.LengthScale = lengthScale;
        settings.CutoffLow = cutOfflow;
        settings.CutoffHigh = CutOffHigh;
        settings.GravityAcceleration = G;
        settings.Depth = 3;


        m_ParamBuffer->SetData(Renderer::GetCurrentFrameInFlight(), Buffer(&settings, sizeof(UBCascadeSettings)));

        {
            FFTWaveRealisticCascade::FillSettingsStruct(waveRealistic->Settings.Local, contain[0]);
            FFTWaveRealisticCascade::FillSettingsStruct(waveRealistic->Settings.Swell, contain[1]);


			m_SpectrumParameters->SetData(Renderer::GetCurrentFrameInFlight(), Buffer((void*)contain.data(), sizeof(UBSpectrumSettings) * contain.size()));
        }
        Renderer::BeginComputePass(cmdBuffer, m_InitialSpectrumPass);
        m_InitialSpectrumPass->Dispatch({ Size / WorkGroup ,Size / WorkGroup,1 });
        Renderer::EndComputePass(m_InitialSpectrumPass);

        Renderer::BeginComputePass(cmdBuffer, m_ConjuagatedSpectrumPass);
        m_ConjuagatedSpectrumPass->Dispatch({ Size / WorkGroup ,Size / WorkGroup,1 });
        Renderer::EndComputePass(m_ConjuagatedSpectrumPass);

    }
    FFTWaveRealistic::FFTWaveRealistic(Water* water)
        : Wave(water, WaveType::RealisticFastFourierTransformWave)
    {
        Init();

    }
	float startTime = 0.0f;
    FFTWaveRealistic::FFTWaveRealistic(Water* water, Count<FFTWaveRealistic> other)
        : Wave(water, WaveType::RealisticFastFourierTransformWave)
    {
        Init();
		startTime = FrameTime::GetTime() / 1000.0f;
    }
    FFTWaveRealistic::~FFTWaveRealistic()
    {
    }

    float CustomLog(float x, float base)
    {
        return glm::log(x) / glm::log(base);
    }
    void FFTWaveRealistic::Render(Count<class WorldRenderer> renderer)
    {
        Renderer::BeginCommandBuffer(m_CommandBuffer);
        //if (m_InitilizedPrecompute == false)
        {
            Renderer::BeginComputePass(Renderer::GetRendererCommandBuffer(), m_TwiddleFacorsPass);
            {
                glm::uvec2 data{ 1,Size };
                m_TwiddleFacorsPass->PushData("params", &data);
            }
            const int logSize = static_cast<int>(CustomLog(Size,2));
           

            const uint32_t WorkGroup = 8;

            m_TwiddleFacorsPass->Dispatch({ logSize ,Size / 2 / WorkGroup,1 });

            Renderer::EndComputePass(m_TwiddleFacorsPass);
            m_InitilizedPrecompute = true;
            std::array<uint32_t, 3> lengthScales = { 250,17,5 };

#if 0
            float boundary1 = 0.0001f;
            for (int i = 0; i < m_Cascades.size(); i++)
            {
                const float boundary2 = (float)i < (float)lengthScales.size() - 1.0f ? 2.0f * glm::pi<float>() / lengthScales[i + 1] * 6.0f : 9999.0f;
                m_Cascades[i]->m_InitialSpectrumContainer.Generate(Count<FFTWaveRealistic>(this), lengthScales[i], boundary1, boundary2, m_CommandBuffer);
                boundary1 = boundary2;
            }

#else
            float boundary1 = 2 * Math::PIE() / lengthScales[1] * 6.0f;
            float boundary2 = 2 * Math::PIE() / lengthScales[2] * 6.0f;
            m_Cascades[0]->m_InitialSpectrumContainer.Generate(Count<FFTWaveRealistic>(this), lengthScales[0], 0.0001f, boundary1,m_CommandBuffer);
            m_Cascades[1]->m_InitialSpectrumContainer.Generate(Count<FFTWaveRealistic>(this), lengthScales[1], boundary1, boundary2, m_CommandBuffer);
            m_Cascades[2]->m_InitialSpectrumContainer.Generate(Count<FFTWaveRealistic>(this), lengthScales[2], boundary2, 9999, m_CommandBuffer);
#endif
        }


        

        for(Count<FFTWaveRealisticCascade> cascade : m_Cascades)
          cascade->CalculateWavesAtTime(m_CommandBuffer,Count<FFTWaveRealistic>(this), (FrameTime::GetTime()/1000.0f) - startTime);



        Renderer::EndCommandBuffer(m_CommandBuffer);
        Renderer::SubmitCommandBuffer(m_CommandBuffer);

        {

            m_RenderMaterial->Set("f_Turbulence_c0", m_Cascades[0]->GetActiveTurbulence());
            m_RenderMaterial->Set("f_Turbulence_c1", m_Cascades[1]->GetActiveTurbulence());
            m_RenderMaterial->Set("f_Turbulence_c2", m_Cascades[2]->GetActiveTurbulence());

            {
                UBOceanParams oceanParams;
                oceanParams.LengthScale0 = m_Cascades[0]->CascadeSettings.Length;
                oceanParams.LengthScale1 = m_Cascades[1]->CascadeSettings.Length;
                oceanParams.LengthScale2 = m_Cascades[2]->CascadeSettings.Length;

                oceanParams.SSSBase = Settings.SSSBase;
                oceanParams.LOD_scale = Settings.LODScale;
                oceanParams.SSSScale = Settings.SSSScale;

                m_UBOceanParamsBuffer->SetData(Renderer::GetCurrentFrameInFlight(), Buffer(&oceanParams, sizeof(oceanParams)));

            }

            {
                UBOceanSettings oceanSettings;
                oceanSettings.Color = Settings.Color;
                oceanSettings.FoamColor = Settings.FoamColor;
                oceanSettings.SSSColor = Settings.SSSColor;
                oceanSettings.SSSStrength = Settings.SSSStrength;
                oceanSettings.Roughness = Settings.Roughness;
                oceanSettings.RoughnessScale = Settings.RoughnessScale;
                oceanSettings.MaxGloss = Settings.MaxGloss;
                oceanSettings.FoamBiasLOD0 = Settings.FoamBiasLOD0;
                oceanSettings.FoamBiasLOD1 = Settings.FoamBiasLOD1;
                oceanSettings.FoamBiasLOD2 = Settings.FoamBiasLOD2;
                oceanSettings.FoamScale = Settings.FoamScale;
                oceanSettings.ContactFoam = Settings.ContactFoam;

                m_UBOceanSettingsBuffer->SetData(Renderer::GetCurrentFrameInFlight(), Buffer(&oceanSettings, sizeof(oceanSettings)));
            }
        }
		renderer->SubmitMesh(m_Grid, m_RenderMaterial, GetTransform());

    }
    void FFTWaveRealistic::IFFT2D(Count<Image2D> inputImage, Count<Image2D> bufferImage)
    {
        const uint32_t WorkGroup = 8;

        
        const uint32_t logSize = static_cast<int>(CustomLog(Size, 2));

        bool pingPong = false;
		// horizontal pass
        {
            Renderer::BeginComputePass(m_CommandBuffer, m_HorizontalStep[0]);
            for (int i = 0; i < logSize; i++)
            {
                pingPong = !pingPong;
                glm::uvec2 data{ i,Size };

				m_HorizontalStep[0]->SetInput("InputBuffer", pingPong ? inputImage : bufferImage);
				m_HorizontalStep[0]->SetInput("OutputBuffer", pingPong ? bufferImage : inputImage);

                m_HorizontalStep[0]->PushData("params", &data);
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

                m_VerticalStep[0]->SetInput("InputBuffer", pingPong ? inputImage : bufferImage);
                m_VerticalStep[0]->SetInput("OutputBuffer", pingPong ? bufferImage : inputImage);

                m_VerticalStep[0]->PushData("params", &data);
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
        return glm::cos(2 * glm::pi<float>() * Random::Real<float>(0,1)) * glm::sqrt(-2 * Math::Loge(Random::Real<float>(0, 1)));
    }
    const float VertexDensity = 35;
    const int ClipMapLevels = 7;
    const float MinMeshScale = 15;


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


       const int logSize = static_cast<int>(CustomLog(Size, 2));

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
        std::array<uint32_t, 3> lengthScales = { 250,17,5 };

        for (int i = 0; i < m_Cascades.size(); i++)
        {

            if(m_Cascades[i] == nullptr)
				m_Cascades[i] = Count<FFTWaveRealisticCascade>::Create(i, Size, m_NoiseTexture);
			m_Cascades[i]->CascadeSettings.Length = lengthScales[i];
        }

       
        m_Grid = FFTClipMap::BuildClipMapPlane(VertexDensity, ClipMapLevels);
        AssetManager::CreateRuntimeAsset(m_Grid.As<Asset>(), "FFTGRID");

		m_RenderMaterial = RenderMaterial::Create("FFT Wave Realistic Material",Renderer::GetShader("FFTRealisticWater"));

		m_UBOceanSettingsBuffer = UniformBufferSet::Create(sizeof(UBOceanSettings));
		m_UBOceanParamsBuffer = UniformBufferSet::Create(sizeof(UBOceanParams));

        m_RenderMaterial->Set("_Displacement_c0", m_Cascades[0]->GetDerivativesMap());
        m_RenderMaterial->Set("_Displacement_c1", m_Cascades[1]->GetDerivativesMap());
        m_RenderMaterial->Set("_Displacement_c2", m_Cascades[2]->GetDerivativesMap());

        m_RenderMaterial->Set("f_Derivatives_c0", m_Cascades[0]->GetDerivativesMap());
        m_RenderMaterial->Set("f_Derivatives_c1", m_Cascades[1]->GetDerivativesMap());
        m_RenderMaterial->Set("f_Derivatives_c2", m_Cascades[2]->GetDerivativesMap());

		m_RenderMaterial->Set("OceanSettings", m_UBOceanSettingsBuffer);
        m_RenderMaterial->Set("fOceanParams", m_UBOceanParamsBuffer);
        m_RenderMaterial->Set("OceanParams", m_UBOceanParamsBuffer);
		m_RenderMaterial->Set("f_FoamTexture", Renderer::GetWhiteTexture());

        Settings.Local.Scale = 0.5f;
        Settings.Local.WindSpeed = 0.1f;
        Settings.Local.WindDirection = -29.81f;
        Settings.Local.Fetch = 100000.0f;
        Settings.Local.SpreadBlend = 1.0f;
        Settings.Local.Swell = 0.198f;
        Settings.Local.PeakEnhancement = 3.3f;
        Settings.Local.ShortWavesFade = 0.01f;

        Settings.Swell.Scale = 0.5f;
        Settings.Swell.WindSpeed = 0.001f;
        Settings.Swell.WindDirection = 90.0f;
        Settings.Swell.Fetch = 300000.0f;
        Settings.Swell.SpreadBlend = 1.0f;
        Settings.Swell.Swell = 1.0f;
        Settings.Swell.PeakEnhancement = 3.3f;
        Settings.Swell.ShortWavesFade = 0.01f;

    }
    
}
