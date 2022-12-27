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
			m_CommandBuffer = CommandBuffer::Create();
		}
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
		Count<CommandBuffer> m_CommandBuffer;
		Count<World>m_World=nullptr;
	};
}
