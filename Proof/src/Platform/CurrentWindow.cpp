#include "Proofprch.h"
#include "CurrentWindow.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
namespace Proof {
	
	GLFWwindow* CurrentWindow::GetWindow(){
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
	void CurrentWindow::SetWindowSize(int width, int height){
		glfwSetWindowSize(GetWindow(),width, height);
		Application::MainWindow->Width = width;
		Application::MainWindow->Height = height;
		
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
	std::pair<int, int> CurrentWindow::GetWindowLocation(){
		int X, Y;
		glfwGetWindowPos(GetWindow(), &X, &Y);
		return { X,Y };
	}
}
