#pragma once
#include "AssetEditor.h"
#include "Proof/ImGui/Editors/EditorWorkspace/ViewPortEditorWorkspace.h"
namespace Proof
{
	class PrefabEditorPanel : public AssetEditor
	{
	public:
		PrefabEditorPanel();
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(FrameTime ts);
		virtual void SetAsset(const Count<class Asset>& asset);
		virtual bool IsSaved() { return !m_NeedsSaving; };
		virtual void Save();
	protected:
		virtual void OnEvent(class Event& e);
		virtual void OnWindowStylePush();
		virtual void OnWindowStylePop();
	private:
		bool m_NeedsSaving = true;
		Count<class Prefab> m_Prefab;
		Count<class SceneHierachyPanel> m_WorldHierarchyPanel;
		Special<EditorWorkspaceManager> m_WorkSpaceManager;
	};
}