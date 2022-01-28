#pragma once
#include "Proof/Core/Core.h"
#include "Panel.h"
namespace Proof {
	class Proof_API InputPanel :public Panel {
	public:
		virtual void ImGuiRender(FrameTime deltaTime)override;
	private:
		void Action();
		void MotionInput();
		std::string m_EditedValue;
	};
}