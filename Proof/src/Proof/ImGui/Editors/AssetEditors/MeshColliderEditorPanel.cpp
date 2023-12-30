#include "Proofprch.h"
#include "MeshColliderEditorPanel.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIUtilities.h"
#include "Proof/ImGui/UIHandlers.h"
#include "Proof/Physics/MeshCollider.h"
#include "Proof/Physics/PhysicsMeshCooker.h"
#include "Proof/Physics/PhysicsMeshCache.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/Editors/EditorWorkspace/ViewPortEditorWorkspace.h"
#include "Proof/ImGui/Editors/Panels/DetailsPanel.h"
#include "imgui_internal.h"
namespace Proof
{
	MeshColliderEditorPanel::MeshColliderEditorPanel()
		:AssetEditor("MeshColliderEditorPanel")

	{
		//m_WorldRenderer = Count<WorldRenderer>::Create();
		//m_WorldRenderer->GeneralOptions.ShowPhysicsColliders = WorldRendererOptions::PhysicsColliderView::Normal;
		//m_WorldRenderer->BloomSettings.Enabled = false;
	}
	void MeshColliderEditorPanel::OnUpdate(FrameTime ts)
	{

		if (!m_MeshCollider)return;

		AssetEditor::OnUpdate(ts);
		m_WorkSpaceManager->OnUpdate(ts);
	}
	void MeshColliderEditorPanel::OnImGuiRender()
	{

		UI::PushModified(m_NeedsSaving);
#if 0
		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable
			| ImGuiTableFlags_SizingFixedFit
			| ImGuiTableFlags_BordersInnerV;
		UI::ScopedStyleVar windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::BeginTable((m_TitleAndId + "RenderTable").c_str(), 2, tableFlags, ImVec2(0.0f, 0.0f));
		ImGui::TableSetupColumn((m_TitleAndId + "Settings").c_str(), 0,ImGui::GetContentRegionAvail().x/3);
		ImGui::TableSetupColumn((m_TitleAndId + "ViewPort").c_str(), ImGuiTableColumnFlags_WidthStretch);
		
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		RenderSettingsPanel();
		ImGui::TableSetColumnIndex(1);
		RenderViewPortPanel();

		ImGui::EndTable();
#endif

		static bool detailsOpen = true;
		m_DetailsPanel->OnImGuiRender(m_DetailsPanelName.c_str(), detailsOpen);
		m_WorkSpaceManager->OnImGuiRender();
		UI::PopModified();
	}
	void MeshColliderEditorPanel::OnEvent(Event& e)
	{

		m_DetailsPanel->OnEvent(e);
		m_WorkSpaceManager->OnEvent(e);
	}
	void MeshColliderEditorPanel::Save()
	{
		if (!m_MeshCollider)
			return;

		m_NeedsSaving = false;
		AssetManager::SaveAsset(m_MeshCollider->GetID());
	}
	void MeshColliderEditorPanel::RenderSettingsPanel()
	{
		//UI::ScopedStyleVar windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		//ImGui::BeginChild((m_TitleAndId + "Settings").c_str());
			

		bool hasValidMesh = AssetManager::HasAsset(m_MeshCollider->ColliderMesh);
		if (hasValidMesh)
		{
			const auto& assetInfo = AssetManager::GetAssetInfo(m_MeshCollider->ColliderMesh);
			if (!(assetInfo.Type == AssetType::Mesh || assetInfo.Type == AssetType::DynamicMesh))
				hasValidMesh = false;

		}

		ImGui::TextUnformatted("Mesh");

		UI::BeginPropertyGrid();

		{
			UI::PropertyAssetReferenceSettings settings;
			settings.AssetMemoryTypes = UI::UIMemoryAssetTypes::Default;
			if (UI::AttributeMultiAssetReference("Collider Mesh", { AssetType::Mesh,AssetType::DynamicMesh }, m_MeshCollider->ColliderMesh, settings))
				UpdatePreviewEntity();
		}
		UI::SetTooltip("Specifies what mesh this collider will use.");
		if (hasValidMesh)
		{
			UI::AttributeAssetReference("Material",AssetType::PhysicsMaterial, m_MeshCollider->PhysicsMaterial);
			UI::SetTooltip("Specifies the Physics Material that this collider will use by default. MeshColliderComponents can specify a material that will be used instead of this one.");
		}
		UI::EndPropertyGrid();

		// Collider Settings
		if (hasValidMesh)
		{
			ImGui::TextUnformatted("Collider Settings");

			ImGui::PushID("ColliderSettingsID");
			UI::BeginPropertyGrid();
			UI::AttributeBool("Always Share Shape", m_MeshCollider->AlwaysShareShape, "Forces All entities that use this collider to share the collider data as opposed to making copies of it. (Default: False)");

			m_NeedsCooking |= UI::EnumCombo("Collision Complexity", m_CollisionComplexity);
			m_NeedsCooking |= UI::AttributeDrag("Scale", m_MeshCollider->ColliderScale, 0.1f, 0.0f, 0.0f, "The scale of the collider. This value is a scalar of the entity scale. (Default: [1, 1, 1])");

			UI::EndPropertyGrid();
			ImGui::PopID();
		}

		//cooking settings
		if (hasValidMesh)
		{
			ImGui::TextUnformatted("Cooking Settings");

			{
				ImGui::PushID("CookingSettingsID");
				UI::BeginPropertyGrid();

				m_NeedsCooking |= UI::AttributeBool("Vertex Welding", m_MeshCollider->EnableVertexWelding, "Enables Vertex Welding, which will merge multiple vertices into a single vertex if they are withing a given distance. (Default: True)");
					
				UI::PushItemDisabled(!m_MeshCollider->EnableVertexWelding);
				m_NeedsCooking |= UI::AttributeDrag("Weld Tolerance", m_MeshCollider->VertexWeldTolerance, 0.05f, 0.05f, 1.0f,
					"Weld Tolerance controls how close 2 vertices has to be to each other before they merge. (Default: 0.1)");

				m_MeshCollider->VertexWeldTolerance = glm::max(0.05f, m_MeshCollider->VertexWeldTolerance);
				UI::PopItemDisabled();

				UI::EndPropertyGrid();

				if (m_CollisionComplexity != ECollisionComplexity::UseComplexAsSimple && UI::AttributeTreeNode("Simple Collider Settings"))
				{
					UI::BeginPropertyGrid();
					m_NeedsCooking |= UI::AttributeBool("Check Zero-Area Triangles", m_MeshCollider->CheckZeroAreaTriangles,
						"If enabled, any triangle with an area less than the \"Zero-Area Threshold\" will not be included. (Default: True)");

					UI::PushItemDisabled(!m_MeshCollider->CheckZeroAreaTriangles);
					m_NeedsCooking |= UI::AttributeDrag("Zero-Area Threshold", m_MeshCollider->AreaTestEpsilon,
						0.1f, 0.0f, 0.0f, "Defines the minimum area a triangle can have before getting discarded. (Default: 0.06)");
					UI::PopItemDisabled();

					m_NeedsCooking |= UI::AttributeBool("Shift Vertices to Origin", m_MeshCollider->ShiftVerticesToOrigin,
						"If enabled the vertices will be shifted to be around the origin. Only enable if you encounter issues. (Default: False)");

					UI::EndPropertyGrid();
					UI::EndTreeNode();
				}

				if (m_CollisionComplexity != ECollisionComplexity::UseSimpleAsComplex && UI::AttributeTreeNode("Complex Collider Settings"))
				{
					UI::BeginPropertyGrid();
					m_NeedsCooking |= UI::AttributeBool("Flip Normals", m_MeshCollider->FlipNormals,
						"If enabled the vertices will have their normals flipped. Effectively changing the winding order of the triangles. (Default: False)");
					UI::EndPropertyGrid();
					ImGui::TreePop();
				}

				ImGui::PopID();
			}

#if 0
			// Preview Settings
			{
				ImGui::TextUnformatted("Preview Settings");

				UI::BeginPropertyGrid();
				tabData->NeedsSaving |= UI::Property("Scale", tabData->ColliderAsset->PreviewScale, 0.1f, 0.0f, 0.0f,
					"Modifies the scale of the preview entity. Does NOT affect the collider in any way. (Default: [1, 1, 1])");
				tabData->ColliderEntity.GetComponent<TransformComponent>().Scale = tabData->ColliderAsset->PreviewScale;
				UI::EndPropertyGrid();
			}
#endif
			{
				UI::ScopedStyleVar framePadding(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 5.0f));

				UI::PushItemDisabled(!m_NeedsCooking);
				if (ImGui::Button("Cook Mesh"))
				{
					CookMeshCollider();
					UpdatePreviewEntity();
				}
				UI::SetTooltip("\"Cooks\" this collider asset, meaning it will generate the collider data.");
				UI::PopItemDisabled();

				ImGui::SameLine();

				//UI::BeginDisabled(!tabData->NeedsSaving);
				//if (ImGui::Button("Save"))
				//UI::EndDisabled();
			}
		}
	//	ImGui::EndChild();
		RenderCookingOutput();
	}
	void MeshColliderEditorPanel::RenderViewPortPanel()
	{
#if 0
		UI::ScopedStyleVar windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::BeginChild((m_TitleAndId + "ViewPort").c_str());
		m_ViewPoartSize = ImGui::GetContentRegionAvail();
		m_IsViewportFocused = ImGui::IsWindowFocused();
		if (m_ViewPoartSize.x > 0 && m_ViewPoartSize.y > 0)
		{
			Count<Image2D> finalImage = m_WorldRenderer->GetFinalPassImage();
			if (finalImage)
				UI::Image(finalImage, m_ViewPoartSize, { 0, 1 }, { 1, 0 });
		}
		ImGui::EndChild();
#endif
	}
	void MeshColliderEditorPanel::UpdatePreviewEntity()
	{

		m_World = Count<World>::Create();
		Entity entity = m_World->CreateEntity("PreviewEntity");

		if (AssetManager::HasAsset(m_MeshCollider->ColliderMesh))
		{
			const auto& assetInfo = AssetManager::GetAssetInfo(m_MeshCollider->ColliderMesh);
			if(assetInfo.Type == AssetType::Mesh)
				entity.AddComponent<MeshComponent>().SetMesh(m_MeshCollider->ColliderMesh);
			else
				entity.AddComponent<DynamicMeshComponent>().SetMesh(m_MeshCollider->ColliderMesh);

			entity.AddComponent<MeshColliderComponent>(m_MeshCollider->GetID());
		}

		{
			Entity light = m_World->CreateEntity("Light");

			light.AddComponent<DirectionalLightComponent>().Intensity = 2.5;
			light.GetComponent<TransformComponent>().SetRotationEuler(glm::radians(glm::vec3{ 0.400, 5.400, 0.100 }));
		}

		m_WorkSpaceManager = CreateSpecial<EditorWorkspaceManager>();
		m_DetailsPanelName = fmt::format("Details##MeshCollider: {}", m_MeshCollider->GetID());
		m_ViewportPanelName = fmt::format("Viewport##MeshCollider: {}", m_MeshCollider->GetID());
		ViewPortEditorData editorData;
		editorData.EnableSelection = false;
		auto viewport = m_WorkSpaceManager->AddWorkspace<ViewPortEditorWorkspace>(m_ViewportPanelName.c_str(), true, m_ViewportPanelName, editorData);
		m_WorkSpaceManager->SetWorldContext(m_World);

		viewport->GetWorldRenderer()->DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders = WorldRendererDebugOptions::PhysicsColliderView::Normal;
		m_DetailsPanel = Count<DetailsPanel>::Create(std::bind(&MeshColliderEditorPanel::RenderSettingsPanel,this));
	}
	bool MeshColliderEditorPanel::CookMeshCollider()
	{
		m_MeshCollider->CollisionComplexity = m_CollisionComplexity;

		std::tie(m_LastSimpleCookingResult, m_LastComplexCookingResult) = PhysicsMeshCooker::CookMesh(m_MeshCollider, true);
		if(m_ShowCookingResults)
			m_IsCookingResultsOpen = true;
		m_NeedsCooking = false;

		// If using the default behavior both colliders must successfully cook
		if (m_MeshCollider->CollisionComplexity == ECollisionComplexity::Default && m_LastSimpleCookingResult == CookingResult::Success && m_LastComplexCookingResult == CookingResult::Success)
		{
			return true;
		}

		// If using UseSimpleAsComplex only the simple collider has to cook successfully
		if (m_MeshCollider->CollisionComplexity == ECollisionComplexity::UseSimpleAsComplex && m_LastSimpleCookingResult == CookingResult::Success)
		{
			return true;
		}

		// If using UseComplexAsSimple only the complex collider has to cook successfully
		if (m_MeshCollider->CollisionComplexity == ECollisionComplexity::UseComplexAsSimple && m_LastComplexCookingResult == CookingResult::Success)
		{
			return true;
		}

		return false;
	}
	void MeshColliderEditorPanel::RenderCookingOutput()
	{
		if (m_IsCookingResultsOpen && !ImGui::IsPopupOpen("Mesh Collider Cooking Output"))
		{
			ImGui::OpenPopup("Mesh Collider Cooking Output");
		}

		ImGui::SetNextWindowSize(ImVec2(600, 0));
		
		if (ImGui::BeginPopupModal("Mesh Collider Cooking Output", &m_IsCookingResultsOpen))
		{
		
			const auto& colliderMeshMetadata = AssetManager::GetAssetInfo( m_MeshCollider->ColliderMesh);
			const std::string filename = colliderMeshMetadata.GetName();

			auto getMessageForFailedCookingResult = [](CookingResult cookingResult)
			{
				switch (cookingResult)
				{
				case Proof::CookingResult::ZeroAreaTestFailed: return "Failed to find 4 initial vertices without a small triangle.";
				case Proof::CookingResult::PolygonLimitReached:
					return "Successfully cooked mesh collider, but mesh polygon count exceeds 255. Either reduce the number of polygons(or use a simplified mesh for the collider), or enable the \"Quantize Input\" option.";
				case Proof::CookingResult::LargeTriangle: return "Failed to cook triangle mesh because one or more triangles are too large. Tessellate the mesh to reduce triangle size.";
				case Proof::CookingResult::InvalidMesh: return "Failed to cook mesh because an invalid mesh was provided. Please make sure you provide a valid mesh.";
				case Proof::CookingResult::Failure: return "An unknown error occurred. Please check the output logs.";
				}

				return "";
			};

			// Simple Collider
			if (UI::AttributeTreeNode("Simple Collider"))
			{
				//UI::Fonts::PushFont("Bold");
				if (m_LastSimpleCookingResult == CookingResult::Success)
					ImGui::Text("Successfully cooked simple mesh collider for %s", filename.c_str());
				else
					ImGui::Text("Failed to cook simple mesh collider for %s", filename.c_str());
				//UI::Fonts::PopFont();

				if (m_LastSimpleCookingResult == CookingResult::Success)
				{
					const auto& colliderData = PhysicsMeshCache::GetMeshData(m_MeshCollider);
					ImGui::Text("Submeshes: %d", colliderData.SimpleColliderData.SubMeshes.size());
				}
				else
				{
					const auto physxMessage = PhysicsEngine::GetLastErrorMessage();
					ImGui::TextWrapped("Message: %s\nPhysX Message: %s", getMessageForFailedCookingResult(m_LastSimpleCookingResult), physxMessage.c_str());
				}

				UI::EndTreeNode();
			}

			// Complex Collider
			if (UI::AttributeTreeNode("Complex Collider"))
			{
				//UI::Fonts::PushFont("Large");
				if (m_LastSimpleCookingResult == CookingResult::Success)
					ImGui::Text("Successfully cooked complex mesh collider for %s", filename.c_str());
				else
					ImGui::Text("Failed to cook complex mesh collider for %s", filename.c_str());
				//UI::Fonts::PopFont();

				if (m_LastComplexCookingResult == CookingResult::Success)
				{
					const auto& colliderData = PhysicsMeshCache::GetMeshData(m_MeshCollider);
					ImGui::Text("Submeshes: %d", colliderData.ComplexColliderData.SubMeshes.size());
				}
				else
				{
					const auto physxMessage = PhysicsEngine::GetLastErrorMessage();
					ImGui::TextWrapped("Message: %s\nPhysX Message: %s", getMessageForFailedCookingResult(m_LastComplexCookingResult), physxMessage.c_str());
				}

				UI::EndTreeNode();
			}

			ImGui::EndPopup();
		}
	}
	void MeshColliderEditorPanel::SetDefaultLayout()
	{
#if 1
		ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id);
		ImGui::DockBuilderSetNodeSize(dockspace_id, GetWindowSize());

		auto dock_id_up = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);
		auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.5f, nullptr, &dockspace_id);
		ImGui::DockBuilderDockWindow(m_DetailsPanelName.c_str(), dock_id_up);
		ImGui::DockBuilderDockWindow(m_ViewportPanelName.c_str(), dock_id_down);

		ImGui::DockBuilderFinish(dockspace_id);
