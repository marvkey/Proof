#pragma once
#include "Platform/WindowsWindow.h"
#include "Proof/Core/Core.h"
#include <memory>
int main(int argc, char** argv);
namespace Proof {
    class Proof_API MousePressedEvent {
    public:
        inline MouseButton GetButtonPressed() { return Instance->EventHandled == true ? Instance->ButtonPressed : (MouseButton)-1; }
        inline bool OnEvent() { return Instance->EventHandled == true ? true : false; }
    private:
        static std::shared_ptr<MousePressedEvent>Instance;
        MouseButton ButtonPressed;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };
    
    class Proof_API MouseReleasedEvent {
    public:
        inline MouseButton GetButtonReleased() { return Instance->EventHandled == true ? Instance->ButtonReleased : (MouseButton)-1; }
        inline bool OnEvent() { return Instance->EventHandled == true ? true : false; }
    private:
        static std::shared_ptr<MouseReleasedEvent>Instance;
        MouseButton ButtonReleased;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API MouseDoubleClickEvent {
    public:
        inline  MouseButton GetButtonDoubleClick() { return Instance->EventHandled == true ? Instance->ButtonDoubleClick : (MouseButton)-1; }
        inline  bool OnEvent() { return Instance->EventHandled == true ? true : false; }
    private:
        static std::shared_ptr<MouseDoubleClickEvent> Instance;
        MouseButton ButtonDoubleClick;
        bool EventHandled;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API MouseMoveEvent {
    public:
        inline unsigned int GetPosX() { return Instance->PosX; }
        inline unsigned int GetPosY() { return Instance->PosY; }
        inline bool OnEvent() {return  Instance->EventHandled == true ? true : false; }
    private:
        static std::shared_ptr<MouseMoveEvent>Instance;
        bool EventHandled =false;
        unsigned int PosX;
        unsigned int PosY;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };
}


