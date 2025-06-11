#pragma once
#include "Proof/Core/Core.h"

#include "Proof/Asset/Asset.h"
#include <unordered_map>
#include <map>
namespace Proof
{
	class UIPanel : public Asset
	{
	public:
		ASSET_CLASS_TYPE(UIPanel);

		UIPanel();
		UIPanel(Count<UIPanel> panel);
		// gonna support multiple menus in the future,for like settings,
		// have a menu for the game, a menu for the settings, voluem settings,
		// graphics settings, 
		// menus can be turned of or on
	
		Count<class UIMenu> Menu; 
		Count<class VariableRegistry> VariableTable;
	};

	class UIPanelInstance : public RefCounted
	{
	public:
		UIPanelInstance();
		UIPanelInstance(Count<UIPanel> panel);
		UIPanelInstance(Count<UIPanelInstance> panelInstance);

		Count<UIPanel> GetUIPanel()
		{
			return m_UIPanel;
		}

		Count<UIMenu> GetInstanceMenu()
		{
			return m_InstanceMenu;
		}

		void SyncWithPanel();
		bool Visible = true;
		void SetPanelInstance(Count<UIPanel> panel);

		Count<class VariableRegistryInstance> GetVariableRegistryInstance() 
		{
			return m_VariableRegistryInstance;
		}

	private:
		Count<UIPanel> m_UIPanel;
		Count<UIMenu> m_InstanceMenu;
		Count<class VariableRegistryInstance> m_VariableRegistryInstance;
	};
	class UILayer
	{
	public:
		UILayer(const std::string& name)
			:Name(name)
		{

		}

		UILayer(const UILayer& other)
		{
			Name = other.Name;
			for (auto panelInstance : other.m_Panels)
			{
				auto instance  = Count<UIPanelInstance>::Create(panelInstance);
				m_Panels.emplace_back(instance);
			}
		}

		Count<UIPanelInstance> PushUI(Count<UIPanel> panel)
		{
			auto instance = Count<UIPanelInstance>::Create(panel);
			m_Panels.emplace_back(instance);

			return m_Panels.back();
		}
		Count<UIPanelInstance> PushUI()
		{
			auto instance = Count<UIPanelInstance>::Create();
			m_Panels.emplace_back(instance);
			return m_Panels.back();
		}
		void PopUI()
		{
			if (!m_Panels.empty())
			{
				m_Panels.pop_back();
			}
		}

		Count<UIPanelInstance> GetUIPanelByIndex(int index) const
		{
			if (index >= 0 && index < static_cast<int>(m_Panels.size()))
			{
				return m_Panels[index];
			}
			return nullptr; // Return nullptr if the index is out of bounds
		}

		Count<UIPanelInstance> GetUIPanelByPanel(Count<UIPanel> panel) const
		{
			for (const auto& panelInstance : m_Panels)
			{
				if (panelInstance->GetUIPanel() == panel)
				{
					return panelInstance;
				}
			}
			return nullptr; // Return nullptr if no matching panel is found
		}
		void PopIndex(int index);
		const std::vector<Count<UIPanelInstance>>& GetUIPanels()const
		{
			return m_Panels;
		}
		bool Visible = true;

	public:
		std::string Name;
	private:
		std::vector<Count<UIPanelInstance>> m_Panels;
		friend class UITable;
		friend class SceneHierachyPanel;
	};

	class UITable : public RefCounted 
	{
	public:
		UITable() 
		{
			// rendered back to front so menu always in front
			// event are checked front to back so menu can block modal events
			m_Layers.push_back({ "Menu" }); // menu of the game 
			m_Layers.push_back({ "Modal" }); // pop and dialouges
			m_Layers.push_back({ "Content" });// thigns taht may not be always visible liek invenotry and stuff
			m_Layers.push_back({ "HUD" }); // heads up dispaly, things always visible
		}

		UITable(Count<UITable> table)
		{
			m_Layers = table->m_Layers;
		}

		UILayer& GetLayer(uint32_t index)
		{
			PF_CORE_ASSERT(m_Layers.size() > index);

			return m_Layers.at(index);
		}

		const std::vector<UILayer>& GetLayers()const 
		{
			return m_Layers;
		}

		const std::vector<UILayer>& GetLayers()
		{
			return m_Layers;
		}

		// Add a new layer
		UILayer& AddLayer(const std::string& layerName)
		{
			m_Layers.push_back({ layerName });
			return m_Layers.back();
		}

		// Insert a layer at a specific index
		bool InsertLayer(uint32_t index, const std::string& layerName)
		{
			if (index > m_Layers.size())
				return false;

			m_Layers.insert(m_Layers.begin() + index, { layerName });
			return true;
		}

		// Remove a layer by index
		bool RemoveLayer(uint32_t index)
		{
			if (index >= m_Layers.size())
				return false;

			m_Layers.erase(m_Layers.begin() + index);
			return true;
		}

		// Move a layer from one index to another
		bool MoveLayer(uint32_t fromIndex, uint32_t toIndex)
		{
			if (fromIndex >= m_Layers.size() || toIndex >= m_Layers.size())
				return false;

			auto layer = std::move(m_Layers[fromIndex]);
			m_Layers.erase(m_Layers.begin() + fromIndex);
			m_Layers.insert(m_Layers.begin() + toIndex, std::move(layer));
			return true;
		}

		// Reorder layers by swapping two indices
		bool SwapLayers(uint32_t index1, uint32_t index2)
		{
			if (index1 >= m_Layers.size() || index2 >= m_Layers.size())
				return false;

			std::swap(m_Layers[index1], m_Layers[index2]);
			return true;
		}

		// Clear all layers
		void ClearLayers()
		{
			m_Layers.clear();
		}

		// Find a layer by name
		UILayer* FindLayerByName(const std::string& name)
		{
			auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [&](const UILayer& layer)
				{
					return layer.Name == name;
				});

			return it != m_Layers.end() ? &(*it) : nullptr;
		}

		// Check if a layer exists by name
		bool HasLayer(const std::string& name) const
		{
			return std::any_of(m_Layers.begin(), m_Layers.end(), [&](const UILayer& layer)
				{
					return layer.Name == name;
				});
		}

		bool HasLayer(int index) const
		{
			if (m_Layers.size() > index) return true;

			return false;
		}
	private:
		std::vector<UILayer> m_Layers;
	};
}
