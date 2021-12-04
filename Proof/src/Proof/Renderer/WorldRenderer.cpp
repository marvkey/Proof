#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/RendererCommand.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
namespace Proof{
	void WorldRenderer::Renderer() {
		if(m_RendererPaused==true)
			return;
		Renderer3DPBR::BeginContext(m_World->m_EditorCamera,m_ScreenFrameBuffer,RenderData);
		auto& meshView =m_World->m_Registry.view<MeshComponent, TransformComponent>();
		for (auto& enity: meshView) {
			auto& [mesh, transform] = meshView.get<MeshComponent, TransformComponent>(enity);
			Renderer3DPBR::Draw(mesh, transform.GetWorldTransform());
		}
		auto& lightView = m_World->m_Registry.view<LightComponent, TransformComponent>();
		for (auto& enity : lightView) {
			auto& [light, transform] = meshView.get<LightComponent, TransformComponent>(enity);
			Renderer3DPBR::Draw(light,transform);
		}
		/*
		if (RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
			Renderer3DPBR::GetRenderer()->m_Shader->Bind();
			Renderer3DPBR::GetRenderer()->m_Shader->SetInt("irradianceMap", 4);
			Renderer3DPBR::GetRenderer()->m_Shader->SetInt("prefilterMap", 5);
			Renderer3DPBR::GetRenderer()->m_Shader->SetInt("brdfLUT", 6);
			
			m_World->m_WorldCubeMap->Bind(4);
			m_World->PrefelterMap->Bind(5);
			
			m_World->m_brdflTexture->Bind(6);
		}
		*/
		Renderer3DPBR::EndContext();
		m_ScreenFrameBuffer->Bind();
		//for (CubeColliderComponent* collider : m_World->Registry.m_CubeColliderComponent) {
		//	Renderer3DPBR::DrawDebugMesh(collider->GetMeshSource(), *collider->GetOwner().GetComponent<TransformComponent>());
		//}
		m_ScreenFrameBuffer->UnBind();
		if (RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
			m_ScreenFrameBuffer->Bind();
			RendererCommand::DepthFunc(DepthType::Equal);
			m_World->backgroundShader->Bind();
			m_World->backgroundShader->SetInt("environmentMap",0);
			m_World->m_WorldCubeMap->Bind(0);
			m_World->m_IBLSkyBoxVertexArray->Bind();
			RendererCommand::DrawArray(36);
			m_World->m_IBLSkyBoxVertexArray->UnBind();
			RendererCommand::DepthFunc(DepthType::Less);
			m_ScreenFrameBuffer->UnBind();
		}
		
	}
}