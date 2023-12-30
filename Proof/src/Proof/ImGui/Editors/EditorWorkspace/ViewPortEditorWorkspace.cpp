#include "Proofprch.h"
#include "ViewPortEditorWorkspace.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/ImGui/Editors/EditorResources.h"
#include "Proof/Math/Ray.h"
#include "Proof/Math/BasicCollision.h"

#include "Proof/Scene/Mesh.h"

#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/Input/Input.h"
#include "Proof/Events/KeyEvent.h"
#include <ImGuizmo.h>
namespace Proof
{
	using IconComponents = ComponentGroup < SkyLightComponent, DirectionalLightComponent, PointLightComponent, SpotLightComponent >;

	std::pair<float, float> GetMouseViewportSpace(glm::vec2 viewportBounds[2])
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= viewportBounds[0].x;
		my -= viewportBounds[0].y;
		auto viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
		auto viewportHeight = viewportBounds[1].y - viewportBounds[0].y;
	
		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}

	//static std::pair<float, float> GetMouseViewportSpace(glm::vec2 viewPortSize)
	//{
	//	auto [mx, my] = ImGui::GetMousePos();
	//	auto viewportWidth = viewPortSize.x;
	//	auto viewportHeight = viewPortSize.y;
	//
	//	return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	//}

	static Ray CastRay(const EditorCamera& camera, float mx, float my)
	{
		glm::vec4 mouseClipPos = { mx, my, -1.0f, 1.0f };

		auto inverseProj = glm::inverse(camera.GetProjectionMatrix());
		auto inverseView = glm::inverse(glm::mat3(camera.GetViewMatrix()));

		glm::vec4 ray = inverseProj * mouseClipPos;
		glm::vec3 rayPos = camera.GetPosition();
		glm::vec3 rayDir = inverseView * glm::vec3(ray);

		return Ray{ rayPos, rayDir };
	}

	static bool TollbarButton(Count<Texture2D> icon, const ImColor& borderTint = ImColor(0.0f, 0.0f, 0.0f, 0.0f), float paddingY = 0.0f)
	{
#if 0
		const float edgeOffset = 4.0f;

		UI::ScopedStyleVar enableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(edgeOffset * 2.0f, 0));
		const float buttonSize = 18.0f;
		const float height = std::min((float)icon->GetHeight(), buttonSize) - paddingY * 2.0f;
		const float width = (float)icon->GetWidth() / (float)icon->GetHeight() * height;
		const bool clicked = ImGui::InvisibleButton(UI::GenerateID(), ImVec2(width, height));
		UI::DrawButtonImage(icon,
			tint,
			tint,
			tint,
			UI::RectOffset(UI::GetItemRect(), 0.0f, paddingY));

		return clicked;
