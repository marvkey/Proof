#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/RendererCommand.h"
namespace Proof{
	void WorldRenderer::Renderer() {
		//if(m_RendererPaused==true)
		//	return;
		Renderer3DPBR::BeginContext(m_World->m_EditorCamera,m_ScreenFrameBuffer);
		for (MeshComponent* Comp : m_World->Registry.SceneMeshComponents) {
			if (Comp->GetMesh() != nullptr) {
				Renderer3DPBR::Draw(*Comp);
			}
		}
		for (LightComponent* Comp : m_World->Registry.LightComponents) {
			Renderer3DPBR::Draw(*Comp);
		}
		Renderer3DPBR::EndContext();
	}
}