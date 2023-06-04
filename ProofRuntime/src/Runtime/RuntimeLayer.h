#pragma once
#include "Proof/Core/Layer.h"

namespace Proof
{
	class RuntimeLayer : public Layer {
	public:
		virtual void OnEvent(Event& e);
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnImGuiDraw(FrameTime DeltaTime) override;

	private:
		void SetDocking(bool* p_open);

		bool m_WindowResize = false;
		Count<class World> m_World;
		uint32_t m_PlayerCount = 1;
		bool m_DrawFirstPlayer =true;
		std::vector<Count<class WorldRenderer>> m_WorldRenderers;
		void RenderScreenPlayerCount(class Entity entity, Count<World> world);
	};
}
