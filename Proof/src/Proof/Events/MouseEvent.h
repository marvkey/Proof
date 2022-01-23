#pragma once
#include "Proof/Core/Core.h"
#include <memory>
#include <sstream>
#include "Event.h"
#include "Proof/Input/KeyCodes.h"
namespace Proof {
    class Proof_API MouseButtonEvent: public Event{
    public:
        EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
        MouseButtonEvent(MouseButton button):
            m_Button(button)
        {

        }
        MouseButton GetButton() const { return m_Button; }
    protected:
        MouseButton m_Button;
    };
    class Proof_API MouseButtonClickedEvent:public MouseButtonEvent {
    public:
        EVENT_CLASS_TYPE(MouseButtonClicked)
            MouseButtonClickedEvent(MouseButton button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseClickedEvent: " << (int)m_Button;
            return ss.str();
        }
    };
    class Proof_API MouseButtonPressedEvent:public MouseButtonEvent {
    public:
        EVENT_CLASS_TYPE(MouseButtonPressed)
            MouseButtonPressedEvent(MouseButton button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << (int)m_Button;
            return ss.str();
        }
    };
    class Proof_API MouseButtonReleasedEvent: public MouseButtonEvent {
    public:
        EVENT_CLASS_TYPE(MouseButtonReleased)
        MouseButtonReleasedEvent(MouseButton button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseReleasedEvent: " << (int)m_Button;
            return ss.str();
        }
    };

    class Proof_API MouseButtonDoubleClickEvent: public MouseButtonEvent {
    public:
        EVENT_CLASS_TYPE(MouseButtonDoubleClick)
        MouseButtonDoubleClickEvent(MouseButton button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseDoubleClickEvent: " << (int)m_Button;
            return ss.str();
        }
    };
    class Proof_API MouseMovementEvent: public Event{
    public:
        EVENT_CLASS_CATEGORY(EventCategoryMouseMovement | EventCategoryInput)
    };
    class Proof_API MouseMoveEvent: public MouseMovementEvent {
    public:
        EVENT_CLASS_TYPE(MouseMoved)
        MouseMoveEvent(float x,float y):
            m_X(x),m_Y(y)
        {
        
        }
        float GetX(){return m_X;}
        float GetY(){return m_Y;}
        std::string ToString()const override {
            std::stringstream ss;
            ss << "MouseMoveEvent X: " << m_X << ", Y: " << m_Y;
            return ss.str();
        }
    private:
        float m_X=0;
        float m_Y=0;
    };

    class Proof_API MouseScrollEvent:public MouseMovementEvent {
    public:
        EVENT_CLASS_TYPE(MouseScrolled)
        MouseScrollEvent(float scrollx,float scrolly):
            m_X(scrollx),m_Y(scrolly) 
        {

        }
        float GetScrollX() { return m_X; }
        float GetScrollY() { return m_Y; }
        std::string ToString()const override {
            std::stringstream ss;
            ss << "MouseScrollEvent X: " << m_X << ", Y: " << m_Y;
            return ss.str();
        }
    private:
        float m_X = 0;
        float m_Y = 0;
    };
}