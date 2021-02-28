#include"Proofprch.h"
#include "Event.h"

namespace Proof {
	int* Event::Window1 = new int(0);
	int* Event::Window2 = new int(0);

	bool Event::OnWindowResize() {
		unsigned int Windowidth = CurrentWindow::GetWindowHeight();
		unsigned int WindowHeight = CurrentWindow::GetWindowWidth();
		//PF_ENGINE_INFO("WINDOW SIZZE IS {}", Windowidth);
		if ((WindowHeight != *Window1) && (Windowidth != *Window2)) {
			*Window1 = WindowHeight; 
			*Window2 = Windowidth;
			return true;
		}
		return false;
	}
}

