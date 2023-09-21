#pragma once
#include "Proof/Core/Log.h"
#include "Panel.h"
#include "Proof/Core/Hash.h"
#include <unordered_map>
namespace Proof
{
	struct PanelData
	{
		const char* ID = "";
		const char* Name = "";
		Count<Panel> Panel = nullptr;
		bool IsOpen = false;
	};

	class PanelManager
	{
	public:
		PanelManager() = default;
		~PanelManager();
		const PanelData* GetPanelData(uint32_t panelID) const;
		PanelData* GetPanelData(const char* strID) ;

		void RemovePanel(const char* strID);
		void OnImGuiRender();
		void OnEvent(class Event& e);
		void SetWorldContext(const Count<class World>& context);
		template<typename TPanel>
		Count<TPanel> AddPanel(const PanelData& panelData)
		{
			static_assert(std::is_base_of<Panel, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from EditorPanel");

			auto& panelMap = m_Panels;

			uint32_t id = Hash::GenerateFNVHash(panelData.ID);
			if (panelMap.find(id) != panelMap.end())
			{
				PF_EC_ERROR("A panel with the id '{0}' has already been added.", panelData.ID);
				return nullptr;
			}

			panelMap[id] = panelData;
			return panelData.Panel.As<TPanel>();
		}

		template<typename TPanel, typename... TArgs>
		Count<TPanel> AddPanel(const char* strID, bool isOpenByDefault, TArgs&&... args)
		{
			return AddPanel<TPanel>(PanelData{ strID, strID, Count<TPanel>::Create(std::forward<TArgs>(args)...), isOpenByDefault });
		}

		template<typename TPanel, typename... TArgs>
		Count<TPanel> AddPanel(const char* strID, const char* displayName, bool isOpenByDefault, TArgs&&... args)
		{
			return AddPanel<TPanel>(PanelData{ strID, displayName,Count<TPanel>::Create(std::forward<TArgs>(args)...), isOpenByDefault });
		}

		template<typename TPanel>
		Count<TPanel> GetPanel(const char* strID)
		{
			static_assert(std::is_base_of<Panel, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from EditorPanel");

			uint32_t id = Hash::GenerateFNVHash(strID);

			if (m_Panels.find(id) != m_Panels.end())
				return m_Panels.at(id).Panel.As<TPanel>();

			PF_EC_ERROR("PanelManager", "Couldn't find panel with id '{0}'", strID);
			return nullptr;
		}
		const std::unordered_map < uint32_t, PanelData>& GetPanels()const { return m_Panels; }
		const std::unordered_map < uint32_t, PanelData>& GetPanels(){ return m_Panels; }
	private:
		std::unordered_map<uint32_t, PanelData> m_Panels;
	};
}