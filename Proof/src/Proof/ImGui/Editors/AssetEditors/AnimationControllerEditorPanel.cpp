#include "Proofprch.h"
#include "AnimationControllerEditorPanel.h"
#include "Proof/Animation/AnimationController.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIColors.h"
#include "Proof/ImGui/UIUtilities.h"
#include "Proof/ImGui/UIWidgets.h"
#include "Proof/ImGui/UIHandlers.h"
namespace Proof
{

	AnimationControllerPanel::AnimationControllerPanel()
		:AssetEditorDetail("AnimationControllerPanel")

	{

	}
	void AnimationControllerPanel::RenderDetailSettings()
	{
		if (GetAsset() == nullptr)
			return;

		Count<AnimationController> animationController = GetAsset().As<AnimationController>();

		UI::BeginPropertyGrid();

		UI::AttributeAssetKeyReference("Skeleton", animationController->m_Skeleton);

		UI::AttributeDrag("RootRootTranslationMask", animationController->m_RootTranslationMask, 0.01f);
		UI::AttributeDrag("RootTranslationExtractMask", animationController->m_RootTranslationExtractMask, 0.01f);
		UI::AttributeDrag("RootRotationMask", animationController->m_RootRotationMask, 0.01f);
		UI::AttributeDrag("m_RootRotationExtractMask", animationController->m_RootRotationExtractMask, 0.01f);
		
		UI::AttributeBool("IsLooping", animationController->m_IsLooping);
		UI::EndPropertyGrid();
	}
}
