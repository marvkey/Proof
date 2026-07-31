#include "Proofprch.h"
#include "EditorResources.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Core/Buffer.h"
namespace Proof {
	void EditorResources::Init()
	{
		DeleteIcon = Texture2D::Create(TextureConfiguration("DeleteIcon"), "Resources/Icons/Delete.png");
		AddIcon = Texture2D::Create(TextureConfiguration("AddIcon"), "Resources/Icons/Add.png");
		InsertIcon = Texture2D::Create(TextureConfiguration("InsertIcon"), "Resources/Icons/Insert.png");

		CheckerBoardWhiteGrey = Texture2D::Create(TextureConfiguration("WhiteAndGreyCheckerBoard"), "Assets/Textures/WhiteAndGreyCheckerBoard.png");
		FolderIcon = Texture2D::Create(TextureConfiguration("FolderIcon"), "Resources/Icons/ContentBrowser/AssetIcons/Folder.png");
		FileIcon = Texture2D::Create(TextureConfiguration("FIleIcon"), "Resources/Icons/ContentBrowser/AssetIcons/DefaultFile.png");
		BackArrowIcon = Texture2D::Create(TextureConfiguration("BackArrowIcon"), "Resources/Icons/ContentBrowser/BackArrow.png");
		FowardArrowIcon = Texture2D::Create(TextureConfiguration("FowardArrowIcon"), "Resources/Icons/ContentBrowser/FowardArrow.png");


		AnimationThumbnailIcon = Texture2D::Create(TextureConfiguration("AnimationThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/Animation.png");
		AnimationControllerThumbnailIcon = Texture2D::Create(TextureConfiguration("AnimationControllerThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/AnimationController.png");
		AudioThumbnailIcon = Texture2D::Create(TextureConfiguration("AudioThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/Audio.png");
		FontThumbnailIcon = Texture2D::Create(TextureConfiguration("FontThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/Font.png");
		InputActionThumbnailIcon = Texture2D::Create(TextureConfiguration("InputActionThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/InputAction.png");
		InputBindingContextThumbnailIcon = Texture2D::Create(TextureConfiguration("InputBindingContextThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/InputBindingContext.png");
		MeshColliderThumbnailIcon = Texture2D::Create(TextureConfiguration("MeshColliderThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/MeshCollider.png");
		ParticleSystemThumbnailIcon = Texture2D::Create(TextureConfiguration("ParticleSystemThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/ParticleSystem.png");
		ParticleEmitterThumbnailIcon = Texture2D::Create(TextureConfiguration("ParticleEmitterThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/ParticleEmitter.png");
		PMatThumbnailIcon = Texture2D::Create(TextureConfiguration("PMatThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/PMat.png");
		PrefabThumbnailIcon = Texture2D::Create(TextureConfiguration("PrefabThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/Prefab.png");
		ScriptFileThumbnailIcon = Texture2D::Create(TextureConfiguration("ScriptFileThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/ScriptFile.png");
		SkeletonThumbnailIcon = Texture2D::Create(TextureConfiguration("SkeletonThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/Skeleton.png");
		UIThumbnailIcon = Texture2D::Create(TextureConfiguration("UIThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/UI.png");
		WorldThumbnailIcon = Texture2D::Create(TextureConfiguration("WorldThumbnailIcon"), "Resources/Icons/ContentBrowser/AssetIcons/World.png");

		// view port
		//light
		SkyLightIcon = Texture2D::Create(TextureConfiguration("SkyLight"), "Resources/Icons/Skylight.png");
		DirectionalLightIcon = Texture2D::Create(TextureConfiguration("DirectionalLight"), "Resources/Icons/Directionallight.png");
		PointLightIcon = Texture2D::Create(TextureConfiguration("PointLight"), "Resources/Icons/Pointlight.png");
		SpotLightIcon = Texture2D::Create(TextureConfiguration("SpotLight"), "Resources/Icons/Spotlight.png");

		//viewport
		CameraIcon = Texture2D::Create(TextureConfiguration("CameraIcon"), "Resources/Icons/Viewport/Camera.png");
		DropdownIcon = Texture2D::Create(TextureConfiguration("DropdownIcon"), "Resources/Icons/Viewport/Dropdown.png");
		LocalSpaceIcon = Texture2D::Create(TextureConfiguration("LocalSpaceIcon"), "Resources/Icons/Viewport/LocalSpace.png");
		WorldSpaceIcon = Texture2D::Create(TextureConfiguration("WorldSpaceIcon"), "Resources/Icons/Viewport/WorldSpace.png");
		TranslationIcon = Texture2D::Create(TextureConfiguration("TranslationIcon"), "Resources/Icons/Viewport/Translation.png");
		RotationIcon = Texture2D::Create(TextureConfiguration("RotationIcon"), "Resources/Icons/Viewport/Rotation.png");
		ScaleIcon = Texture2D::Create(TextureConfiguration("ScaleIcon"), "Resources/Icons/Viewport/Scale.png");
		UniversalTransformIcon = Texture2D::Create(TextureConfiguration("UniversalTransformIcon"), "Resources/Icons/Viewport/UniversalTransform.png");
		PointerIcon = Texture2D::Create(TextureConfiguration("PointerIcon"), "Resources/Icons/Viewport/Pointer.png");
		ViewIcon = Texture2D::Create(TextureConfiguration("ViewIcon"), "Resources/Icons/Viewport/View.png");
		PlayerStartIcon = Texture2D::Create(TextureConfiguration("PlayerStartICon"), "Resources/Icons/Viewport/PlayerStart.png");

	}
	void EditorResources::Unizilize()
	{
		CheckerBoardWhiteGrey = nullptr;
		FolderIcon = nullptr;
		FileIcon = nullptr;
		BackArrowIcon = nullptr;
		FowardArrowIcon = nullptr;
		PlayerStartIcon = nullptr;
	}
}
