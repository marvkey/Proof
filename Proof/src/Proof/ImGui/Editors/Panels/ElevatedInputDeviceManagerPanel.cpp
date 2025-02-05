#include "Proofprch.h"
#include "ElevatedInputDeviceManagerPanel.h"
#include "Proof/Input/ElevatedInputSystem/ElevatedInputDevices/ElevatedInputDeviceManager.h"

namespace Proof
{

	void ElevatedInputDeviceManagerPanel::SetWorldContext(const Count<class World>& context)
	{
	}

	void ElevatedInputDeviceManagerPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen)
	{
		PF_PROFILE_FUNC();


		if (ImGui::Begin(dsiplayName, &isOpen) && m_InputDeviceManager != nullptr)
		{
			UI::EnumCombo("NewInputDeviceMode", m_InputDeviceManager->NewInputDeviceMode);

			std::vector<std::string> devicesNames;
			for (auto& devices : m_InputDeviceManager->GetInputDevices())
				devicesNames.push_back(devices->GetDeviceName());

			EnumReflection::ForEach<Players>([&](Players& player)
				{
					if (player == Players::None)
						return;
					if (UI::AttributeTreeNode(EnumReflection::EnumString(player), true, 3, 3))
					{

						auto playerDevices = m_InputDeviceManager->GetDevicePlayer(player);

						std::string currentDeviceName;

						if (!playerDevices.empty())
						{
							currentDeviceName = playerDevices.front()->GetDeviceName();
						}
						auto [changed, outSelectionIndex, outSelectiongString] =
						UI::Combo("Device", devicesNames, currentDeviceName);

						if (changed)
						{
							m_InputDeviceManager->GetInputDevices().at(outSelectionIndex)->m_Player = player;
						}

						UI::EndTreeNode();
					}

				});

		}
		ImGui::End();

	}

}

