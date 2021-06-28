#pragma once
#include "Core.h"
#include "Proof/Core/FrameTime.h"
namespace Proof {
	class Proof_API Window {
	public:
	public:
		virtual void WindowUpdate(class FrameTime DeltaTime) = 0;
		virtual int createWindow() = 0;
	private:
		virtual int WindowEnd() = 0;
	};
}
