#pragma once
#include "Proof/Core/Core.h"

namespace Proof {
	class Input {
	protected:
		static enum  InputKeys {
			UnknownKey = 1,
			KeyReleased =0,
			KeyPressed =1,
			KeyRepeat =2,
		};
	};
}
