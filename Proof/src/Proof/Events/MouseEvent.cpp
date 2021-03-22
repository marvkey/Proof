#pragma once
#include "Proofprch.h"
#include "MouseEvent.h"

namespace Proof {
    std::shared_ptr<MousePressedEvent> MousePressedEvent::Instance;
    std::shared_ptr<MouseReleasedEvent> MouseReleasedEvent::Instance;
    std::shared_ptr<MouseDoubleClickEvent> MouseDoubleClickEvent::Instance;
    std::shared_ptr<MouseMoveEvent>MouseMoveEvent::Instance;
    std::shared_ptr<MouseScrollEvent>MouseScrollEvent::Instance;
}