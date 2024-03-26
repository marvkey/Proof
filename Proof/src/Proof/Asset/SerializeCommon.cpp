#include "Proofprch.h"
#include "SerializeCommon.h"
#include "Proof/Input/ElevatedInputSystem/InputInteractions.h"
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

}
