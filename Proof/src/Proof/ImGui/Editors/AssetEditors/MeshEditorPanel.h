#pragma once
#include "AssetEditor.h"
#include "Proof/Scene/Camera/EditorCamera.h"
namespace Proof
{
	class MeshEditorPanel : public AssetEditor
	{
	public:
		MeshEditorPanel();
		virtual void OnUpdate(FrameTime ts);
		virtual void OnImGuiRender()override;
		virtual void SetAsset(const Count<class Asset>& asset);
	private:
		bool m_IsViewportFocused = false;
		EditorCamera m_Camera;
		Count<class  WorldRenderer> m_WorldRenderer;
		Count<class World> m_World;
		Count<class Mesh> m_Mesh;
	};
}