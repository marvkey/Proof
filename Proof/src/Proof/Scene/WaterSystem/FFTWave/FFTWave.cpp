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
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/SceneUtils.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include "Proof/Math/Random.h"
#include "Proof/Math/Math.h"
#include "Proof/Asset/AssetManager.h"

#include "Proof/Platform/Vulkan/Vulkan.h"
#include "Proof/Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
namespace Proof
{


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

    Count<Image2D> CreateRenderImage(std::string debugName, uint32_t width, uint32_t height, ImageFormat  format, uint32_t numLayers =1, SamplerWrap wrap = SamplerWrap::ClampBorder, SamplerFilter filter = SamplerFilter::Linear)
    {
        ImageConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;
        rt.Usage = ImageUsage::Storage;
        rt.Layers = numLayers;
        rt.Transfer = true;

        auto tex = Image2D::Create(rt, wrap, filter);
        return tex;
    }

    Count<ImageView> CreateRenderImageView(std::string debugName, Count<Image2D> image,uint32_t layer =0, uint32_t mip =0,uint32_t layerCount =1,uint32_t mipCount =1, ImageViewType View = ImageViewType::View2D)
    {
        ImageViewConfiguration imageViewConfig;
        imageViewConfig.DebugName = debugName;
        imageViewConfig.Layer = layer;
        imageViewConfig.Mip = mip;
        imageViewConfig.LayerCount= layerCount;
        imageViewConfig.MipCount = mipCount;
        imageViewConfig.View = View;
        imageViewConfig.Image = image;

        Count<ImageView> imageView = ImageView::Create(imageViewConfig);

        return imageView;
    }

    FFTWaveCascade::FFTWaveCascade(uint32_t cascadeIndex, class FFTWave* wave)
    {
        m_CascadeIndex = cascadeIndex;
        Settings.SpectrumSeed = glm::ivec2(Random::Int<int>(-1000, 1000), Random::Int<int>(-1000, 1000));
        SpectrumMap = CreateRenderImageView(fmt::format("FFT Spectrum Cascade {}", m_CascadeIndex), wave->GetSpectrumTexture(), m_CascadeIndex);
        DisplacementMap = CreateRenderImageView(fmt::format("FFT Displacement Cascade {}", m_CascadeIndex), wave->GetDisplacementMap(), m_CascadeIndex);
        NormalMap = CreateRenderImageView(fmt::format("FFT Normal Cascade {}", m_CascadeIndex), wave->GetNormalMap(),m_CascadeIndex);
        InternalSettings.Time = 120 + glm::pi<float>() * m_CascadeIndex; // so cascades dont have same time
    }

    void FFTWaveCascade::Resize(uint32_t mapSize)
    {

    }


