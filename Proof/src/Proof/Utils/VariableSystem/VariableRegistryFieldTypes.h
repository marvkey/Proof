#pragma once
#include "Variable.h"
#include "Proof/Utils/MultiUse.h"
#include <glm/glm.hpp>
namespace Proof
{
	using BindableVariableBool = BindableVariable<bool, VariableTypes::Bool>;
	using BindableVariableInt = BindableVariable<int, VariableTypes::Int>;
	using BindableVariableFloat = BindableVariable<float, VariableTypes::Float>;
	using BindableVariableVec2 = BindableVariable<glm::vec2, VariableTypes::Vec2>;
	using BindableVariableVec3 = BindableVariable<glm::vec3, VariableTypes::Vec3>;
	using BindableVariableVec4 = BindableVariable<glm::vec4, VariableTypes::Vec4>;
	using BindableVariableString = BindableVariable<std::string, VariableTypes::String>;

	template<typename T, T Min, T Max, VariableTypes VT>
	using BindableClampedVariable = BindableVariable<ClampedValue<T, Min, Max>, VT>;

	template<int Min, int Max>
	using BindableClampedInt = BindableVariable<ClampedValue<int, Min, Max>, VariableTypes::Int>;
	template<float Min, float Max>
	using BindableClampedFloat = BindableVariable<ClampedValue<float, Min, Max>, VariableTypes::Float>;
}