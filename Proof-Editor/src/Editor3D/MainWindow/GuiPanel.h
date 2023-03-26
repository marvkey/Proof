#pragma once
#include "Proof/Core/Core.h"
#include "Panel.h"
#include "Proof/Scene/Camera/EditorCamera.h"

namespace Proof
{
	class GuiPanel : public Panel {
	public:
		GuiPanel();
		virtual void ImGuiRender(FrameTime deltaTime)override;

	private:
		Count<class UIPanel> m_UIPanel;

		EditorCamera2D m_Camera;
	};
}
