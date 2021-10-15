#include "Proofprch.h"
#include "CurrentWindow.h"
#include "Glad/glad.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32 // lets us get win 32 stuff
#include "GLFW/glfw3native.h"
#include "Proof/Core/Core.h"
#include "Proof/Core/Application.h"
namespace Proof {
	
	::GLFWwindow* CurrentWindow::GetWindow(){
		
		if(Application::MainWindow ==nullptr)
			return nullptr;
			
		if (Application::MainWindow->MainWindow != nullptr)
			return Application::MainWindow->MainWindow;
		else
			return nullptr;
	}
	WindowsWindow& CurrentWindow::GetWindowClass(){
		return *Application::MainWindow;
	}
	int CurrentWindow::GetWindowPosX(){
		return GetWindowLocation().first;
	}
	int CurrentWindow::GetWindowPosY(){ 
		return GetWindowLocation().second;
	}
	void CurrentWindow::SetSwapInterval(bool Vsync) {
		Application::MainWindow->Vsync = Vsync;
	}
	void CurrentWindow::SetWindowSize(int width, int height){
		glfwSetWindowSize(GetWindow(),width, height);
	
	}
	unsigned int CurrentWindow::GetWindowWidth() {
		return Application::MainWindow->Width;
	}
	unsigned int CurrentWindow::GetWindowHeight() {
		return Application::MainWindow->Height;
	}
	std::pair<double, double> CurrentWindow::GetMouseLocation(){
		double X, Y;
		glfwGetCursorPos(GetWindow(), &X, &Y);
		return { (float)X,(float)Y };
	}
	std::pair<float,float> CurrentWindow::GetMouseScrollWheel() {
		float x=0,y=0;
		if(Application::MainWindow->MouseScrollX.size()>0){
			x=Application::MainWindow->MouseScrollX[0];
		}
		if (Application::MainWindow->MouseScrollY.size()>0) {
			y = Application::MainWindow->MouseScrollY[0];
		}
		return {x,y};
	}
	std::pair<int, int> CurrentWindow::GetWindowLocation(){
		int X, Y;
		glfwGetWindowPos(GetWindow(), &X, &Y);
		return { X,Y };
	}

	void CurrentWindow::SetWindowInputEvent(bool var){
		CurrentWindow::GetWindowClass().m_InputEventEnabled=var;
	}
}
