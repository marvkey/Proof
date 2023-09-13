#include "PanelManager.h"

namespace Proof
{
	
	PanelManager::~PanelManager()
	{
		m_Panels.clear();
	}
	const PanelData* PanelManager::GetPanelData(uint32_t panelID) const
	{
		if (m_Panels.contains(panelID))
		{
			return &m_Panels.at(panelID);
		}
		return nullptr;
	}
	void PanelManager::RemovePanel(const char* strID)
	{
		uint32_t id = Hash::GenerateFNVHash(strID);

		if (m_Panels.contains(id))
		{
			m_Panels.erase(id);
			return;
		}
		PF_ENGINE_ERROR("Couldn't find panel with id {}", strID);
	}
	void PanelManager::OnImGuiRender()
	{
		for (auto& [id, panelData] : m_Panels)
		{
			panelData.Panel->OnImGuiRender(panelData.Name,panelData.IsOpen);
		}
	}
	void PanelManager::OnEvent(Event& e)
	{
		for (const auto& [id, panelData] : m_Panels)
		{
			panelData.Panel->OnEvent(e);
		}
	}
	void PanelManager::SetWorldContext(const Count<class World>& context)
	{
		for (const auto& [id, panelData] : m_Panels)
		{
			panelData.Panel->SetWorldContext(context);
		}
	}
}

