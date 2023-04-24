#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"

#include "Proof/Renderer/Font.h"
#include "Proof/Renderer/Vertex.h"
namespace Proof{

	struct UIButton {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		std::string Text;
		glm::vec4 TintColour{ 1 };

	};
		
	struct UIButtonImage {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		Count<Texture2D> Texture;

		glm::vec4 TintColour{ 1 };

	};


	struct UIText {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		std::string Text;
		TextParams Param;
		Count<Font>Font = Font::GetDefault();
	};
}
