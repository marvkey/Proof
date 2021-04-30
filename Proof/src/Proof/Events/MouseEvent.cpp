#pragma once
#include "Proofprch.h"
#include "MouseEvent.h"

namespace Proof {
    std::unique_ptr<MouseClickedEvent> MouseClickedEvent::Instance;
    std::unique_ptr<MouseReleasedEvent> MouseReleasedEvent::Instance;
    std::unique_ptr<MouseDoubleClickEvent> MouseDoubleClickEvent::Instance;
    std::unique_ptr<MouseMoveEvent>MouseMoveEvent::Instance;
    std::unique_ptr<MouseScrollEvent>MouseScrollEvent::Instance;
}