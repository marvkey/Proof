#include "Proofprch.h"
#include "FFTOceanSimulationInput.h"
#include "Proof/Renderer/Texture.h"

namespace Proof
{
    Count<Texture2D> FFTEqualizerPreset::GetDefaultRamp()
    {
        TextureConfiguration initialsDescriptor;
        initialsDescriptor.DebugName = "DefaultRamp";
        initialsDescriptor.Height = 1;
        initialsDescriptor.Width = 1;
        initialsDescriptor.Storage = true;
        initialsDescriptor.GenerateMips = false;
        initialsDescriptor.Format = ImageFormat::RG16F;;

        if (s_DefaultRamp == nullptr)
            s_DefaultRamp = Texture2D::Create(initialsDescriptor,SamplerWrap::ClampEdge, SamplerFilter::Linear);
        return s_DefaultRamp;
    }

    Count<Texture2D> FFTEqualizerPreset::GetRamp()
    {
        if (Ramp == nullptr)
            BakeRamp();
        return Ramp;
    }

    void FFTEqualizerPreset::BakeRamp()
    {
        if (Ramp == nullptr || Ramp->GetWidth() != Resolution)
        {
            TextureConfiguration initialsDescriptor;
            initialsDescriptor.DebugName = "Ramp";
            initialsDescriptor.Width = Resolution;
            initialsDescriptor.Height = 1;
            initialsDescriptor.Storage = true;
            initialsDescriptor.GenerateMips = false;
            initialsDescriptor.Format = ImageFormat::RG16F;;
            Ramp = Texture2D::Create(initialsDescriptor, SamplerWrap::ClampEdge, SamplerFilter::Linear);
        }

        for (int i = 0; i < Resolution; i++)
        {
            float x = glm::mix(XMin, XMax, (float)i / Resolution);
            Colors[i].r = EvaluateFiltersArray(ScaleFilters, x);
            Colors[i].g = EvaluateFiltersArray(ChopFilters,x);
        }

        Buffer buffer((const void*)Colors.data(), Colors.size() * sizeof(glm::vec4));
        Ramp->SetData(buffer);
    }
}
