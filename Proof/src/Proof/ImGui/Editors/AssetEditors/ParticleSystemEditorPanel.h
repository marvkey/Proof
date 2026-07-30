#pragma once

#include "AssetEditor.h"
#include "Proof/Scene/Camera/EditorCamera.h"

#include <cstdint>
#include <memory>
#include <string>

namespace Proof
{
	class ParticleSequencerInterface;
	struct ParticleSystemTrack;
	struct ParticleSystemEmitterClip;

	class ParticleSystemEditorPanel : public AssetEditor
	{
	public:
		ParticleSystemEditorPanel();
		~ParticleSystemEditorPanel() override;

		void OnUpdate(FrameTime ts) override;
		void OnImGuiRender() override;
		void SetAsset(const Count<class Asset>& asset) override;

		bool IsSubWindowsHovered() override { return false; }
		bool IsSubWindowsFocused() override { return false; }

	protected:
		bool IsSaved() override { return !m_NeedsSaving; }
		void Save() override;

	private:
		void RebuildSequencer();
		void DrawSequencerToolbar();
		void DrawSequencer();
		void DrawAddTrackPopup();
		void DrawPlaybackPanel();
		void DrawDetailsPanel();

		void CommitSequencerEdit(int itemIndex);
		void AddTrack();
		void RemoveTrack(AssetID trackID);
		void AddClipToSelectedTrack();
		void DeleteSelectedClip();

		void MarkTimelineChanged();
		void SyncPreviewWithAsset();
		void RestartPreview();

		ParticleSystemTrack* FindTrack(AssetID trackID);
		ParticleSystemEmitterClip* FindClip(ParticleSystemTrack& track, uint64_t clipID);

		bool DoesClipOverlap(const ParticleSystemTrack& track, uint64_t ignoredClipID, float startTime) const;
		float FindAvailableStartTime(const ParticleSystemTrack& track, float requestedStartTime) const;
		float GetEmitterDuration(const ParticleSystemTrack& track) const;
		float GetParticleSystemDuration() const;
		std::string GetTrackName(AssetID trackID) const;

		static int TimeToFrame(float time);
		static float FrameToTime(int frame);

	private:
		friend class ParticleSequencerInterface;

		static constexpr float TimelineFramesPerSecond = 30.0f;
		static constexpr float MinimumTimelineDuration = 1.0f;
		static constexpr float TimelinePanelHeight = 320.0f;
		static constexpr float DetailsPanelWidth = 360.0f;

		bool m_IsViewportFocused = false;
		bool m_NeedsSaving = false;
		bool m_RebuildSequencerRequested = false;
		bool m_IsPlaying = true;

		int m_CurrentFrame = 0;
		float m_CurrentTime = 0.0f;
		int m_FirstVisibleFrame = 0;
		int m_SelectedEntry = -1;
		bool m_SequencerExpanded = true;

		AssetID m_SelectedTrackID = AssetID(0);
		AssetID m_NewTrackEmitterID = AssetID(0);

		glm::vec2 m_ViewportSize = glm::vec2(1.0f);

		Count<class ParticleSystem> m_ParticleSystem;
		Count<class ParticleEffect> m_ParticleSystemInstance;
		Count<class WorldRenderer> m_WorldRenderer;
		Count<class World> m_World;

		EditorCamera m_Camera;

		std::unique_ptr<ParticleSequencerInterface> m_Sequencer;
	};
}