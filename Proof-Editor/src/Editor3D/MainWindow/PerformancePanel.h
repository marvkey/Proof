#pragma once
#pragma once
#include "Proof/Core/Core.h"
#include "Panel.h"
namespace Proof
{

	class Proof_API PerformancePanel :public Panel {
	public:
		virtual void ImGuiRender(class FrameTime deltaTime);
	private:
	};
};