    Count<Mesh> GenerateGrid(int gridDimension, float texCoordScale =2.0f)
    {
        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        const int vertex_count = gridDimension + 1;
        vertices.resize(vertex_count * vertex_count);
        indices.resize(gridDimension * gridDimension * 2); // Two triangles per cell

        unsigned int idx = 0;
        for (int z = -gridDimension / 2; z <= gridDimension / 2; ++z) {
            for (int x = -gridDimension / 2; x <= gridDimension / 2; ++x) {
                vertices[idx].Position = glm::vec3(float(x), 0.f, float(z));
                vertices[idx].Normal = glm::vec3(0, 1, 0); // Default normal pointing up
                vertices[idx].Tangent = glm::vec3(1, 0, 0); // Default tangent
                vertices[idx].Bitangent = glm::vec3(0, 0, 1); // Default bitangent

                float u = ((float)x / gridDimension) + 0.5f;
                float v = ((float)z / gridDimension) + 0.5f;
                vertices[idx++].TexCoord = glm::vec2(u, v) ;
            }
        }
        PF_CORE_ASSERT(idx == vertices.size());

        // Clockwise winding for the grid indices
        idx = 0;
        for (unsigned int y = 0; y < gridDimension; ++y) {
            for (unsigned int x = 0; x < gridDimension; ++x) {
                uint32_t topLeft = (vertex_count * y) + x;
                uint32_t bottomLeft = (vertex_count * (y + 1)) + x;
                uint32_t topRight = (vertex_count * y) + x + 1;
                uint32_t bottomRight = (vertex_count * (y + 1)) + x + 1;

                indices[idx++] = { topLeft, bottomLeft, topRight };
                indices[idx++] = { topRight, bottomLeft, bottomRight };
            }
        }
        PF_CORE_ASSERT(idx == indices.size());

        return Count<Mesh>::Create("FFTGrid",vertices, indices);
    }
    FFTWave::FFTWave(Count<FFTWave> other)
        : Wave(other->m_Water, WaveType::FastFourierTransformWave)
    {
        InitTextures();
        InitPasses();

      //  m_Grid = GenerateGrid(GRID_DIM);
       // AssetManager::CreateRuntimeAsset(m_Grid.As<Asset>(), "FFTGRID");

    }
    FFTWave::FFTWave(Count<class Water> water)
        : Wave(water, WaveType::FastFourierTransformWave)
    {
        InitTextures();
        InitPasses();
       // m_Grid = GenerateGrid(GRID_DIM);
      //  AssetManager::CreateRuntimeAsset(m_Grid.As<Asset>(), "FFTGRID");


    }
    void FFTWave::Update(float deltaTime)
    {
        PF_PROFILE_FUNC();

        for (Count<FFTWaveCascade> cascade : m_Cascades)
        {
            float cascadeDelta = deltaTime * cascade->Settings.TimeScale;
            cascade->InternalSettings.Time += cascadeDelta;

            // Note: The constants are used to normalize parameters between 0 and 10.
            cascade->InternalSettings.FoamGrowRate = cascadeDelta * cascade->Settings.FoamAmount* 7.5f;
            cascade->InternalSettings.FoamDecayRate = cascadeDelta * std::max(0.5f, 10.0f - cascade->Settings.FoamAmount) * 1.15f;
        }
    }

