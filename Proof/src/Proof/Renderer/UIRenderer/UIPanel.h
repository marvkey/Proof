#pragma once
#include "Proof/Core/Core.h"

#include "Proof/Asset/Asset.h"

#include "UIButton.h"
namespace Proof{
	
	class UIPanel : public Asset{
	public:
		ASSET_CLASS_TYPE(UIPanel);

		void AddButton(const UIButton& button, UUID ID = UUID());
		void AddButtonImage(const UIButtonImage& button, UUID ID = UUID());
		void AddText(const UIText& button, UUID ID = UUID());

		bool ButtonHas(UUID ID);
		bool ImageButtonHas(UUID ID);
		bool TextHas(UUID ID);
		

		UIButton& ButtonGet(UUID ID);
		UIButtonImage& GetImageButton(UUID ID);
		UIText& TextGet(UUID ID);

		const std::unordered_map<uint64_t, UIButton>& GetButtons() {
			return m_Buttons;
		};

		const std::unordered_map<uint64_t, UIButtonImage>& GetImageButtons() {
			return m_ImageButtons;
		};

		const std::unordered_map<uint64_t, UIText>& GetTexts() {
			return m_Text;
		};
	private:

		std::unordered_map<uint64_t, UIButton> m_Buttons;
		std::unordered_map<uint64_t, UIButtonImage> m_ImageButtons;
		std::unordered_map<uint64_t, UIText> m_Text;
	};
}
