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
#include "Proof/Asset/MeshImpoter.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Asset/AssetManager.h"
#include "FFTClipMap.h"

#include "Proof/Platform/Vulkan/Vulkan.h"
#include "Proof/Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
#include "Proof/Scene/SceneUtils.h"
#include "Proof/Renderer/Renderer2D.h"

#include <tuple>

#define FFT_MAX_CASCADES 8
namespace Proof
{

    struct alignas(16) FFTWaterUBBuffer
    {
        glm::vec4 WaterColor;

        glm::vec3 FoamColor;
        float Roughness;


        int NumCascades;
        float NormalStrength;
        float MinMeshScale;
        int LevelhalfSize;

        glm::vec3  ViewerPosition;


    };

    struct alignas(16)FFTWaterUBScalesBuffer
    {
        glm::vec4 MapScales[FFT_MAX_CASCADES];
    };
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

    Count<Image2D> CreateRenderImage(std::string debugName, uint32_t width, uint32_t height, ImageFormat  format, uint32_t numLayers =1, SamplerWrap wrap = SamplerWrap::Repeat, SamplerFilter filter = SamplerFilter::Linear)
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

    glm::ivec2 GetCenterPlaneSize(int vertexDensity, int overlap)
    {
        int halfSize = FFTClipMap::ClipLevelHalfSize(vertexDensity);
        int size = 2 * halfSize + overlap;
        return glm::ivec2(size, size);
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
    
    FFTWave::FFTWave(class Water* water, Count<FFTWave> other)
        : Wave(water, WaveType::FastFourierTransformWave)

    {

        m_Grid = other->m_Grid;
        WaveInfo = other->WaveInfo;
        InitBuffer();
        InitTextures();
        InitPasses();

        for (int i = 0; i < m_Cascades.size(); i++)
        {
            auto casccade = m_Cascades[i];
            casccade->Settings = other->m_Cascades[i]->Settings;
        }
    }
    FFTWave::~FFTWave()
    {
    }

    const float VertexDensity = 35;
    const int ClipMapLevels = 7;
    const float MinMeshScale = 15;
    FFTWave::FFTWave(class Water* water)
        : Wave(water, WaveType::FastFourierTransformWave)
    {
        InitBuffer();
        InitTextures();
        InitPasses();
        InitialWaveparams();
       // MeshImporter importer = MeshImporter("Assets/Meshes/clipmap_high.obj");
       // m_Grid = Count<Mesh>::Create(importer.ImportToMeshSource());
        m_Grid = FFTClipMap::BuildClipMapPlane(VertexDensity, ClipMapLevels);

        //m_Grid = MeshWorkShop::GeneratePlane(1024, 1024);
        AssetManager::CreateRuntimeAsset(m_Grid.As<Asset>(), "FFTGRID");


    }
    void FFTWave::Update(float deltaTime)
    {
        PF_PROFILE_FUNC();

        for (Count<FFTWaveCascade> cascade : m_Cascades)
        {
            float cascadeDelta = deltaTime * cascade->Settings.FoamTimeScale * WaveInfo.FoamTimeScale;
            cascade->InternalSettings.Time += cascadeDelta;

            // Note: The constants are used to normalize parameters between 0 and 10.
            cascade->InternalSettings.FoamGrowRate = cascadeDelta * cascade->Settings.FoamAmount * WaveInfo.FoamGrowthScale * 7.5f;
            cascade->InternalSettings.FoamDecayRate = cascadeDelta * std::max(0.5f, 10.0f - cascade->Settings.FoamAmount ) * 1.15f * WaveInfo.FoamDecayScale;
        }

        if (m_Cascades.size() != WaveInfo.NumCascades || m_OldWaveInfo.OceanSize != WaveInfo.OceanSize)
        {
            InitTextures();
            InitPasses();
            m_HasInitialRun = false;
        }

        if (WaveInfo.ViewWaveHeight && m_SampleWaveheightAtPos.empty())
        {
         //   std::vector<glm::vec3> positions = Utils::SamplePlanePoints((uint64_t)WaveInfo.OceanSize);
            std::vector<glm::vec3> positions = Utils::SamplePlanePoints(GetCenterPlaneSize(VertexDensity,2).x);

            for (int i = 0; i < positions.size(); i++)
            {
                m_SampleWaveheightAtPos.push_back(std::make_tuple( UUID(),positions[i]));
            }

            for (auto& data: m_SampleWaveheightAtPos)
            {
                PushWaveHeightQueryID(std::get<0>(data));
                UpdateWaveHeightQueryID(std::get<0>(data), std::get<1>(data));
            }
        }

        if (WaveInfo.ViewWaveHeight == false && m_SampleWaveheightAtPos.empty() == false)
        {

            for (auto& data : m_SampleWaveheightAtPos)
            {
                RemoveWaveHeightQueryID(std::get<0>(data));
            }
        }
    }

    void FFTWave::Render(Count<class WorldRenderer> renderer)
    {
        PF_PROFILE_FUNC();

        Renderer::BeginCommandBuffer(m_CommandBuffer);

        const uint32_t oceanSize = (uint32_t)WaveInfo.OceanSize;
        const uint32_t numFFTStages = static_cast<int>(glm::log((uint32_t)WaveInfo.OceanSize)/glm::log(2));

        // doing the intial frame count
        // cause we are wrting to a storage buffer
        // wich has multiple bufer cause of frame in flight
        // so basicaly, if we fill one we will skip the rest and not fill them cause of this 
        // system of only chanign if m_OldWaveInfo is changed
        //if(m_HasInitialRun == false || m_OldWaveInfo != WaveInfo) 
        {
            PF_PROFILE_SCOPE_DYNAMIC("ButterflyPass");
            const uint32_t WorkGroup = 64;

            Renderer::BeginComputePass(m_CommandBuffer, m_ButterflyPass);
            m_ButterflyPass->Dispatch((uint32_t)WaveInfo.OceanSize / 2/ WorkGroup, numFFTStages, 1);
            Renderer::EndComputePass(m_ButterflyPass);

           // PF_ENGINE_TRACE("Generated FFTWave Ocean butterfly pass");

        }

		for (Count<FFTWaveCascade> cascade : m_Cascades)
		{
			UpdateCascade(cascade);
		}

        {
            FFTWaterUBBuffer ubf;
            ubf.NumCascades = m_Cascades.size();
            FFTWaterUBScalesBuffer scalesBuffer;
            for (Count<FFTWaveCascade> cascade : m_Cascades)
            {
#if 1
                glm::vec2 uvScale = glm::vec2(1) / cascade->Settings.TileLength;
                scalesBuffer.MapScales[cascade->m_CascadeIndex] = glm::vec4(uvScale.x, uvScale.y, cascade->Settings.DisplacementScale * WaveInfo.DisplacementScale, cascade->Settings.NormalScale * WaveInfo.NormalScale);

#else

                uint32_t index = cascade->m_CascadeIndex;

                glm::vec2 tileLength = cascade->Settings.TileLength;
                float displacementScale = cascade->Settings.DisplacementScale;
                float normalScale = cascade->Settings.NormalScale;

                glm::vec2 uvScale = glm::vec2(1.0f) / tileLength;

                // Offset based on camera position in world-space (XZ)
                glm::vec2 offset = -glm::vec2(renderer->GetCameraUBData().Position.x, renderer->GetCameraUBData().Position.z) * uvScale;

                // Pack: xy = scale, zw = offset
                ubf.MapScales[index] = glm::vec4(uvScale, offset);
#endif
            }
            
            ubf.NormalStrength = WaveInfo.NormalStrength;
            ubf.WaterColor = WaveInfo.WaterColor;
            ubf.FoamColor = WaveInfo.FoamColor;
            ubf.Roughness = WaveInfo.Roughness;
            
            {
                // Replace this with your actual FFT ocean grid scale
				
                int clipLevelHalfSize = FFTClipMap::ClipLevelHalfSize(VertexDensity);
                int pow = glm::floor(std::max(0.0f, glm::log2(std::abs(renderer->GetCameraUBData().Position.y) / (2.0f * MinMeshScale)) + 1));
                float meshScale = MinMeshScale / clipLevelHalfSize * std::pow(2, pow);

                // Send these as uniform
                ubf.MinMeshScale= meshScale;
                ubf.LevelhalfSize = clipLevelHalfSize;
                ubf.ViewerPosition = renderer->GetCameraUBData().Position;

                Buffer buffer(&ubf, sizeof(ubf), true);
                m_WaterBuffer->SetData(Renderer::GetCurrentFrameInFlight(), buffer);
                buffer.Release();

            }

            {
                Buffer buffer(&scalesBuffer, sizeof(scalesBuffer), true);
                m_WaterScalesBuffer->SetData(Renderer::GetCurrentFrameInFlight(), buffer);
                buffer.Release();
            }
        }
   
        if(!m_WaveHeightQueryPositons.empty())
        {
            PF_PROFILE_SCOPE_DYNAMIC("Query Wave Height");

            struct PushConstant
            {
                uint32_t NumQueries;
                int NumCascades;
            }pc;

            pc.NumCascades = m_Cascades.size();
            pc.NumQueries = m_WaveHeightQueryPositons.size();

            const uint32_t WorkGroup = 64;
            {
                int i = 0;
                for (auto& [id, pos] : m_WaveHeightQueryPositons)
                {
                    m_FFTQueryBuffer.Get()[i].ID = id;
                    m_FFTQueryBuffer.Get()[i].WorldPosition = pos;
                    i++;
                }

                Buffer buffer (m_FFTQueryBuffer.Get().data(),m_FFTQueryBuffer.Get().size() * sizeof(SBWaveHeightQuery),true);
                m_FFTSBQueryBuffer->SetData(Renderer::GetCurrentFrameInFlight(), buffer);
                buffer.Release();

            }


            Renderer::BeginComputePass(m_CommandBuffer, m_QueryWaaveHeightPass);
            m_QueryWaaveHeightPass->PushData("u_PC", &pc);
            m_QueryWaaveHeightPass->Dispatch((63 + pc.NumQueries) / WorkGroup, 1, 1);

            Renderer::Submit([commandBuffer = m_CommandBuffer]()
                {

                    VkMemoryBarrier barrier = {};
                    barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
                    barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;

                    vkCmdPipelineBarrier(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_PIPELINE_STAGE_HOST_BIT,
                        0,
                        1, &barrier,
                        0, nullptr,
                        0, nullptr);
                });
            Renderer::EndComputePass(m_QueryWaaveHeightPass);
            Buffer buffer = m_FFTSBQueryBuffer->GetBuffer(Renderer::GetCurrentFrameInFlight())->GetDataRaw();


            std::memcpy(m_FFTQueryBufferResult.Get().data(), buffer.Data, buffer.GetSize());
            buffer.Release();

            for (const auto& ref : m_FFTQueryBufferResult.Get())
            {
                if (HasWaveHeightQueryID(ref.ID))
                {
                    m_WaveHeightAndDisplacmentQueryResult[ref.ID].first = ref.WaveHeight;
                    m_WaveHeightAndDisplacmentQueryResult[ref.ID].second = ref.WaveDisplacment;
                }
            }
            //TODO when multithreaded this code will not be safe
           /*
            Count<FFTWave> instance = this;
            Renderer::Submit([instance] 
                {
                    PF_PROFILE_SCOPE_DYNAMIC("Rendther thread Query Wave Height ");
                    
                    Buffer buffer = instance->m_FFTSBQueryBuffer->GetBuffer(Renderer::RT_GetCurrentFrameInFlight())->GetDataRaw();

                    std::memcpy(instance->m_FFTQueryBufferResult.RT_Get().data(), buffer.Data, buffer.GetSize());
                    buffer.Release();

                    for (const auto& ref : instance->m_FFTQueryBufferResult.RT_Get())
                    {
                        if (instance->HasWaveHeightQueryID(ref.ID))
                            instance->m_WaveHeightQueryResultHeight[ref.ID] = ref.WaveHeight;
                    }
                });
                */
        }
        Renderer::EndCommandBuffer(m_CommandBuffer);
        Renderer::SubmitCommandBuffer(m_CommandBuffer);


   
        renderer->SubmitMesh(m_Grid, m_RenderMaterial, GetTransform());
        //renderer->SubmitMesh(m_Grid, m_Grid->GetMaterialTable(),GetTransform());


        m_OldWaveInfo = WaveInfo;

        m_HasInitialRun = true;

        m_SpectrumTexture->GenerateMips();
        m_DisplacementMap->GenerateMips();
        m_NormalMap->GenerateMips();
    }

    void FFTWave::Render2D(Count<class Renderer2D> renderer2D)
    {
        if (WaveInfo.ViewWaveHeight)
        {

		    for (auto& data : m_SampleWaveheightAtPos)
		    {
                
                if (!IsWaveheightQueryReady(std::get<0>(data)))
                    continue;
                auto localPos = std::get<1>(data);
                auto worldPos = localPos;// Utils::LocalToWorld(localPos, GetTransform());

			    glm::vec3 pos = localPos;

			    pos.y += GetWaveHeightAndDisplacment(std::get<0>(data)).first;

			    //pos = Utils::LocalToWorld(pos, GetTransform());

			    renderer2D->DrawLine(worldPos, pos);
                renderer2D->FillCircle(pos, glm::vec3(glm::radians(90.0f), 0.0f, 0), 0.8, glm::vec4(1, 0, 0, 1));
                renderer2D->DrawPoint(pos, 20, glm::vec4(1, 0, 0, 1));
              //  renderer2D->DrawCircle(pos, glm::vec3(glm::radians(90.0f), 0.0f, 0),0.8,glm::vec4(1,0,0,1));

		    }
        }

    }

    void FFTWave::UpdateCascade(Count<FFTWaveCascade> cascade)
    {
        PF_PROFILE_FUNC();
        const uint32_t oceanSize = (uint32_t)WaveInfo.OceanSize;
        const uint32_t numFFTStages = static_cast<int>(glm::log((uint32_t)WaveInfo.OceanSize) / glm::log(2));
        const float DEPTH = 20.0;

        // dont need the frame count here, not writng to any buffer only images
       // if (m_HasInitialRun == false || cascade->Settings != cascade->m_OldSettings || m_OldWaveInfo != WaveInfo)
        {
            const uint32_t WorkGroup = 16;
            PF_PROFILE_SCOPE_DYNAMIC("Spectrum");
            Renderer::BeginComputePass(m_CommandBuffer, m_SpectrumPass);

            float alpha = JONSWAPAlpha(cascade->Settings.WindSpeed * WaveInfo.WindSpeedScale, cascade->Settings.FetchLength * 1e3);
            float omega = JONSWAPPeakAngularFrequency(cascade->Settings.WindSpeed * WaveInfo.WindSpeedScale, cascade->Settings.FetchLength * 1e3);

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
            pc.WindSpeed = cascade->Settings.WindSpeed * WaveInfo.WindSpeedScale;

            pc.Angle = glm::radians((float)cascade->Settings.WindDirection); 
            pc.Depth = DEPTH;
            pc.Swell = cascade->Settings.Swell * WaveInfo.SwellScale;
            pc.Detail = cascade->Settings.Detail * WaveInfo.DetailScale;

            pc.Spread = cascade->Settings.Spread * WaveInfo.SpreadScale;
            pc.CascadeIndex = cascade->GetCascadeIndex();

            m_SpectrumPass->PushData("u_PC", &pc);
            m_SpectrumPass->Dispatch(oceanSize / WorkGroup, oceanSize / WorkGroup, 1);
            Renderer::EndComputePass(m_SpectrumPass);

            //PF_ENGINE_TRACE("Generated Cascade {} FFt Ocean ",cascade->GetCascadeIndex());
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

            //pc.simulationTime = FrameTime::GetTime(); //TODO is this right
            pc.simulationTime = cascade->InternalSettings.Time;

            pc.cascadeLayerIndex = cascade->GetCascadeIndex();

            m_SpectrumModulatePass->PushData("u_PC", &pc);
            m_SpectrumModulatePass->Dispatch(oceanSize / WorkGroup, oceanSize / WorkGroup, 1);
            Renderer::EndComputePass(m_SpectrumModulatePass);
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
            pc.WhiteCap = cascade->Settings.Whitecap * WaveInfo.WhitecapScale;

            pc.FoamGrowRate = cascade->InternalSettings.FoamGrowRate;
            pc.FoamDecayRate = cascade->InternalSettings.FoamDecayRate;

            m_FFTUnpack->PushData("u_PC", &pc);
            m_FFTUnpack->Dispatch(oceanSize / WorkGroup, oceanSize / WorkGroup, 1);
            Renderer::EndComputePass(m_FFTUnpack);
        }
        cascade->m_OldSettings = cascade->Settings;

    }

    void FFTWave::InitialWaveparams()
    {
        
        if (m_Cascades.size() < 3)
            return;

        m_Cascades[0]->Settings.TileLength = { 400,400 };
        m_Cascades[1]->Settings.TileLength = { 200,200 };
        m_Cascades[2]->Settings.TileLength = { 100,100 };
    }

    void FFTWave::InitBuffer()
    {
        const uint32_t count = 256;

        for (int i = 0; i < m_FFTQueryBuffer.GetCount(); i++)
        {
            m_FFTQueryBuffer.GetByIndex(i).resize(count);
            m_FFTQueryBufferResult.GetByIndex(i).resize(count);
        }

        m_FFTSBQueryBuffer = StorageBufferSet::Create(count * sizeof(SBWaveHeightQuery));
    }

    bool FFTWave::HasWaveHeightQueryID(UUID ID)
    {
        return m_WaveHeightQueryPositons.contains(ID);
    }

    void FFTWave::PushWaveHeightQueryID(UUID ID)
    {
        PF_CORE_ASSERT(!HasWaveHeightQueryID(ID), "Already contains that ID");
        m_WaveHeightQueryPositons.insert({ ID,glm::vec3(0) });
    }

    void FFTWave::UpdateWaveHeightQueryID(UUID ID, glm::vec3 pos)
    {
        PF_CORE_ASSERT(m_WaveHeightQueryPositons.contains(ID), "Does not contain ID");
        m_WaveHeightQueryPositons[ID] = pos;
    }

    void FFTWave::RemoveWaveHeightQueryID(UUID ID)
    {
        PF_CORE_ASSERT(m_WaveHeightQueryPositons.contains(ID), "Does contain ID");
        m_WaveHeightQueryPositons.extract(ID);

        if (m_WaveHeightAndDisplacmentQueryResult.contains(ID))
            m_WaveHeightAndDisplacmentQueryResult.extract(ID);
    }

    bool FFTWave::IsWaveheightQueryReady(UUID ID)
    {
        PF_CORE_ASSERT(HasWaveHeightQueryID(ID), "ID was never pushed to be queried");

        if(!m_WaveHeightAndDisplacmentQueryResult.contains(ID))
            return false;

        return true;
    }

    std::pair<float, glm::vec3> FFTWave::GetWaveHeightAndDisplacment(UUID ID)
    {
        PF_CORE_ASSERT(IsWaveheightQueryReady(ID), "Query Wave heigh is not ready");

        return m_WaveHeightAndDisplacmentQueryResult.at(ID);
    }

    void FFTWave::InitPasses()
	{
        PF_PROFILE_FUNC();

        const uint32_t oceanSize = (uint32_t)WaveInfo.OceanSize;
        const uint32_t numFFTStages = static_cast<int>(glm::log((uint32_t)WaveInfo.OceanSize) / glm::log(2));

        {
            m_RenderMaterial = RenderMaterial::Create("FFTRenderMaterial", Renderer::GetShader("FFTWater"));
            m_WaterBuffer = UniformBufferSet::Create(sizeof(FFTWaterUBBuffer));
            m_WaterScalesBuffer = UniformBufferSet::Create(sizeof(FFTWaterUBScalesBuffer));

            m_RenderMaterial->Set("WaterUniforms", m_WaterBuffer);
            m_RenderMaterial->Set("WaterUniformsF", m_WaterBuffer);

            m_RenderMaterial->Set("ScalesWaterUniform", m_WaterScalesBuffer);
            m_RenderMaterial->Set("ScalesWaterUniformF", m_WaterScalesBuffer);

        }

        m_CommandBuffer = RenderCommandBuffer::Create("FFTCommandBuffer");
        m_SBButterflyFactors = StorageBuffer::Create(numFFTStages * oceanSize * 4 * 4);
        // Size: (map size^2 * 4 FFTs * 2 temp buffers (for Stockham FFT) * sizeof(vec2))
        m_FFTBuffer = StorageBufferSet::Create(WaveInfo.NumCascades * oceanSize * oceanSize * 4 * 2 * 2 * 4);

      
        m_ButterflyPass = ComputePass::Create({ "FFTOceanButterfly",ComputePipeline::Create({"FFTOceanButterfly",Renderer::GetShader("FFTOceanButterfly")}) });
        m_ButterflyPass->SetInput("FFTBuffer", m_SBButterflyFactors);

        m_SpectrumPass = ComputePass::Create({ "FFTOceanSpectrum",ComputePipeline::Create({"FFTOceanSpectrum",Renderer::GetShader("FFTOceanSpectrum")}) });
        {
            auto imageView = CreateRenderImageView("SpectrumImageView", m_SpectrumTexture, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);
            m_SpectrumPass->SetInput("spectrum", imageView);
            //m_SpectrumPass->SetInput("o_Spectrum", m_SpectrumTexture);
        }

        m_SpectrumModulatePass = ComputePass::Create({ "FFTOceanModulateSpectrum",ComputePipeline::Create({"FFTOceanModulateSpectrum",Renderer::GetShader("FFTOceanModulateSpectrum")}) });
        {
            auto imageView = CreateRenderImageView("SpectrumImageView", m_SpectrumTexture, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);
            //m_SpectrumModulatePass->SetInput("u_WaveSpectrumTexture", imageView);
            m_SpectrumModulatePass->SetInput("spectrum", imageView);
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
        auto displacement = CreateRenderImageView("FFTDisplacementView", m_DisplacementMap, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);

        {
            auto normal = CreateRenderImageView("FFTNormalView", m_NormalMap, 0, 0, WaveInfo.NumCascades, 1, ImageViewType::View2DArray);

            m_RenderMaterial->Set("u_Displacements", displacement);
            m_RenderMaterial->Set("u_Normals", normal);

           // m_FFTUnpack->SetInput("o_DisplacementMap", displacement);
           // m_FFTUnpack->SetInput("o_NormalMap", normal);

            m_FFTUnpack->SetInput("displacement_map", displacement);
            m_FFTUnpack->SetInput("normal_map", normal);
        }
        m_Cascades.resize(WaveInfo.NumCascades);
        for (uint32_t i = 0; i < WaveInfo.NumCascades; i++)
        {
            if (m_Cascades[i] != nullptr)
                continue;

            Count<FFTWaveCascade> cascade = Count<FFTWaveCascade>::Create(i,this);

            m_Cascades[i] = cascade;

        }

        m_QueryWaaveHeightPass = ComputePass::Create({ "FFTWaveHeightQuery",ComputePipeline::Create({"FFTWaveheight",Renderer::GetShader("FFTSampleWaveHeight")}) });

        m_QueryWaaveHeightPass->SetInput("u_Displacements", displacement);
        m_QueryWaaveHeightPass->SetInput("WaveQueryBuffer", m_FFTSBQueryBuffer);
        m_QueryWaaveHeightPass->SetInput("ScalesWaterUniform", m_WaterScalesBuffer);

	}
    void FFTWave::InitTextures()
    {
        const uint32_t oceanSize = (uint32_t)WaveInfo.OceanSize;

        m_SpectrumTexture = CreateRenderImage("FFTSpectrum", oceanSize, oceanSize, ImageFormat::RGBA32F, WaveInfo.NumCascades);
        m_DisplacementMap = CreateRenderImage("FFTDisplacement", oceanSize, oceanSize, ImageFormat::RGBA16F, WaveInfo.NumCascades);
        m_NormalMap = CreateRenderImage("FFTNormal", oceanSize, oceanSize, ImageFormat::RGBA16F, WaveInfo.NumCascades);

        m_SpectrumTexture->GenerateMips();
        m_DisplacementMap->GenerateMips();
        m_NormalMap->GenerateMips();
    }

    Count<class RenderMaterial> FFTWave::GetRenderMaterial()
    {
        return m_RenderMaterial;
    }
   
}
