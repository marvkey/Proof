#pragma once
#include "AssetEditor.h"
#include <glm/glm.hpp>
namespace Proof
{
	class MeshEditorPanel : public AssetEditor
	{
	public:
		MeshEditorPanel();
		virtual void OnUpdate(FrameTime ts);
		virtual void OnImGuiRender()override;
		virtual void SetAsset(const Count<class Asset>& asset);
		virtual void Save();
		virtual bool IsSaved() {return !m_NeedsSaving;};
	private:
		Count<class MeshCollider> m_MeshCollider;
		Count<class World> m_World;
		Count<class DetailsPanel> m_DetailsPanel;
		Special<class EditorWorkspaceManager> m_WorkSpaceManager;
		Count<class MeshBase> m_MeshBase;
		std::string m_ViewportPanelName;
		std::string m_DetailsPanelName;
		bool m_NeedsSaving = true;
	private:
		virtual void SetDefaultLayout();
		virtual bool IsSubWindowsHovered();
		virtual bool IsSubWindowsFocused();
		void UpdatePreviewEntity();
		void RenderSettingsPanel();
		void RenderStaticMeshPanel();
		void RenderDynamicMeshPanel();

		void DrawMeshNode();

	};
}