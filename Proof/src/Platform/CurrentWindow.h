#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Application.h"
#if defined(_WIN64)
namespace Proof {
	class Proof_API CurrentWindow {
	public:
		//virtual void Abstract() = 0;
		static unsigned int GetWindowHeight();
		static unsigned int GetWindowWidth();
		static void SetWindowSize(int width, int height);
		static GLFWwindow* GetWindow();
		static WindowsWindow& GetWindowClass();
		static int GetWindowPosX();
		static int GetWindowPosY();
	protected:
		static std::pair<double, double>GetMouseLocation();
	private:
		static std::pair<int, int>GetWindowLocation();
	};
}
#endif
