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
		UI::EndPropertyGrid();

		if (ImGui::Button("+"))
			animationController->AddAnimationState(UUID(0));

		for (uint32_t i = 0; i < animationController->m_AnimationStates.size(); i++)
		{
			UI::ScopedID id(fmt::format("AnimaitaControlelrState {}", i));

			if(UI::AttributeTreeNode(fmt::format("AnimationStateTreeNode {}", i),true,1.0f,1.0f))
			{
				Count<AnimationState> state = animationController->m_AnimationStates[i];

				UI::BeginPropertyGrid();

				UI::AttributeAssetKeyReference("Animation", state->Animation);
				
				UI::AttributeDrag("RootRootTranslationMask", state->RootTranslationMask, 0.01f);
				UI::AttributeDrag("RootTranslationExtractMask", state->RootTranslationExtractMask, 0.01f);
				UI::AttributeDrag("RootRotationMask", state->RootRotationMask, 0.01f);
				UI::AttributeDrag("m_RootRotationExtractMask", state->RootRotationExtractMask, 0.01f);

				UI::AttributeBool("IsLooping", state->IsLooping);

				UI::EndPropertyGrid();

				UI::EndTreeNode();
			}

			if (ImGui::Button("Remove"))
				animationController->m_AnimationStates.erase(animationController->m_AnimationStates.begin() + i);
		}


	
	}
}
