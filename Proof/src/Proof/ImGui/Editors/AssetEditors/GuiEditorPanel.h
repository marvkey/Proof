#pragma once
#include "AssetEditor.h"
#include "Proof/Scene/Camera/EditorCamera.h"

namespace Proof
{
	class UIElement;
	class GuiEditorPanel : public AssetEditor 
	{
	public:
		GuiEditorPanel();
		virtual void OnImGuiRender()override;
		virtual void OnUpdate(FrameTime ts);
		virtual void SetAsset(const Count<class Asset>& asset);
		virtual bool IsSubWindowsHovered() { return false; };
		virtual bool IsSubWindowsFocused() {
			return false;
		};
	protected:
		virtual void OnEvent(class Event& e);
		virtual bool IsSaved() { return !m_NeedsSaving; }
		virtual void Save();
		void Compile();
	private:
		void AddItemMenu(UUID onwer = 0);
		void RenderHierarchyPanel();
		void DrawElementNode(class UIElement element);
		void DrawElementProperty(class UIElement element);
		void RenderViewPortPanel();
		template<typename T, typename UIFunction>
		void DrawElementType(class UIElement element, UIFunction function);
		virtual void SetWorldContext(const Count<class World>& context);

	private:
		glm::vec2 m_WindowSize = { 0,0 };
		EditorCamera m_Camera;
		Count<class Renderer2D> m_Renderer;
		bool m_NeedsSaving = true;
		Count<class UIPanel> m_UIPanel = nullptr;
		Count<class VariableRegistrySubPanel> m_VariableRegistrySubPanel;

		Count<class World> m_CurrentMainWorld; // world of editor 3d
		std::unordered_set<UUID> m_DeletedElements; // elements that are deleted but not removed from the hierarchy yet
	};
}
