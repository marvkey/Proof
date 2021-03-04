#include "Proofprch.h"
#include "KeyEvent.h"

namespace Proof {
    std::shared_ptr<KeyPressedEvent> KeyPressedEvent::Instance;
    std::shared_ptr<KeyReleasedEvent> KeyReleasedEvent::Instance;
    std::shared_ptr<KeyDoubleClickEvent> KeyDoubleClickEvent::Instance;
    std::shared_ptr<KeyRepeatEvent> KeyRepeatEvent::Instance;
}


