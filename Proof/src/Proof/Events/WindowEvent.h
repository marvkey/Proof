#pragma once
#include "Proof/Core/Core.h"
#include <memory>
#include "Event.h"
#include "Proof/Input/KeyCodes.h"
#include <sstream>
namespace Proof {
    class Proof_API WindowEvent: public Event{
    public:
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    }; 
    class Proof_API WindowResizeEvent:public WindowEvent {
    public:
        EVENT_CLASS_TYPE(WindowResize)
        WindowResizeEvent(uint32_t width,uint32_t height):
            m_Width(width),m_Height(height)
        {
        
        };
        uint32_t GetWhidt()const {return m_Width;}
        uint32_t GetHeight()const { return m_Height;}
        std::string ToString()const override {
            std::stringstream ss;
            ss << "WindowResizeEvent Width: " <<(int)m_Width << ", Height: " << m_Height;
            return ss.str();
        }
    private:
        uint32_t m_Width,m_Height;
    };
    class Proof_API WindowMinimizeEvent:public WindowEvent {
    public:
        EVENT_CLASS_TYPE(WindowMinimize)
        WindowMinimizeEvent(bool temp):
            m_Minimized(temp)
        {

        };
        bool IsWIndowMinimized(){
            return m_Minimized;
        }
        std::string ToString()const override {
            std::stringstream ss;
            if (m_Minimized)
                ss << "WindowMinimizeEvent windowMinimzed";
            else
                ss << "WindowMinimizeEvent window Un Minimize";
            return ss.str();
        }
    private:
        bool m_Minimized=false;
    };
    class Proof_API WindowMoveEvent:public WindowEvent{
    public:
        EVENT_CLASS_TYPE(WindowMoved)
            WindowMoveEvent(int posX,int posY):
            m_X(posX),m_Y(posY) {

        };
        int GetX()const { return m_X; }
        int GetY()const { return m_Y; }
        std::string ToString()const override {
            std::stringstream ss;
            ss << "WindowMoveEvent X: " << (int)m_X << ", Y: " << m_Y;
            return ss.str();
        }
    private:
        int m_X,m_Y;
    };

    class Proof_API WindowCloseEvent: public WindowEvent {
    public:
        EVENT_CLASS_TYPE(WindowClose)
        WindowCloseEvent(bool closed):
            m_Closed(closed)
        {
        
        };
        bool IsWindowClosed(){return m_Closed;};
        std::string ToString()const override {
            std::stringstream ss;
            ss << "WindowCloseEvent Window Has Been Closed ";
            return ss.str();
        }
    private:
        bool m_Closed=false;
    };

    class Proof_API WindowFocusEvent:public WindowEvent {
    public:
        EVENT_CLASS_TYPE(WindowFocus)
        WindowFocusEvent(bool focused):
            m_Focused(focused)
        {

        };
        bool IsWindowFocused(){return m_Focused;}
        std::string ToString()const override {
            std::stringstream ss;
            if(m_Focused)
                ss << "WindowFocusEvent Window Gained Focus ";
            else
                ss << "WindowFocusEvent Window Lost Focus ";
            return ss.str();
        }
    private:
        bool m_Focused =false;
    };
}


