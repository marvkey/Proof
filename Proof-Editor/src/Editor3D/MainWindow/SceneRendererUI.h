#pragma once
#include "Proof/ProofCore.h"
#include "Panel.h"

namespace Proof
{
	class SceneRendererUI:public Panel{
	public:
		virtual void ImGuiRender(FrameTime deltaTime)override;
		// only supports asset mesh, asset meshourceFile, prefab
		SceneRendererUI(AssetID id);
	private:
		EditorCamera m_Camera{200,200};
		void RenderAsset(FrameTime deltaTime);
		Entity m_SceneEntity;
		void MeshAssetUI();
		void MeshAssetSetUp();

		void MeshSourceAssetSetUp();
		void MeshSourceAssetUI();
		ScreenSize m_ScreenSize;
		Count<class World> m_World = nullptr;
		AssetID m_ID;
		Special<class WorldRenderer> m_WorldRenderer;
		enum class SceneRendererType {
			None,MeshAsset,MeshSourceFile,Prefab
		};
		SceneRendererType m_Type;
	};
}