#include "Proofprch.h"
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

		// view port
		//light
		SkyLightIcon = Texture2D::Create(TextureConfiguration("SkyLight"), "Resources/Icons/Skylight.png");
		DirectionalLightIcon = Texture2D::Create(TextureConfiguration("DirectionalLight"), "Resources/Icons/Directionallight.png");
		PointLightIcon = Texture2D::Create(TextureConfiguration("PointLight"), "Resources/Icons/Pointlight.png");
		SpotLightIcon = Texture2D::Create(TextureConfiguration("SpotLight"), "Resources/Icons/Spotlight.png");

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
