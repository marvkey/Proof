#pragma once
#include "Proof/Core/Core.h"

#include "Proof/Asset/Asset.h"
#include <unordered_map>
#include <map>
#include "UIButton.h"
namespace Proof{
	
	class UIPanel : public Asset{
	public:
		ASSET_CLASS_TYPE(UIPanel);

		void SetPlaceHolder(const UIPlaceHolder& placeHolder, const std::string&);
		void SetImage(const UIImage& placeHolder, const std::string&);

		void SetButton(const UIButton& button, const std::string&);
		void SetButtonImage(const UIButtonImage& button, const std::string&);
		void SetText(const UIText& button, const std::string&);


		bool PlaceHolderHas(const std::string& ID);
		bool ImageHas(const std::string& ID);

		bool ButtonHas(const std::string& ID);
		bool ImageButtonHas(const std::string& ID);
		bool TextHas(const std::string& ID);
		

		UIPlaceHolder& PlaceHolderGet(const std::string& ID);
		UIImage& ImageGet(const std::string& ID);
		UIButton& ButtonGet(const std::string& ID);
		UIButtonImage& GetImageButton(const std::string& ID);
		UIText& TextGet(const std::string& ID);

		const std::unordered_map<std::string, UIPlaceHolder>& GetPlaceHolders() {
			return m_PlaceHolders;
		};
		const std::unordered_map<std::string, UIImage>& GetImages() {
			return m_Images;
		};
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

		std::unordered_map<std::string, UIPlaceHolder> m_PlaceHolders;
		std::unordered_map<std::string, UIImage> m_Images;
		std::unordered_map<std::string, UIButton> m_Buttons;
		std::unordered_map<std::string, UIButtonImage> m_ImageButtons;
		std::unordered_map<std::string, UIText> m_Text;

	};

	class UITable : public RefCounted {
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
