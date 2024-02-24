#include "Proofprch.h"
#include "RendererSampler.h"
#include "Proof/Platform/Vulkan/VulkanSampler.h"
#include "Renderer.h"
namespace Proof
{
    Count<RenderSampler> RenderSampler::Create(const SamplerResourceConfig& config)
    {
        switch (Renderer::GetAPI())
        {
        case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
        case Renderer::API::OpenGL: return nullptr;
        case Renderer::API::Vulkan: return Count<VulkanRendererSampler>::Create(config);
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

        Count<RenderSampler> Linear;
        Count<RenderSampler> Point;
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
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::Clamp;
            s_SamplerStorage->PointClampEdge = RenderSampler::Create(config);
        }
        //point clamp border0000
        {
            SamplerResourceConfig config;
            config.DebugName = "PointClampBorder0000";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::MirrorOnce;
            config.BorderColor = SamplerBorderColor::TransperantBlack;
            s_SamplerStorage->PointClampBorder0000 = RenderSampler::Create(config);
        }

        //point clamp border1111
        {
            SamplerResourceConfig config;
            config.DebugName = "PointClampBorder1111";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::MirrorOnce;
            config.BorderColor = SamplerBorderColor::OpaqueWhite;
            s_SamplerStorage->PointClampBorder1111 = RenderSampler::Create(config);
        }

        //PointRepeat
        {
            SamplerResourceConfig config;
            config.DebugName = "PointRepeat";
            config.Filter = SamplerFilter::Nearest;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::Wrap;
            s_SamplerStorage->PointRepeat = RenderSampler::Create(config);
        }

        //LinearClampEdge
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampEdge";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::LINEAR;
            config.AddressMode = SamplerAddressMode::Clamp;
            s_SamplerStorage->LinearClampEdge = RenderSampler::Create(config);
        }

        //LinearClampEdgeMipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampEdgeMipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::Clamp;
            s_SamplerStorage->LinearClampEdgeMipPoint = RenderSampler::Create(config);
        }

        //LinearClampBorder0000MipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampBorder0000MipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::MirrorOnce;
            config.BorderColor = SamplerBorderColor::TransperantBlack;
            s_SamplerStorage->LinearClampBorder0000MipPoint = RenderSampler::Create(config);
        }

        //LinearClampBorder1111MipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearClampBorder1111MipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::MirrorOnce;
            config.BorderColor = SamplerBorderColor::OpaqueWhite;
            s_SamplerStorage->LinearClampBorder1111MipPoint = RenderSampler::Create(config);
        }

        //LinearRepeatMipPoint
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearRepeatMipPoint";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.AddressMode = SamplerAddressMode::Wrap;
            s_SamplerStorage->LinearRepeatMipPoint = RenderSampler::Create(config);
        }

        //LinearRepeat
        {
            SamplerResourceConfig config;
            config.DebugName = "LinearRepeat";
            config.Filter = SamplerFilter::Linear;
            config.MipMapMode = SamplerMipMapMode::LINEAR;
            config.AddressMode = SamplerAddressMode::Wrap;
            s_SamplerStorage->LinearRepeat = RenderSampler::Create(config);
        }

        //point sampler
        {

            SamplerResourceConfig config;
            config.DebugName = "Point";
            config.AddressMode.AddressW = SamplerAddressMode::Wrap;
            config.AddressMode.AddressU = SamplerAddressMode::Clamp;
            config.AddressMode.AddressV = SamplerAddressMode::Clamp;
            config.CompareOp = SamplerCompare::Never;
            config.MipMapMode = SamplerMipMapMode::NEAREST;
            config.Filter = SamplerFilter::Nearest;
            s_SamplerStorage->Point = RenderSampler::Create(config);
        }

        //linear sampler
        {

            SamplerResourceConfig config;
            config.DebugName = "Linear";
            config.MipMapMode = SamplerMipMapMode::LINEAR;
            config.Filter = SamplerFilter::Linear;
            config.AddressMode.AddressW = SamplerAddressMode::Wrap;
            config.AddressMode.AddressU = SamplerAddressMode::Clamp;
            config.AddressMode.AddressV = SamplerAddressMode::Clamp;
            config.CompareOp = SamplerCompare::Never;
            s_SamplerStorage->Linear = RenderSampler::Create(config);
        }
    }
    void SamplerFactory::ShutDown()
    {
        delete s_SamplerStorage;
        s_SamplerStorage = nullptr;
    }
    Count<RenderSampler> SamplerFactory::GetLinear()
    {
        return s_SamplerStorage->Linear;
    }
    Count<RenderSampler> SamplerFactory::GetPoint()
    {
        return s_SamplerStorage->Point;
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
}
