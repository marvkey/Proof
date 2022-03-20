#include "Proofprch.h"
#include "CurrentWindow.h"
#include "Glad/glad.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32 // lets us get win 32 stuff
#include "GLFW/glfw3native.h"
#include "Proof/Core/Core.h"
#include "Proof/Core/Application.h"
#include <GLFW/glfw3.h>
namespace Proof {
	
	void* CurrentWindow::GetWindowAPI(){
		
		if(Application::MainWindow ==nullptr)
			return nullptr;
		return (::GLFWwindow * )Application::MainWindow->m_Window;
	}
	WindowsWindow& CurrentWindow::GetWindowClass(){
		return *Application::MainWindow.get();
	}
	int CurrentWindow::GetWindowPosX(){
		return GetWindowLocation().first;
	}
	int CurrentWindow::GetWindowPosY(){ 
		return GetWindowLocation().second;
	}
	void CurrentWindow::SetvSync(bool Vsync) {
		Application::MainWindow->Vsync = Vsync;
	}
	bool CurrentWindow::GetVSync() {
		return Application::MainWindow->Vsync;
	}
	void CurrentWindow::SetWindowSize(int width, int height){
		glfwSetWindowSize((GLFWwindow*)CurrentWindow::GetWindowAPI(),width, height);
	
	}
	uint32_t CurrentWindow::GetWindowWidth() {
		return Application::MainWindow->Width;
	}
	uint32_t CurrentWindow::GetWindowHeight() {
		return Application::MainWindow->Height;
	}
	std::pair<double, double> CurrentWindow::GetMouseLocation(){
		double X, Y;
		glfwGetCursorPos((GLFWwindow*)CurrentWindow::GetWindowAPI(), &X, &Y);
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
		glfwGetWindowPos((GLFWwindow*)GetWindowAPI(), &X, &Y);
		return { X,Y };
	}

	void CurrentWindow::SetWindowInputEvent(bool var){
		CurrentWindow::GetWindowClass().m_InputEventEnabled=var;
	}
}
