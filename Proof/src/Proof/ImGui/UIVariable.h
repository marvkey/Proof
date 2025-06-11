#pragma once
#include "Proof/Core/Core.h"
#include <imgui.h>
#include "Proof/ImGui/UI.h"
#include "UiUtilities.h"

#include "Proof/Utils/VariableSystem/Variable.h"
#include "Proof/Utils/VariableSystem/VariableRegistryFieldTypes.h"

namespace Proof::UI
{
	bool VariableAttribute(const std::string& label, Count<Variable> value);
	bool EditVariableInstance(Count<VariableRegistryInstance> instance);

	bool VariablesSearchUp(const char* ID,Count<VariableRegistry> registry, VariableTypes variableTypes, UUID& selected, bool allowClear = true, const char* hint ="Search Variables", ImVec2 size = ImVec2{ 250.0f, 350.0f });


	bool BindableVariableAttributeBool(const std::string& label, BindableVariableBool& val,Count< VariableSetStorage> storage);

	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableInt& val, Count<VariableRegistry> registry,float speed = 1.0f);
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableFloat& val, Count<VariableRegistry> registry, float speed = 1.0f);
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableVec2& val, Count<VariableRegistry> registry, float speed = 1.0f);
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableVec3& val, Count<VariableRegistry> registry, float speed = 1.0f);
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableVec4& val, Count<VariableRegistry> registry, float speed = 1.0f);
	bool BindableVariableAttributeInputText(const std::string& label, BindableVariableString& val);


	bool BindableVariableAttrubuteSlider(const std::string& label, BindableVariableInt& val, Count<VariableRegistry> registry, int min, int max);
	bool BindableVariableAttrubuteSlider(const std::string& label, BindableVariableFloat& val, Count<VariableRegistry> registry, float min, float max);



	template<int Min, int Max>
	bool BindableVariableAttributeSlider(const std::string& label, BindableClampedInt<Min, Max>& val, Count<VariableRegistry> registry)
	{
		bool modified = false;
		if (val.IsSet())
		{
			UI::PushNoEndNextColumn();
			UI::AttributeTextBar(label, fmt::format("Bound to: {}", registry->GetVariableAsName(val.GetVariableID())));
			ImGui::SameLine();
			UI::PopNoEndNextColumn();
			if (UI::AttributeButton("Unbind"))
			{
				modified = true;
				val.UnBind();
			}
		}
		else
		{
			UI::PushNoEndNextColumn();
			int currentvalue = val.GetValue().GetValue();
			if (UI::AttributeSlider(label, currentvalue, Min, Max))
			{
				modified = true;
				val.SetValue(currentvalue);
			}
			std::string searchValidvalidVariablesID = UI::GenerateLabelID("Valid Variables");
			ImGui::SameLine();
			UI::PopNoEndNextColumn();
			if (UI::AttributeButton(GenerateLabelID("Bind")))
			{
				ImGui::OpenPopup(searchValidvalidVariablesID.c_str());
			}
			auto uuid = val.GetVariableID();
			if (UI::VariablesSearchUp(searchValidvalidVariablesID.c_str(), registry, VariableTypes::Int, uuid))
			{
				val.SetUseAsVariable(true, registry->GetVariabelSetStorage());
				val.SetVariable(uuid);
				modified = true;
			}
		}
		return modified;
	}

	template<float Min, float Max>
	bool BindableVariableAttributeSlider(const std::string& label, BindableClampedFloat<Min, Max>& val, Count<VariableRegistry> registry)
	{
		bool modified = false;
		if (val.IsSet())
		{
			UI::PushNoEndNextColumn();
			UI::AttributeTextBar(label, fmt::format("Bound to: {}", registry->GetVariableAsName(val.GetVariableID())));
			ImGui::SameLine();
			UI::PopNoEndNextColumn();
			if (UI::AttributeButton("Unbind"))
			{
				modified = true;
				val.UnBind();
			}
		}
		else
		{
			std::string searchValidvalidVariablesID = UI::GenerateLabelID("Valid Variables");
			UI::PushNoEndNextColumn();
			float currentvalue = val.GetValue().GetValue();
			if (UI::AttributeSlider(label, currentvalue, Min, Max))
			{
				modified = true;
				val.SetValue(currentvalue);
			}
			ImGui::SameLine();
			UI::PopNoEndNextColumn();
			if (UI::AttributeButton(GenerateLabelID("Bind")))
			{
				ImGui::OpenPopup(searchValidvalidVariablesID.c_str());
			}
			auto uuid = val.GetVariableID();
			if (UI::VariablesSearchUp(searchValidvalidVariablesID.c_str(), registry, VariableTypes::Float, uuid))
			{
				val.SetUseAsVariable(true, registry->GetVariabelSetStorage());
				val.SetVariable(uuid);
				modified = true;
			}
		}

		return modified;
	}
}