    void FFTWave::Render(Count<class WorldRenderer> renderer)
    {
        PF_PROFILE_FUNC();
        Renderer::BeginCommandBuffer(m_CommandBuffer);

        const uint32_t oceanSize = (uint32_t)WaveInfo.OceanSize;
        const uint32_t numFFTStages = static_cast<int>(std::log2((uint32_t)WaveInfo.OceanSize));
        static bool checkedButterfly = false;
        // onlu once if map size is chagned
        if (!checkedButterfly)
        {
            PF_PROFILE_SCOPE_DYNAMIC("ButterflyPass");
            checkedButterfly = true;
            const uint32_t WorkGroup = 64;

            Renderer::BeginComputePass(m_CommandBuffer, m_ButterflyPass);
            m_ButterflyPass->Dispatch((uint32_t)WaveInfo.OceanSize / 2/ WorkGroup, numFFTStages, 1);
            Renderer::EndComputePass(m_ButterflyPass);

        }
        static bool checked = false;
        const float DEPTH = 20.0;
        Count<FFTWaveCascade> cascade = m_Cascades[0];
        if (!checked)
        {
            const uint32_t WorkGroup = 16;
            PF_PROFILE_SCOPE_DYNAMIC("Spectrum");
            Renderer::BeginComputePass(m_CommandBuffer, m_SpectrumPass);

            checked = true;
            float alpha = JONSWAPAlpha(cascade->Settings.WindSpeed, cascade->Settings.FetchLength * 1e3);
            float omega = JONSWAPPeakAngularFrequency(cascade->Settings.WindSpeed, cascade->Settings.FetchLength * 1e3);

            struct PushConstant
            {
                glm::ivec2 Seed;
                glm::vec2 TileLength;

                float Alpha;
                float PeakFrequency;
                float WindSpeed;
                float Angle; // radians

                float Depth;
                float Swell;
                float Detail;
                float Spread;

                uint32_t CascadeIndex;
            }pc;

            pc.Seed = cascade->Settings.SpectrumSeed;
            pc.TileLength = cascade->Settings.TileLength;
            pc.Alpha = alpha;
            pc.PeakFrequency = omega;
            pc.WindSpeed = cascade->Settings.WindSpeed;

            glm::vec2 windDir = cascade->Settings.WindDirection;
            pc.Angle = atan2f(windDir.y, windDir.x); // tan^-1(y/x) inverse tan
            pc.Depth = DEPTH;
            pc.Swell = cascade->Settings.Swell;
            pc.Detail = cascade->Settings.Detail;

            pc.Spread = cascade->Settings.Spread;
            pc.CascadeIndex = cascade->GetCascadeIndex();

            m_SpectrumPass->PushData("u_PC", &pc);
            m_SpectrumPass->Dispatch(oceanSize / WorkGroup, oceanSize / WorkGroup, 1);
            Renderer::EndComputePass(m_SpectrumPass);
        }
        {
            PF_PROFILE_SCOPE_DYNAMIC("SpectrumModulate");
            const uint32_t WorkGroup = 16;

            Renderer::BeginComputePass(m_CommandBuffer, m_SpectrumModulatePass);

            struct PushConstant
            {
                glm::vec2 tileSize;
                float oceanDepth;
                float simulationTime;

                uint32_t cascadeLayerIndex;
            }pc;

            pc.tileSize = cascade->Settings.TileLength;
            pc.oceanDepth = DEPTH;

            pc.simulationTime = FrameTime::GetTime(); //TODO is this right
            pc.cascadeLayerIndex = cascade->GetCascadeIndex();

            m_SpectrumModulatePass->PushData("u_PC", &pc);
            m_SpectrumModulatePass->Dispatch(oceanSize / WorkGroup, oceanSize / WorkGroup, 1);
            Renderer::EndComputePass(m_SpectrumModulatePass);
        }

        {
            PF_PROFILE_SCOPE_DYNAMIC("FFTCompute");

            Renderer::BeginComputePass(m_CommandBuffer, m_FFTPass);
           
            uint32_t index = cascade->GetCascadeIndex();
            m_FFTPass->PushData("u_PC",&index);
            m_FFTPass->Dispatch(1, oceanSize, 4);
            Renderer::EndComputePass(m_FFTPass);
        }
    #if 1

        {
            PF_PROFILE_SCOPE_DYNAMIC("Transpose");

            const uint32_t WorkGroup = 32;

            Renderer::BeginComputePass(m_CommandBuffer, m_TransposePass);

            uint32_t index = cascade->GetCascadeIndex();
            m_TransposePass->PushData("u_PC", &index);
            m_TransposePass->Dispatch(oceanSize / WorkGroup, oceanSize / WorkGroup, 4);
            Renderer::EndComputePass(m_TransposePass);
        }

        {
            PF_PROFILE_SCOPE_DYNAMIC("FFTCompute");

            Renderer::BeginComputePass(m_CommandBuffer, m_FFTPass);

            uint32_t index = cascade->GetCascadeIndex();
            m_FFTPass->PushData("u_PC", &index);
            m_FFTPass->Dispatch(1, oceanSize, 4);
            Renderer::EndComputePass(m_FFTPass);
        }

        {
            PF_PROFILE_SCOPE_DYNAMIC("Unpack");
            const uint32_t WorkGroup = 16;

            Renderer::BeginComputePass(m_CommandBuffer, m_FFTUnpack);

            struct PushConstant
            {
                uint32_t CascadeIndex;
                float WhiteCap;
                float FoamGrowRate;
                float FoamDecayRate;
            }pc;

            pc.CascadeIndex = cascade->GetCascadeIndex();
            pc.WhiteCap = cascade->Settings.Whitecap;

            pc.FoamGrowRate = cascade->InternalSettings.FoamGrowRate;
            pc.FoamDecayRate = cascade->InternalSettings.FoamDecayRate;

            m_FFTUnpack->PushData("u_PC", &pc);
            m_FFTUnpack->Dispatch(oceanSize / WorkGroup, oceanSize / WorkGroup, 1);
            Renderer::EndComputePass(m_FFTUnpack);
        }

    #endif
        Renderer::EndCommandBuffer(m_CommandBuffer);
        Renderer::SubmitCommandBuffer(m_CommandBuffer);
   
    }

