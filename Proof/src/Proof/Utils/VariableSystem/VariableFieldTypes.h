#pragma once
#include "VariableField.h"
#include <glm/glm.hpp>
namespace Proof
{
	using VariableFieldInt = VariableField<int, VariableTypes::Int>;
	using VariableFieldFloat = VariableField<float, VariableTypes::Float>;
	using VariableFieldString = VariableField<std::string, VariableTypes::String>;
	using VariableFieldVec2 = VariableField<glm::vec2, VariableTypes::Vec2>;
	using VariableFieldVec3 = VariableField<glm::vec3, VariableTypes::Vec3>;
	using VariableFieldVec4 = VariableField<glm::vec4, VariableTypes::Vec4>;
}