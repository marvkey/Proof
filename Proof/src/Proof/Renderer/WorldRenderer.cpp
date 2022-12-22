#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <tuple>
#include <variant>
#include "MeshWorkShop.h"
#include "Renderer.h"
namespace Proof
{
	void WorldRenderer::Resize(ScreenSize windowSize) {
		m_ScreenFrameBuffer->Resize(windowSize.X, windowSize.Y);
	}
	void WorldRenderer::Render(EditorCamera& camera) {
		if (m_RendererPaused == true)
			return;
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_World->GetName().c_str());
	
		Renderer3DPBR::BeginContext(camera, m_ScreenFrameBuffer);
		// MESHES
		{
			m_World->ForEachEnitityWith<MeshComponent>([&](Entity entity) {
				Renderer3DPBR::SubmitMesh(*entity.GetComponent<MeshComponent>(), m_World->GetWorldTransform(entity));
			});
		}
		Renderer3DPBR::EndContext();
		
		// for some reason this triggers the imgui vkmcd cannot draw 
		#if 0
			Renderer2D::BeginContext(m_World->m_EditorCamera.m_Projection, m_World->m_EditorCamera.m_View, m_World->m_EditorCamera.m_Positon, m_ScreenFrameBuffer);
			// MESHES
			{
				m_World->ForEachComponent<SpriteComponent, TransformComponent>([&](SpriteComponent& sprite, TransformComponent& transform) {
					Renderer2D::DrawQuad(sprite, transform);
				});

			}
			Renderer2D::EndContext();
		#endif
	}
	void WorldRenderer::Render(CameraComponent& comp, Vector& location) {


		Renderer3DPBR::BeginContext(comp.GetProjection(), comp.GetView(), location, m_ScreenFrameBuffer);
		// MESHES
		{
			m_World->ForEachEnitityWith<MeshComponent>([&](Entity entity) {
				Renderer3DPBR::SubmitMesh (*entity.GetComponent<MeshComponent>(), m_World->GetWorldTransform(entity));
			});

		}
		

		Renderer3DPBR::EndContext();
		
		#if 0
		Renderer2D::BeginContext(comp.GetProjection(), comp.GetView(), location, m_ScreenFrameBuffer);
		// MESHES
		{
			m_World->ForEachComponent<SpriteComponent,TransformComponent>([&](SpriteComponent& sprite,TransformComponent& transform) {
				Renderer2D::DrawQuad(sprite, transform);
			});

		}
		Renderer2D::EndContext();
		#endif
	}
}