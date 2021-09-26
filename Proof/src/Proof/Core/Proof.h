#pragma once
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Platform/Window/WindowsWindow.h"
#include "Proof/Core/Application.h"
#include "Proof/Core/Log.h"
#include "Debug/Instrumentor.h"
#if defined(_WIN64)
extern Proof::Application* Proof::CreateApplication();
int main(int argc, char** argv){
		
		Proof::KeyClickedEvent::Instance = std::make_unique<Proof::KeyClickedEvent>();
		Proof::KeyReleasedEvent::Instance = std::make_unique<Proof::KeyReleasedEvent>();
		Proof::KeyRepeatEvent::Instance = std::make_unique<Proof::KeyRepeatEvent>();
		Proof::KeyDoubleClickEvent::Instance = std::make_unique<Proof::KeyDoubleClickEvent>();
		
		Proof::MouseClickedEvent::Instance = std::make_unique<Proof::MouseClickedEvent>();
		Proof::MouseReleasedEvent::Instance = std::make_unique<Proof::MouseReleasedEvent>();
		Proof::MouseDoubleClickEvent::Instance = std::make_unique<Proof::MouseDoubleClickEvent>();
		Proof::MouseMoveEvent::Instance = std::make_unique<Proof::MouseMoveEvent>();
		Proof::MouseScrollEvent::Instance = std::make_unique<Proof::MouseScrollEvent>();

		Proof::WindowResizeEvent::Instance = std::make_unique<Proof::WindowResizeEvent>();
		Proof::WindowMoveEvent::Instance = std::make_unique<Proof::WindowMoveEvent>();
		Proof::WindowCloseEvent::Instance = std::make_unique<Proof::WindowCloseEvent>();
		Proof::WindowFocusEvent::Instance = std::make_unique<Proof::WindowFocusEvent>();
		Proof::Window_ViewPortResize::Instance = std::make_unique<Proof::Window_ViewPortResize>();
		srand(time(NULL));

		Proof::Log::Init();
		//PF_PROFILE_BEGIN_SESSION("Startup","ProofProfile-Startup.json");
		auto app = Proof::CreateApplication();
		//PF_PROFILE_END_SESSION();

		//PF_PROFILE_BEGIN_SESSION("Runtime","ProofProfile-Runtime.json");
		app->Run();
		//PF_PROFILE_END_SESSION();

		//PF_PROFILE_BEGIN_SESSION("Shutdonw","ProofProfile-Shotdown.json");
		delete app;
		//PF_PROFILE_END_SESSION();
}
#endif
