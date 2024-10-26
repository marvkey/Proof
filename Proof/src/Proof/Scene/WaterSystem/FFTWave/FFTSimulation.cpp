#include "Proofprch.h"
#include "FFTSimulation.h"
#include "FFTWave.h"

#include "ClipMapMeshBuilder.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RendererSampler.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/SceneUtils.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include "ClipMapMeshBuilder.h"
#include "Proof/Renderer/ComputePass.h"
#include "Proof/Renderer/ComputePipeline.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "FFTOceanSimulationInput.h"
#include "../Water.h"
namespace Proof
{
	FFTSimulation::FFTSimulation(Count<class FFTWave> wave)
	{
		m_Wave = wave;
        Init();
	}
    void FFTSimulation::Init()
    {
        
        m_InitialSpectrumPass = ComputePass::Create({ "FFTSimulationIInitialSpectrum", 
            ComputePipeline::Create({"FFTSimulationIInitialSpectrum",Renderer::GetShader("InitialSpectrum")}) });

        m_InitialSpectrumPass = ComputePass::Create({ "TimeDependentSpectrum",
           ComputePipeline::Create({"TimeDependentSpectrum",Renderer::GetShader("TimeDependentSpectrum")}) });

      //      ComputePipeline::Create({"TimeDependentSpectrum",Renderer::GetShader("TimeDependentSpectrum")}) });
      //  m_FoamSimulationPass = ComputePass::Create({ "TimeDependentSpectrum",

        m_CurrentTextureParams = -glm::ivec2{ 1 };

    }

 

    void FFTSimulation::Update(float deltaTime)
    {

    }

    bool FFTSimulation::SetUp()
    {
        m_SBSpectrumBuffer = StorageBufferSet::Create(m_Spectrums.size() * sizeof(UBSpectrumParams));

        glm::ivec2 newTextureParams = glm::ivec2(SimulationSettings.Resolution,
            SimulationSettings.CascadeNumber);
        if (newTextureParams == m_CurrentTextureParams)
        {
            //_fftInOut.anisoLevel = SimulationSettings.AnisoLevel;
            //_turbulence.anisoLevel = SimulationSettings.AnisoLevel;
            return true;
        }

        m_IsSpectrumInitialized = false;
        m_CurrentTextureParams = newTextureParams;
        m_Size = (uint32_t)SimulationSettings.Resolution;
        InitializeRenderTextures(m_Size, SimulationSettings.CascadeNumber, SimulationSettings.AnisoLevel);
        //SetCascadesKeywords(SimulationSettings.CascadeNumber);

        switch (SimulationSettings.CascadeNumber)
        {
            case Proof::Two:
                m_Wave->m_GlobalStruct.ActiveCascades = glm::vec4(1, 1, 0, 0);
                break;
            case Proof::Three:
                m_Wave->m_GlobalStruct.ActiveCascades = glm::vec4(1, 1, 1, 0);
                break;
            case Proof::Four:
                m_Wave->m_GlobalStruct.ActiveCascades = glm::vec4(1, 1, 1, 1);
                break;
            default:
                break;
        }
    }

    void FFTSimulation::InitializeRenderTextures(uint32_t size, uint32_t cascadesNumber, uint32_t anisoLevel)
	{
        
        TextureConfiguration initialsDescriptor;
        initialsDescriptor.Height = size;
        initialsDescriptor.Width = size;
        initialsDescriptor.Depth = cascadesNumber;
        initialsDescriptor.Storage = true;
        initialsDescriptor.GenerateMips = false;
        initialsDescriptor.Array = true;

        initialsDescriptor.DebugName = "InitialSpectrum";
        Textures.InitialSpectrum = Texture2D::Create(initialsDescriptor, SamplerFactory::GetPoint());

        initialsDescriptor.DebugName = "WavesData";
        Textures.WavesData = Texture2D::Create(initialsDescriptor, SamplerFactory::GetPoint());

        initialsDescriptor.DebugName = "WavesData";
        Textures.InitializationBuffer = Texture2D::Create(initialsDescriptor, SamplerFactory::GetPoint());

        auto displacementAndDerivativesTextureDescriptor = initialsDescriptor;
        displacementAndDerivativesTextureDescriptor.GenerateMips = true;
        displacementAndDerivativesTextureDescriptor.Depth = 2 * cascadesNumber;
        displacementAndDerivativesTextureDescriptor.DebugName = "DisplacementAndDerivativesTexture";

        auto turbulenceTextureDescriptor = initialsDescriptor;
        turbulenceTextureDescriptor.GenerateMips = true;
        turbulenceTextureDescriptor.DebugName = "TurbulenceTexture";

        SamplerResourceConfig samplerConfig = SamplerFactory::GetTrilinear()->GetConfig();
        samplerConfig.DebugName = "Turbulence and FFtInOut";
        samplerConfig.Anisotropy = anisoLevel;
        Count<RenderSampler> turbulenceAndFFTinOutSampler = RenderSampler::Create(samplerConfig);

        Textures.Turbulence = Texture2D::Create(initialsDescriptor, turbulenceAndFFTinOutSampler);
        Textures.WavesData = Texture2D::Create(initialsDescriptor, turbulenceAndFFTinOutSampler);
	}

    void FFTSimulation::CalculateInitialCascade()
    {
        m_InputProvider.PopulateInputs(m_Inputs, WindForce);

        glm::vec4 cutOffsLow, cutOffsHigh;

        SimulationSettings.CalculateCascadeDomains(cutOffsLow, cutOffsHigh);
    }
  
}
