#include "Proofprch.h"
#include "SerializeCommon.h"
#include "Proof/Input/ElevatedInputSystem/InputInteractions.h"
#include "Proof/Input/ElevatedInputSystem/InputKeyBinding.h"
namespace Proof
{

	static void SerializeInputInteraction(YAML::Emitter& out, Count<class InputInteraction> inputInteraction)
	{
		out << YAML::BeginMap; // Interaction

		out << YAML::Key << "InputInteractionType" << YAML::Value << EnumReflection::EnumString(inputInteraction->GetInteractionType());
		out << YAML::Key << "InteractionThreshold" << YAML::Value << inputInteraction->InteractionThreshold;

		switch (inputInteraction->GetInteractionType())
		{
			case Proof::InputInteractionType::Default:
				break;
			case Proof::InputInteractionType::Down:
				break;
			case Proof::InputInteractionType::ClickRelease:
			{
				auto clickReleaseInteraction = inputInteraction.As<InputInteractionClickRelease>();
				out << YAML::Key << "Mode" << YAML::Value << EnumReflection::EnumString( clickReleaseInteraction->Mode);
			}
				break;
			case Proof::InputInteractionType::Hold:
			{
				auto holdInteraction = inputInteraction.As<InputInteractionHold>();
				out << YAML::Key << "HoldTimeThreshold" << YAML::Value << holdInteraction->HoldTimeThreshold;
				out << YAML::Key << "FireOnce" << YAML::Value << holdInteraction->FireOnce;
			}
				break;
			case Proof::InputInteractionType::MultiTap:
			{
				auto multiTap = inputInteraction.As<InputInteractionMultiTap>();
				out << YAML::Key << "TapCount" << YAML::Value << multiTap->TapCount;
				out << YAML::Key << "TapSpacing" << YAML::Value << multiTap->TapSpacing;
				out << YAML::Key << "TapTime" << YAML::Value << multiTap->TapTime;
			}
				break;
			default:
				break;
		}
		out << YAML::EndMap;// Interaction
	}
	void SerializeCommon::SerializeInputInteractions(YAML::Emitter& out, const std::vector<Count<class InputInteraction>>& interactions)
	{
		if (interactions.empty())
			return;

		out << YAML::Key << "InputInteractions" << YAML::BeginSeq; //Interactions

		for (auto& interaction : interactions)
		{
			SerializeInputInteraction(out, interaction);
		}
		out << YAML::EndSeq;//Interactions
	}
	void LoadInputInteraction(YAML::Node& inInteraction, std::vector<Count<class InputInteraction>>& interactions)
	{
		InputInteractionType interactionMode = EnumReflection::StringEnum<InputInteractionType>(inInteraction["InputInteractionType"].as<std::string>(EnumReflection::EnumString(InputInteractionType::Default)));

		if (interactionMode == InputInteractionType::Default)
			return;

		auto inputInteraction = InputInteraction::CreateInputInteraction(interactionMode);

		switch (interactionMode)
		{
			case Proof::InputInteractionType::Default:
				break;
			case Proof::InputInteractionType::Down:
				break;
			case Proof::InputInteractionType::ClickRelease:
			{
				auto clickReleaseInteraction = inputInteraction.As<InputInteractionClickRelease>();
				clickReleaseInteraction->Mode = EnumReflection::StringEnum<InputInteractionClickRelease::ClickReleaseMode>(inInteraction["Mode"].as<std::string>
					(EnumReflection::EnumString(InputInteractionClickRelease::ClickReleaseMode::Click)));
			}
			break;
			case Proof::InputInteractionType::Hold:
			{
				auto holdInteraction = inputInteraction.As<InputInteractionHold>();
				holdInteraction->HoldTimeThreshold = inInteraction["HoldTimeThreshold"].as<float>(holdInteraction->HoldTimeThreshold);
				holdInteraction->FireOnce = inInteraction["FireOnce"].as<bool>(0);
			}
			break;
			case Proof::InputInteractionType::MultiTap:
			{
				auto multiTap = inputInteraction.As<InputInteractionMultiTap>();
				multiTap->TapCount = inInteraction["TapCount"].as<uint32_t>(multiTap->TapCount);
				multiTap->TapSpacing = inInteraction["TapSpacing"].as<uint32_t>(multiTap->TapSpacing);
				multiTap->TapTime = inInteraction["TapTime"].as<uint32_t>(multiTap->TapTime);
			}
			break;
			default:
				break;
		}
		interactions.push_back(inputInteraction);

	}
	void SerializeCommon::LoadInputInteractions(YAML::Node& inData, std::vector<Count<class InputInteraction>>& interactions)
	{
		if (!inData["InputInteractions"])return;
		auto inputInteractions = inData["InputInteractions"];

		if (!inputInteractions.IsSequence())
			return;

		for (auto interaction : inputInteractions)
		{
			LoadInputInteraction(interaction, interactions);
		}
	}
	static void SerializeInputKeyBinding(YAML::Emitter& out, Count<class InputKeyBindingBase> keyBinding)
	{
		out << YAML::BeginMap; // KeyBinding

		out << YAML::Key << "InputKeyBindingType" << YAML::Value << EnumReflection::EnumString(keyBinding->GetInputKeyBindingType());

		switch (keyBinding->GetInputKeyBindingType())
		{
			case Proof::InputKeyBindingType::Default:
				break;
			case Proof::InputKeyBindingType::Key:
			{
				out << YAML::Key << "InputKeyName" << YAML::Value << keyBinding.As<InputKeyBinding>()->InputKey.GetKeyName();
			}
				break;
			case Proof::InputKeyBindingType::Bundle:
			{
				auto bindingBundle = keyBinding.As<InputKeyBindingBundle>();
				out << YAML::Key << "InputKeyBindingBundleName" << bindingBundle->Name;
				out << YAML::Key << "InputKeyBundleKeys";
				out << YAML::BeginSeq; //BundleKeys
				for (auto& [bindingBundleTypes, keyBinding] : bindingBundle->GetBindings())
				{
					out << YAML::BeginMap; // bindingBundleTypes

					out << YAML::Key << "InputKeyBindingBundleType" << YAML::Value << EnumReflection::EnumString(bindingBundleTypes);
					out << YAML::Key << "InputKeySettings";
					
					SerializeInputKeyBinding(out,keyBinding);

					out << YAML::EndMap;// bindingBundleTypes

				}
				out << YAML::EndSeq;//BundleKeys

			}
				break;
		}

		std::vector<Count<InputKeyBindingBase>> modifiers;
		for (auto& key : keyBinding->GetModifiersKeys())
			modifiers.push_back(key);

		SerializeCommon::SerializeInputKeyBindings(out, modifiers,true);

		SerializeCommon::SerializeInputInteractions(out, keyBinding->Interactions);
		out << YAML::EndMap;// KeyBinding
	}

