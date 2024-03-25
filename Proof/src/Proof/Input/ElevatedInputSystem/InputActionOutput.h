#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include <limits>

namespace Proof
{
	enum class InputActionOutputType
	{
		Bool,
		Float,
		Vector2D,
		Vector3D
	};

	struct InputActionOutput
	{
	public:
		InputActionOutput()
		{

		}
		InputActionOutput(bool inValue) : m_Output(inValue ? 1.f : 0.f, 0.f, 0.f), m_OutputType(InputActionOutputType::Bool) {}
		InputActionOutput(float inValue) : m_Output(inValue, 0, 0), m_OutputType(InputActionOutputType::Float) {}
		InputActionOutput(glm::vec2 inValue) : m_Output(inValue.x,inValue.y,0), m_OutputType(InputActionOutputType::Vector2D) {}
		InputActionOutput(glm::vec3 inValue) : m_Output(inValue), m_OutputType(InputActionOutputType::Vector3D) {}
		InputActionOutput(InputActionOutputType InValueType, glm::vec3 value) : m_Output(value), m_OutputType(InValueType)
		{
			// Adjust value components based on the specified type
			if (m_OutputType == InputActionOutputType::Bool || m_OutputType == InputActionOutputType::Float)
			{
				m_Output.y = 0.f;
				m_Output.z = 0.f;
			}
			if (m_OutputType == InputActionOutputType::Vector2D)
			{
				m_Output.z = 0.f;
			}
		}

		InputActionOutputType GetOutputType()const { return m_OutputType; }

		template<typename T>
		inline T Get() const { static_assert(sizeof(T) == 0, "Type not Supported"); }

		float operator[](int Index) const { return m_Output[Index]; }

		float GetMagnitudeSq() const
		{
			switch (GetOutputType())
			{
			case InputActionOutputType::Bool:
			case InputActionOutputType::Float:
				return m_Output.x * m_Output.x;
			case InputActionOutputType::Vector2D:
				return m_Output.x * m_Output.x + m_Output.y * m_Output.y ;
			case InputActionOutputType::Vector3D:
				return m_Output.x * m_Output.x + m_Output.y * m_Output.y + m_Output.z * m_Output.z;
			}
		}

		void Reset()
		{
			m_Output = glm::vec3(0);
		}

		bool IsNonZero(float tolerance = std::numeric_limits<float>::epsilon()) const { return float{ m_Output.x * m_Output.x + m_Output.y * m_Output.y + m_Output.z * m_Output.z } >= tolerance * tolerance; }
	private:
		glm::vec3 m_Output = glm::vec3(0);

		InputActionOutputType m_OutputType = InputActionOutputType::Bool;
	};

	template<>
	inline bool InputActionOutput::Get() const
	{
		return IsNonZero();
	}

	template<>
	inline float InputActionOutput::Get() const
	{
		return m_Output.x;
	}

	template<>
	inline glm::vec2 InputActionOutput::Get() const
	{
		return glm::vec2(m_Output.x, m_Output.y);
	}

	template<>
	inline glm::vec3 InputActionOutput::Get() const
	{
		return m_Output;
	}
}