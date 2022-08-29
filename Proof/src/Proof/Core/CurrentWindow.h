#pragma once
#include "Window.h"
namespace Proof {
	class Proof_API CurrentWindow {
	public:
		static Window& GetWindow();
		friend class Editore3D;
	};
}
