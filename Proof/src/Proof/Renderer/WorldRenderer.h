#pragma once
#include "ScreenFrameBuffer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
namespace Proof
{
	class WorldRenderer {
	public:
		WorldRenderer()=default;

		WorldRenderer(class World* world,uint32_t textureWidth,uint32_t textureHeight):
			m_World(world)
		{
			m_ScreenFrameBuffer=ScreenFrameBuffer::Create(textureWidth,textureHeight);
		}
		
		void Renderer();
		uint32_t GetWorldTexture(){
			return m_ScreenFrameBuffer->GetTexture();
		}
		void SetRendererPause(bool value){
			m_RendererPaused =value;
		}
		bool GetRendererPaused(){return m_RendererPaused;};
		RendererData RenderData;
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;
	private:
		bool  m_RendererPaused =false;
		class World* m_World=nullptr;
	};
}
