#include "Proofprch.h"
#include "UIPanel.h"

namespace Proof {
	void UIPanel::SetButton(const UIButton& button, std::string ID)
	{
		m_Buttons[ID] = button;
	}

	void UIPanel::SetButtonImage(const UIButtonImage& button, std::string ID)
	{
		m_ImageButtons[ID] = button;
	}
	void UIPanel::SetText(const UIText& button, std::string ID)
	{
		m_Text[ID] = button;
	}
	bool UIPanel::ButtonHas(std::string ID)
	{
		return m_Buttons.contains(ID);
	}
	bool UIPanel::ImageButtonHas(std::string ID)
	{
		return m_ImageButtons.contains(ID);

	}
	bool UIPanel::TextHas(std::string ID)
	{
		return m_Text.contains(ID);
	}
	UIButton& UIPanel::ButtonGet(std::string ID)
	{
		return m_Buttons[ID];
	}
	UIButtonImage& UIPanel::GetImageButton(std::string ID)
	{
		return m_ImageButtons[ID];
	}
	UIText& UIPanel::TextGet(std::string ID)
	{
		return m_Text[ID];
	}
}
