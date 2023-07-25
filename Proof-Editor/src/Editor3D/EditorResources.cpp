#include "EditorResources.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Core/Buffer.h"
namespace Proof {
	void EditorResources::Init()
	{
		CheckerBoardWhiteGrey = Texture2D::Create(TextureConfiguration("WhiteAndGreyCheckerBoard"), "Assets/Textures/WhiteAndGreyCheckerBoard.png");
	}
	void EditorResources::Unizilize()
	{
		CheckerBoardWhiteGrey = nullptr;
	}
}
