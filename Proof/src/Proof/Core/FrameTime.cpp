#include "Proofprch.h"
#include "FrameTime.h"
#include <GLFW/glfw3.h>

namespace Proof {
	float FrameTime::GetTime(){
		return glfwGetTime();
	}
	float FrameTime::GetFrameFPS(){
		return Application::GetFPS();
	}
	float FrameTime::GetFrameMS(){
		return Application::GetFrameMS();
	}
}