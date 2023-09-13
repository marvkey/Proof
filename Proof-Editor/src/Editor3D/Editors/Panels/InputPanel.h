#pragma once
#include "Panel.h"
namespace Proof {
	class InputPanel :public Panel {
	public:
		InputPanel() 
		{
			
		}
		virtual void OnImGuiRender(const char* dsiplayName, bool& isOpen)override;
	private:
		void Action();
		void MotionInput();
		std::string m_EditedValue;
	};
}