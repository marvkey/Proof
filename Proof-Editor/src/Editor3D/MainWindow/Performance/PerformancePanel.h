#pragma once
#pragma once
#include "Proof/Core/Core.h"
#include "../Panel.h"
#include "RenderPanel.h"
namespace Proof
{

	class Proof_API PerformancePanel :public Panel {
	public:
		virtual void ImGuiRender(class FrameTime deltaTime);
		
	private:
		RenderPanel m_RenderPanel;
		struct Compare {
			float Time;
			uint32_t Pos;
		};
		void PerformanceData(Compare& comp);
	};
};