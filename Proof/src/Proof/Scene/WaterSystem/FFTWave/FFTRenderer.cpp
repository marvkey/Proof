#include "Proofprch.h"
#include "FFTRenderer.h"
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
#include "../Water.h"
namespace Proof
{
	FFTRenderer::FFTRenderer(Count<class FFTWave> wave)
	{
		m_Wave = wave;
        UpdateMesh();
	}

	void FFTRenderer::UpdateMesh()
	{
        Transform transform;

        auto world = m_Wave->GetWorld();
        auto water = m_Wave->GetWater();
        if (m_Wave->GetWorld())
        {
            if (Entity entity = world->TryGetEntityWithUUID(m_MeshSettings.EntityViewer))
            {
                transform.SetTransform(world->GetWorldSpaceTransform(entity));
            }
            else
            {
                transform.SetTransform(water->GetTransform());
            }
        }
        else
        {
            transform.SetTransform(water->GetTransform());
        }

        int clipLevelHalfSize = ClipMeshBuilder::ClipLevelHalfSize(m_MeshSettings.VertexDensity);
        int pow = (int)glm::floor(glm::max(0.f,
            glm::log2(glm::abs(transform.Location.y) / (2.0f * m_MeshSettings.MeshScale)) + 1.0f));
        float meshScale = m_MeshSettings.MeshScale / clipLevelHalfSize * glm::pow(2.0f, pow);

        m_Wave->m_GlobalStruct.ClipMapViewerPosition = transform.Location;
        m_Wave->m_GlobalStruct.ClipMapScale = meshScale;
        m_Wave->m_GlobalStruct.ClipMapLevelHalfSize = clipLevelHalfSize;

        glm::ivec2 newMeshParams = glm::ivec2(m_MeshSettings.VertexDensity, m_MeshSettings.ClipMapLevels);

        if (newMeshParams == m_CurrentMeshSettings && WaveMesh != nullptr)
            return;

        m_CurrentMeshSettings = newMeshParams;
        WaveMesh = ClipMeshBuilder::BuildClipMap(newMeshParams.x, newMeshParams.y);
	}
    void FFTRenderer::UpdateVariables()
    {
        auto& globalStruct = m_Wave->m_GlobalStruct;
        globalStruct.OceanDeepScatterColor = ColorPreset.DeepScatter;
        globalStruct.OceanSssColor = ColorPreset.ShallowScatter;
        globalStruct.OceanDiffuseColor = ColorPreset.Diffuse;
        //m_GlobalStruct. = ColorPreset.ReflectionMask;

        // TODO TEMPORAL NEEED TO FIX
        {
            globalStruct.OceanAbsorptionColor0 = ColorPreset.Absorption[0];
            globalStruct.OceanAbsorptionColor1 = ColorPreset.Absorption[1];
            globalStruct.OceanAbsorptionColor2 = ColorPreset.Absorption[2];
            globalStruct.OceanAbsorptionColor3 = ColorPreset.Absorption[3];
            globalStruct.OceanAbsorptionColor4 = ColorPreset.Absorption[4];
            globalStruct.OceanAbsorptionColor5 = ColorPreset.Absorption[5];
            globalStruct.OceanAbsorptionColor6 = ColorPreset.Absorption[6];
            globalStruct.OceanAbsorptionColor7 = ColorPreset.Absorption[7];
        }

        globalStruct.OceanFogDensity = MaterialProps.FogDensity;
        globalStruct.OceanAbsorptionDepthScale = MaterialProps.AbsorptionDepthScale;
        globalStruct.ReflectionMaskRadius = MaterialProps.ReflectionMaskRadius;
        globalStruct.ReflectionMaskSharpness = MaterialProps.ReflectionMaskSharpness;
    }
}
