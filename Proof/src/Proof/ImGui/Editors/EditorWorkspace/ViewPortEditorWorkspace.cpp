#include "Proofprch.h"
#include "ViewPortEditorWorkspace.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/ImGui/Editors/EditorResources.h"

#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/Input/Input.h"
#include "Proof/Events/KeyEvent.h"
#include <ImGuizmo.h>
namespace Proof
{
	ViewPortEditorWorkspace::ViewPortEditorWorkspace(const std::string& viewPortname, ViewPortEditorData viewPortData) :
		EditorWorkspace(viewPortname), 
		m_ViewPortEditorData(viewPortData)
	{
		m_WorldRenderer = Count<WorldRenderer>::Create();
		m_GizmoType = ImGuizmo::TRANSLATE;
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
		ImVec2 currentviewPortPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *((glm::vec2*)&currentviewPortPanelSize))
		{
			m_ViewPortSize = { currentviewPortPanelSize.x,currentviewPortPanelSize.y };
		}
		UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 }); 
		DrawGizmos();
		if (m_ViewPortEditorData.HandleOnImGuiDrop)
			m_ViewPortEditorData.HandleOnImGuiDrop();

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
			case KeyBoardKey::Q:
			{
				if (IsFocused() && SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
				{
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
					return true;
				}
				break;
			}

			case KeyBoardKey::W:
			{
				if (IsFocused() && SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
				{
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
					return true;
				}
				break;
			}
			case KeyBoardKey::E:
			{
				if (IsFocused() && SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
				{
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
					return true;
				}
				break;
			}

			case KeyBoardKey::R:
			{
				if (IsFocused() && SelectionManager::GetSelectionCount(SelectionContext::Scene) > 0)
				{
					m_GizmoType = ImGuizmo::OPERATION::UNIVERSALV2;
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
				renderer2D->DrawQuadBillboard(EditorResources::SkyLightIcon,m_WorldContext->GetWorldSpaceLocation(entity), { glm::radians(90.f),0,0 });
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
				renderer2D->DrawQuadBillboard(EditorResources::PointLightIcon,m_WorldContext->GetWorldSpaceLocation(entity),{glm::radians(90.f),0,0});
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

		if (selectedEntity)
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
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(selectedEntitytransform),
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
