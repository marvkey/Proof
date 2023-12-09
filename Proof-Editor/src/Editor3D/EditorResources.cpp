#include "EditorResources.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Core/Buffer.h"
namespace Proof {
	void EditorResources::Init()
	{
		CheckerBoardWhiteGrey = Texture2D::Create(TextureConfiguration("WhiteAndGreyCheckerBoard"), "Assets/Textures/WhiteAndGreyCheckerBoard.png");
		FolderIcon = Texture2D::Create(TextureConfiguration("FolderIcon"), "Resources/Icons/ContentBrowser/FolderIcon.png");
		FileIcon = Texture2D::Create(TextureConfiguration("FIleIcon"), "Resources/Icons/ContentBrowser/FileIcon.png");
		BackArrowIcon = Texture2D::Create(TextureConfiguration("BackArrowIcon"), "Resources/Icons/ContentBrowser/BackArrow.png");
		FowardArrowIcon = Texture2D::Create(TextureConfiguration("FowardArrowIcon"), "Resources/Icons/ContentBrowser/FowardArrow.png");
	}
	void EditorResources::Unizilize()
	{
		CheckerBoardWhiteGrey = nullptr;
		FolderIcon = nullptr;
		FileIcon = nullptr;
		BackArrowIcon = nullptr;
		FowardArrowIcon = nullptr;
	}
}
