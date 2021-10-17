#pragma once
#include "ScreenFrameBuffer.h"
namespace Proof
{
	class WorldRenderer {
	public:
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
			m_ScreenFrameBuffer->GetFrameBufferID();
		}
	private:
		uint32_t m_Width,m_Height;
		class World* m_World=nullptr;
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;
	};
}
