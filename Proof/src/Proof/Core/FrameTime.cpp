#include "Proofprch.h"
#include "FrameTime.h"

#include "Application.h"
#include <GLFW/glfw3.h>

namespace Proof {
	float FrameTime::WorldDeltaTime = 0.0f;
	uint64_t FrameTime::FrameCount = 0;

	float FrameTime::GetTime(){
		return glfwGetTime();
	}
	float FrameTime::GetFrameFPS(){
		return Application::GetFPS();
	}
	float FrameTime::GetFrameMS(){
		return Application::GetFrameMS();
	}
	float FrameTime::GetWorldDeltaTime(){
		return WorldDeltaTime;
	}
}