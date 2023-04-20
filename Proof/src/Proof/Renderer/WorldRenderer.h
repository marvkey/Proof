#pragma once
#include "FrameBuffer.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Scene/World.h"
namespace Proof
{
	class WorldRenderer {
	public:
		WorldRenderer()=default;
		virtual ~WorldRenderer();

		WorldRenderer(Count<World>world, uint32_t textureWidth, uint32_t textureHeight);
		void Resize(ScreenSize windowSize);
		void SetContext(Count<World>world) {
			m_World = world;
		}
		
		void Clear();
		void Render(EditorCamera& camera);
		void Render(CameraComponent& comp, Vector& location);
		void Render(CameraComponent& comp, Vector& location, Viewport viewport, ViewportScissor scissor, bool clearPreviousFrame = true);
		Image GetImage(){
			return m_ScreenFrameBuffer->GetImage();
		}
		Count<ScreenFrameBuffer>m_ScreenFrameBuffer;
		

	private:
		void Render(const glm::mat4&projection, const glm::mat4& view,const Vector& location, Viewport viewPort, ViewportScissor scissor, bool clearPreviousFrame = true);

		Special<Renderer3DPBR> m_Renderer3D;
		Special<class DebugMeshRenderer> m_DebugMeshRenderer;
		Special<class Renderer2D>  m_Renderer2D;
		Count<RenderCommandBuffer> m_CommandBuffer;
		Count<class RenderPass> m_RenderPass;
		Count<World>m_World=nullptr;
	};
}
