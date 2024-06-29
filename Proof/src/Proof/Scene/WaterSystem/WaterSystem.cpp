#include "Proofprch.h"
#include "WaterSystem.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Asset/MeshImpoter.h"
namespace Proof
{
	WaterSystem::WaterSystem()
	{
		BaseInit();
	}

	WaterSystem::WaterSystem(Count<WaterSystem> other)
	{
		BaseInit();

		Texture = other->Texture;
		WaterData = other->WaterData;
	}

	void WaterSystem::Update(float deltaTime)
	{
		
	}

	void WaterSystem::Render(Count<class WorldRenderer> renderer, const glm::mat4& transform)
	{
		m_UBWaterDataSet->SetData(Renderer::GetCurrentFrameInFlight(), Buffer(&WaterData, sizeof(WaterData)));
		m_RenderMaterial->Set("u_TextureMap", Renderer::GetWhiteTexture());
		//renderer->SubmitWaterMesh(m_Plane, m_RenderMaterial, transform);
		renderer->SubmitMesh(m_Plane, m_RenderMaterial, transform);
	}

	void WaterSystem::BaseInit()
	{
		m_RenderMaterial = RenderMaterial::Create(RenderMaterialConfiguration{ "WaterSystem", Renderer::GetShader("WaterSystem") });
		m_UBWaterDataSet = UniformBufferSet::Create(sizeof(WaterData));
		m_Plane = MeshWorkShop::GeneratePlane(10, 50);

		//MeshImporter importer("DefaultPlane.fbx");
		//m_Plane = Count<Mesh>::Create(importer.ImportToMeshSource());
		m_Plane->SetName("WaterSystemPlane");
		AssetManager::CreateRuntimeAsset(m_Plane.As<Asset>(), "WaterSystemPlane");
		m_RenderMaterial->Set("WaterData", m_UBWaterDataSet);
	}

}