#include "Proofprch.h"
#include "UIPanel.h"

namespace Proof {
	void UIPanel::AddButton(const UIButton& button, UUID ID)
	{
		m_Buttons[ID] = button;
	}

	void UIPanel::AddButtonImage(const UIButtonImage& button, UUID ID)
	{
		m_ImageButtons[ID] = button;

	}
	void UIPanel::AddText(const UIText& button, UUID ID)
	{

		m_Text[ID] = button;
	}
	bool UIPanel::ButtonHas(UUID ID)
	{
		return m_Buttons.contains(ID);
	}
	bool UIPanel::ImageButtonHas(UUID ID)
	{
		return m_ImageButtons.contains(ID);

	}
	bool UIPanel::TextHas(UUID ID)
	{
		return m_Text.contains(ID);
	}
	UIButton& UIPanel::ButtonGet(UUID ID)
	{
		return m_Buttons[ID];
	}
	UIButtonImage& UIPanel::GetImageButton(UUID ID)
	{
		return m_ImageButtons[ID];
	}
	UIText& UIPanel::TextGet(UUID ID)
	{
		return m_Text[ID];
	}
}
