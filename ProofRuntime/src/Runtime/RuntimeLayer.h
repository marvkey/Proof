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
	private:
		bool m_WindowResize = false;
		Count<class World> m_World;
		uint32_t m_PlayerCount = 1;
		Count<class WorldRenderer> m_WorldRenderer;
	};
}
