#pragma once

#include "AssetEditor.h"
#include "Proof/Scene/Camera/EditorCamera.h"

namespace Proof
{
	class ParticleEmitterPanel : public AssetEditor
	{
	public:
		ParticleEmitterPanel();
		~ParticleEmitterPanel() override = default;

		void OnUpdate(FrameTime deltaTime) override;
		void OnImGuiRender() override;
		void SetAsset(const Count<class Asset>& asset) override;

		bool IsSubWindowsHovered() override { return false; }
		bool IsSubWindowsFocused() override { return false; }

	protected:
		bool IsSaved() override { return !m_NeedsSaving; }
		void Save() override;

	private:
		void DrawPropertiesPanel();
		void DrawViewportPanel();
		void DrawPlaybackPanel();
		Count<class ParticleEmitterInstance> GetPreviewEmitterInstance();

	private:
		bool m_IsViewportFocused = false;
		bool m_NeedsSaving = false;
		bool m_Resimulate = true;

		float m_SaveTimer = 100.0f;
		glm::vec2 m_ViewportSize = glm::vec2(1.0f);

		Count<class ParticleEmitter> m_Emitter;
		Count<class ParticleEmitterInstance> m_EmitterInstance;
		Count<class ParticleEffect> m_ParticleSystemInstance;
		Count<class WorldRenderer> m_WorldRenderer;
		Count<class World> m_World;

		EditorCamera m_Camera;
	};
}