#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include <limits>

namespace Proof
{
	enum class InputActionValueType
	{
		Bool,
		Float,
		Vector2D,
		Vector3D
	};

	struct InputActionValue
	{
	public:
		InputActionValue(bool inValue) : m_Value(inValue ? 1.f : 0.f, 0.f, 0.f), m_ValueType(InputActionValueType::Bool) {}
		InputActionValue(float inValue) : m_Value(inValue, 0, 0), m_ValueType(InputActionValueType::Float) {}
		InputActionValue(glm::vec2 inValue) : m_Value(inValue.x,inValue.y,0), m_ValueType(InputActionValueType::Vector2D) {}
		InputActionValue(glm::vec3 inValue) : m_Value(inValue), m_ValueType(InputActionValueType::Vector3D) {}
		InputActionValue(InputActionValueType InValueType, glm::vec3 value) : m_Value(value), m_ValueType(InValueType)
		{
			// Clear out value components to match type
			switch (m_ValueType)
			{
			case InputActionValueType::Bool:
			case InputActionValueType::Float:
				m_Value.y = 0.f;
				//[[fallthrough]];
			case InputActionValueType::Vector2D:
				m_Value.z = 0.f;
				//[[fallthrough]];
			case InputActionValueType::Vector3D:
			default:
				return;
			}
		}

		InputActionValueType GetValueType()const { return m_ValueType; }

		template<typename T>
		inline T Get() const { static_assert(sizeof(T) == 0, "Unsupported conversion for type"); }

		// Read only index based value accessor, doesn't care about type. Expect 0 when accessing unused components.
		float operator[](int Index) const { return m_Value[Index]; }

		float GetMagnitudeSq() const
		{
			switch (GetValueType())
			{
			case InputActionValueType::Bool:
			case InputActionValueType::Float:
				return m_Value.x * m_Value.x;
			case InputActionValueType::Vector2D:
				return m_Value.x * m_Value.x + m_Value.y * m_Value.y ;
			case InputActionValueType::Vector3D:
				return m_Value.x * m_Value.x + m_Value.y * m_Value.y + m_Value.z * m_Value.z;
			}
		}

		// Resets Value without affecting ValueType
		void Reset()
		{
			m_Value = glm::vec3(0);
		}

		bool IsNonZero(float Tolerance = std::numeric_limits<float>::epsilon()) const { return float{ m_Value.x * m_Value.x + m_Value.y * m_Value.y + m_Value.z * m_Value.z } >= Tolerance * Tolerance; }
	private:
		glm::vec3 m_Value = glm::vec3(0);

		InputActionValueType m_ValueType;
	};

	// Supported getter specializations
	template<>
	inline bool InputActionValue::Get() const
	{
		// True if any component is non-zero
		return m_Value.x > 0;
	}

	template<>
	inline float InputActionValue::Get() const
	{
		return m_Value.x;
	}

	template<>
	inline glm::vec2 InputActionValue::Get() const
	{
		return glm::vec2(m_Value.x, m_Value.y);
	}

	template<>
	inline glm::vec3 InputActionValue::Get() const
	{
		return m_Value;
	}
}