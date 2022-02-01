#pragma once
#include "Event.h"
#include "Proof/Core/Core.h"
#include <memory>
#include <sstream>
#include "Proof/Input/KeyCodes.h"
namespace Proof {
    class Proof_API ControllerConnectEvent : public Event {
    public:
            EVENT_CLASS_CATEGORY(EventCategoryInput)
        EVENT_CLASS_TYPE(ControllerConnect)

        ControllerConnectEvent(int Index) :
            m_Index(Index)
        {

        }
        int GetIndex() { return m_Index; }
        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index<<" ConnectEvent: " << (int)m_Index;
            return ss.str();
        }
    private:
        int m_Index;
    };
    class Proof_API ControllerDisconnectEvent : public Event {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryInput)
        EVENT_CLASS_TYPE(ControllerDisconnect)

        ControllerDisconnectEvent(int Index) :
            m_Index(Index)
        {

        }
        int GetIndex() { return m_Index; }
        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index<<" DisconnectEvent: " << (int)m_Index;
            return ss.str();
        }
    private:
        int m_Index;
    };
    class Proof_API ControllerButtonEvent : public Event {
        EVENT_CLASS_CATEGORY(EventCategoryInput)
    public:
        ControllerButtonEvent(int index,ControllerButton button) :
        m_Button(button),m_Index(index)
        {

        }
        ControllerButton GetButton()const { return m_Button; }
        int GetIndex()const { return m_Index; }
    protected:
        ControllerButton m_Button;
        int m_Index =0;
    };
    class Proof_API ControllerButtonClickedEvent : public ControllerButtonEvent {
    public:
        EVENT_CLASS_TYPE(ControllerButtonClicked)
            ControllerButtonClickedEvent(int index, ControllerButton button)
            : ControllerButtonEvent(index, button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index<<" ButtonClickedEvent: " << (int)m_Button;
            return ss.str();
        }
    };

    class Proof_API ControllerButtonPressedEvent : public ControllerButtonEvent {
    public:
        EVENT_CLASS_TYPE(ControllerButtonPressed)
            ControllerButtonPressedEvent(int index, ControllerButton button)
            : ControllerButtonEvent(index, button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index << " ButtonPressedEvent: " << (int)m_Button;
            return ss.str();
        }
    };

    class Proof_API ControllerButtonReleasedEvent : public ControllerButtonEvent {
    public:
        EVENT_CLASS_TYPE(ControllerButtonReleased)
        ControllerButtonReleasedEvent(int index, ControllerButton button)
            : ControllerButtonEvent(index,button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index << " ButtonReleasedEvent: " << (int)m_Button;
            return ss.str();
        }
    };
    class Proof_API ControllerButtonDoubleClickEvent : public ControllerButtonEvent {
    public:
        EVENT_CLASS_TYPE(ControllerButtonDoubleClicked)
            ControllerButtonDoubleClickEvent(int index, ControllerButton button)
            : ControllerButtonEvent(index, button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index << " ButtonDoubleClickEvent: " << (int)m_Button;
            return ss.str();
        }
    };
    class Proof_API ControllerJoystickAxisEvent : public Event {
        EVENT_CLASS_CATEGORY(EventCategoryAxis | EventCategoryInput)
    public:
        ControllerJoystickAxisEvent(int index,float x, float y, float distanceX, float distanceY) :
            X(x), Y(y), m_distanceX(distanceX), m_distanceY(distanceY),m_Index(index)
        {

        }
        float GetX() { return X; }
        float GetY() { return Y; }
        float GetDistanceX() { return m_distanceX; }
        float GetDistanceY() { return m_distanceY; }
        int GetIndex()const { return m_Index; }
    protected:
        int m_Index = 0;
        float X = 0, Y = 0;
        float m_distanceX=0, m_distanceY=0;// distnacce moved
    };

    class Proof_API ControllerLeftJoystickAxisEvent : public ControllerJoystickAxisEvent {
    public:
        EVENT_CLASS_TYPE(ControllerLeftJoystickAxis)
            ControllerLeftJoystickAxisEvent(int index,float x, float y, float distanceX, float distanceY)
            : ControllerJoystickAxisEvent(index,x,y,distanceX, distanceY) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index << " LeftJoystickAxisEvent X: "<< X <<", Y: " << Y<<", distanceX: "<<m_distanceX <<", distanceY: "<<m_distanceY;
            return ss.str();
        }
    };

    class Proof_API ControllerRightJoystickAxisEvent : public ControllerJoystickAxisEvent {
    public:
        EVENT_CLASS_TYPE(ControllerLeftJoystickAxis)
            ControllerRightJoystickAxisEvent(int index, float x, float y, float distanceX, float distanceY)
            : ControllerJoystickAxisEvent(index, x, y, distanceX, distanceY) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Controller " << m_Index << " RightJoystickAxisEvent X: " << X << ", Y: " << Y << ", distanceX: " << m_distanceX << ", distanceY: " << m_distanceY;
            return ss.str();
        }
    };

    class Proof_API ControllerTriggerAxisEvent : public Event {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryAxis | EventCategoryInput)
        EVENT_CLASS_TYPE(ControllerTriggerAxis)
            ControllerTriggerAxisEvent(int index,float axis, float distance, ControllerAxis triggerAxis) :
            m_Axis(axis),m_Distance(distance), m_TriggerAxis(triggerAxis),m_Index(index)
        {

        }
        float GetAxis() { return m_Axis; }
        float GetDistance() { return m_Distance; };
        ControllerAxis GetTriggerAxis() { return m_TriggerAxis; }
        std::string ToString() const override {
            std::stringstream ss;
            if (m_TriggerAxis == ControllerAxis::LeftTrigger) {
                ss << "Controller " << m_Index << " TriggerAxisEvent axis: " << m_Axis << ", distance: " << m_Distance << ", Trigger: " << (int)m_TriggerAxis;
                return ss.str();
            }
            ss << "Controller " << m_Index << " RightTriggerAxisEvent axis: " << m_Axis << ", distance: " << m_Distance << ", Trigger: " << (int)m_TriggerAxis;
            return ss.str();
        }
        int GetIndex() const { return m_Index; }
    private:
        int m_Index = 0;
        float m_Axis = 0;
        float m_Distance = 0;
        ControllerAxis m_TriggerAxis;
    };
}