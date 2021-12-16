#pragma once
#include "Proof/Renderer/GraphicsContext.h"
namespace Proof {
	class Proof_API CurrentWindow {
	public:
		static uint32_t GetWindowHeight();
		static uint32_t GetWindowWidth();
		static void SetWindowSize(int width, int height);
		static void* GetWindowAPI();
		static class WindowsWindow& GetWindowClass();
		static int GetWindowPosX();
		static int GetWindowPosY();
		static void SetvSync(bool Vsync);
	protected:
		static std::pair<double, double>GetMouseLocation();
		static std::pair<float, float>GetMouseScrollWheel();
	private:
		static void SetWindowInputEvent(bool var);
		static std::pair<int, int>GetWindowLocation();
		friend class Editore3D;
	};
}
