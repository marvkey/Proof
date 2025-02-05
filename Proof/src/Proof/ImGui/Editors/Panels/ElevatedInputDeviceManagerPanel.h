#pragma once
#include "Panel.h"
#include "Proof/ImGui/UI.h"
namespace Proof
{
	class ElevatedInputDeviceManagerPanel : public Panel
	{
	public:
		ElevatedInputDeviceManagerPanel(Count<class ElevatedInputDeviceManager> manager)
		{
			m_InputDeviceManager = manager;
		}
		virtual void SetWorldContext(const Count<class World>& context) override;
		virtual void OnImGuiRender(const char* dsiplayName, bool& isOpen);
	private:
		Count<class ElevatedInputDeviceManager> m_InputDeviceManager;
	};
}