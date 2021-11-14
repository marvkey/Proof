#pragma once
#include "ScreenFrameBuffer.h"
namespace Proof
{
	class WorldRenderer {
	public:
		WorldRenderer()=default;

		WorldRenderer(class World* world,uint32_t textureWidth,uint32_t textureHeight):
			m_World(world),m_Width(textureHeight),m_Height(textureHeight)
		{
			m_ScreenFrameBuffer=ScreenFrameBuffer::Create(textureWidth,textureHeight);
		}
		void Resize(uint32_t textureWidth,uint32_t textureHeight){
			m_ScreenFrameBuffer->Resize(textureWidth,textureHeight);
			m_Width =textureWidth;
			m_Height = textureHeight;
		}
		void Renderer();
		uint32_t GetWorldTexture(){
			return m_ScreenFrameBuffer->GetTexture();
		}
		void SetRendererPause(bool value){
			m_RendererPaused =value;
		}
		bool GetRendererPaused(){return m_RendererPaused;};
	private:
		bool  m_RendererPaused =false;
		uint32_t m_Width=200,m_Height=200;
		class World* m_World=nullptr;
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;	
	};
}
