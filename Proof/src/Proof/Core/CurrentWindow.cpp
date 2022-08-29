#include "Proofprch.h"
#include "CurrentWindow.h"
#include "Proof/Core/Application.h"
namespace Proof {
	
	Window& CurrentWindow::GetWindow(){
		return *Application::MainWindow.get();
	}
}
