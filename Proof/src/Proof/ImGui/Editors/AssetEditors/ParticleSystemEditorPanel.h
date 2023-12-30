#pragma once
#include "AssetEditor.h"
#include "Proof/Scene/Camera/EditorCamera.h"
namespace Proof
{
	class ParticleSystemEditorPanel : public AssetEditor 
	{
	public:
		ParticleSystemEditorPanel();
		virtual void OnUpdate(FrameTime ts);
		virtual void OnImGuiRender()override;
		virtual void SetAsset(const Count<class Asset>& asset);
	private:
		bool m_IsViewportFocused = false;
		float m_SaveTimer = 100.0f;
		Count<class ParticleHandler> m_ParticleHandler;
		Count<class ParticleSystem> m_ParticleSystem;
		Count<class  WorldRenderer> m_WorldRenderer;
		Count<class World> m_World;
		EditorCamera m_Camera;
	};
}