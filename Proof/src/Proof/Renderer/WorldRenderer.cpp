#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/RendererCommand.h"
namespace Proof{
	void WorldRenderer::Renderer() {
		if(m_RendererPaused==true)
			return;
		RenderData.RendererTechnique = RenderTechnique::FowardRendering;
		Renderer3DPBR::BeginContext(m_World->m_EditorCamera,m_ScreenFrameBuffer,&RenderData);
		for (MeshComponent* Comp : m_World->Registry.SceneMeshComponents) {
			if (Comp->GetMeshSource() != nullptr) {
				Renderer3DPBR::Draw(*Comp);
			}
		}
		for (LightComponent* Comp : m_World->Registry.LightComponents) {
			Renderer3DPBR::Draw(*Comp);
		}
		Renderer3DPBR::GetRenderer()->m_Shader->Bind();
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("irradianceMap",4);
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("prefilterMap",5);
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("brdfLUT",6);

		m_World->m_WorldCubeMap->Bind(4);
		m_World->PrefelterMap->Bind(5);

		m_World->m_brdflTexture->Bind(6);
		Renderer3DPBR::EndContext();
		
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