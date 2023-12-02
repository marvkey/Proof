#include "Proofprch.h"
#include "RenderereCustomTypeSet.h"
#include "Renderer.h"

namespace Proof
{
    uint32_t RenderCustomTypeSetHelper::GetCurrentFrameInFlight()
    {
        return Renderer::GetCurrentFrameInFlight();
    }

    uint32_t RenderCustomTypeSetHelper::RenderCustomTypeSetHelper::RT_GetCurrentFrameInFlight()
    {
        return Renderer::RT_GetCurrentFrameInFlight();
    }

    uint32_t RenderCustomTypeSetHelper::GetMaxFrameFlight()
    {
        return Renderer::GetConfig().FramesFlight;
    }
}
