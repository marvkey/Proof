#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/RendererCommand.h"
namespace Proof{
	static glm::mat4 Projection;
	void WorldRenderer::Renderer() {
		m_ScreenFrameBuffer->Bind();

		RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		RendererCommand::SetClearColor(0.1f,0.1f,0.1f,1.0f);

		Projection = glm::perspective(glm::radians(45.f),(float)m_Width / (float)m_Height,0.1f,100.0f);
		Renderer2D::BeginContext(Projection,m_World->EditorCamera.GetCameraView());
		for (SpriteComponent* Comp : m_World->Registry.SpriteComponents) {
			Renderer2D::DrawQuad(*Comp);
		}

		Renderer3DPBR::BeginContext(Projection,m_World->EditorCamera);
		for (MeshComponent* Comp : m_World->Registry.SceneMeshComponents) {
			if (Comp->GetMesh() != nullptr) {
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
		Renderer3DPBR::Reset();

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