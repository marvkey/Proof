#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"

#include "Proof/Renderer/Font.h"
#include "Proof/Renderer/Vertex.h"
namespace Proof{
	struct UIPlaceHolder
	{
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		bool Visible = true;
		glm::vec4 TintColour{ 1 };
	};
	struct UIImage {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		bool Visible = true;
		glm::vec4 TintColour{ 1 };
		Count<Texture2D> Texture;
	};

	struct UIButton {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		bool Visible = true;
		glm::vec4 TintColour{ 1 };
		std::string Text;

	};
		
	struct UIButtonImage {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		bool Visible = true;
		glm::vec4 TintColour{ 1 };
		Count<Texture2D> Texture;

	};


	struct UIText {
		glm::vec2 Postion = { 0,0 };
		glm::vec2 Rotation = { 0,0 };
		glm::vec2 Size = { 1,1 };
		bool Visible = true;
		TextParams Param;
		std::string Text;
		Count<Font>Font = Font::GetDefault();
	};
}
