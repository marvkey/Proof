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
		auto& meshGroup =m_World->m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
		for (auto& enity: meshGroup) {
			auto& [mesh, transform] = meshGroup.get(enity);
			Renderer3DPBR::Draw(mesh, transform.GetWorldTransform());
		}
		auto& lightGroup = m_World->m_Registry.group<LightComponent>(entt::get<TransformComponent>);
		for (auto& enity : lightGroup) {
			const auto& [light, transform] = lightGroup.get(enity);
			Renderer3DPBR::Draw(light,transform);
		}
		
		if (RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
			Renderer3DPBR::GetRenderer()->m_Shader->Bind();
			Renderer3DPBR::GetRenderer()->m_Shader->SetInt("irradianceMap", 4);
			Renderer3DPBR::GetRenderer()->m_Shader->SetInt("prefilterMap", 5);
			Renderer3DPBR::GetRenderer()->m_Shader->SetInt("brdfLUT", 6);
			
			m_World->m_WorldCubeMap->Bind(4);
			m_World->PrefelterMap->Bind(5);
			
			m_World->m_brdflTexture->Bind(6);
		}
		
		Renderer3DPBR::EndContext();
		m_ScreenFrameBuffer->Bind();
		auto& CubeColliderGroup = m_World->m_Registry.group<CubeColliderComponent>(entt::get<TransformComponent>);
		for (auto& enity : CubeColliderGroup) {
			const auto& [collider, transform] = CubeColliderGroup.get(enity);
			TransformComponent temp = transform + collider.Offset;
			Renderer3DPBR::DrawDebugMesh(collider.GetMeshSource(),temp.GetLocalTransform());
		}
		
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