#pragma once
#include "Proof/Core/Layer.h"

namespace Proof
{
	class RuntimeLayer : public Layer {
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(FrameTime DeltaTime) override;
	private:
		Count<class World> m_World;
	};
}
