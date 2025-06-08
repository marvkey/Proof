#include "Proofprch.h"
#include "InputCustomizers.h"

namespace Proof
{

	Count<InputCustomizer> InputCustomizer::CreateInputInteraction(InputCustomizerType type)
	{
		switch (type)
		{
			case Proof::InputCustomizerType::Default:
				PF_CORE_ASSERT(type == InputCustomizer::GetStaticCustomizerType(), "Interaction Type does not match");
				return Count<InputCustomizer>::Create();
				break;
			case Proof::InputCustomizerType::Scale:
				PF_CORE_ASSERT(type == InputCustomizerScale::GetStaticCustomizerType(), "Interaction Type does not match");
				return Count<InputCustomizerScale>::Create();
				break;
			case Proof::InputCustomizerType::Invert:
				PF_CORE_ASSERT(type == InputCustomizerInvert::GetStaticCustomizerType(), "Interaction Type does not match");
				return Count<InputCustomizerInvert>::Create();
				break;
		}

		PF_CORE_ASSERT(false);
		return nullptr;
	}
    InputActionOutput Proof::InputCustomizerInvert::CustomizeActionOutput(Count<ElevatedPlayer> player, const InputActionOutput& currentValue, float deltaTime)
    {
		auto axes = static_cast<std::underlying_type_t<InputCustomizerInvertAxis>>(InvertedAxes);

		switch (currentValue.GetOutputType())
		{
			case InputActionOutputType::Bool:
				// Invert only if X is set (assumes Bool maps to X axis logic)
				if (EnumReflection::HasAnyFlags(InvertedAxes, InputCustomizerInvertAxis::X))
					return InputActionOutput(!currentValue.Get<bool>());
				return currentValue;

			case InputActionOutputType::Float:
				return InputActionOutput(currentValue.Get<float>() * ((EnumReflection::HasAnyFlags(InvertedAxes, InputCustomizerInvertAxis::X)) ? -1.0f : 1.0f));

			case InputActionOutputType::Vector2D:
			{
				glm::vec2 value = currentValue.Get<glm::vec2>();
				if (EnumReflection::HasAnyFlags(InvertedAxes, InputCustomizerInvertAxis::X)) value.x *= -1.0f;
				if (EnumReflection::HasAnyFlags(InvertedAxes, InputCustomizerInvertAxis::Y)) value.y *= -1.0f;
				return InputActionOutput(value);
			}

			case InputActionOutputType::Vector3D:
			{
				glm::vec3 value = currentValue.Get<glm::vec3>();
				if (EnumReflection::HasAnyFlags(InvertedAxes, InputCustomizerInvertAxis::X)) value.x *= -1.0f;
				if (EnumReflection::HasAnyFlags(InvertedAxes, InputCustomizerInvertAxis::Y)) value.y *= -1.0f;
				if (EnumReflection::HasAnyFlags(InvertedAxes, InputCustomizerInvertAxis::Z)) value.z *= -1.0f;
				return InputActionOutput(value);
			}
		}

    }
	
}
