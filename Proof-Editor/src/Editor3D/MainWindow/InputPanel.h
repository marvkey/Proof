#pragma once
#include "Panel.h"
namespace Proof {
	class Proof_API InputPanel :public Panel {
	public:
		InputPanel(): 
			Panel(false) 
		{
			
		}
		virtual void ImGuiRender(FrameTime deltaTime)override;
	private:
		void Action();
		void MotionInput();
		std::string m_EditedValue;
	};
}