    void FFTWave::InitPasses()
	{
        PF_PROFILE_FUNC();

        const uint32_t oceanSize = (uint32_t)WaveInfo.OceanSize;

        m_CommandBuffer = RenderCommandBuffer::Create("FFTCommandBuffer");
        m_SBButterflyFactors = StorageBufferSet::Create(oceanSize * oceanSize * sizeof(glm::vec4));
        // Size: (map size^2 * 4 FFTs * 2 temp buffers (for Stockham FFT) * sizeof(vec2))
        m_FFTBuffer = StorageBufferSet::Create(WaveInfo.NumCascades * oceanSize * oceanSize * 4 * 2 * 2 * 4);

      
        m_ButterflyPass = ComputePass::Create({ "FFTOceanButterfly",ComputePipeline::Create({"FFTOceanButterfly",Renderer::GetShader("FFTOceanButterfly")}) });
        m_ButterflyPass->SetInput("FFTBuffer", m_SBButterflyFactors);

        m_SpectrumPass = ComputePass::Create({ "FFTOceanSpectrum",ComputePipeline::Create({"FFTOceanSpectrum",Renderer::GetShader("FFTOceanSpectrum")}) });
        {
            auto imageView = CreateRenderImageView("SpectrumImageView", m_SpectrumTexture, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);
            m_SpectrumPass->SetInput("o_Spectrum", imageView);
            //m_SpectrumPass->SetInput("o_Spectrum", m_SpectrumTexture);
        }

        m_SpectrumModulatePass = ComputePass::Create({ "FFTOceanModulateSpectrum",ComputePipeline::Create({"FFTOceanModulateSpectrum",Renderer::GetShader("FFTOceanModulateSpectrum")}) });
        {
            auto imageView = CreateRenderImageView("SpectrumImageView", m_SpectrumTexture, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);
            m_SpectrumModulatePass->SetInput("u_WaveSpectrumTexture", imageView);
        }
        m_SpectrumModulatePass->SetInput("FFTBuffer", m_FFTBuffer);

        m_FFTPass = ComputePass::Create({ "FFT",ComputePipeline::Create({"FFTOceanCompute",Renderer::GetShader("FFTOceanCompute")}) });
        m_FFTPass->SetInput("ButterflyFactorBuffer", m_SBButterflyFactors);
        m_FFTPass->SetInput("FFTBuffer", m_FFTBuffer);

        m_TransposePass = ComputePass::Create({ "FFTOceanTranspose",ComputePipeline::Create({"FFTOceanTranspose",Renderer::GetShader("FFTOceanTranspose")}) });
        m_TransposePass->SetInput("ButterflyFactorBuffer", m_SBButterflyFactors);
        m_TransposePass->SetInput("FFTBuffer", m_FFTBuffer);

        m_FFTUnpack = ComputePass::Create({ "FFTOceanUnpack",ComputePipeline::Create({"FFTOceanUnpack",Renderer::GetShader("FFTOceanUnpack")}) });
        m_FFTUnpack->SetInput("FFTBuffer", m_FFTBuffer);
        {
            auto displacement = CreateRenderImageView("FFTDisplacementView", m_DisplacementMap, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);
            auto normal = CreateRenderImageView("FFTNormalView", m_NormalMap, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);

            m_FFTUnpack->SetInput("o_DisplacementMap", displacement);
            m_FFTUnpack->SetInput("o_NormalMap", normal);
        }

        for (uint32_t i = 0; i < WaveInfo.NumCascades; i++)
        {
            Count<FFTWaveCascade> cascade = Count<FFTWaveCascade>::Create(i,this);

            m_Cascades.emplace_back(cascade);
        }

	}
    void FFTWave::InitTextures()
    {
        const uint32_t oceanSize = (uint32_t)WaveInfo.OceanSize;

        m_SpectrumTexture = CreateRenderImage("FFTSpectrum", oceanSize, oceanSize, ImageFormat::RGBA32F, WaveInfo.NumCascades);
        m_DisplacementMap = CreateRenderImage("FFTDisplacement", oceanSize, oceanSize, ImageFormat::RGBA16F, WaveInfo.NumCascades);
        m_NormalMap = CreateRenderImage("FFTNormal", oceanSize, oceanSize, ImageFormat::RGBA16F, WaveInfo.NumCascades);
    }

    Count<class RenderMaterial> FFTWave::GetRenderMaterial()
    {
        return m_RenderMaterial;
    }
}
