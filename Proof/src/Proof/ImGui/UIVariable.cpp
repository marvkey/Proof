#include "Proofprch.h"
#include "UIVariable.h"
#include "UI.h"
#include "UiUtilities.h"
#include "UIWidgets.h"

namespace Proof::UI
{
    bool VariableAttribute(const std::string& label, Count<Variable> value)
    {
		bool modified = false;
		switch (value->GetType())
		{
		case Proof::VariableTypes::None:
			break;
		case Proof::VariableTypes::Bool:
		{
			bool data = value->GetValue<bool>();
			if (UI::AttributeBool(label, data))
			{
				modified = true;
				value->SetValue(data);
			}
		}
			break;
		case Proof::VariableTypes::Int:
		{
			int data = value->GetValue<int>();
			if (UI::AttributeDrag(label, data))
			{
				modified = true;
				value->SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Float:
		{
			float data = value->GetValue<float>();
			if (UI::AttributeDrag(label, data,0.25))
			{
				modified = true;
				value->SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Vec2:
		{
			glm::vec2 data = value->GetValue<glm::vec2>();
			if (UI::AttributeDrag(label, data,0.25))
			{
				modified = true;
				value->SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Vec3:
		{
			glm::vec3 data = value->GetValue<glm::vec3>();
			if (UI::AttributeDrag(label, data,0.25))
			{
				modified = true;
				value->SetValue(data);
			}
		}
		break;

		case Proof::VariableTypes::Vec4:
		{
			glm::vec4 data = value->GetValue<glm::vec4>();
			if (UI::AttributeDrag(label, data,0.25))
			{
				modified = true;
				value->SetValue(data);
			}
		}
		break;
		case Proof::VariableTypes::String:
		{
			std::string valuestr = value->GetValue<std::string>();
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

    bool EditVariableInstance(Count<VariableRegistryInstance> instance)
    {
		if (instance == nullptr)
			return false;
		bool modified = false;

		BeginPropertyGrid();

		for (auto& [id, var] : instance->GetVariableRegistry()->GetVariables())
		{
			Count<Variable> nonConstVar = instance->GetVariable(id);
			PushID();
			modified |= VariableAttribute(instance->GetVariableRegistry()->GetVariableAsName(id), nonConstVar);
			PopID();
		}

		EndPropertyGrid();
        return modified;
    }
	bool VariablesSearchUp(const char* ID, Count<VariableRegistry> registry, VariableTypes variableTypes, UUID& selected, bool allowClear, const char* hint, ImVec2 size)
	{

		UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f).Value);

		bool modified = false;

		AssetID current = selected;

		ImGui::SetNextWindowSize({ size.x, 0.0f });

		static bool grabFocus = true;

		if (UI::BeginPopup(ID, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			static std::string searchString;

			if (ImGui::GetCurrentWindow()->Appearing)
			{
				grabFocus = true;
				searchString.clear();
			}

			// Search widget
			UI::ShiftCursor(3.0f, 2.0f);
			ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetCursorPosX() * 2.0f);
			Widgets::SearchWidget(searchString, hint, &grabFocus);

			const bool searching = !searchString.empty();

			// Clear property button
			if (allowClear)
			{
				UI::ScopedColourStack buttonColours(
					ImGuiCol_Button, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.0f),
					ImGuiCol_ButtonHovered, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.2f),
					ImGuiCol_ButtonActive, UI::ColourWithMultipliedValue(Colours::Theme::Background, 0.9f));

				UI::ScopedStyleVar border(ImGuiStyleVar_FrameBorderSize, 0.0f);

				ImGui::SetCursorPosX(0);

				ImGui::PushItemFlag(ImGuiItemFlags_NoNav, searching);

				if (ImGui::Button("CLEAR", { ImGui::GetWindowWidth(), 0.0f }))
				{
					allowClear = true;
					modified = true;
				}

				ImGui::PopItemFlag();
			}

			// List of assets
			{
				UI::ScopedStyleColor listBoxBg(ImGuiCol_FrameBg, IM_COL32_DISABLE);
				UI::ScopedStyleColor listBoxBorder(ImGuiCol_Border, IM_COL32_DISABLE);

				ImGuiID listID = ImGui::GetID("##SearchListBox");
				if (ImGui::BeginListBox("##SearchListBox", ImVec2(-FLT_MIN, 0.0f)))
				{
					bool forwardFocus = false;

					ImGuiContext& g = *GImGui;
					if (g.NavJustMovedToId != 0)
					{
						if (g.NavJustMovedToId == listID)
						{
							forwardFocus = true;
							// ActivateItem moves keyboard navigation focuse inside of the window
							ImGui::ActivateItem(listID);
							ImGui::SetKeyboardFocusHere(1);
						}
					}

					const auto& assetsRegistry = registry->GetVariables();

					for (auto& [variableID,variable] : assetsRegistry)
					{
						if (variable->GetType() != variableTypes)
							continue;

						const std::string assetName = registry->GetVariableAsName(variable->GetUUID());
						if (!searchString.empty() && !UI::IsMatchingSearch(assetName, searchString))
							continue;
						bool is_selected = (current == variable->GetUUID());
						if (ImGui::Selectable(assetName.c_str(), is_selected))
						{
							current = variableID;
							selected = variableID;
							modified = true;
						}

						if (forwardFocus)
						{
							forwardFocus = false;
						}
						else if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndListBox();
				}
			}
			if (modified)
				ImGui::CloseCurrentPopup();

			UI::EndPopup();
		}
		HandleModified(modified);

		return modified;
	}
	bool BindableVariableAttributeBool(const std::string& label, BindableVariableBool& val)
	{
		UI::BeginPropertyGrid(3);


		if (ImGui::Button("Bind"))
		{

		}
		UI::EndPropertyGrid();
		return false;
	}
	template<class T, VariableTypes VT>
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariable<T, VT>& val, Count<VariableRegistry> registry, float speed)
	{
		bool modified = false;
		if (val.IsSet())
		{
			UI::AttributeTextBar(label, fmt::format("Bound to: {}", registry->GetVariableAsName(val.GetVariableID())));
			if (UI::AttributeButton("##Unbind", "Unbind"))
			{
				modified = true;
				val.UnBind();
			}
		}
		{
			T currentvalue = val.GetValue<T>();
			if (UI::AttributeDrag(label, currentvalue, speed))
			{
				modified = true;
				val.SetValue(currentvalue);
			}
			std::string searchValidvalidVariablesID = UI::GenerateLabelID("Valid Variables");
			{
				auto uuid = val.GetVariableID();
				if (UI::VariablesSearchUp(searchValidvalidVariablesID.c_str(), registry, val.GetVariableType(), uuid))
				{
					val.SetUseAsVariable(true, registry->GetVariableSetStorage());
					val.SetVariable(uuid);
					modified = true;
				}

				if (ImGui::Button(GenerateLabelID("Bind")))
				{
					ImGui::OpenPopup(searchValidvalidVariablesID.c_str());
				}

			}
		}
		return modified;
	}

	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableInt& val, Count<VariableRegistry> registry,float speed)
	{
		return BindableVariableAttributeDrag(label, val, registry, speed);
	}
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableFloat& val,Count<VariableRegistry> registry, float speed)
	{
		return BindableVariableAttributeDrag(label, val, registry, speed);
	}
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableVec2& val, Count<VariableRegistry> registry, float speed)
	{
		return BindableVariableAttributeDrag(label, val, registry, speed);
	}
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableVec3& val, Count<VariableRegistry> registry, float speed)
	{
		return BindableVariableAttributeDrag(label, val, registry, speed);
	}
	bool BindableVariableAttributeDrag(const std::string& label, BindableVariableVec4& val, Count<VariableRegistry> registry, float speed)
	{
		return BindableVariableAttributeDrag(label, val, registry, speed);
	}
	bool BindableVariableAttributeInputText(const std::string& label, BindableVariableString& val)
	{
		return false;
		//return BindableVariableAttributeDrag(label, val, registry, speed);
	}
	
	template<class T, VariableTypes VT>
	bool BindableVariableAttrubuteSlider(const std::string& label, BindableVariable<T, VT>& val , Count<VariableRegistry> registry, T min, T max)
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

			T currentvalue = val.GetValue<T>();
			if (UI::AttributeSlider(label, currentvalue, min, max))
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

			{
				auto uuid = val.GetVariableID();
				if (UI::VariablesSearchUp(searchValidvalidVariablesID.c_str(), registry, val.GetVariableType(), uuid))
				{
					val.SetUseAsVariable(true, registry->GetVariableSetStorage());
					val.SetVariable(uuid);
					modified = true;
				}


			}
		}
		return modified;
	}

	bool BindableVariableAttrubuteSlider(const std::string& label, BindableVariableInt& val, Count<VariableRegistry> registry, int min, int max)
	{
		return BindableVariableAttrubuteSlider(label, val, registry, min, max);
	}

	bool BindableVariableAttrubuteSlider(const std::string& label, BindableVariableFloat& val, Count<VariableRegistry> registry, float min, float max)
	{
		return BindableVariableAttrubuteSlider(label, val, registry, min, max);
	}
}
