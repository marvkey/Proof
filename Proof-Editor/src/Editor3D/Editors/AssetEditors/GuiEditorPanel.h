#pragma once
#include "AssetEditor.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Scene/Camera/EditorCamera.h"

namespace Proof
{
	class GuiEditorPanel : public AssetEditor 
	{
	public:
		GuiEditorPanel();
		virtual void OnImGuiRender()override;
		virtual void OnUpdate(FrameTime deltaTime);
		virtual void SetAsset(const Count<class Asset>& asset);

	private:
		Count<class UIPanel> m_UIPanel;
		void DrawButtonNode(const std::string& name, UIButton& button);
		void DrawImageButtonNode(const std::string& name, UIButtonImage& button);
		void DrawTextNode(const std::string& name, UIText& text);

		void DrawButtonComponent(const std::string& name, UIButton& button);
		void DrawImageButtonComponent(const std::string& name, UIButtonImage& button);
		void DrawTextComponent(const std::string& name, UIText& text);
		bool AddElementMenu();
	private:
		EditorCamera2D m_Camera;
		bool m_IsViewportFocused = false;
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
