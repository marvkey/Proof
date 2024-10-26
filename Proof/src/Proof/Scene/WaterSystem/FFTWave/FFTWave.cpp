#include "Proofprch.h"
#include "FFTWave.h"
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
#include "FFTRenderer.h"
namespace Proof
{
    Count<Image2D> CreateRenderTex(std::string debugName, uint32_t width, uint32_t height, uint32_t depth, ImageFormat  format, bool useMips) {

        ImageConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;

        rt.Layers = depth;
        //rt.ImageLayerViewsBaseType = ;
        return Image2D::Create(rt);
    }

    Count<Image2D> CreateRenderTex(std::string debugName, uint32_t width, uint32_t height, ImageFormat  format, bool useMips) 
    {
        ImageConfiguration rt;
        rt.DebugName = debugName;
        rt.Width = width;
        rt.Height = height;
        rt.Format = format;

        return Image2D::Create(rt);
    }
    FFTWave::FFTWave(Count<FFTWave> other)
        : Wave(other->m_Water, WaveType::FastFourierTransformWave)
    {
    }
    FFTWave::FFTWave(Count<class Water> water, WaveType type)
        : Wave(water, type)
    {
        m_FFTRenderer = Count<FFTRenderer>::Create(this);

    }
    void FFTWave::Update(float deltaTime)
    {
        m_FFTRenderer->UpdateMesh();
        m_FFTRenderer->UpdateVariables();
    }
    void FFTWave::InitPasses()
	{
        m_Material = Count<Material>::Create("FFTWave",Renderer::GetShader("FFTWave"));
        m_FFTRenderer->UpdateMesh();
	}
  
    Count<class RenderMaterial> FFTWave::GetRenderMaterial()
    {
        return m_Material->GetRenderMaterial();
    }
}
