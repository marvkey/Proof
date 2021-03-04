#pragma once


#include "Platform/WindowsWindow.h"
#include "Proof/Core/Core.h"
#include <memory>
int main(int argc, char** argv);
namespace Proof {
    class Proof_API KeyPressedEvent {
    public:
        inline KeyBoardKey GetKeyPressed() { return Instance->EventHandled == true ? Instance->KeyPressed : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
    private:
        static std::shared_ptr<KeyPressedEvent>Instance;
        KeyBoardKey KeyPressed;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API KeyReleasedEvent {
    public:
        inline KeyBoardKey GetKeyReleased() { return Instance->EventHandled == true ? Instance->KeyReleased : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
    private:
        static std::shared_ptr<KeyReleasedEvent>Instance;
        KeyBoardKey KeyReleased;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API KeyDoubleClickEvent {
    public:
        inline KeyBoardKey GetKeyDoubleClick() { return Instance->EventHandled == true ? Instance->KeyDoubleClick : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
    private:
        static std::shared_ptr<KeyDoubleClickEvent>Instance;
        KeyBoardKey KeyDoubleClick;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API KeyRepeatEvent {
    public:
        inline KeyBoardKey GetKeyRepeat() { return Instance->EventHandled == true ? Instance->KeyRepeat : (KeyBoardKey)-1; }
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
    private:
        static std::shared_ptr<KeyRepeatEvent>Instance;
        KeyBoardKey KeyRepeat;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

}
