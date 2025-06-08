#include "Proofprch.h"
#include "SerializeCommon.h"
#include "Proof/Resources/ExternalCreations.h"
#include "Proof/Input/ElevatedInputSystem/InputInteractions.h"
#include "Proof/Input/ElevatedInputSystem/InputCustomizers.h"
#include "Proof/Input/ElevatedInputSystem/InputKeyBinding.h"

#include "Proof/Scripting/ScriptField.h"
#include "Proof/Scripting/ScriptWorld.h"
#include "Proof/Scripting/ScriptFile.h"
namespace Proof
{
	static void SerializeInputCustomizer(YAML::Emitter& out, Count<class InputCustomizer> inputCustomizer)
	{
		out << YAML::BeginMap; // Customizers

		out << YAML::Key << "InputCustomizerType" << YAML::Value << EnumReflection::EnumString(inputCustomizer->GetCustomizerType());

		switch (inputCustomizer->GetCustomizerType())
		{
			case InputCustomizerType::Default:
			break;
			case InputCustomizerType::Scale:
			{
				auto scaleCustomizer = inputCustomizer.As<InputCustomizerScale>();
				out << YAML::Key << "Scale" << YAML::Value << scaleCustomizer->Scale;
				break;
			}
			case InputCustomizerType::Invert:
			{
				auto invertCustomizer = inputCustomizer.As<InputCustomizerInvert>();
				out << YAML::Key << "InvertedAxes" << YAML::Value << (uint8_t)invertCustomizer->InvertedAxes;
				break;
			}

		default:
			break;
		}

		out << YAML::EndMap;// Customizers

	}
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
				multiTap->TapSpacing = inInteraction["TapSpacing"].as<float>(multiTap->TapSpacing);
				multiTap->TapTime = inInteraction["TapTime"].as<float>(multiTap->TapTime);
			}
			break;
			default:
				break;
		}
		interactions.push_back(inputInteraction);

	}

	void LoadInputCustomizer(YAML::Node& inInteraction, std::vector<Count<class InputCustomizer>>& customizers)
	{
		InputCustomizerType interactionMode = EnumReflection::StringEnum<InputCustomizerType>(inInteraction["InputCustomizerType"].as<std::string>(EnumReflection::EnumString(InputInteractionType::Default)));

		if (interactionMode == InputCustomizerType::Default)
			return;

		auto inputCustomizer = InputCustomizer::CreateInputInteraction(interactionMode);
		switch (interactionMode)
		{
			case Proof::InputCustomizerType::Default:
				break;

			case Proof::InputCustomizerType::Scale:
			{
				auto scaleCustomizer = inputCustomizer.As<InputCustomizerScale>();
				scaleCustomizer->Scale = inInteraction["Scale"].as<glm::vec3>(scaleCustomizer->Scale);
				break;
			}

			case Proof::InputCustomizerType::Invert:
			{
				auto invertCustomizer = inputCustomizer.As<InputCustomizerInvert>();
				invertCustomizer->InvertedAxes = (InputCustomizerInvertAxis)inInteraction["InvertedAxes"].as<uint8_t>(0);
				break;
			}
		}
		customizers.emplace_back(inputCustomizer);
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
	void SerializeCommon::SerializeInputCustomizers(YAML::Emitter& out, const std::vector<Count<class InputCustomizer>>& customizers)
	{
		if (customizers.empty())
			return;

		out << YAML::Key << "InputCustomizers" << YAML::BeginSeq; //Customizers

		for (auto& customizer : customizers)
		{
			SerializeInputCustomizer(out, customizer);
		}
		out << YAML::EndSeq;//Interactions
	}
	void SerializeCommon::LoadInputCustomizers(YAML::Node& inData, std::vector<Count<class InputCustomizer>>& customizers)
	{

		if (!inData["InputCustomizers"])return;
		auto inputCustomizers = inData["InputCustomizers"];

		if (!inputCustomizers.IsSequence())
			return;

		for (auto customizer : inputCustomizers)
		{
			LoadInputCustomizer(customizer, customizers);
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
		SerializeCommon::SerializeInputCustomizers(out, keyBinding->Customizers);
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
		SerializeCommon::LoadInputCustomizers(inData, keyBindingBase->Customizers);

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

	static void SaveScriptFieldEnum(YAML::Emitter& out, Count<EnumFieldStorage> fieldStorage)
	{
		out << YAML::Key << "Data" << YAML::Value;
		switch (fieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Int8:
			{
				out << fieldStorage->GetValue<int8_t>();
				break;
			}
			case ScriptFieldType::Int16:
			{
				out << fieldStorage->GetValue<int16_t>();
				break;
			}
			case ScriptFieldType::Int32:
			{
				out << fieldStorage->GetValue<int32_t>();
				break;
			}
			case ScriptFieldType::Int64:
			{
				out << fieldStorage->GetValue<int64_t>();
				break;
			}
			case ScriptFieldType::UInt8:
			{
				out << fieldStorage->GetValue<uint8_t>();
				break;
			}
			case ScriptFieldType::UInt16:
			{
				out << fieldStorage->GetValue<uint16_t>();
				break;
			}
			case ScriptFieldType::UInt32:
			{
				out << fieldStorage->GetValue<uint32_t>();
				break;
			}
			case ScriptFieldType::UInt64:
			{
				out << fieldStorage->GetValue<uint64_t>();
				break;
			}
			default:
			{
				PF_CORE_ASSERT(false);
				out << "";
				break;
			}
		}
	}

	static void SaveScriptFieldStorage(YAML::Emitter& out, Count<FieldStorage> fieldStorage)
	{

		out << YAML::Key << "Data" << YAML::Value;

		if (IsScriptFieldAssetType(fieldStorage->GetFieldInfo()->Type))
		{
			out << fieldStorage->GetValue<uint64_t>();
			return;
		}
		switch (fieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Bool:
			{
				out << fieldStorage->GetValue<bool>();
				break;
			}
			case ScriptFieldType::Int8:
			{
				out << fieldStorage->GetValue<int8_t>();
				break;
			}
			case ScriptFieldType::Int16:
			{
				out << fieldStorage->GetValue<int16_t>();
				break;
			}
			case ScriptFieldType::Int32:
			{
				out << fieldStorage->GetValue<int32_t>();
				break;
			}
			case ScriptFieldType::Int64:
			{
				out << fieldStorage->GetValue<int64_t>();
				break;
			}
			case ScriptFieldType::UInt8:
			{
				out << fieldStorage->GetValue<uint8_t>();
				break;
			}
			case ScriptFieldType::UInt16:
			{
				out << fieldStorage->GetValue<uint16_t>();
				break;
			}
			case ScriptFieldType::UInt32:
			{
				out << fieldStorage->GetValue<uint32_t>();
				break;
			}
			case ScriptFieldType::UInt64:
			{
				out << fieldStorage->GetValue<uint64_t>();
				break;
			}
			case ScriptFieldType::Float:
			{
				out << fieldStorage->GetValue<float>();
				break;
			}
			case ScriptFieldType::Double:
			{
				out << fieldStorage->GetValue<double>();
				break;
			}
			case ScriptFieldType::String:
			{
				out << fieldStorage->GetValue<std::string>();
				break;
			}
			case ScriptFieldType::Vector2:
			{
				out << fieldStorage->GetValue<glm::vec2>();
				break;
			}
			case ScriptFieldType::Vector3:
			{
				out << fieldStorage->GetValue<glm::vec3>();
				break;
			}
			case ScriptFieldType::Vector4:
			{
				out << fieldStorage->GetValue<glm::vec4>();
				break;
			}
			case ScriptFieldType::Prefab:
			case ScriptFieldType::Entity:
			case ScriptFieldType::Mesh:
			case ScriptFieldType::DynamicMesh:
			case ScriptFieldType::Material:
			case ScriptFieldType::PhysicsMaterial:
			case ScriptFieldType::Texture2D:
			{
				out << fieldStorage->GetValue<uint64_t>();
				break;
			}
			default:
			{
				PF_CORE_ASSERT(false);
				out << "";
				break;
			}
		}
	}
	static void SaveScriptFieldArray(YAML::Emitter& out, Count<ArrayFieldStorage> arrayStorage)
	{

		if (arrayStorage == nullptr)
			return;

		out << YAML::Key << "Length" << YAML::Value << arrayStorage->GetLength(); // Not strictly necessary but useful for readability
		out << YAML::Key << "Data" << YAML::Value;

		out << YAML::BeginSeq;


		for (uintptr_t i = 0; i < arrayStorage->GetLength(); i++)
		{
			if (IsScriptFieldAssetType(arrayStorage->GetFieldInfo()->Type))
			{
				out << arrayStorage->GetValue<UUID>(i);
				continue;
			}
			switch (arrayStorage->GetFieldInfo()->Type)
			{
				case ScriptFieldType::Bool:
				{
					out << arrayStorage->GetValue<bool>(i);
					break;
				}
				case ScriptFieldType::Int8:
				{
					out << arrayStorage->GetValue<int8_t>(i);
					break;
				}
				case ScriptFieldType::Int16:
				{
					out << arrayStorage->GetValue<int16_t>(i);
					break;
				}
				case ScriptFieldType::Int32:
				{
					out << arrayStorage->GetValue<int32_t>(i);
					break;
				}
				case ScriptFieldType::Int64:
				{
					out << arrayStorage->GetValue<int64_t>(i);
					break;
				}
				case ScriptFieldType::UInt8:
				{
					out << arrayStorage->GetValue<uint8_t>(i);
					break;
				}
				case ScriptFieldType::UInt16:
				{
					out << arrayStorage->GetValue<uint16_t>(i);
					break;
				}
				case ScriptFieldType::UInt32:
				{
					out << arrayStorage->GetValue<uint32_t>(i);
					break;
				}
				case ScriptFieldType::UInt64:
				{
					out << arrayStorage->GetValue<uint64_t>(i);
					break;
				}
				case ScriptFieldType::Float:
				{
					out << arrayStorage->GetValue<float>(i);
					break;
				}
				case ScriptFieldType::Double:
				{
					out << arrayStorage->GetValue<double>(i);
					break;
				}
				case ScriptFieldType::String:
				{
					out << arrayStorage->GetValue<std::string>(i);
					break;
				}
				case ScriptFieldType::Vector2:
				{
					out << arrayStorage->GetValue<glm::vec2>(i);
					break;
				}
				case ScriptFieldType::Vector3:
				{
					out << arrayStorage->GetValue<glm::vec3>(i);
					break;
				}
				case ScriptFieldType::Vector4:
				{
					out << arrayStorage->GetValue<glm::vec4>(i);
					break;
				}
				case ScriptFieldType::Prefab:
				case ScriptFieldType::Entity:
				case ScriptFieldType::Mesh:
				case ScriptFieldType::Material:
				case ScriptFieldType::PhysicsMaterial:
				case ScriptFieldType::Texture2D:
				case ScriptFieldType::InputAction:
				case ScriptFieldType::InputBindingContext:
				{
					out << arrayStorage->GetValue<UUID>(i);
					break;
				}
				default:
				{
					PF_CORE_ASSERT(false);
					out << "";
					break;
				}
			}
		}

		out << YAML::EndSeq;
	}
	void SerializeCommon::SaveScriptField(YAML::Emitter& out, Count<FieldStorageBase> fieldStorage)
	{
		if (fieldStorage == nullptr)
			return;

		out << YAML::BeginMap; // ScriptField
		out << YAML::Key << "NameID" << YAML::Value << fieldStorage->GetFieldInfo()->FullName;
		out << YAML::Key << "Type" << YAML::Value << EnumReflection::EnumString(fieldStorage->GetFieldInfo()->Type);
		if (fieldStorage->GetFieldInfo()->IsArray())
		{
			SaveScriptFieldArray(out, fieldStorage.As<ArrayFieldStorage>());
		}
		else if (fieldStorage->GetFieldInfo()->IsEnum())
		{
			SaveScriptFieldEnum(out, fieldStorage.As<EnumFieldStorage>());
		}
		else
		{
			SaveScriptFieldStorage(out, fieldStorage.As<FieldStorage>());
		}
		out << YAML::EndMap; // ScriptField
	}

	static void LoadScriptFieldEnum(YAML::iterator::value_type& scriptField, Count<EnumFieldStorage> enumFieldStorage, ScriptFieldType savedScriptFieldType)
	{
		auto dataNode = scriptField["Data"];
		switch (enumFieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Int8:
			{
				enumFieldStorage->SetValue(static_cast<int8_t>(dataNode.as<int16_t>()));
				break;
			}
			case ScriptFieldType::Int16:
			{
				enumFieldStorage->SetValue(dataNode.as<int16_t>());
				break;
			}
			case ScriptFieldType::Int32:
			{
				enumFieldStorage->SetValue(dataNode.as<int32_t>());
				break;
			}
			case ScriptFieldType::Int64:
			{
				enumFieldStorage->SetValue(dataNode.as<int64_t>());
				break;
			}
			case ScriptFieldType::UInt8:
			{
				enumFieldStorage->SetValue(dataNode.as<uint8_t>());
				break;
			}
			case ScriptFieldType::UInt16:
			{
				enumFieldStorage->SetValue(dataNode.as<uint16_t>());
				break;
			}
			case ScriptFieldType::UInt32:
			{
				enumFieldStorage->SetValue(dataNode.as<uint32_t>());
				break;
			}
			case ScriptFieldType::UInt64:
			{
				enumFieldStorage->SetValue(dataNode.as<uint64_t>());
				break;
			}
		}
	}

	static void LoadScriptFieldStorage(YAML::iterator::value_type& scriptField, Count<FieldStorage> fieldStorage, ScriptFieldType savedScriptFieldType)
	{
		auto dataNode = scriptField["Data"];

		if (IsScriptFieldAssetType(fieldStorage->GetFieldInfo()->Type))
		{
			fieldStorage->SetValue(dataNode.as<uint64_t>());
			return;
		}
		switch (fieldStorage->GetFieldInfo()->Type)
		{
			case ScriptFieldType::Bool:
			{
				fieldStorage->SetValue(dataNode.as<bool>());
				break;
			}
			case ScriptFieldType::Int8:
			{
				fieldStorage->SetValue(static_cast<int8_t>(dataNode.as<int16_t>()));
				break;
			}
			case ScriptFieldType::Int16:
			{
				fieldStorage->SetValue(dataNode.as<int16_t>());
				break;
			}
			case ScriptFieldType::Int32:
			{
				fieldStorage->SetValue(dataNode.as<int32_t>());
				break;
			}
			case ScriptFieldType::Int64:
			{
				fieldStorage->SetValue(dataNode.as<int64_t>());
				break;
			}
			case ScriptFieldType::UInt8:
			{
				fieldStorage->SetValue(dataNode.as<uint8_t>());
				break;
			}
			case ScriptFieldType::UInt16:
			{
				fieldStorage->SetValue(dataNode.as<uint16_t>());
				break;
			}
			case ScriptFieldType::UInt32:
			{
				fieldStorage->SetValue(dataNode.as<uint32_t>());
				break;
			}
			case ScriptFieldType::UInt64:
			{
				fieldStorage->SetValue(dataNode.as<uint64_t>());
				break;
			}
			case ScriptFieldType::Float:
			{
				fieldStorage->SetValue(dataNode.as<float>());
				break;
			}
			case ScriptFieldType::Double:
			{
				fieldStorage->SetValue(dataNode.as<double>());
				break;
			}
			case ScriptFieldType::String:
			{
				fieldStorage->SetValue(dataNode.as<std::string>());
				break;
			}
			case ScriptFieldType::Vector2:
			{
				fieldStorage->SetValue(dataNode.as<glm::vec2>());
				break;
			}
			case ScriptFieldType::Vector3:
			{
				fieldStorage->SetValue(dataNode.as<glm::vec3>());
				break;
			}
			case ScriptFieldType::Vector4:
			{
				fieldStorage->SetValue(dataNode.as<glm::vec4>());
				break;
			}
			case ScriptFieldType::Prefab:
			case ScriptFieldType::Entity:
			case ScriptFieldType::Mesh:
			case ScriptFieldType::DynamicMesh:
			case ScriptFieldType::Material:
			case ScriptFieldType::PhysicsMaterial:
			case ScriptFieldType::Texture2D:
			{
				fieldStorage->SetValue(dataNode.as<uint64_t>());
				break;
			}
			default:
			{
				PF_CORE_ASSERT(false);
				break;
			}
		}
	}
	static void LoadScriptFieldArray(YAML::iterator::value_type& scriptField, Count<ArrayFieldStorage> arrayStorage, ScriptFieldType savedScriptFieldType)
	{
		if (!scriptField["Length"])
			return;
		if (!scriptField["Data"])
			return;

		auto dataNode = scriptField["Data"];
		if (!dataNode.IsSequence())
			return;

		arrayStorage->Resize(dataNode.size());

		for (uintptr_t i = 0; i < dataNode.size(); i++)
		{
			if (IsScriptFieldAssetType(arrayStorage->GetFieldInfo()->Type))
			{
				arrayStorage->SetValue(i, static_cast<uint64_t>(dataNode[i].as<int16_t>()));
				continue;
			}

			switch (arrayStorage->GetFieldInfo()->Type)
			{
				case ScriptFieldType::Bool:
				{
					arrayStorage->SetValue(i, dataNode[i].as<bool>());
					break;
				}
				case ScriptFieldType::Int8:
				{
					arrayStorage->SetValue(i, static_cast<int8_t>(dataNode[i].as<int16_t>()));
					break;
				}
				case ScriptFieldType::Int16:
				{
					arrayStorage->SetValue(i, dataNode[i].as<int16_t>());
					break;
				}
				case ScriptFieldType::Int32:
				{
					arrayStorage->SetValue(i, dataNode[i].as<int32_t>());
					break;
				}
				case ScriptFieldType::Int64:
				{
					arrayStorage->SetValue(i, dataNode[i].as<int64_t>());
					break;
				}
				case ScriptFieldType::UInt8:
				{
					arrayStorage->SetValue(i, dataNode[i].as<uint8_t>());
					break;
				}
				case ScriptFieldType::UInt16:
				{
					arrayStorage->SetValue(i, dataNode[i].as<uint16_t>());
					break;
				}
				case ScriptFieldType::UInt32:
				{
					arrayStorage->SetValue(i, dataNode[i].as<uint32_t>());
					break;
				}
				case ScriptFieldType::UInt64:
				{
					arrayStorage->SetValue(i, dataNode[i].as<uint64_t>());
					break;
				}
				case ScriptFieldType::Float:
				{
					arrayStorage->SetValue(i, dataNode[i].as<float>());
					break;
				}
				case ScriptFieldType::Double:
				{
					arrayStorage->SetValue(i, dataNode[i].as<double>());
					break;
				}
				case ScriptFieldType::String:
				{
					arrayStorage->SetValue(i, dataNode[i].as<std::string>());
					break;
				}
				case ScriptFieldType::Vector2:
				{
					arrayStorage->SetValue(i, dataNode[i].as<glm::vec2>());
					break;
				}
				case ScriptFieldType::Vector3:
				{
					arrayStorage->SetValue(i, dataNode[i].as<glm::vec3>());
					break;
				}
				case ScriptFieldType::Vector4:
				{
					arrayStorage->SetValue(i, dataNode[i].as<glm::vec4>());
					break;
				}
				case ScriptFieldType::Entity:
				{
					arrayStorage->SetValue(i, dataNode[i].as<uint64_t>());
					break;
				}
				default:
					PF_CORE_ASSERT(false);

			}
		}

	}
	void SerializeCommon::LoadScriptField(YAML::iterator::value_type& scriptField, Count<FieldStorageBase> fieldStorage)
	{

		if (!fieldStorage)
			return;

		ScriptFieldType scriptFieldType = EnumReflection::StringEnum<ScriptFieldType>(scriptField["Type"].as<std::string>());

		if (fieldStorage->GetFieldInfo()->IsArray())
		{
			LoadScriptFieldArray(scriptField, fieldStorage.As<ArrayFieldStorage>(), scriptFieldType);
		}
		else if (fieldStorage->GetFieldInfo()->IsEnum())
		{

			LoadScriptFieldEnum(scriptField, fieldStorage.As<EnumFieldStorage>(), scriptFieldType);
		}
		else
		{
			LoadScriptFieldStorage(scriptField, fieldStorage.As<FieldStorage>(), scriptFieldType);
		}
	}
}