	void SerializeCommon::SerializeInputKeyBindings(YAML::Emitter& out, const std::vector<Count<class InputKeyBindingBase>>& keyBindings, bool modifierKeys)
	{
		if (keyBindings.empty())
			return;
		if(!modifierKeys)
			out << YAML::Key << "KeyBindings" << YAML::BeginSeq; //KeyBindings
		else
			out << YAML::Key << "ModifierKeyBindings" << YAML::BeginSeq; //ModifierKeyBindings

		for (auto& keyBinding : keyBindings)
		{
			SerializeInputKeyBinding(out, keyBinding);
		}
		out << YAML::EndSeq;//KeyBindings
	}
	void DeserializeInputKeyBinding(YAML::Node& inData, std::vector<Count<class InputKeyBindingBase>>& bindings, bool modifierKeys)
	{
		InputKeyBindingType keyBindingType = EnumReflection::StringEnum<InputKeyBindingType>(inData["InputKeyBindingType"].as<std::string>(EnumReflection::EnumString(InputKeyBindingType::Default)));

		if (keyBindingType == InputKeyBindingType::Default)
			return;
		Count<InputKeyBindingBase> keyBindingBase;
		switch (keyBindingType)
		{
			case Proof::InputKeyBindingType::Default:
			{
				return;
			}
			break;
			case Proof::InputKeyBindingType::Key:
			{
				keyBindingBase = Count<InputKeyBinding>::Create(ElevatedInputKeys::GetKeyByName(inData["InputKeyName"].as<std::string>("Invalid")));
			}
			break;
			case Proof::InputKeyBindingType::Bundle:
			{
				keyBindingBase = Count<InputKeyBindingBundle>::Create();
				auto keyBundle = keyBindingBase.As<InputKeyBindingBundle>();
				keyBundle->Name = inData["InputKeyBindingBundleName"].as<std::string>("");

				if (inData["InputKeyBundleKeys"])
				{
					for (auto keyBundlekey : inData["InputKeyBundleKeys"])
					{
						if (keyBundlekey["InputKeyBindingBundleType"])
						{
							InputKeyBindingBundleTypes keyBundleType = EnumReflection::StringEnum<InputKeyBindingBundleTypes>(keyBundlekey["InputKeyBindingBundleType"].as<std::string>());
							
							auto keySettings = keyBundlekey["InputKeySettings"];
							std::vector<Count<InputKeyBindingBase>> bindingsList;
							DeserializeInputKeyBinding(keySettings, bindingsList,false);
							keyBundle->SetKey(keyBundleType, bindingsList.front().As<InputKeyBinding>());

						}

					}
				}
			}
				break;
			default:
				break;
		}
		bindings.push_back(keyBindingBase);
		{
			std::vector<Count<InputKeyBindingBase>> modiferKeys;
			SerializeCommon::DeserializeInputKeyBindings(inData, modiferKeys, true);
			for (auto modiferKey : modiferKeys)
			{
				keyBindingBase->AddModifierKey(modiferKey.As<InputKeyBinding>());
			}
		}
		SerializeCommon::LoadInputInteractions(inData, keyBindingBase->Interactions);

	}
	void SerializeCommon::DeserializeInputKeyBindings(YAML::Node& inData, std::vector<Count<class InputKeyBindingBase>>& bindings, bool modifierKeys)
	{
		const std::string bindingType = modifierKeys ? "ModifierKeyBindings" : "KeyBindings";
		if (!inData[bindingType])return;
		auto keyBindings = inData[bindingType];

		if (!keyBindings.IsSequence())
			return;

		for (auto keyBinding : keyBindings)
		{
			DeserializeInputKeyBinding(keyBinding,bindings,modifierKeys);
		}
	}

}
