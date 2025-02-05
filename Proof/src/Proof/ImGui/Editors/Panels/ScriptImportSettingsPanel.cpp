#include "Proofprch.h"
#include "Proof/ImGui/UI.h"
#include "ScriptImportSettingsPanel.h"
#include "Proof/Scripting/ScriptImportSettings.h"
#include "Proof/Scripting/ScriptField.h"
#include "Proof/Scripting/ScriptRegistry.h"
#include "Proof/Scripting/ScriptTypes.h"
#include "Proof/Asset/AssetCustomData/AssetCustomDataManager.h"
#include "Proof/ImGui/UIWidgets.h"
namespace Proof
{
	void ScriptSettingsPanelPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen)
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_MenuBar;
		if (m_Modified)
			window_flags |= ImGuiWindowFlags_UnsavedDocument;

		UI::PushModified(m_Modified);
		
		if (ImGui::Begin(dsiplayName, &isOpen, window_flags))
		{
			ImGui::BeginMenuBar();
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save", "ctrl+s"))
					{
						m_Modified = false;
						AssetCustomDataManager::SaveAssetCustomData(AssetCustomDataType::ScriptImportSettings);
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndMenuBar();

			static bool grabFocus = true;
			bool searchModified = UI::Widgets::SearchWidget<512,std::string>(m_SearchString, "ClassName", &grabFocus);

			for (auto& [className, classMetaData] : ScriptImportSettings::s_Classes)
			{
				if (m_SearchString.empty() == false)
				{
					if (!UI::IsMatchingSearch(className, m_SearchString))
					{
						continue;
					}
					else
						ImGui::SetNextItemOpen(true);
				}
				UI::PushID();

				if(searchModified && m_SearchString.empty())
					ImGui::SetNextItemOpen(false);

				if (UI::AttributeTreeNode(className, false, 6.0f, 3))
				{
					UI::BeginPropertyGrid();
					// doing this to have in order how they are listed in the script
					ManagedClass* managedClass = ScriptRegistry::GetManagedClassByName(classMetaData.className);
					if (managedClass != nullptr)
					{
						for (const auto& fieldName : managedClass->Fields)
						{
							if (!classMetaData.Fields.contains(fieldName))
								continue;

							Count<FieldStorageBase> field = classMetaData.Fields.at(fieldName);
							std::string actualFieldName = field->GetFieldInfo()->DisplayName.empty() ? Utils::String::SubStr(field->GetFieldInfo()->Name, field->GetFieldInfo()->Name.find(':') + 1) : field->GetFieldInfo()->DisplayName;

							// the draw field value is null as because
							// it would need if we are using entities but this does not set entities
							if (field->GetFieldInfo()->IsArray())
							{
								Count<ArrayFieldStorage> storage = field.As<ArrayFieldStorage>();
								UI::DrawFieldValue(nullptr, actualFieldName, storage);
							}
							else if (field->GetFieldInfo()->IsEnum())
							{
								Count<EnumFieldStorage> storage = field.As<EnumFieldStorage>();
								UI::DrawFieldValue(nullptr, actualFieldName, storage);
							}
							else
							{
								Count<FieldStorage> storage = field.As<FieldStorage>();
								UI::DrawFieldValue(nullptr, actualFieldName, storage);
							}
						}

					}

					UI::EndPropertyGrid();

					UI::EndTreeNode();
				}

				UI::PopID();
			}
			ImGui::End();
		}
		UI::PopModified();
	}
}
