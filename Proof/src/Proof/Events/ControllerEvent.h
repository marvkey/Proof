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
            ss << "ControllerConnectEvent: " << (int)m_Index;
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
            ss << "ControllerDisconnectEvent: " << (int)m_Index;
            return ss.str();
        }
    private:
        int m_Index;
    };
    class Proof_API ControllerButtonEvent : public Event {
        EVENT_CLASS_CATEGORY(EventCategoryInput)
    public:
        ControllerButtonEvent(ControllerButton button) :
        m_Button(button)
        {

        }
        ControllerButton GetButton()const { return m_Button; }
    protected:
        ControllerButton m_Button;
    };
    class Proof_API ControllerButtonClickedEvent : public ControllerButtonEvent {
    public:
        EVENT_CLASS_TYPE(ControllerButtonClicked)
            ControllerButtonClickedEvent(ControllerButton button)
            : ControllerButtonEvent(button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "ControllerButtonClickedEvent: " << (int)m_Button;
            return ss.str();
        }
    };

    class Proof_API ControllerButtonPressedEvent : public ControllerButtonEvent {
    public:
        EVENT_CLASS_TYPE(ControllerButtonPressed)
            ControllerButtonPressedEvent(ControllerButton button)
            : ControllerButtonEvent(button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "ControllerButtonPressedEvent : " << (int)m_Button;
            return ss.str();
        }
    };

    class Proof_API ControllerButtonReleasedEvent : public ControllerButtonEvent {
    public:
        EVENT_CLASS_TYPE(ControllerButtonReleased)
        ControllerButtonReleasedEvent(ControllerButton button)
            : ControllerButtonEvent(button) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "ControllerButtonReleasedEvent : " << (int)m_Button;
            return ss.str();
        }
    };

    class Proof_API ControllerJoystickAxisEvent : public Event {
        EVENT_CLASS_CATEGORY(EventCategoryAxis | EventCategoryInput)
    public:
        ControllerJoystickAxisEvent(float x, float y, float distanceX, float distanceY) :
            X(x), Y(y), m_distanceX(distanceX), m_distanceY(distanceY)
        {

        }
        float GetX() { return X; }
        float GetY() { return Y; }
        float GetDistanceX() { return m_distanceX; }
        float GetDistanceY() { return m_distanceY; }
    protected:
        float X = 0, Y = 0;
        float m_distanceX=0, m_distanceY=0;// distnacce moved
    };

    class Proof_API ControllerLeftJoystickAxisEvent : public ControllerJoystickAxisEvent {
    public:
        EVENT_CLASS_TYPE(ControllerLeftJoystickAxis)
            ControllerLeftJoystickAxisEvent(float x, float y, float distanceX, float distanceY)
            : ControllerJoystickAxisEvent(x,y,distanceX, distanceY) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "ControllerLeftJoystickAxisEvent X: "<< X <<", Y: " << Y<<", distanceX: "<<m_distanceX <<", distanceY: "<<m_distanceY;
            return ss.str();
        }
    };

    class Proof_API ControllerRightJoystickAxisEvent : public ControllerJoystickAxisEvent {
    public:
        EVENT_CLASS_TYPE(ControllerLeftJoystickAxis)
            ControllerRightJoystickAxisEvent(float x, float y, float distanceX, float distanceY)
            : ControllerJoystickAxisEvent(x, y, distanceX, distanceY) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "ControllerRightJoystickAxisEvent X: " << X << ", Y: " << Y << ", distanceX: " << m_distanceX << ", distanceY: " << m_distanceY;
            return ss.str();
        }
    };

    class Proof_API ControllerTriggerAxisEvent : public Event {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryAxis | EventCategoryInput)
        EVENT_CLASS_TYPE(ControllerTriggerAxis)
            ControllerTriggerAxisEvent(float axis, float distance, ControllerAxis triggerAxis) :
            m_Axis(axis),m_Distance(distance), m_TriggerAxis(triggerAxis)
        {

        }
        float GetAxis() { return m_Axis; }
        float GetDistance() { return m_Distance; };
        ControllerAxis GetTriggerAxis() { return m_TriggerAxis; }
        std::string ToString() const override {
            std::stringstream ss;
            if (m_TriggerAxis == ControllerAxis::LeftTrigger) {
                ss << "ControllerLeftTriggerAxisEvent axis: " << m_Axis << ", distance: " << m_Distance << ", Trigger: " << (int)m_TriggerAxis;
                return ss.str();
            }
            ss << "ControllerRightTriggerAxisEvent axis: " << m_Axis << ", distance: " << m_Distance << ", Trigger: " << (int)m_TriggerAxis;
            return ss.str();
        }
    private:
        float m_Axis = 0;
        float m_Distance = 0;
        ControllerAxis m_TriggerAxis;
    };
}