#pragma once
#include <glm/glm.hpp>
#include "Proof/Math/MathResource.h"
#include <glm/gtx/quaternion.hpp>
namespace Proof
{
	struct Transform
	{
		glm::vec3 Location = glm::vec3(1);
		glm::vec3 Rotation = glm::vec3(0);
		glm::vec3 Scale = glm::vec3(1);

		void SetTransform(const glm::mat4& transform)
		{
			glm::quat rotationQuat;
			MathResource::DecomposeTransform(transform, Location, rotationQuat, Scale);
			Rotation = glm::eulerAngles(rotationQuat);
		}
	};

    // still neeeds some work to be ready
    // able to edit the min and  max at runtime could be a positibily
    template<typename T, T MinVal = std::numeric_limits<T>::min(), T MaxVal = std::numeric_limits<T>::max()>
    struct VariableWithRange
    {
        T value;   // Only the value takes up space

        // Constructor that clamps the initial value
        VariableWithRange(T val) : value(std::clamp(val, MinVal, MaxVal)) {}

        // Overload assignment operator to clamp the value on assignment
        VariableWithRange& operator=(T val) {
            value = std::clamp(val, MinVal, MaxVal);
            return *this;
        }

        // Implicit conversion operator to retrieve the value
        operator T() const {
            return value;
        }
    };
}