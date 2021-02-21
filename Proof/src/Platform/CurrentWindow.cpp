#include "Proofprch.h"
#include "CurrentWindow.h"

namespace Proof {
	
	void CurrentWindow::SetWindowSize(int width, int height){
		Application::SetWindowSize(width, height);
	}
	int CurrentWindow::GetWindowWidth() {
		return Application::GetWindowSize().first;
	}
	int CurrentWindow::GetWindowHeight() {
		return Application::GetWindowSize().second;
	}
}
