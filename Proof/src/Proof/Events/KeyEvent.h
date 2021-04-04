#pragma once
#include "Platform/WindowsWindow.h"
#include "Proof/Core/Core.h"
#include <memory>
#include "Event.h"
#include <sstream>
#include <functional>
int main(int argc, char** argv);
namespace Proof {
    class Proof_API KeyClickedEvent: public Event{
    public:
        inline KeyBoardKey GetKeyClicked() { return Instance->EventHandled == true ? Instance->KeyClicked : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        std::string ToString() override {
            std::stringstream ss;
            ss << "Key Clicked Event " << (char)Instance->KeyClicked;
            return ss.str();
        }
    private:
        static std::shared_ptr<KeyClickedEvent>Instance;
        KeyBoardKey KeyClicked;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API KeyReleasedEvent:public Event {
    public:
        inline KeyBoardKey GetKeyReleased() { return Instance->EventHandled == true ? Instance->KeyReleased : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        std::string ToString() override {
            std::stringstream ss;
            ss << "Key Released Event " << (char)Instance->KeyReleased;
            return ss.str();
        }
    private:
        static std::shared_ptr<KeyReleasedEvent>Instance;
        KeyBoardKey KeyReleased;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API KeyDoubleClickEvent :public Event {
    public:
        inline KeyBoardKey GetKeyDoubleClick() { return Instance->EventHandled == true ? Instance->KeyDoubleClick : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        std::string ToString()override {
            std::stringstream ss;
            ss << "Key Double Click Event " <<(char) Instance->KeyDoubleClick;
            return ss.str();
        }
    private:
        static std::shared_ptr<KeyDoubleClickEvent>Instance;
        KeyBoardKey KeyDoubleClick;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API KeyRepeatEvent:public Event {
    public:
        inline KeyBoardKey GetKeyRepeat() { return Instance->EventHandled == true ? Instance->KeyRepeat : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        std::string ToString() override {
            std::stringstream ss;
            ss << "Key Repeat Event " << (char) Instance->KeyRepeat;
            return ss.str();
        }
    private:
        static std::shared_ptr<KeyRepeatEvent>Instance;
        KeyBoardKey KeyRepeat;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };
}
