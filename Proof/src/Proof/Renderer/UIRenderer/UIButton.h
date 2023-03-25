#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"

#include "Proof/Renderer/Font.h"
namespace Proof{

	struct UIButton {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Size = { 1,1 };
		glm::vec2 Rotation = { 0,0 };
		std::string TextName;
		glm::vec4 TintColour{ 1 };

	};

	struct UIButtonImage {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Size = { 1,1 };
		glm::vec2 Rotation = { 0,0 };
		Count<Texture2D> Texture;

		glm::vec4 TintColour{ 1 };

	};


	struct UIText {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Size = { 1,1 };
		glm::vec2 Rotation = { 0,0 };
		std::string Text;
		glm::vec4 Colour{ 1 };
		// horizontal distnace between each character
		float Kerning = 0.0f;
		// spacing of new line 
		float LineSpacing = 1.0f;
		//Todo background color

		Count<Font>Font = Font::GetDefault();
	};
}
