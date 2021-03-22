#pragma once
#include "Proof/Core/Core.h"
#include<iostream>
#include <sstream>
#include <functional>

namespace Proof {
	class Proof_API Event {
	public:
		virtual std::string ToString()const { return "Empty"; };
		friend  std::ostream& operator<<(std::ostream& os, const Event& dt);
	};
}

