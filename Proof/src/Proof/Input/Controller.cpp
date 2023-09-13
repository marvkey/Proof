#include "Proofprch.h"
#include "Controller.h"

namespace Proof {
	Controller::Controller()
	{
		EnumReflection::ForEach<ControllerButton>([&](ControllerButton& button) {
			Buttons[button] = InputEvent::None;
		});
	}
}
