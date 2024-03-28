#include "Proofprch.h"
#include "InputBindingContextPanel.h"
#include "InputActionPanel.h"
#include "Proof/Input/ElevatedInputSystem/InputBindingContext.h"
#include "Proof/Input/ElevatedInputSystem/InputAction.h"
#include "Proof/Input/ElevatedInputSystem/InputKeyBinding.h"
#include "Proof/ImGui/Editors/EditorResources.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIColors.h"
#include "Proof/ImGui/UIUtilities.h"
#include "Proof/ImGui/UIWidgets.h"

namespace Proof
{
	InputBindingContextPanel::InputBindingContextPanel()
		:AssetEditorDetail("InputBindingContextPanel")
	{
	}

	void InputBindingContextPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::InputBindingContext)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to InputBindingContextPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_BindingContext = asset.As<InputBindingContext>();

		AssetEditorDetail::SetAsset(m_BindingContext);
	}

	// retunrs false if key has been deleted
	bool ViewInputKeyBinding(Count<InputAction> action, Count<InputKeyBindingBase> keyBindingBase, InputBindingContextPanel::SelectedInputBinding& selected, bool bindingBundleUsing = false, InputKeyBindingBundleTypes type = InputKeyBindingBundleTypes::PositiveX)
	{
		bool deleted = false;
		std::string attributeName;

		switch (keyBindingBase->GetInputKeyBindingType())
		{
			case InputKeyBindingType::Key:
			{
				auto inputKeyBinding = keyBindingBase.As<InputKeyBinding>();

				if (inputKeyBinding->InputKey == ElevatedInputKeys::Invalid || inputKeyBinding->InputKey.GetKeyName().empty())
					attributeName = "{No Binding}";
				else
					attributeName = fmt::format("{} {}",inputKeyBinding->InputKey.GetKeyName(),EnumReflection::EnumString(inputKeyBinding->InputKey.GetDevice()));

				if (bindingBundleUsing)
				{
					attributeName = fmt::format("{}: {}", EnumReflection::EnumString(type), attributeName);
				}

				ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_Leaf
					| ImGuiTreeNodeFlags_SpanAvailWidth
					| ImGuiTreeNodeFlags_AllowItemOverlap
					| ImGuiTreeNodeFlags_FramePadding;

				if (selected.SelectedInputKeyBinding.IsValid())
				{
					if (selected.SelectedInputKeyBinding.Lock() == keyBindingBase)
						treeNodeFlags |= ImGuiTreeNodeFlags_Selected;

				}
				bool isBindingOpen = UI::AttributeTreeNode(attributeName, treeNodeFlags);
				if(!bindingBundleUsing)
				{
					if (ImGui::IsItemClicked())
					{
						selected.InputBindingBundle = false;
						auto other = inputKeyBinding.As<InputKeyBindingBase>();
						selected.SelectedInputKeyBinding = other.Get();
					}
					ImGui::SameLine();
					if (UI::ImageButton(EditorResources::DeleteIcon, { 25,25 }))
					{
						deleted = true;
						//elevateInputKey.();
					}
				}
				else
				{
					if (ImGui::IsItemClicked())
					{
						selected.InputBindingBundle = true;
						auto other = inputKeyBinding.As<InputKeyBindingBase>();
						selected.SelectedInputKeyBinding = other.Get();
					}
				}

				if (isBindingOpen)
				{
					UI::EndTreeNode();
				}
				break;
			}
			case InputKeyBindingType::Bundle:
			{
				auto inputKeyBinding = keyBindingBase.As<InputKeyBindingBundle>();

				std::string attributeName = inputKeyBinding->Name;

				if (attributeName.empty())
				{
					switch (action->OutputType)
					{
						case Proof::InputActionOutputType::Bool:
						case Proof::InputActionOutputType::Float:
							attributeName = "1D Axis";
							break;
						case Proof::InputActionOutputType::Vector2D:
							attributeName = "2D Vector";
							break;
						case Proof::InputActionOutputType::Vector3D:
							attributeName = "3D Vector";
							break;
						default:
							break;
					}
				}
				bool selectedBundle = false;
				if (selected.SelectedInputKeyBinding.IsValid())
				{
					if (selected.SelectedInputKeyBinding.Lock() == keyBindingBase)
						selectedBundle = true;

				}
				bool isBindingOpen = UI::AttributeTreeNode(attributeName, true,6.0f,3.0f, selectedBundle);
				
				if (ImGui::IsItemClicked())
				{
					selected.InputBindingBundle = true;
					auto other = inputKeyBinding.As<InputKeyBindingBase>();
					selected.SelectedInputKeyBinding = other.Get();
				}
				{

					ImGui::SameLine();	
					if (UI::ImageButton(EditorResources::DeleteIcon, { 25,25 }))
					{
						deleted = true;
						//elevateInputKey.();
					}
				}

				if (isBindingOpen)
				{
				
					for (auto& [bindingBundleType, keyBindingBundle] : inputKeyBinding->GetBindings())
					{
						if (action->OutputType == InputActionOutputType::Bool || action->OutputType == InputActionOutputType::Float)
						{
							if (bindingBundleType > InputKeyBindingBundleTypes::NegativeX)
								continue;
						}
						if (action->OutputType == InputActionOutputType::Vector2D)
						{
							if (bindingBundleType > InputKeyBindingBundleTypes::NegativeY)
								continue;
						}

						ViewInputKeyBinding(action, keyBindingBundle, selected,true, bindingBundleType);
					}
					UI::EndTreeNode();
				}
				break;
			}
			default:
				break;
		}
		UI::HandleModified(deleted);
		if (deleted)
			return false;

		return true;
	}

	void ViewInputActionKeyBinding(ElevatedActionKeyBinding& elevatedInputKey, InputBindingContextPanel::SelectedInputBinding& selected)
	{
		bool isElevatedKeyOpen = UI::AttributeTreeNode(AssetManager::GetAssetInfo(elevatedInputKey.InputAction).GetName());
		{
			ImGui::SameLine();

			if (UI::ImageButton(EditorResources::AddIcon, { 25,25 }))
			{
				ImGui::OpenPopup("AddElevatedInputKey");
			}

			ImGui::SameLine();

			if (UI::ImageButton(EditorResources::DeleteIcon, { 25,25 }))
			{
				//elevateInputKey.();
			}
		}

		if (ImGui::BeginPopup("AddElevatedInputKey"))
		{
			EnumReflection::ForEach<InputKeyBindingType>([&](InputKeyBindingType type)
				{
					if (type == InputKeyBindingType::Default)
						return;
					if (ImGui::MenuItem(EnumReflection::EnumString(type).c_str()))
					{
						switch (type)
						{
							case Proof::InputKeyBindingType::Default:
								break;
							case Proof::InputKeyBindingType::Key:
								elevatedInputKey.KeyBindings.push_back(Count<InputKeyBinding>::Create());
								break;
							case Proof::InputKeyBindingType::Bundle:
								elevatedInputKey.KeyBindings.push_back(Count<InputKeyBindingBundle>::Create());
								break;
							default:
								break;
						}
						UI::HandleModified(true);
					}
				});
			ImGui::EndPopup();
		}


		if (isElevatedKeyOpen)
		{
			EditInputInteractions(elevatedInputKey.Interactions);

			for (int i = 0; i < elevatedInputKey.KeyBindings.size(); i++)
			{
				if (!ViewInputKeyBinding(elevatedInputKey.InputAction, elevatedInputKey.KeyBindings[i], selected))
				{
					elevatedInputKey.KeyBindings.erase(elevatedInputKey.KeyBindings.begin() + i);
					UI::HandleModified(true);
				}
			}

			UI::EndTreeNode();
		}
	}
	void InputBindingContextPanel::RenderDetailSettings()
	{

		UI::ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
		UI::ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

		UI::ScopedStyleVar cellPadding(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 2.0f));

		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable
			| ImGuiTableFlags_SizingFixedFit
			| ImGuiTableFlags_BordersInnerV;


		UI::PushID();
		if (ImGui::BeginTable(fmt::format("InputBindingContextTable##{}",m_BindingContext->GetID()).c_str(), 2, tableFlags, ImVec2(0.0f, 0.0f)))
		{

			ImGui::TableSetupColumn("Outliner", 0,ImGui::GetContentRegionAvail().x/2);
			ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			ImGui::BeginChild("##BindingDetails");
			{
				bool bindings = UI::AttributeTreeNode("Bindings");

				{
					ImGui::SameLine();

					if (UI::ImageButton(EditorResources::AddIcon, { 25,25 }))
					{
						ImGui::OpenPopup("InputBindingAddInputAction");
					}

					ImGui::SameLine();

					if (UI::ImageButton(EditorResources::DeleteIcon, { 25,25 }))
					{
						m_BindingContext->RemoveAllKeys();
					}

					AssetID id = AssetID(0);
					UI::Widgets::AssetSearchPopup("InputBindingAddInputAction", AssetType::InputAction, id,UI::UIMemoryAssetTypes::None);

					if (id != 0)
					{
						if (AssetManager::HasAsset(id) && AssetManager::GetAssetInfo(id).Type == AssetType::InputAction)
						{
							m_BindingContext->AddOrGetBinding(AssetManager::GetAsset<InputAction>(id));
						}
					}
				}

				if (bindings)
				{
					for (uint32_t i = 0; i < m_BindingContext->GetBindings().size(); i++)
					{
						ElevatedActionKeyBinding* elevateInputKey = m_BindingContext->AddOrGetBinding(m_BindingContext->GetBindings()[i].InputAction);
						ViewInputActionKeyBinding(*elevateInputKey,m_SelectedInputBinding);
					}
					UI::EndTreeNode();
				}
			}
			ImGui::EndChild();

			ImGui::TableSetColumnIndex(1);
			PropertyWindow();

			ImGui::EndTable();
		}
		UI::PopID();

	}
	Count<class Asset> InputBindingContextPanel::GetAsset()
	{
		return m_BindingContext;
	}
	void InputBindingContextPanel::PropertyWindow()
	{
		ImGui::BeginChild("##BindingProperty");

		if (m_SelectedInputBinding.SelectedInputKeyBinding.IsValid())
		{
			auto selectedInputKeyBinding = m_SelectedInputBinding.SelectedInputKeyBinding.Lock();
			switch (selectedInputKeyBinding->GetInputKeyBindingType())
			{
				case InputKeyBindingType::Key:
				{
					if (m_SelectedInputBinding.InputBindingBundle)
					{
						UI::Widgets::ElevatedInputKeySearchPopup("KeySearchBindingPopUp", &selectedInputKeyBinding.As<InputKeyBinding>()->InputKey,ElevatedInputKeyFlags::NoFlags,ElevatedInputKeyDeviceType::None,false);

						{
							std::string attributeName;

							if (selectedInputKeyBinding.As<InputKeyBinding>()->InputKey == ElevatedInputKeys::Invalid || selectedInputKeyBinding.As<InputKeyBinding>()->InputKey.GetKeyName().empty())
								attributeName = "Invalid";
							else
								attributeName = fmt::format("{} {}", selectedInputKeyBinding.As<InputKeyBinding>()->InputKey.GetKeyName(), EnumReflection::EnumString(selectedInputKeyBinding.As<InputKeyBinding>()->InputKey.GetDevice()));


							if (UI::AttributeButton("", attributeName))
								ImGui::OpenPopup("KeySearchBindingPopUp");
						}
					}
					else
					{
						UI::Widgets::ElevatedInputKeySearchPopup("KeySearchBindingPopUp", &selectedInputKeyBinding.As<InputKeyBinding>()->InputKey);

						{
							std::string attributeName;

							if (selectedInputKeyBinding.As<InputKeyBinding>()->InputKey == ElevatedInputKeys::Invalid || selectedInputKeyBinding.As<InputKeyBinding>()->InputKey.GetKeyName().empty())
								attributeName = "Invalid";
							else
								attributeName = fmt::format("{} {}", selectedInputKeyBinding.As<InputKeyBinding>()->InputKey.GetKeyName(), EnumReflection::EnumString(selectedInputKeyBinding.As<InputKeyBinding>()->InputKey.GetDevice()));


							if (UI::AttributeButton("", attributeName))
								ImGui::OpenPopup("KeySearchBindingPopUp");
						}
					}
				}
				break;
				case InputKeyBindingType::Bundle:
				{
					UI::AttributeInputText("Name", selectedInputKeyBinding.As<InputKeyBindingBundle>()->Name);
				}
					break;
				default:
					break;
			}

			EditInputInteractions(selectedInputKeyBinding->Interactions);
		}
		
		ImGui::EndChild();

	}
}
