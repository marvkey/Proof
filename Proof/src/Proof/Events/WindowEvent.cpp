#include "Proofprch.h"
#include "WindowEvent.h"

namespace Proof {
    std::unique_ptr<WindowResizeEvent>WindowResizeEvent::Instance;
    std::unique_ptr<WindowMoveEvent>WindowMoveEvent::Instance;
    std::unique_ptr<WindowCloseEvent>WindowCloseEvent::Instance;
    std::unique_ptr<WindowFocusEvent>WindowFocusEvent::Instance;
}