#pragma once
#include "EditorWorkspace.h"
#include <glm/glm.hpp>
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/ImGui/SelectionManager.h"
#include <functional>
namespace Proof
{
	struct ViewPortEditorData
	{
		bool CanClose = true;
		bool CanMove = false;
		std::function<void()> HandleOnImGuiDrop = nullptr;
		bool EnableSelection = true;
		bool IsWorld = true;
		//only if world is set to false
		AssetSelectionContext SelectionContext = AssetSelectionContext::Prefab;
		UUID SelectionContextID = 0;
	};
	class ViewPortEditorWorkspace : public EditorWorkspace
	{
	public:
		ViewPortEditorWorkspace(const std::string& viewPortname, ViewPortEditorData viewPortData = {});
		~ViewPortEditorWorkspace();
		virtual void SetWorldContext(const Count<class World>& context);
		virtual void OnImGuiRender();
		virtual void OnUpdate(FrameTime ts);
		virtual void OnEvent(class Event& e);
		Count<WorldRenderer> GetWorldRenderer() { return m_WorldRenderer;}
	protected:
		virtual void OnWindowStylePush();
		virtual void OnWindowStylePop();
	private:
		float GetSnapValue();
		void DrawGizmos();
		bool OnKeyClicked(class KeyClickedEvent& e);
		bool OnMouseClicked(class MouseButtonClickedEvent& e);
		void OnRender2D();
		void DrawIcons();
		void DrawBoundingBoxes();
	private:
		Count<class WorldRenderer> m_WorldRenderer = nullptr;
		Count<class World> m_WorldContext = nullptr;
		class EditorCamera m_Camera;

		bool m_CanClose = false;
		bool m_CanMove = false;
		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_ViewPortSize;
		//GIZMO Tools
		int m_GizmoType; 
		int m_GizmoMode = 0; // 0 = local
		float m_SnapValue = 0.5f;
		float m_RotationSnapValue = 45.0f;
		ViewPortEditorData m_ViewPortEditorData;

		// icons
		bool m_ShowComponentsIcon = true;
		float m_IconSize = 1;
		//
		bool m_ShowBoundingBoxes = false;
		glm::vec4 m_BoundingBoxColor = { 1.0f, 0.9f, 0.2f, 1.0f };
	};
}