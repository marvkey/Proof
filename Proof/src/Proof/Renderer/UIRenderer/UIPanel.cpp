#include "Proofprch.h"
#include "UIPanel.h"

namespace Proof {
	void UIPanel::SetPlaceHolder(const UIPlaceHolder& placeHolder, const std::string& ID)
	{
		m_PlaceHolders[ID] = placeHolder;
	}
	void UIPanel::SetImage(const UIImage& image, const std::string& ID)
	{
		m_Images[ID] = image;
	}
	void UIPanel::SetButton(const UIButton& button, const std::string& ID)
	{
		m_Buttons[ID] = button;
	}

	void UIPanel::SetButtonImage(const UIButtonImage& button, const std::string& ID)
	{
		m_ImageButtons[ID] = button;
	}
	void UIPanel::SetText(const UIText& button, const std::string& ID)
	{
		m_Text[ID] = button;
	}
	bool UIPanel::PlaceHolderHas(const std::string& ID)
	{
		return m_PlaceHolders.contains(ID);
	}
	bool UIPanel::ImageHas(const std::string& ID)
	{
		return m_Images.contains(ID);
	}
	bool UIPanel::ButtonHas(const std::string& ID)
	{
		return m_Buttons.contains(ID);
	}
	bool UIPanel::ImageButtonHas(const std::string& ID)
	{
		return m_ImageButtons.contains(ID);

	}
	bool UIPanel::TextHas(const std::string& ID)
	{
		return m_Text.contains(ID);
	}
	UIButton& UIPanel::ButtonGet(const std::string& ID)
	{
		return m_Buttons[ID];
	}
	UIButtonImage& UIPanel::GetImageButton(const std::string& ID)
	{
		return m_ImageButtons[ID];
	}
	UIText& UIPanel::TextGet(const std::string& ID)
	{
		return m_Text[ID];
	}
	UIPlaceHolder& UIPanel::PlaceHolderGet(const std::string& ID)
	{
		return m_PlaceHolders[ID];
	}
	UIImage& UIPanel::ImageGet(const std::string& ID)
	{
		return m_Images[ID];
	}
}
