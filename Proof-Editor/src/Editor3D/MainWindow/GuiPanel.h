#pragma once
#include "Proof/Core/Core.h"
#include "Panel.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"

namespace Proof
{
	class GuiPanel : public Panel {
	public:
		GuiPanel(Count<class UIPanel> panel);
		virtual void ImGuiRender(FrameTime deltaTime)override;

	private:
		Count<class UIPanel> m_UIPanel;
		void DrawButtonNode(const std::string& name, UIButton& button);
		void DrawImageButtonNode(const std::string& name, UIButtonImage& button);
		void DrawTextNode(const std::string& name, UIText& text);

		void DrawButtonComponent(const std::string& name, UIButton& button);
		void DrawImageButtonComponent(const std::string& name, UIButtonImage& button);
		void DrawTextComponent(const std::string& name, UIText& text);
		bool AddElementMenu();
		EditorCamera2D m_Camera;
		std::string m_SelectedName;
		enum class Selected{
			None, 
			Button,
			ImageButton, 
			Text
		};
		Selected m_SelectedType = Selected::None;
	};
}
