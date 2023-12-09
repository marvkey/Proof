#include "Proofprch.h"
#include "ProjectSettingsPanel.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIUtilities.h"
#include "Proof/ImGui/UIHandlers.h"
#include "Proof/Project/Project.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/ImGui/UIColors.h"
namespace Proof
{
	ProjectSettingsPanel::ProjectSettingsPanel()
	{
		m_Project = Project::GetActive();
	}
	ProjectSettingsPanel::~ProjectSettingsPanel()
	{
	}
	void ProjectSettingsPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen)
	{
		if (ImGui::Begin(dsiplayName, &isOpen))
		{
			RenderGeneralSettings();
			RenderPhysicsSettings();
			ImGui::End();
		}
	}
	void ProjectSettingsPanel::RenderGeneralSettings()
	{
		UI::ScopedID scopedID("RenderGeneralSettings");
		
		if (UI::AttributeTreeNode("General"))
		{
			UI::BeginPropertyGrid();

			{

				UI::AttributeTextBar("Name", m_Project->GetProjectName());
				UI::AttributeTextBar("Project Directory", m_Project->GetProjectDirectory().string());
				UI::AttributeTextBar("Asset Directory", m_Project->GetAssetDirectory().string());
			}
			{
				AssetID id = m_Project->GetConfig().StartWorld;
				UI::AttributeAssetReference("StartWorld", AssetType::World, id);
				m_Project->m_ProjectConfig.StartWorld = id.Get();
			}
			{
				UI::AttributeBool("OnCloseStartWorldEditorLastOpen", m_Project->m_ProjectConfig.OnCloseStartWorldEditLastOpen);
				if (!m_Project->m_ProjectConfig.OnCloseStartWorldEditLastOpen)
				{

					AssetID id = m_Project->GetConfig().StartWorldEdit;
					UI::AttributeAssetReference("StartWorldEditor", AssetType::World, id);
					m_Project->m_ProjectConfig.StartWorldEdit = id.Get();
				}
			}
			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}
	}
	void ProjectSettingsPanel::RenderPhysicsSettings()
	{
		UI::ScopedID scopedID("RenderPhysicsSettings");

		if (UI::AttributeTreeNode("Physics"))
		{
			UI::BeginPropertyGrid();
			PhysicsSettings& physicsSettings = PhysicsEngine::GetSettings();

			UI::AttributeDrag("Gravity", physicsSettings.Gravity);
			UI::AttributeDrag("BounceThresholdVelocity", physicsSettings.BounceThresholdVelocity,0.25,0);
			UI::AttributeDrag("SleepThreshold", physicsSettings.SleepThreshold, 0.1, 0);
			UI::EnumCombo("BroadPhase Type", physicsSettings.BroadPhaseType);

			if (physicsSettings.BroadPhaseType != BroadphaseType::AutomaticBoxPrune)
			{
				UI::AttributeDrag("World Bounds (Min)", physicsSettings.WorldBoundsMin);
				UI::AttributeDrag("World Bounds (Max)", physicsSettings.WorldBoundsMax);
				UI::AttributeSlider("Grid Subdivisions", physicsSettings.WorldBoundsSubdivisions, 1, 10000);
			}

			UI::EnumCombo("Friction Model", physicsSettings.FrictionModel);
			UI::AttributeSlider("Solver Iterations", physicsSettings.SolverIterations, 1, 512);
			UI::AttributeSlider("Solver Velocity Iterations", physicsSettings.SolverVelocityIterations, 1, 512);

			UI::EndPropertyGrid();

			ImGui::Separator();

			ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable
				| ImGuiTableFlags_SizingFixedFit
				| ImGuiTableFlags_BordersInnerV;
			UI::PushID();
			if (ImGui::BeginTable("PhysicsLayerManager", 2, tableFlags, ImVec2(0.0f, 0.0f)))
			{

				ImGui::TableSetupColumn("LayersView", 0, 400.0f);
				ImGui::TableSetupColumn("LayersEditor", ImGuiTableColumnFlags_WidthStretch);


				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				auto singleColumnSeparator = []
				{
					ImDrawList* draw_list = ImGui::GetWindowDrawList();
					ImVec2 p = ImGui::GetCursorScreenPos();
					draw_list->AddLine(ImVec2(p.x - 9999, p.y), ImVec2(p.x + 9999, p.y), ImGui::GetColorU32(ImGuiCol_Border));
				};
				auto GetUniqueName = []()
				{
					int counter = 0;
					auto checkID = [&counter](auto checkID) -> std::string
					{
						++counter;
						const std::string counterStr = [&counter] {
							if (counter < 10)
								return "0" + std::to_string(counter);
							else
								return std::to_string(counter);
						}();

						std::string idstr = "NewLayer_" + counterStr;
						if (PhysicsLayerManager::IsLayerValid(idstr))
							return checkID(checkID);
						else
							return idstr;
					};

					return checkID(checkID);
				};
				int32_t layerToDelete = -1;

				bool renameSelectedLayer = false;
				if (ImGui::Button("New Layer", { 80.0f, 28.0f }))
				{
					std::string name = GetUniqueName();
					m_PhysicsSelectedLayer = PhysicsLayerManager::AddLayer(name);
					renameSelectedLayer = true;
				}
				ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 100.0f), ImVec2(9999.0f, 9999.0f));
				{

					UI::ScopedStyleColor scopedChildBg(ImGuiCol_ChildBg, {0,0,0,1});
					UI::ScopedStyleColor scopedColBorder(ImGuiCol_Border, { 0,0,0,1 });
					ImGui::BeginChild("LayersList");
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.0f, 0.0f });

					for (auto& layer : PhysicsLayerManager::GetLayers())
					{
						UI::ScopedID scopelisId(layer.Name.c_str());
						ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
						bool selected = m_PhysicsSelectedLayer == layer.LayerID;

						if (selected)
						{
							ImGui::PushStyleColor(ImGuiCol_Text, UI::Colours::Theme::BackgroundDark);
							flags |= ImGuiTreeNodeFlags_Selected;
						}

						if (ImGui::TreeNodeEx(layer.Name.c_str(), flags))
							ImGui::TreePop();


						bool itemClicked = ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None) &&
							ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 1.0f).x == 0.0f &&
							ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 1.0f).y == 0.0f;

						if (itemClicked)
							m_PhysicsSelectedLayer = layer.LayerID;

						if (selected)
							ImGui::PopStyleColor();

						if (layer.LayerID > 0 && ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Rename", "F2"))
							{
								m_PhysicsSelectedLayer = layer.LayerID;
								renameSelectedLayer = true;
							}

							if (ImGui::MenuItem("Delete", "Delete"))
							{
								if (selected)
									m_PhysicsSelectedLayer = 0;

								layerToDelete = layer.LayerID;
							}

							ImGui::EndPopup();
						}
					}
					ImGui::PopStyleVar();
					ImGui::EndChild();
				}

				ImGui::TableSetColumnIndex(1);

				if (PhysicsLayerManager::IsLayerValid(m_PhysicsSelectedLayer))
				{
					PhysicsLayer& currentLayerInfo = PhysicsLayerManager::GetLayer(m_PhysicsSelectedLayer);

					auto propertyGridSpacing = []
					{
						ImGui::Spacing();
						ImGui::NextColumn();
						ImGui::NextColumn();
					};

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					singleColumnSeparator();

					ImGui::Spacing();
					ImGui::Spacing();

					static std::string stringBuffer;
					stringBuffer = currentLayerInfo.Name;
					ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.08f,0.08f,0.08f,1.0f });
					UI::ShiftCursorY(3.0f);
					ImGuiInputTextFlags text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;
					if (currentLayerInfo.Name == "Default")
						text_flags |= ImGuiInputTextFlags_ReadOnly;
					if (UI::AttributeInputText("Layer Name: ", stringBuffer, text_flags))
					{
						PhysicsLayerManager::UpdateLayerName(currentLayerInfo.LayerID, stringBuffer);
					}
					UI::ShiftCursorY(3.0f);
					ImGui::PopStyleColor();

					const float width = ImGui::GetColumnWidth();
					const float borderOffset = 7.0f;
					const float bottomAreaHeight = 50.0f;
					auto layersBounds = ImGui::GetContentRegionAvail();
					layersBounds.x = width - borderOffset;
					//layersBounds.y -= bottomAreaHeight;

					ImGui::BeginChild("Collides With", layersBounds, false);
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 1.0f });
					ImGui::Dummy({ width, 0.0f }); // 1px space offset

					UI::BeginPropertyGrid();
					for (const auto& layer : PhysicsLayerManager::GetLayers())
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.0f); // adding 1px "border"
						bool shouldCollide = PhysicsLayerManager::ShouldCollide(layer.LayerID, currentLayerInfo.LayerID);
						if (UI::AttributeBool(layer.Name.c_str(), shouldCollide))
						{
							PhysicsLayerManager::SetLayerCollision(currentLayerInfo.LayerID, layer.LayerID, shouldCollide);
						}
					}
					UI::EndPropertyGrid();
					ImGui::PopStyleVar();
					ImGui::EndChild();
				}
				else
				{
					m_PhysicsSelectedLayer = 0;
				}
				if (layerToDelete > 0)
					PhysicsLayerManager::RemoveLayer(layerToDelete);

				ImGui::EndTable();
			}
			UI::PopID();
			UI::EndTreeNode();
		}
	}
}
