#pragma once
#include "Event.h"
#include "Proof/Core/Core.h"
#include <memory>
#include <sstream>
#include "Proof/Input/KeyCodes.h"
namespace Proof {
    class Proof_API KeyEvent:public Event{
    public:
        EVENT_CLASS_CATEGORY(EventCategoryInput)
        KeyEvent(KeyBoardKey keycode):
            m_Key(keycode)
        {
        
        }
        KeyBoardKey GetKey() const { return m_Key; }
    protected:
        KeyBoardKey m_Key;
    };
    class Proof_API KeyClickedEvent: public KeyEvent {
    public:
        EVENT_CLASS_TYPE(KeyClicked)
        KeyClickedEvent(KeyBoardKey keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyClickedEvent: " << (char)m_Key;
            return ss.str();
        }
    };

    class Proof_API KeyReleasedEvent:public KeyEvent{
    public:
        EVENT_CLASS_TYPE(KeyReleased)
        KeyReleasedEvent(KeyBoardKey keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleased: " << (char)m_Key;
            return ss.str();
        }
    };

    class Proof_API KeyDoubleClickEvent :public KeyEvent {
    public:
        EVENT_CLASS_TYPE(KeyDoubleClick)
        KeyDoubleClickEvent(KeyBoardKey keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyDoubleClick: " << (char)m_Key;
            return ss.str();
        }
    };
    class Proof_API KeyHoldEvent :public KeyEvent {
    public:
        EVENT_CLASS_TYPE(KeyHold)
            KeyHoldEvent(KeyBoardKey keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyHoldEvent: " << (char)m_Key;
            return ss.str();
        }
    };

    class Proof_API KeyPressedEvent:public KeyEvent {
    public:
        EVENT_CLASS_TYPE(KeyPressed)
        KeyPressedEvent(KeyBoardKey keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressed: " << (char)m_Key;
            return ss.str();
        }
    };
}
