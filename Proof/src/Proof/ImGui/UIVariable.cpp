#include "Proofprch.h"
#include "UIVariable.h"
#include "UI.h"

namespace Proof::UI
{
    bool VariableAttributeSlider(const std::string& label, Variable& value)
    {
		bool modified = false;
		switch (value.GetType())
		{
		case Proof::VariableTypes::None:
			break;

		case Proof::VariableTypes::Int:
		{
			int data = value.GetValue<int>();
			if (UI::AttributeDrag(label, data))
			{
				modified = true;
				value.SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Float:
		{
			float data = value.GetValue<float>();
			if (UI::AttributeDrag(label, data))
			{
				modified = true;
				value.SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Vec2:
		{
			glm::vec2 data = value.GetValue<glm::vec2>();
			if (UI::AttributeDrag(label, data))
			{
				modified = true;
				value.SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Vec3:
		{
			glm::vec3 data = value.GetValue<glm::vec3>();
			if (UI::AttributeDrag(label, data))
			{
				modified = true;
				value.SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Vec4:
		{
			glm::vec4 data = value.GetValue<glm::vec4>();
			if (UI::AttributeDrag(label, data))
			{
				modified = true;
				value.SetValue(data);
			}
		}
		break;
		case Proof::VariableTypes::String:
		{
			std::string valuestr = value.GetValue<std::string>();
			char buffer[256];
			memset(buffer, 0, 256);
			memcpy(buffer, valuestr.c_str(), valuestr.length());
			//if (UI::AttributeInputText(label, valuestr, 0))
			//{
			//	value.SetValue<std::string>(valuestr);
			//}
			modified = true;

		}
		break;
		default:
			break;
		}

        return modified;
    }	

    bool EditVariableRegistryRaw(Count<VariableRegistry> registry)
    {
		bool modified = false;

		BeginPropertyGrid();

		for (auto& [id, var] : registry->GetVariables())
		{
			Variable& nonConstVar = registry->GetVariable(id);
			PushID();
			modified |= VariableAttributeSlider(registry->GetVariableAsName(id), nonConstVar);
			PopID();
		}

		EndPropertyGrid();
        return modified;
    }
}
