#pragma once
#include "Proof/Core/Core.h"
#include <memory>
#include "Event.h"
#include "Proof/Input/KeyCodes.h"

int main(int argc, char** argv);
namespace Proof {
    /* The initilization of the these will need to run on a sperate thread */
    class Proof_API WindowResizeEvent:public Event {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        inline uint8_t GetWhidt() {return Instance->Whidt;}
        inline uint8_t GetHeight() { return Instance->Height;}
        std::string ToString()override {
            std::stringstream ss;
            ss << "Window Resize Event " <<(int) GetWhidt() << ", " << (int)GetHeight ();
            return ss.str();
        }
    private:
        uint8_t Whidt, Height;
        static std::unique_ptr<WindowResizeEvent>Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend class WindowsWindow;
    };

    class Proof_API WindowMoveEvent:public Event {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        inline uint8_t GetPosX()const { return Instance->PosX; }
        inline uint8_t GetPosY()const { return Instance->PosY; }
        std::string ToString()override {
            std::stringstream ss;
            ss << "Window Move Event " << Instance->PosX << ", " << Instance->PosY;
            return ss.str();
        }
    private:
        uint8_t PosX, PosY;
        static std::unique_ptr<WindowMoveEvent> Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend class WindowsWindow;
    };

    class Proof_API WindowCloseEvent: public Event {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        std::string ToString()override {
            std::stringstream ss;
            ss << "Window Has Been Closed ";
            return ss.str();
        }
    private:
        static std::unique_ptr<WindowCloseEvent>Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend class WindowsWindow;
    };

    class Proof_API WindowFocusEvent:public Event {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        std::string ToString()override {
            std::stringstream ss;
            ss << "Window is Focus ";
            return ss.str();
        }
    private:
        static std::unique_ptr<WindowFocusEvent>Instance;
        bool EventHandled = false;
        friend int ::main(int argc, char** argv);
        friend class WindowsWindow;
    };

    class Proof_API Window_ViewPortResize: public Event {
    public:
        inline bool OnEvent() { return (Instance->EventHandled == true) ? true : false; }
        std::string ToString()override {
            std::stringstream ss;
            ss << "Window is Focus ";
            return ss.str();
        }
        float GetWidth() {
            return Instance->X;
        };
        float GetHeihgt() {
            return Instance->Y;
        };
    private:
        static std::unique_ptr<Window_ViewPortResize>Instance;
        float X,Y;
        bool EventHandled = false;
        friend int ::main(int argc,char** argv);
        friend class Editore3D;
        friend class WindowsWindow;
    };
}


