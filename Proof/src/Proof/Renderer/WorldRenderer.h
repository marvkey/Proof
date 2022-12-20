#pragma once
#include "ScreenFrameBuffer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Scene/World.h"
namespace Proof
{
	class WorldRenderer {
	public:
		WorldRenderer()=default;

		WorldRenderer(Count<World>world,uint32_t textureWidth,uint32_t textureHeight):
			m_World(world)
		{
			m_ScreenFrameBuffer=ScreenFrameBuffer::Create(textureWidth,textureHeight);
		}
		void Resize(ScreenSize windowSize);
		void SetContext(Count<World>world) {
			m_World = world;
		}
		void Render();
		void Render(CameraComponent& comp, Vector& location);
		void* GetWorldTexture(){
			return m_ScreenFrameBuffer->GetTexture();
		}
		void SetRendererPause(bool value){
			m_RendererPaused =value;
		}
		bool GetRendererPaused(){return m_RendererPaused;};
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;
	private:
		bool  m_RendererPaused =false;
		Count<World>m_World=nullptr;
	};
}