#endif
		const float edgeOffset = 4.0f;

		UI::ScopedStyleVar enableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(edgeOffset , 0));
		const float buttonSize = 18.0f;
		const float height = std::min((float)icon->GetHeight(), buttonSize) - paddingY * 2.0f;
		const float width = (float)icon->GetWidth() / (float)icon->GetHeight() * height;

		//static ImColor borderColor(15, 15, 15, 127);
		static ImColor borderColor(0,0,0,0);

		// Use ImGui::ImageButton instead of ImGui::InvisibleButton

		/*
		ImGui::Image (
			(ImTextureID)UI::GetTextureID(icon->GetImage()),  // Assuming icon is an instance of an image class with a texture ID
			ImVec2(width, height),
			ImVec2(0, 0),  // UV min
			ImVec2(1, 1),  // UV max
			tint,
			borderColor.Value
		);
		*/
		ImGui::Image(
			(ImTextureID)UI::GetTextureID(icon->GetImage()),  // Assuming icon is an instance of an image class with a texture ID
			ImVec2(width, height),
			ImVec2(0, 0),  // UV min
			ImVec2(1, 1),  // UV max
			ImVec4(1,1,1,1),
			borderTint
		);
		if (ImGui::IsItemClicked())
			return true;
		return false;
	}
	enum class UIToolbarAlign
	{
		Left =0,
		Center,
		Right
	};

	struct ToolBarWindowSettings
	{
		float Width;
		float xOffset;
	};
	static ToolBarWindowSettings BeginTollBarWindow(const std::string& windowName,uint32_t nButtons, UIToolbarAlign align = UIToolbarAlign::Center,float xOffset = 0)
	{
		UI::PushID();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImVec2 windowPos = ImGui::GetWindowPos();

		const float buttonSize = 18.0f;
		const float edgeOffset = 4.0f;
		const float windowHeight = 32.0f;
		const float numberOfButtons = static_cast<float>(nButtons);
		const float backgroundWidth = edgeOffset * 6.0f + buttonSize * numberOfButtons + edgeOffset * (numberOfButtons - 1.0f) * 2.0f;

		float xPosition = windowPos.x + edgeOffset;

		if (align == UIToolbarAlign::Center)
		{
			xPosition += (ImGui::GetContentRegionAvail().x - backgroundWidth) / 2.0f;
		}
		else if (align == UIToolbarAlign::Right)
		{
			xPosition += ImGui::GetContentRegionAvail().x - backgroundWidth - edgeOffset;
		}

		xPosition += xOffset;

		ImGui::SetNextWindowPos(ImVec2(xPosition, windowPos.y + edgeOffset));
		ImGui::SetNextWindowSize(ImVec2(backgroundWidth, windowHeight));
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::Begin(windowName.c_str(), 0,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);

		// Background
		const float desiredHeight = 26.0f;
		ImRect background = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, -(windowHeight - desiredHeight) / 2.0f);
		ImGui::GetWindowDrawList()->AddRectFilled(background.Min, background.Max, IM_COL32(15, 15, 15, 127), 4.0f);

		ImGui::BeginVertical(UI::GenerateID(), { backgroundWidth, ImGui::GetContentRegionAvail().y });
		ImGui::Spring();
		ImGui::BeginHorizontal(UI::GenerateID(), { backgroundWidth, ImGui::GetContentRegionAvail().y });
		ImGui::Spring();

		return { backgroundWidth,xOffset };
	}
	static void EndToolbarWindow()
	{
		ImGui::Spring();
		ImGui::EndHorizontal();
		ImGui::Spring();
		ImGui::EndVertical();

		ImGui::End();
		ImGui::PopStyleVar(4);
		UI::PopID();
	}
	ViewPortEditorWorkspace::ViewPortEditorWorkspace(const std::string& viewPortname, ViewPortEditorData viewPortData) :
		EditorWorkspace(viewPortname), 
		m_ViewPortEditorData(viewPortData)
	{
		m_WorldRenderer = Count<WorldRenderer>::Create();
		if (!m_ViewPortEditorData.EnableSelection)
			m_GizmoType = -1;
		else
			m_GizmoType = ImGuizmo::TRANSLATE;
		m_GizmoMode = ImGuizmo::WORLD;
	}
	ViewPortEditorWorkspace::~ViewPortEditorWorkspace()
	{
	}
	void ViewPortEditorWorkspace::SetWorldContext(const Count<class World>& context)
	{
		m_WorldContext = context;
		m_WorldRenderer->SetContext(m_WorldContext);
	}
	void ViewPortEditorWorkspace::OnEvent(class Event& e)
	{
		EventDispatcher dispatcher(e);

		m_Camera.OnEvent(e);
		dispatcher.Dispatch<KeyClickedEvent>(PF_BIND_FN(ViewPortEditorWorkspace::OnKeyClicked));
		dispatcher.Dispatch<MouseButtonClickedEvent>(PF_BIND_FN(ViewPortEditorWorkspace::OnMouseClicked));
	}
	void ViewPortEditorWorkspace::OnUpdate(FrameTime ts)
	{
		m_Camera.SetViewportSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_Camera.SetActive(IsFocused() || IsHovered());
		m_Camera.OnUpdate(ts);
		m_WorldRenderer->SetViewportSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_WorldContext->OnRenderEditor(m_WorldRenderer, ts, m_Camera);
		OnRender2D();
	}
	void ViewPortEditorWorkspace::OnImGuiRender()
	{
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x,viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x,viewportMaxRegion.y + viewportOffset.y };
		auto windowPos = ImGui::GetWindowPos();

		m_ViewPortSize = { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y };
		
		UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		DrawGizmos();
		if (m_ViewPortEditorData.HandleOnImGuiDrop)
			m_ViewPortEditorData.HandleOnImGuiDrop();

		//left toolbar
		{
			auto data = BeginTollBarWindow(("##DropwDown" + m_TitleAndID).c_str(), 1, UIToolbarAlign::Left, 14.0f);
			TollbarButton(EditorResources::DropdownIcon);
			EndToolbarWindow();

			BeginTollBarWindow(("##View" + m_TitleAndID).c_str(), 1, UIToolbarAlign::Left, data.xOffset + data.Width + 14);
			if (TollbarButton(EditorResources::ViewIcon))
				ImGui::OpenPopup(("##" + m_TitleAndID + "ViewSettings").c_str());
			{
				// 30 for each attributes
			//	ImGui::SetNextWindowSize({ 200, 120.0f });
#if 1
				ImGui::SetNextWindowSizeConstraints({ 200, 30 }, { FLT_MAX, FLT_MAX });
				std::string name = ("##" + m_TitleAndID + "ViewSettings").c_str();
				if (UI::BeginPopup(name.c_str(), ImGuiWindowFlags_AlwaysAutoResize))
				{
					UI::BeginPropertyGrid();
					

					UI::AttributeBool("LightGrid", m_WorldRenderer->DebugOptions.LightDebugOptions.ShowLightGrid,
						"Shows How much point and spot light is affecting a certain area");

					UI::EnumCombo("PhysicsColliders", m_WorldRenderer->DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders);

					UI::AttributeBool("ShadowCascades", m_WorldRenderer->DebugOptions.ShadowDebugOptions.ShowCascades);
					UI::AttributeBool("Icons", m_ShowComponentsIcon);

					UI::EndPropertyGrid();

					UI::EndPopup();
				}
#endif
			}
			EndToolbarWindow();
		}

		// right toolbar
		{
			auto data = BeginTollBarWindow(("##Gizmo" + m_TitleAndID).c_str(), 5, UIToolbarAlign::Right, -250.0f);
			const ImColor c_SelectedGizmoButtonColor = UI::Colours::Theme::Accent;
			const ImColor c_UnselectedGizmoButtonColor = ImColor(0,0,0,0);

			ImColor buttonTint = m_GizmoType == -1 ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::PointerIcon, buttonTint))
				m_GizmoType = -1;
			UI::SetTooltip("NoTransformation");

			buttonTint = m_GizmoType == ImGuizmo::OPERATION::TRANSLATE ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::TranslationIcon, buttonTint))
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			UI::SetTooltip("Translate");

			buttonTint = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::RotationIcon, buttonTint))
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			UI::SetTooltip("Rotate");

			buttonTint = m_GizmoType == ImGuizmo::OPERATION::SCALE ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::ScaleIcon, buttonTint))
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			UI::SetTooltip("Scale");
			
			buttonTint = m_GizmoType == ImGuizmo::OPERATION::UNIVERSALV2 ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::UniversalTransformIcon, buttonTint))
				m_GizmoType = ImGuizmo::OPERATION::UNIVERSALV2;
			UI::SetTooltip("Universal Transform");

			EndToolbarWindow();

			BeginTollBarWindow(("##GizmoSpace" + m_TitleAndID).c_str(), 2, UIToolbarAlign::Right, -175);

			buttonTint = m_GizmoMode == ImGuizmo::MODE::WORLD ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::WorldSpaceIcon, buttonTint))
				m_GizmoMode = ImGuizmo::MODE::WORLD;
			UI::SetTooltip("World Space");

			buttonTint = m_GizmoMode == ImGuizmo::MODE::LOCAL ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::LocalSpaceIcon, buttonTint))
				m_GizmoMode = ImGuizmo::MODE::LOCAL;
			UI::SetTooltip("Local Space");

			EndToolbarWindow();

			BeginTollBarWindow(("##Camera" + m_TitleAndID).c_str(), 1, UIToolbarAlign::Right, -125);
			if (TollbarButton(EditorResources::CameraIcon))
			{
				ImGui::OpenPopup(("##" + m_TitleAndID + "CameraSettings").c_str());
			}
			{
				//UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(UI::Colours::Theme::Background, 1.6f).Value);

				ImGui::SetNextWindowSize({ 200, 60.0f });
				if (UI::BeginPopup(("##" + m_TitleAndID+"CameraSettings").c_str(), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
				{

					UI::BeginPropertyGrid();
					float cameraSpeed = m_Camera.GetSpeed();
					UI::AttributeDrag("Speed", cameraSpeed, m_Camera.MIN_SPEED, m_Camera.MAX_SPEED);
					m_Camera.SetSpeed(cameraSpeed);

					float cameraFov = m_Camera.GetFOV();
					UI::AttributeDrag("FieldOfView", cameraFov, m_Camera.MIN_FOV, m_Camera.MIN_FOV);
					m_Camera.SetFOV(cameraFov);

					m_Camera.Recalculate();
					UI::EndPropertyGrid();
					UI::EndPopup();
				}
			}
			EndToolbarWindow();
		}

	}
	bool ViewPortEditorWorkspace::OnKeyClicked(class KeyClickedEvent& e)
	{
		bool control = Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl);
		bool shift = Input::IsKeyPressed(KeyBoardKey::LeftShift) || Input::IsKeyPressed(KeyBoardKey::RightShift);
		//UI::is
		//basically means that m_editor camera is beign used 
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight) == true)
			return false;
		if (m_ViewPortEditorData.EnableSelection)
		{
			switch (e.GetKey())
			{
				case KeyBoardKey::W:
				{
					bool hasSelections;

					if (m_ViewPortEditorData.IsWorld)
						hasSelections = SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0;
					else
						hasSelections = AssetSelectionManager::HasSelections(AssetSelectionContext::Prefab, m_ViewPortEditorData.SelectionContextID);

					if (IsFocused() && hasSelections)
					{
						m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
						return true;
					}
					break;
				}

				case KeyBoardKey::E:
				{
					bool hasSelections;

					if (m_ViewPortEditorData.IsWorld)
						hasSelections = SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0;
					else
						hasSelections = AssetSelectionManager::HasSelections(AssetSelectionContext::Prefab, m_ViewPortEditorData.SelectionContextID);

					if (IsFocused() && hasSelections)
					{
						m_GizmoType = ImGuizmo::OPERATION::ROTATE;
						return true;
					}
					break;
				}
				case KeyBoardKey::R:
				{
					bool hasSelections;

					if (m_ViewPortEditorData.IsWorld)
						hasSelections = SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0;
					else
						hasSelections = AssetSelectionManager::HasSelections(AssetSelectionContext::Prefab, m_ViewPortEditorData.SelectionContextID);

					if (IsFocused() && hasSelections)
					{
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
						return true;
					}
					break;
				}
			}
		}
	}

	AABB GenerateBoundingBox(float iconSize, const glm::vec3& position)
	{
		AABB box;
		// Calculate half of the size
		float halfSize = 0.5f * iconSize;

		// Calculate min and max coordinates based on the cube's position
		box.Min = glm::vec3(halfSize, halfSize, halfSize);
		box.Max = glm::vec3(halfSize, halfSize, halfSize);

		return box;
	}
	struct SelectionData
	{
		Entity Entity;
		float Distance = 0.0f;
	};
	template<typename... Componnent>
	static void CheckBoudningBoxSingle(std::vector<SelectionData>& selectionData, Count<World> world, glm::vec3 origin, glm::vec3 direction)
	{
		([&]()
			{
				auto entities = world->GetAllEntitiesWith<Componnent>();
				for (auto e : entities)
				{
					Entity entity = { e, world.Get() };

					auto transform = world->GetWorldSpaceTransformComponent(entity);
					Ray ray =
					{
						glm::inverse(transform.GetTransform()) * glm::vec4(origin, 1.0f),
						glm::inverse(glm::mat3(transform.GetTransform())) * direction
					};

					float t;
					bool intersects = BasicCollision::RayInAABB(ray, AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Cube).As<Mesh>()->GetMeshSource()->GetBoundingBox(), 0.3, 50.0f, t);
					if (intersects)
					{
						selectionData.push_back(SelectionData{ entity,t });
					}
				}
			}(), ...);
	}

	template<typename... Component>
	static void CheckBoudningBox(ComponentGroup<Component...>, std::vector<SelectionData>& selectionData,Count<World> world,glm::vec3 origin, glm::vec3 directin)
	{
		CheckBoudningBoxSingle<Component...>(selectionData,world,origin,directin);
	}
	bool ViewPortEditorWorkspace::OnMouseClicked(MouseButtonClickedEvent& e)
	{
		if (!m_Camera.IsActive())
			return false;

		if (Input::IsKeyPressed(KeyBoardKey::LeftAlt) || Input::IsMouseButtonPressed(MouseButton::ButtonRight))
			return false;

		if (ImGuizmo::IsOver())
			return false;

		if (e.GetButton() != MouseButton::ButtonLeft)
			return false;
		if (!m_ViewPortEditorData.EnableSelection)
			return false;
		ImGui::ClearActiveID();

		bool state = false;
		std::vector<SelectionData> selectionData;
		//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/e7c349b70bd95af3ab673556cdb56cb2cc40b48e/Engine/X2/EditorLayer.cpp#L3340
		auto [mouseX, mouseY] = GetMouseViewportSpace(m_ViewportBounds);

		if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f)
		{
			const auto& camera = m_Camera;
			auto [origin, direction] = CastRay(camera, mouseX, mouseY);
			if(m_ShowComponentsIcon)
				CheckBoudningBox(IconComponents{}, selectionData, m_WorldContext, origin, direction);

			auto meshEntities = m_WorldContext->GetAllEntitiesWith<DynamicMeshComponent>();
			for (auto e : meshEntities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				auto& mc = entity.GetComponent<DynamicMeshComponent>();
				auto mesh = mc.GetMesh();
				if (!mesh)
					continue;

				auto& submeshes = mesh->GetMeshSource()->GetSubMeshes();
				float lastT = std::numeric_limits<float>::max();
				const auto& subMesh = submeshes[mc.GetSubMeshIndex()];
				glm::mat4 transform = m_WorldContext->GetWorldSpaceTransform(entity);
				Ray ray =
				{
					glm::inverse(transform) * glm::vec4(origin, 1.0f),
					glm::inverse(glm::mat3(transform)) * direction
				};

				float t;
				bool intersects = BasicCollision::RayInAABB(ray, subMesh.BoundingBox, 0.3, 50.0f, t);
				if (intersects)
				{

					selectionData.push_back(SelectionData{ entity,t });

					/*
					const auto& triangleCache = mesh->GetMeshSource()->GetTriangleCache(mc.SubmeshIndex);
					for (const auto& triangle : triangleCache)
					{
						if (ray.Intersects(triangle.V0.Position, triangle.V1.Position, triangle.V2.Position, t))
						{
							selectionData.push_back({ entity, &submesh, t });
							break;
						}
					}
					*/
				}
			}

			auto staticMeshEntities = m_WorldContext->GetAllEntitiesWith<MeshComponent>();
			for (auto e : staticMeshEntities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				auto& smc = entity.GetComponent<MeshComponent>();
				auto staticMesh = smc.GetMesh();
				if (!staticMesh)
					continue;

				auto& submeshes = staticMesh->GetMeshSource()->GetSubMeshes();
				glm::mat4 transform = m_WorldContext->GetWorldSpaceTransform(entity);
				Ray ray =
				{
					glm::inverse(transform) * glm::vec4(origin, 1.0f),
					glm::inverse(glm::mat3(transform)) * direction
				};
				float t;
				bool intersects = BasicCollision::RayInAABB(ray, staticMesh->GetMeshSource()->GetBoundingBox(), 0.3, 50.0, t);

				if (intersects)
				{
					selectionData.push_back(SelectionData{ entity, t });
				}
# if 0
				float lastT = std::numeric_limits<float>::max();
				for (uint32_t i = 0; i < submeshes.size(); i++)
				{
					auto& subMesh = submeshes[i];
					glm::mat4 transform = m_ActiveWorld->GetWorldSpaceTransform(entity);
					Ray ray =
					{
						glm::inverse(transform) * glm::vec4(origin, 1.0f),
						glm::inverse(glm::mat3(transform)) * direction
					};

					float t;
					bool intersects = BasicCollision::RayInAABB(ray, subMesh.BoundingBox, 0.3, 50.0, t);
					if (intersects)
					{
						selectionData.push_back(SelectionData{ entity, (SubMesh*)&subMesh, t });

						/*
						const auto& triangleCache = staticMesh->GetMeshSource()->GetTriangleCache(i);
						for (const auto& triangle : triangleCache)
						{
							if (ray.Intersects(triangle.V0.Position, triangle.V1.Position, triangle.V2.Position, t))
							{
								selectionData.push_back({ entity, &submesh, t });
								break;
							}
						}
						*/
#endif
			}

			std::sort(selectionData.begin(), selectionData.end(), [](auto& a, auto& b) { return a.Distance > b.Distance; });

			bool ctrlDown = Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl);
			bool shiftDown = Input::IsKeyPressed(KeyBoardKey::LeftShift) || Input::IsKeyPressed(KeyBoardKey::RightShift);
			if (!ctrlDown)
			{
				if(m_ViewPortEditorData.IsWorld)
					SelectionManager::DeselectAll();
				else
					AssetSelectionManager::DeselectAll(AssetSelectionContext::Prefab, m_ViewPortEditorData.SelectionContextID);

			}

			if (!selectionData.empty())
			{
				Entity entity = selectionData.front().Entity;
				if (shiftDown)
				{
					while (entity.GetParent())
					{
						entity = entity.GetParent();
					}
				}
				
				if (m_ViewPortEditorData.IsWorld)
				{
					if (SelectionManager::IsSelected(SelectionContext::Scene, entity.GetUUID()) && ctrlDown)
						SelectionManager::Deselect(SelectionContext::Scene, entity.GetUUID());
					else
						SelectionManager::Select(SelectionContext::Scene, entity.GetUUID());
				}
				else
				{

					if (AssetSelectionManager::IsSelected(AssetSelectionContext::Prefab,m_ViewPortEditorData.SelectionContextID, entity.GetUUID()) && ctrlDown)
						AssetSelectionManager::Deselect(AssetSelectionContext::Prefab, m_ViewPortEditorData.SelectionContextID, entity.GetUUID());
					else
						AssetSelectionManager::Select(AssetSelectionContext::Prefab, m_ViewPortEditorData.SelectionContextID, entity.GetUUID());
				}


				state = true;
			}
		}

		return state;
	}
	void ViewPortEditorWorkspace::OnRender2D()
	{
		Count<Renderer2D> renderer2D =  m_WorldRenderer->GetRenderer2D();
		renderer2D->SetTargetFrameBuffer(m_WorldRenderer->GetExternalCompositePassFrameBuffer());
		
		renderer2D->BeginContext(m_Camera.GetProjectionMatrix(), m_Camera.GetViewMatrix(), GlmVecToProof(m_Camera.GetPosition()));
		DrawIcons();
		DrawBoundingBoxes();
		renderer2D->EndContext();

	}

	void ViewPortEditorWorkspace::DrawIcons()
	{
		Count<Renderer2D> renderer2D = m_WorldRenderer->GetRenderer2D();
		if (!m_ShowComponentsIcon)
			return;

		{
			auto entities = m_WorldContext->GetAllEntitiesWith<SkyLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::SkyLightIcon, m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}

		{
			auto entities = m_WorldContext->GetAllEntitiesWith<DirectionalLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::DirectionalLightIcon, m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}

		{
			auto entities = m_WorldContext->GetAllEntitiesWith<PointLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::PointLightIcon, m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}
		{
			auto entities = m_WorldContext->GetAllEntitiesWith<SpotLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::SpotLightIcon, m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}
	}

	void ViewPortEditorWorkspace::DrawBoundingBoxes()
	{
		if (!m_ShowBoundingBoxes)
			return;

		Count<Renderer2D> renderer2D = m_WorldRenderer->GetRenderer2D();

		auto meshEntities = m_WorldContext->GetAllEntitiesWith<MeshComponent>();
		for (auto e : meshEntities)
		{
			Entity entity = { e, m_WorldContext.Get() };
			auto& mc = entity.GetComponent<MeshComponent>();
			auto mesh = mc.GetMesh();
			if (!mesh)
				continue;

			renderer2D->DrawAABB(mesh, m_WorldContext->GetWorldSpaceTransform(entity), m_BoundingBoxColor);
		}

		auto dynamicMeshEntities = m_WorldContext->GetAllEntitiesWith<DynamicMeshComponent>();
		for (auto e : dynamicMeshEntities)
		{
			Entity entity = { e, m_WorldContext.Get() };
			auto& mc = entity.GetComponent<DynamicMeshComponent>();
			auto mesh = mc.GetMesh();
			if (!mesh)
				continue;

			auto& submeshes = mesh->GetMeshSource()->GetSubMeshes();
			const auto& subMesh = submeshes[mc.GetSubMeshIndex()];
			glm::mat4 transform = m_WorldContext->GetWorldSpaceTransform(entity);

			renderer2D->DrawAABB(subMesh, transform, m_BoundingBoxColor);
		}
	}

	float ViewPortEditorWorkspace::GetSnapValue()
	{
		switch (m_GizmoType)
		{
			case  ImGuizmo::OPERATION::TRANSLATE: return 0.5f;
			case  ImGuizmo::OPERATION::ROTATE: return 45.0f;
			case  ImGuizmo::OPERATION::SCALE: return 0.5f;
		}
		return 0.0f;
	}

	void ViewPortEditorWorkspace::OnWindowStylePush()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	}
	void ViewPortEditorWorkspace::OnWindowStylePop()
	{
		ImGui::PopStyleVar();
	}

	void ViewPortEditorWorkspace::DrawGizmos()
	{
		if (!IsHovered() || !IsFocused())
			return;

		Entity selectedEntity;
		if (m_ViewPortEditorData.IsWorld)
		{

			if (SelectionManager::GetSelections(SelectionContext::Scene).size() > 0)
				selectedEntity = m_WorldContext->GetEntity(SelectionManager::GetSelections(SelectionContext::Scene).front());
		}
		else
		{
			if(AssetSelectionManager::HasSelections(AssetSelectionContext::Prefab,m_ViewPortEditorData.SelectionContextID))
				selectedEntity = m_WorldContext->GetEntity(AssetSelectionManager::GetSelections(AssetSelectionContext::Prefab, m_ViewPortEditorData.SelectionContextID).front());
		}

		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			const glm::mat4& cameraProjection = m_Camera.GetProjectionMatrix();
			glm::mat4 cameraView = m_Camera.GetViewMatrix();

			auto& selectedentityTc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 selectedEntitytransform = selectedentityTc.GetTransform();

			bool snap = ImGui::IsKeyPressed(ImGuiKey_LeftCtrl);
			float snapValue = GetSnapValue(); // Snap to 0.5m for translation/scale

			float snapValues[3] = { snapValue,snapValue,snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, (ImGuizmo::MODE)m_GizmoMode, glm::value_ptr(selectedEntitytransform),
				nullptr, snap ? snapValues : nullptr);
			if (ImGuizmo::IsUsing())
			{
				Entity parent = m_WorldContext->TryGetEntityWithUUID(selectedEntity.GetParentUUID());

				if (parent)
				{
					glm::mat4 parentTransform = m_WorldContext->GetWorldSpaceTransform(parent);
					selectedEntitytransform = glm::inverse(parentTransform) * selectedEntitytransform;
					glm::vec3 translation, rotation, scale;
					MathResource::DecomposeTransform(selectedEntitytransform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - selectedentityTc.GetRotationEuler();
					selectedentityTc.Location = translation;
					selectedentityTc.SetRotation(selectedentityTc.GetRotationEuler() += deltaRotation);
					selectedentityTc.Scale = scale;

				}
				else
				{
					glm::vec3 translation, rotation, scale;
					MathResource::DecomposeTransform(selectedEntitytransform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - selectedentityTc.GetRotationEuler();
					selectedentityTc.Location = translation;
					selectedentityTc.SetRotation(selectedentityTc.GetRotationEuler() += deltaRotation);
					selectedentityTc.Scale = scale;
				}
			}

		}
	}
}
