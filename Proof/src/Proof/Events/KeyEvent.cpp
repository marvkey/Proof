#include "Proofprch.h"
#include "KeyEvent.h"

namespace Proof {
    std::unique_ptr<KeyClickedEvent> KeyClickedEvent::Instance;
    std::unique_ptr<KeyReleasedEvent> KeyReleasedEvent::Instance;
    std::unique_ptr<KeyDoubleClickEvent> KeyDoubleClickEvent::Instance;
    std::unique_ptr<KeyRepeatEvent> KeyRepeatEvent::Instance;
}


