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
		UIPanel(const UIPanel& panel);
		Count<class UIMenu> Menu;
	///	Count<class VariableRegistry> m_VariableTable;
	};

	class UIPanelInstance
	{
	public:
		UIPanelInstance(Count<UIPanel> panel);
		UIPanelInstance();
	private:
		void SetPanelInstance(Count<UIPanel> panel);
	private:

		Count<UIPanel> m_UIPanel;
		Count<class VariableRegistry> m_VariableTable;
	};
	class UILayer
	{
	public:
		UILayer(const std::string& name)
			:m_Name(name)
		{

		}

		UILayer(const UILayer& other)
		{
			m_Name = other.m_Name;
			for (auto panel : other.m_Panels)
			{
				m_Panels.emplace_back(Count<UIPanel>::CreateFrom(panel));
			}
		}
		void PushUI(Count<UIPanel> panel)
		{
			m_Panels.emplace_back(panel);
		}

		void Pop()
		{
			m_Panels.pop_back();
		}

		const std::vector<Count<UIPanel>>& GetUIPanel()const
		{
			return m_Panels;
		}

	private:
		std::vector<Count<UIPanel>> m_Panels;
		std::string m_Name;
		friend class UITable;
		friend class SceneHierachyPanel;
	};

	class UITable : public RefCounted 
	{
	public:
		Count<UIPanel> Panel;
		UITable() 
		{
		#if 0
			// rendered back to front so menu always in front
			// event are checked front to back so menu can block modal events
			m_Layers.push_back({ "Menu" }); // menu of the game 
			m_Layers.push_back({ "Modal" }); // pop and dialouges
			m_Layers.push_back({ "HUD" }); // heads up dispaly
			m_Layers.push_back({ "BackGround" });// background
		#endif
		}
	#if 0
		UILayer& GetLayer(uint32_t index)
		{
			PF_CORE_ASSERT(m_Layers.size() > index);

			return m_Layers.at(index);
		}
	#endif
	private:
		//std::vector<UILayer> m_Layers;
	};
}
