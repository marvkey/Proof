#include "Proofprch.h"
#include "RendererSampler.h"
#include "Proof/Platform/Vulkan/VulkanSampler.h"
#include "Renderer.h"
#include "SamplerFactory.h"
namespace Proof
{
    Count<RenderSampler> RenderSampler::Create(const SamplerResourceConfig& config)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!");; return nullptr;
        case Renderer::API::OpenGL: return nullptr;
        case Renderer::API::Vulkan: return Count<VulkanRenderSampler>::Create(config);
        }
        return nullptr;
    }
    struct SamplerStorage
    {
        Count<RenderSampler> PointClampEdge;
        Count<RenderSampler> PointClampBorder0000;
        Count<RenderSampler> PointClampBorder1111;
        Count<RenderSampler> PointRepeat;
        Count<RenderSampler> LinearClampEdge;
        Count<RenderSampler> LinearClampEdgeMipPoint;
        Count<RenderSampler> LinearClampBorder0000MipPoint;
        Count<RenderSampler> LinearClampBorder1111MipPoint;
        Count<RenderSampler> LinearRepeatMipPoint;
        Count<RenderSampler> LinearRepeat;

        Count<RenderSampler> Point;
        Count<RenderSampler> Trilinear;
        Count<RenderSampler> Bilinear;

        Count<RenderSampler> ClampEdgePoint;
        Count<RenderSampler> ClampEdgeTrilinear;
        Count<RenderSampler> ClampEdgeBilinear;

        std::map<SamplerResourceConfig, WeakCount<RenderSampler>> SamplerStorage;
    };
    SamplerStorage* s_SamplerStorage = nullptr;

   
    void SamplerFactory::Init()
    {
        s_SamplerStorage = new SamplerStorage();
        //point clamp edge
        {
            SamplerResourceConfig config;
            config.DebugName = "PointClampEdge";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::ClampEdge;
            s_SamplerStorage->PointClampEdge = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->PointClampEdge);
        }
        //point clamp border0000
        {
            SamplerResourceConfig config;
            config.DebugName = "PointClampBorder0000";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::ClampBorder;
            config.BorderColor = SamplerBorderColor::TransperantBlack;
            s_SamplerStorage->PointClampBorder0000 = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->PointClampBorder0000);
        }

        //point clamp border1111
        {
            SamplerResourceConfig config;
            config.DebugName = "PointClampBorder1111";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::ClampBorder;
            config.BorderColor = SamplerBorderColor::OpaqueWhite;
            s_SamplerStorage->PointClampBorder1111 = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->PointClampBorder1111);
        }

        //PointRepeat
        {
            SamplerResourceConfig config;
            config.DebugName = "PointRepeat";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::Repeat;
            s_SamplerStorage->PointRepeat = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->PointRepeat);
        }

        //LinearClampEdge
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampEdge";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Linear;
            config.Wrap = SamplerWrap::ClampEdge;
            s_SamplerStorage->LinearClampEdge = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->LinearClampEdge);
        }

        //LinearClampEdgeMipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampEdgeMipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::ClampEdge;
            s_SamplerStorage->LinearClampEdgeMipPoint = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->LinearClampEdgeMipPoint);
        }

        //LinearClampBorder0000MipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampBorder0000MipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::ClampBorder;
            config.BorderColor = SamplerBorderColor::TransperantBlack;
            s_SamplerStorage->LinearClampBorder0000MipPoint = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->LinearClampBorder0000MipPoint);
        }

        //LinearClampBorder1111MipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampBorder1111MipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::ClampBorder;
            config.BorderColor = SamplerBorderColor::OpaqueWhite;
            s_SamplerStorage->LinearClampBorder1111MipPoint = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->LinearClampBorder1111MipPoint);
        }

        //LinearRepeatMipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearRepeatMipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::Repeat;
            s_SamplerStorage->LinearRepeatMipPoint = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->LinearRepeatMipPoint);
        }

        //LinearRepeat
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearRepeat";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Linear;
            config.Wrap = SamplerWrap::Repeat;
            s_SamplerStorage->LinearRepeat = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->LinearRepeat);
        }

        //point sampler
        {

            SamplerResourceConfig config;
            config.DebugName = "Point";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::Repeat;
            s_SamplerStorage->Point = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->Point);

            config.Wrap = SamplerWrap::ClampEdge;
            config.DebugName = "ClampEdgePoint";
            s_SamplerStorage->ClampEdgePoint = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->ClampEdgePoint);
        }

        //Trilinear sampler
        {

            SamplerResourceConfig config;
            config.DebugName = "Bilinear";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Linear;
            config.Wrap = SamplerWrap::Repeat;
            config.CompareOp = SamplerCompare::Never;
            s_SamplerStorage->Trilinear = RenderSampler::Create(config);

            config.Wrap = SamplerWrap::ClampEdge;
            config.DebugName = "ClampEdgeTrilinear";
            s_SamplerStorage->ClampEdgeTrilinear = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->ClampEdgeTrilinear);
        }

        //Bilinear sampler
        {

            SamplerResourceConfig config;
            config.DebugName = "Bilinear";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::Nearest;
            config.Wrap = SamplerWrap::Repeat;
            config.CompareOp = SamplerCompare::Never;
            s_SamplerStorage->Bilinear = RenderSampler::Create(config);


            config.Wrap = SamplerWrap::ClampEdge;
            config.DebugName = "ClampEdgeBilinear";
            s_SamplerStorage->ClampEdgeBilinear = RenderSampler::Create(config);
            SamplerFactory::AddSamplerToFactory(s_SamplerStorage->ClampEdgeBilinear);
        }
    }
    void SamplerFactory::ShutDown()
    {
        delete s_SamplerStorage;
        s_SamplerStorage = nullptr;
    }
    Count<RenderSampler> SamplerFactory::GetTrilinear()
    {
        return s_SamplerStorage->Trilinear;
    }
    Count<RenderSampler> SamplerFactory::GetBilinear()
    {
        return s_SamplerStorage->Bilinear;
    }
    Count<RenderSampler> SamplerFactory::GetPoint()
    {
        return s_SamplerStorage->Point;
    }

    Count<RenderSampler> SamplerFactory::GetClampEdgeTrilinear()
    {
        return s_SamplerStorage->ClampEdgeTrilinear;
    }

    Count<RenderSampler> SamplerFactory::GetClampEdgeBilinear()
    {
        return s_SamplerStorage->ClampEdgeBilinear;
    }

    Count<RenderSampler> SamplerFactory::GetClampEdgePoint()
    {
        return s_SamplerStorage->ClampEdgePoint;
    }
 
    Count<RenderSampler> SamplerFactory::GetPointClampEdge()
    {
        return s_SamplerStorage->PointClampEdge;
    }

    Count<RenderSampler> SamplerFactory::GetPointClampBorder0000()
    {
        return s_SamplerStorage->PointClampBorder0000;
    }

    Count<RenderSampler> SamplerFactory::GetPointClampBorder1111()
    {
        return s_SamplerStorage->PointClampBorder1111;
    }

    Count<RenderSampler> SamplerFactory::GetPointRepeat()
    {
        return s_SamplerStorage->PointRepeat;
    }

    Count<RenderSampler> SamplerFactory::GetLinearClampEdge()
    {
        return s_SamplerStorage->LinearClampEdge;
    }

    Count<RenderSampler> SamplerFactory::GetLinearClampEdgeMipPoint()
    {
        return s_SamplerStorage->LinearClampEdgeMipPoint;
    }

    Count<RenderSampler> SamplerFactory::GetLinearClampBorder0000MipPoint()
    {
        return s_SamplerStorage->LinearClampBorder0000MipPoint;
    }

    Count<RenderSampler> SamplerFactory::GetLinearClampBorder1111MipPoint()
    {
        return s_SamplerStorage->LinearClampBorder1111MipPoint;
    }

    Count<RenderSampler> SamplerFactory::GetLinearRepeatMipPoint()
    {
        return s_SamplerStorage->LinearRepeatMipPoint;
    }

    Count<RenderSampler> SamplerFactory::GetLinearRepeat()
    {
        return s_SamplerStorage->LinearRepeat;
    }
    void SamplerFactory::AddSamplerToFactory(Count<RenderSampler> sampler)
    {
        if (!s_SamplerStorage->SamplerStorage.contains(sampler->GetConfig()))
            s_SamplerStorage->SamplerStorage[sampler->GetConfig()] = WeakCount<RenderSampler>(sampler.Get());
    }

    Count<RenderSampler> SamplerFactory::GetOrAddSamplerToFactory(const SamplerResourceConfig& samplerConfig)
    {
        if (s_SamplerStorage->SamplerStorage.contains(samplerConfig))
        {
           auto sampler = s_SamplerStorage->SamplerStorage.at(samplerConfig);
           return sampler.Lock();

        }
        Count<RenderSampler> sampler = RenderSampler::Create(samplerConfig);
        s_SamplerStorage->SamplerStorage[samplerConfig] = WeakCount<RenderSampler>( sampler.Get());

        return sampler;
    }

    bool SamplerResourceConfig::operator<(const SamplerResourceConfig& other) const
    {
        // have to use everythign to make sure we are keeping unique

            // dont use memory location because each material table will propably have a different materialtabel because eahc mesh componet has a mateiral table
            //uint64_t memlocation1 = (uint64_t)MaterialTable.Get();
            //uint64_t memlocation2 = (uint64_t)other.MaterialTable.Get();

        if (Wrap < other.Wrap)
            return true;

        if (Wrap > other.Wrap)
            return false;

        if (Filter < other.Filter)
            return true;

        if (Filter > other.Filter)
            return false;

        if (BorderColor < other.BorderColor)
            return true;

        if (BorderColor > other.BorderColor)
            return false;

        if (MipMapMode < other.MipMapMode)
            return true;

        if (MipMapMode > other.MipMapMode)
            return false;

        if (CompareOp < other.CompareOp)
            return true;

        if (CompareOp > other.CompareOp)
            return false;

        return Anisotropy < other.Anisotropy;
    }
}
