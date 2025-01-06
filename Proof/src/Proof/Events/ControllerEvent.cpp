#include "Proofprch.h"
#include "ControllerEvent.h"

std::string Proof::ControllerButtonEvent::GetButtonName(ControllerButton button) const
{
    return EnumReflection::EnumString(button);
}
