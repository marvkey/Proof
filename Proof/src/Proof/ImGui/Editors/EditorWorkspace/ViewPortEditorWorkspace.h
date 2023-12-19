#pragma once
#include "EditorWorkspace.h"
#include <glm/glm.hpp>
#include "Proof/Scene/Camera/EditorCamera.h"
#include <functional>
namespace Proof
{
	struct ViewPortEditorData
	{
		bool CanClose = false;
		bool CanMove = false;
		std::function<void()> HandleOnImGuiDrop = nullptr;
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

	protected:
		virtual void OnWindowStylePush();
		virtual void OnWindowStylePop();

	private:
		Count<class WorldRenderer> m_WorldRenderer = nullptr;
		Count<class World> m_WorldContext = nullptr;
		class EditorCamera m_Camera;

		bool m_CanClose = false;
		bool m_CanMove = false;
		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_ViewPortSize;
		//GIZMO Tools
		int m_GizmoType = -1; // -1 = no gizmo
		int m_GizmoMode = 0; // 0 = local
		float m_SnapValue = 0.5f;
		float m_RotationSnapValue = 45.0f;
		ViewPortEditorData m_ViewPortEditorData;
	};
}