#pragma once
#include "Proof/Core/Core.h"

#include "Proof/Asset/Asset.h"

#include "UIButton.h"
namespace Proof{
	
	class UIPanel : public Asset{
	public:
		ASSET_CLASS_TYPE(UIPanel);

		void SetButton(const UIButton& button, std::string);
		void SetButtonImage(const UIButtonImage& button, std::string);
		void SetText(const UIText& button, std::string);

		bool ButtonHas(std::string ID);
		bool ImageButtonHas(std::string ID);
		bool TextHas(std::string ID);
		

		UIButton& ButtonGet(std::string ID);
		UIButtonImage& GetImageButton(std::string ID);
		UIText& TextGet(std::string ID);

		const std::unordered_map<std::string, UIButton>& GetButtons() {
			return m_Buttons;
		};

		const std::unordered_map<std::string, UIButtonImage>& GetImageButtons() {
			return m_ImageButtons;
		};

		const std::unordered_map<std::string, UIText>& GetTexts() {
			return m_Text;
		};

		std::string	Name;
		bool Visible = true;
	private:

		std::unordered_map<std::string, UIButton> m_Buttons;
		std::unordered_map<std::string, UIButtonImage> m_ImageButtons;
		std::unordered_map<std::string, UIText> m_Text;

	};

	class UITable {
	public:
		UITable() {
		}

		UITable(const std::map<uint32_t, Count<UIPanel>>& panels) {
			m_Panels = panels;
		}
		// panel can be nulltr
		// index cna be existing or non exisitng
		void SetUI(uint32_t uiIndex, Count<UIPanel> panel) {
			m_Panels[uiIndex] = panel;
		}

		void RemovePanel(uint32_t panelIndex) {
			PF_CORE_ASSERT(HasPanel(panelIndex), "Does not conatin panel index");
			m_Panels.erase(panelIndex);
		}

		Count<UIPanel> GetPanel(uint32_t panelIndex) {
			PF_CORE_ASSERT(HasPanel(panelIndex), "Does not conatin Panel index");
			return m_Panels[panelIndex];
		}

		uint32_t GetPanelSize() {
			return m_Panels.size();
		}

		bool HasPanel(uint32_t panelIndex)const {
			return m_Panels.contains(panelIndex);
		}

		const std::map<uint32_t, Count<UIPanel>>& GetPanels()const {
			return m_Panels;
		};

		std::map<uint32_t, Count<UIPanel>> Generate() {
			std::map<uint32_t, Count<UIPanel>> newPanel;
			for (auto& [id, panel] : m_Panels)
			{
				newPanel [id] = Count<UIPanel>::CreateFrom(panel);
			}
			return newPanel;
		}
	private:
		// index, panels
		std::map<uint32_t, Count<UIPanel>> m_Panels;
	};
}
