#pragma once
#include "Proof/Core/Core.h"
#include "InputTypes.h"
#include "InputActionOutput.h"

namespace Proof
{

	enum class InputCustomizerType
	{
		Default = 0,
		Scale,
		Invert
	};

#define INPUT_CUSTOMIZER_TYPE(type) static InputCustomizerType GetStaticCustomizerType() { return InputCustomizerType::type; }\
								virtual InputCustomizerType GetCustomizerType() const { return GetStaticCustomizerType(); }

	class ElevatedPlayer;
	class InputCustomizer : public RefCounted
	{
	public:
		INPUT_CUSTOMIZER_TYPE(Default);
		static Count<InputCustomizer> CreateInputInteraction(InputCustomizerType type);

	protected:
		virtual InputActionOutput CustomizeActionOutput(Count<ElevatedPlayer> player, const InputActionOutput& currentValue, float deltaTime)
		{
			return currentValue;
		}

		friend class ElevatedPlayer;
	};


	class InputCustomizerScale : public InputCustomizer
	{
	public:
		INPUT_CUSTOMIZER_TYPE(Scale);
		glm::vec3 Scale = glm::vec3(1);
	protected:
		virtual InputActionOutput CustomizeActionOutput(Count<ElevatedPlayer> player, const InputActionOutput& currentValue, float deltaTime)
		{
			glm::vec3 output;
			switch (currentValue.GetOutputType())
			{
			case InputActionOutputType::Bool:
				return InputActionOutput((bool)currentValue.Get<bool>() * Scale.x);
			case InputActionOutputType::Float:
				return InputActionOutput(currentValue.Get<float>() * Scale.x);
			case InputActionOutputType::Vector2D:
				return InputActionOutput(currentValue.Get<glm::vec2>() * glm::vec2(Scale.x, Scale.y));
			case InputActionOutputType::Vector3D:
				return InputActionOutput(currentValue.Get<glm::vec3>() * glm::vec3(Scale));
			}
		}
		friend class ElevatedPlayer;
	};
	enum class InputCustomizerInvertAxis : uint8_t
	{
		None = 0,
		X = BIT(0),
		Y = BIT(1),
		Z = BIT(2)
	};
	DEFINE_ENUM_CLASS_FLAGS(InputCustomizerInvertAxis);

	class InputCustomizerInvert : public InputCustomizer
	{
	public:
		INPUT_CUSTOMIZER_TYPE(Invert);
		InputCustomizerInvertAxis InvertedAxes = InputCustomizerInvertAxis::None;
	protected:
		virtual InputActionOutput CustomizeActionOutput(Count<ElevatedPlayer> player, const InputActionOutput& currentValue, float deltaTime);

		friend class ElevatedPlayer;
	};
}