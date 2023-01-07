#pragma once
#include "ScreenFrameBuffer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Scene/World.h"
namespace Proof
{
	class WorldRenderer {
	public:
		WorldRenderer()=default;

		WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight);
		void Resize(ScreenSize windowSize);
		void SetContext(Count<World>world) {
			m_World = world;
		}
		
		void Render(EditorCamera& camera);
		void Render(CameraComponent& comp, Vector& location);
		void* GetWorldTexture(){
			return m_ScreenFrameBuffer->GetTexture();
		}
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;
		
	private:
		Special<Renderer3DPBR> m_Renderer3D;
		Special<class Renderer2D>  m_Renderer2D;
		Count<CommandBuffer> m_CommandBuffer;
		Count<class RenderPass> m_RenderPass;
		Count<World>m_World=nullptr;
	};
}
