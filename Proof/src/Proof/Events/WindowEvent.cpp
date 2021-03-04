#include "Proofprch.h"
#include "WindowEvent.h"

namespace Proof {
    std::shared_ptr<WindowResizeEvent>WindowResizeEvent::Instance;
    std::shared_ptr<WindowMoveEvent>WindowMoveEvent::Instance;
    std::shared_ptr<WindowCloseEvent>WindowCloseEvent::Instance;
    std::shared_ptr<WindowFocusEvent>WindowFocusEvent::Instance;
}