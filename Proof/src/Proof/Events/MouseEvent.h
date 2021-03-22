#pragma once
#include "Platform/WindowsWindow.h"
#include "Proof/Core/Core.h"
#include <memory>
#include <sstream>
#include <functional>
#include "Event.h"
int main(int argc, char** argv);
namespace Proof {
    class Proof_API MousePressedEvent:public Event{
    public:
        inline MouseButton GetButtonPressed() { return Instance->EventHandled == true ? Instance->ButtonPressed : (MouseButton)-1; }
        inline bool OnEvent() { return Instance->EventHandled == true ? true : false; }
        std::string ToString() override {
            std::stringstream ss;
            ss << "Mouse Pressed Event " << (int)Instance->ButtonPressed;
            return ss.str();
        }
    private:
        static std::shared_ptr<MousePressedEvent>Instance;
        MouseButton ButtonPressed;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };
    
    class Proof_API MouseReleasedEvent: public Event {
    public:
        inline MouseButton GetButtonReleased() { return Instance->EventHandled == true ? Instance->ButtonReleased : (MouseButton)-1; }
        inline bool OnEvent() { return Instance->EventHandled == true ? true : false; }
        std::string ToString() override {
            std::stringstream ss;
            ss << "Mouse Button Released Event " << (int) Instance-> ButtonReleased;
            return ss.str();
        }
    private:
        static std::shared_ptr<MouseReleasedEvent>Instance;
        MouseButton ButtonReleased;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API MouseDoubleClickEvent: public Event {
    public:
        inline  MouseButton GetButtonDoubleClick() { return Instance->EventHandled == true ? Instance->ButtonDoubleClick : (MouseButton)-1; }
        inline  bool OnEvent() { return Instance->EventHandled == true ? true : false; }
        std::string ToString() override {
            std::stringstream ss;
            ss << "Mouse Button Double Click " <<(int) Instance->ButtonDoubleClick;
            return ss.str();
        }
    private:
        static std::shared_ptr<MouseDoubleClickEvent> Instance;
        MouseButton ButtonDoubleClick;
        bool EventHandled;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API MouseMoveEvent: public Event {
    public:
        inline unsigned int GetPosX() { return Instance->PosX; }
        inline unsigned int GetPosY() { return Instance->PosY; }
        inline bool OnEvent() {return  Instance->EventHandled == true ? true : false; }
        std::string ToString() override {
            std::stringstream ss;
            ss << "Mouse Move Event " << Instance->PosX << ", " << Instance->PosY;
            return ss.str();
        }
    private:
        static std::shared_ptr<MouseMoveEvent>Instance;
        bool EventHandled =false;
        unsigned int PosX;
        unsigned int PosY;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
        
    };

    class Proof_API MouseScrollEvent:public Event {
    public:
        inline float GetPosX() { return Instance->PosX; }
        inline float GetPosY() { return Instance->PosY; }
        inline bool OnEvent() { return  Instance->EventHandled == true ? true : false; }
        
        std::string ToString() override {
            std::stringstream ss;
            ss << "MouseScrolled Event " << Instance->PosX << ", " << Instance->PosY;
            return ss.str();
        }

    private:
        static std::shared_ptr<MouseScrollEvent>Instance;
        bool EventHandled = false;
        float PosX;
        float PosY;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };
}


