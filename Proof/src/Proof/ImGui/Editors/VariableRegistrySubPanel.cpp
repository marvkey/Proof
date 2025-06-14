#include "Proofprch.h"
#include "VariableRegistrySubPanel.h"
#include "Proof/Utils/VariableSystem/Variable.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIVariable.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/ImGui/UiUtilities.h"

namespace Proof
{

	VariableRegistrySubPanel::VariableRegistrySubPanel(Count<class VariableRegistry> registry)
		:m_Registry(registry)
	{
	}
	void VariableRegistrySubPanel::OnImguiRender()
	{
		ImGui::Begin(fmt::format("Variables##{}",m_Registry->SpecialID.Get()).c_str());
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::IsAnyItemHovered() == false)
		{
			AssetSelectionManager::DeselectAll(AssetSelectionContext::VariableRegistry, m_Registry->SpecialID.Get());

		}
		if (ImGui::Button("AddVariable"))
		{
			ImGui::OpenPopup("AddVariable");
		}
		if (ImGui::BeginPopup("AddVariable"))
		{
			EnumReflection::ForEach<VariableTypes>([&](VariableTypes type)
				{
					if (type == VariableTypes::None)
						return;
					UI::ScopedID sopeID(fmt::format("VariabelTupe {}", EnumReflection::EnumString(type)));
					if (ImGui::MenuItem(EnumReflection::EnumString(type).c_str()))
					{
						m_Registry->AddVariable(type);
					}
				});
			ImGui::EndPopup();
		}

		HierarchyPanel();
		PropertyPanel();
		ImGui::End();
	}

	void DrawVariableNode(Count<VariableRegistry> registry, UUID variableID)
	{
		UI::ScopedID scopeId(fmt::format("{}",variableID.Get()).c_str());

		ImGuiTreeNodeFlags flags;
		flags = ((AssetSelectionManager::IsSelected(AssetSelectionContext::VariableRegistry, registry.GetMemoryAddress(), variableID.Get()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow);

		flags |= ImGuiTreeNodeFlags_Leaf;//makes the tree not use an arrow

		flags |= ImGuiTreeNodeFlags_SpanFullWidth;

		Count<Variable> var = registry->GetVariable(variableID);
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)variableID.Get(), flags, registry->GetVariableAsName(variableID).c_str());
		if (ImGui::IsItemClicked())
		{
			AssetSelectionManager::DeselectAll(AssetSelectionContext::VariableRegistry, registry.GetMemoryAddress());
			AssetSelectionManager::Select(AssetSelectionContext::VariableRegistry, registry.GetMemoryAddress(), variableID);
		}


		if (opened)
		{
			//for (auto& children : element->Children)
			//	DrawElementNode(children);
			ImGui::TreePop();
		}
	}

	void DrawVariableProperty(Count<VariableRegistry> registry, UUID variableID)
	{
		{

			auto currentname = registry->GetVariableAsName(variableID);
			auto oldName = registry->GetVariableAsName(variableID);
			if (UI::AttributeInputText("Name", currentname))
			{
				registry->UpdateVariableName(oldName, currentname);
			}
		}
		UI::BeginPropertyGrid(3);

		Count<Variable> var = registry->GetVariable(variableID);
		{
			auto type = var->GetType();
			if (UI::EnumCombo("Type", type, { VariableTypes::None }))
				var->SetType(type);
		}
		{
			bool isArray = var->IsArray();
			ImGui::Checkbox("IsArray", &isArray);
		}
		UI::EndPropertyGrid();


		UI::BeginPropertyGrid();
		UI::VariableAttribute("Default Value", var);
		UI::EndPropertyGrid();
	}

	void VariableRegistrySubPanel::HierarchyPanel()
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0,0,0,1 });

		ImGui::BeginChild("Child Hierarchy", { ImGui::GetContentRegionAvail().x,ImGui::GetWindowHeight() / 2 });

		for (auto& [id, var] : m_Registry->GetVariables())
			DrawVariableNode(m_Registry, id);
		ImGui::EndChild();

		ImGui::PopStyleColor();
	}
	void VariableRegistrySubPanel::PropertyPanel()
	{
		ImGui::BeginChild("Properties", ImGui::GetContentRegionAvail());

		if (AssetSelectionManager::HasSelections(AssetSelectionContext::VariableRegistry, m_Registry->SpecialID.Get()))
		{
			auto selectedItem = AssetSelectionManager::GetSelections(AssetSelectionContext::VariableRegistry, m_Registry->SpecialID.Get()).front();
			DrawVariableProperty(m_Registry,selectedItem);
		}

		ImGui::EndChild();
	}
}