#endif

		//ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());
		//ImGuiWindow* window = ImGui::FindWindowByName(m_DetailsPanelName.c_str());
		//if (m_DetailsPanel->GetImGuiWindow())
		//{
		//	ImGui::SetWindowDock(m_DetailsPanel->GetImGuiWindow(), dockspace_id, 0);
		//}
		//auto viewport = m_WorkSpaceManager->GetWorkspace<ViewPortEditorWorkspace>(m_ViewportPanelName.c_str());
		//if(viewport->GetImGuiWindow())
		//	ImGui::SetWindowDock(viewport->GetImGuiWindow(), dockspace_id, 0);
	}
	bool MeshColliderEditorPanel::IsSubWindowsHovered()
	{
		for (auto workspaceData : m_WorkSpaceManager->GetWorkspaceData())
		{
			if (workspaceData.second.EditorWorkspace->IsHovered())
				return true;
		}

		if (m_DetailsPanel->IsHovered())
			return true;

		return false;
	}
	bool MeshColliderEditorPanel::IsSubWindowsFocused()
	{

		for (auto workspaceData : m_WorkSpaceManager->GetWorkspaceData())
		{
			if (workspaceData.second.EditorWorkspace->IsFocused())
				return true;
		}

		if (m_DetailsPanel->IsFocused())
			return true;

		return false;
	}
	void MeshColliderEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (asset->GetAssetType() != AssetType::MeshCollider)
		{
			PF_ENGINE_ERROR("Cannot pass {} Asset to MeshColliderEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
			return;
		}
		m_MeshCollider = asset.As<MeshCollider>();
		m_CollisionComplexity = m_MeshCollider->CollisionComplexity;
		UpdatePreviewEntity();
	}
	
}
