#pragma once
#include "Platform/WindowsWindow.h"
#include "Proof/Core/Core.h"
#include <memory>
int main(int argc, char** argv);
namespace Proof {
    /* The initilization of the these will need to run on a sperate thread */
    class Proof_API WindowResizeEvent {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        inline uint8_t GetWhidt() {return Instance->Whidt;}
        inline uint8_t GetHeight() { return Instance->Height;}
    private:
        uint8_t Whidt, Height;
        static std::shared_ptr<WindowResizeEvent>Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API WindowMoveEvent {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        inline uint8_t GetPosX()const { return Instance->PosX; }
        inline uint8_t GetPosY()const { return Instance->PosY; }
    private:
        uint8_t PosX, PosY;
        static std::shared_ptr<WindowMoveEvent> Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API WindowCloseEvent {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
    private:
        static std::shared_ptr<WindowCloseEvent>Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };

    class Proof_API WindowFocusEvent {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
    private:
        static std::shared_ptr<WindowFocusEvent>Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend WindowsWindow;
    };
}


