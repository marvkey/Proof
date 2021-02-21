#pragma once
#if defined(_WIN64)
namespace Proof {

	class CurrentWindow {
		virtual void Abstract() = 0;
		static int GetWindowHeight();
		static int GetWindowWidth();
		static void SetWindowSize(int width, int height);
	private:
		
	};
}
#endif

namespace Proof {

	class CurrentWindow {

	};
}
