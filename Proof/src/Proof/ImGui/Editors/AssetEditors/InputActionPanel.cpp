#include "Proofprch.h"
#include "InputActionPanel.h"
#include "Proof/Input/ElevatedInputSystem/InputAction.h"
#include "Proof/Input/ElevatedInputSystem/InputInteractions.h"

#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIHandlers.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/ImGui/Editors/EditorResources.h"
namespace Proof
{
	InputActionEditorPanel::InputActionEditorPanel()
		:
		AssetEditorDetail("InputActionEditorPanel")
	{
	}
	void InputActionEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::InputAction)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to InputActionEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_Action = asset.As<InputAction>();

		AssetEditorDetail::SetAsset(m_Action);
	}
	void InputActionEditorPanel::RenderDetailSettings()
	{
		UI::BeginPropertyGrid();
		UI::PushModified(m_NeedsSaving);
		UI::EnumCombo("OutputType", m_Action->OutputType);
		UI::EnumCombo("OutputValueBehavior", m_Action->OutputValueBehavior, {},{
			R"( 
		 Takes the value from the mapping with the highest Absolute Value.
		
		For example, in a game where the player controls character movement using the arrow keys :
		 Pressing the "Up" arrow key produces a positive value(e.g., +0.8) for forward movement.
		 Simultaneously pressing the "Down" arrow key produces a negative value(e.g., -0.6) for backward movement.
		 Despite conflicting inputs, the resulting action's value reflects the highest absolute movement input (e.g., 0.8).
		 This behavior is particularly useful for player movement in games, where simultaneous input from multiple keys may lead to conflicting movement directions.
		 By considering only the highest absolute input value, the player's movement accurately reflects the most significant directional change,
		 ensuring smooth and intuitive character control even amidst conflicting key presses.)",
		R"(
			Aggregates the values from different mappings.
			
			For example, if pressing the up arrow key contributes a value of +0.5 and pressing the down arrow key contributes -0.5 to the same input action,
			the resulting value would be 0.0, effectively canceling each other out.
			This behavior might be useful in scenarios where you want conflicting inputs, such as pressing up and down arrow keys simultaneously, to result in no net movement or action.
			)"
			});
		UI::EndPropertyGrid();

		EditInputInteractions(m_Action->Interactions);
		UI::PopModified();
	}
	Count<class Asset> InputActionEditorPanel::GetAsset()
	{
		return m_Action;
	}
	void InputActionEditorPanel::PropertyWindow()
	{
	}
	void EditInputInteractions(std::vector<Count<class InputInteraction>>& interactions)
	{
		bool isOpen = UI::AttributeTreeNode("InputInteractions",false);
		auto imageSize = ImVec2{ 25,25 };
		{
			ImGui::SameLine();

			if (UI::ImageButton(EditorResources::AddIcon, { 25,25 }))
			{
				ImGui::OpenPopup("AddInputInteraction");
			}

			ImGui::SameLine();

			if (UI::ImageButton(EditorResources::DeleteIcon, { 25,25 }))
			{
				interactions.clear();
			}
		}

		if (ImGui::BeginPopup("AddInputInteraction")) 
		{
			EnumReflection::ForEach<InputInteractionType>([&](InputInteractionType type) 
				{
					if (type == InputInteractionType::Default)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(type).c_str()))
					{
						auto interaction = InputInteraction::CreateInputInteraction(type);
						interactions.emplace_back(interaction);
						UI::HandleModified(true);
					}
				});
			ImGui::EndPopup();
		}
		if (isOpen)
		{
			for (uint32_t i= 0; i<interactions.size(); i++ )
			{
				auto& interaction = interactions[i];

				bool attributeOpen = UI::AttributeTreeNode(EnumReflection::EnumString(interaction->GetInteractionType()));

				{
					ImGui::SameLine();

					if (UI::ImageButton(EditorResources::DeleteIcon, { 25,25 }))
					{
						UI::HandleModified(true);
						interactions.erase(interactions.begin() + i);
						if(attributeOpen)
							UI::EndTreeNode();
						continue;
					}
				}
				if(attributeOpen)
				{
					UI::BeginPropertyGrid();
					UI::AttributeDrag("InteractionThreshold", interaction->InteractionThreshold, 0.05, 0,Math::GetMaxType<float>(),
						R"(
							The InteractionThreshold within the InputInteraction class serves as a pivotal parameter dictating the sensitivity of interaction detection. 
							Represented by a floating-point value, this threshold acts as a cutoff point against which the magnitude of an 
							input action value is measured. By comparing the squared magnitude of an input action value with the squared 
							InteractionThreshold, the class swiftly determines whether an interaction has occurred. 
							If the squared magnitude surpasses or equals the squared threshold value, the interaction is promptly recognized. 
							This mechanism not only streamlines computation by obviating the need for costly square root calculations 
							but also affords developers granular control over the responsiveness of interaction detection. 
							Fine-tuning the InteractionThreshold enables developers to tailor interaction sensitivity to the unique demands of 
							their application or game, ensuring a seamless and intuitive user experience.
						)");

					switch (interaction->GetInteractionType())
					{
						case Proof::InputInteractionType::Default:
							break;
						case Proof::InputInteractionType::Down:
							break;
						case Proof::InputInteractionType::ClickRelease:
						{
							Count<InputInteractionClickRelease> clickRelease = interaction.As< InputInteractionClickRelease>();
							UI::EnumCombo("Mode", clickRelease->Mode, {}, {
								"Return trigger event when the key is Click",
								"Return trigger event when the key is Released",
								"return trigger event when the key is click or released"
								});
						}
							break;
						case Proof::InputInteractionType::Hold:
						{
							Count<InputInteractionHold> hold = interaction.As< InputInteractionHold>();
							UI::AttributeDrag("HoldTimeThreshold", hold->HoldTimeThreshold, 0.25, 0, Math::GetMaxType<float>(), "How Long to hold before Trigger Event fired");
							UI::AttributeBool("FireOnce", hold->FireOnce, "Determine whether this trigger should activate only once or every frame after the hold time threshold is met.");
						}
							break;
						case Proof::InputInteractionType::MultiTap:
						{
							Count<InputInteractionMultiTap> multiTap = interaction.As<InputInteractionMultiTap>();
							UI::AttributeDrag("TapCount", multiTap->TapCount, 1, 0, Math::GetMaxType<uint32_t>(), "How many taps to trigger");
							UI::AttributeDrag("TapSpacing", multiTap->TapSpacing, 0.05, 0, Math::GetMaxType<float>(), "Maximum time allowed to be passed between clicks,if time exceded this sate will be set to None");
							UI::AttributeDrag("TapTime", multiTap->TapTime, 0.05, 0, Math::GetMaxType<float>(),"Maximum Time for a key to be released to be registered as a Tap");

						}
							break;
					}
					UI::EndPropertyGrid();
					UI::EndTreeNode();
				}
			}
			UI::EndTreeNode();
		}

	}
}
