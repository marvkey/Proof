#include "Proofprch.h"
#include "ViewPortEditorWorkspace.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/ImGui/Editors/EditorResources.h"

#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/Input/Input.h"
#include "Proof/Events/KeyEvent.h"
#include <ImGuizmo.h>
namespace Proof
{

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

		ImVec2 currentviewPortPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *((glm::vec2*)&currentviewPortPanelSize))
		{
			m_ViewPortSize = { currentviewPortPanelSize.x,currentviewPortPanelSize.y };
		}
		
		UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		DrawGizmos();
		if (m_ViewPortEditorData.HandleOnImGuiDrop)
			m_ViewPortEditorData.HandleOnImGuiDrop();

		//left toolbar
		{
			auto data = BeginTollBarWindow("##DropwDown", 1, UIToolbarAlign::Left, 14.0f);
			TollbarButton(EditorResources::DropdownIcon);
			EndToolbarWindow();

			//BeginTollBarWindow("##DropwDown2", 1, UIToolbarAlign::Left, data.xOffset + data.Width + 14);
			//TollbarButton(EditorResources::DropdownIcon);
			//EndToolbarWindow();
		}

		// right toolbar
		{
			auto data = BeginTollBarWindow("##Gizmo", 5, UIToolbarAlign::Right, -250.0f);
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

			BeginTollBarWindow("##GizmoSpace", 2, UIToolbarAlign::Right, -175);

			buttonTint = m_GizmoMode == ImGuizmo::MODE::WORLD ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::WorldSpaceIcon, buttonTint))
				m_GizmoMode = ImGuizmo::MODE::WORLD;
			UI::SetTooltip("World Space");

			buttonTint = m_GizmoMode == ImGuizmo::MODE::LOCAL ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
			if (TollbarButton(EditorResources::LocalSpaceIcon, buttonTint))
				m_GizmoMode = ImGuizmo::MODE::LOCAL;
			UI::SetTooltip("Local Space");

			EndToolbarWindow();

			BeginTollBarWindow("##Camera", 1, UIToolbarAlign::Right, -125);
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
		switch (e.GetKey())
		{
			case KeyBoardKey::W:
			{
				if (IsFocused() && SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
				{
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
					return true;
				}
				break;
			}

			case KeyBoardKey::E:
			{
				if (IsFocused() && SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
				{
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
					return true;
				}
				break;
			}
			case KeyBoardKey::R:
			{
				if (IsFocused() && SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
				{
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
					return true;
				}
				break;
			}
		}
	}

	void ViewPortEditorWorkspace::OnRender2D()
	{
		Count<Renderer2D> renderer2D =  m_WorldRenderer->GetRenderer2D();
		renderer2D->SetTargetFrameBuffer(m_WorldRenderer->GetExternalCompositePassFrameBuffer());
		
		renderer2D->BeginContext(m_Camera.GetProjectionMatrix(), m_Camera.GetViewMatrix(), GlmVecToProof(m_Camera.GetPosition()));
		{
			auto entities = m_WorldContext->GetAllEntitiesWith<SkyLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::SkyLightIcon,m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}

		{
			auto entities = m_WorldContext->GetAllEntitiesWith<DirectionalLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::DirectionalLightIcon,m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}

		{
			auto entities = m_WorldContext->GetAllEntitiesWith<PointLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::PointLightIcon,m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}
		{
			auto entities = m_WorldContext->GetAllEntitiesWith<SpotLightComponent>();
			for (auto e : entities)
			{
				Entity entity = { e, m_WorldContext.Get() };
				renderer2D->DrawQuadBillboard(EditorResources::SpotLightIcon,m_WorldContext->GetWorldSpaceLocation(entity));
			}
		}
		renderer2D->EndContext();

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
		//if (!IsHovered())
		//	return;

		Entity selectedEntity;
		if (SelectionManager::GetSelections(SelectionContext::Scene).size() > 0)
			selectedEntity = m_WorldContext->GetEntity(SelectionManager::GetSelections(SelectionContext::Scene).front());

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
