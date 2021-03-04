#pragma once
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Platform/WindowsWindow.h"
#include "Proof/Core/Application.h"
#include "Platform/WindowsWindow.h"
#if defined(_WIN64)
extern Proof::Application* Proof::CreateApplication();
int  main(int argc, char** argv){

		
		Proof::KeyPressedEvent::Instance = std::make_shared<Proof::KeyPressedEvent>();
		Proof::KeyReleasedEvent::Instance = std::make_shared<Proof::KeyReleasedEvent>();
		Proof::KeyRepeatEvent::Instance = std::make_shared<Proof::KeyRepeatEvent>();
		Proof::KeyDoubleClickEvent::Instance = std::make_shared<Proof::KeyDoubleClickEvent>();

		Proof::MousePressedEvent::Instance = std::make_shared<Proof::MousePressedEvent>();
		Proof::MouseReleasedEvent::Instance = std::make_shared<Proof::MouseReleasedEvent>();
		Proof::MouseDoubleClickEvent::Instance = std::make_shared<Proof::MouseDoubleClickEvent>();
		Proof::MouseMoveEvent::Instance = std::make_shared<Proof::MouseMoveEvent>();

		Proof::WindowResizeEvent::Instance = std::make_shared<Proof::WindowResizeEvent>();
		Proof::WindowMoveEvent::Instance = std::make_shared<Proof::WindowMoveEvent>();
		Proof::WindowCloseEvent::Instance = std::make_shared<Proof::WindowCloseEvent>();
		Proof::WindowFocusEvent::Instance = std::make_shared<Proof::WindowFocusEvent>();
		
		
		Proof::Log::Init();
		
		auto app = Proof::CreateApplication();
		app->Run();
		delete app;
}
#endif