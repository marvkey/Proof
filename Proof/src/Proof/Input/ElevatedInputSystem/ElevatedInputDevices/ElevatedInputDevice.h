#pragma once
#include "Proof/Core/Core.h"
#include "../InputTypes.h"
#include "Proof/Events/Event.h"
#include "Proof/Core/Delegate.h"
#
namespace Proof
{
    class ElevatedInputDevice : RefCounted
    {
    public:
        ElevatedInputDevice(Count<class ElevatedInputDeviceManager> deviceManager, Players player)
            : m_Player(player), m_DeviceManager(deviceManager)
        {

        }
        virtual void OnEvent(Event& event) {};
        virtual void OnUpdate(float deltaTime) {};

        Players GetPlayer()
        {
            return m_Player;
        }


        virtual std::string GetDeviceName() = 0;
        friend class ElevatedInputDeviceManagerPanel;
        Count<class ElevatedInputDeviceManager> GetDeviceManager() { return m_DeviceManager; }

    private:
        Players m_Player;
        Count<class ElevatedInputDeviceManager> m_DeviceManager;
    };
}
		