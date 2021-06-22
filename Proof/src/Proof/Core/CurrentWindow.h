#pragma once
#include "Proof/Renderer/GraphicsContext.h"
namespace Proof {
	class Proof_API CurrentWindow {
	public:
		//virtual void Abstract() = 0;
		static unsigned int GetWindowHeight();
		static unsigned int GetWindowWidth();
		static void SetWindowSize(int width, int height);
		static GLFWwindow* GetWindow();
		static class WindowsWindow& GetWindowClass();
		static int GetWindowPosX();
		static int GetWindowPosY();
		static Count<GraphicsContext> GetGraphicsContext();
		static void SetSwapInterval(bool Vsync);
	protected:
		static std::pair<double, double>GetMouseLocation();
	private:
		static std::pair<int, int>GetWindowLocation();
	};
}
