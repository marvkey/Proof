#include "Proofprch.h"
#include "ParticleSystemEditorPanel.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Renderer/ParticleSystem/ParticleSystem.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Scene/Entity.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UIUtilities.h"
#include "Proof/ImGui/UIHandlers.h"
#include <ImSequencer.h>
#include "ParticleEmitterPanel.h"

#include "Proofprch.h"
#include "ParticleSystemEditorPanel.h"
#include "Proof/Renderer/Renderer.h"
#include "../Panels/SceneHierachyPanel.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/ParticleSystem/ParticleSystem.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Core/Application.h"
#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/ImGui/UIHandlers.h"

#include "Proof/Renderer/Image.h"
#include "Proof/Renderer/Texture.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
#include <unordered_map>

namespace Proof
{
	class ParticleSequencerInterface final : public ImSequencer::SequenceInterface
	{
	public:
		enum class ItemKind
		{
			Group,
			Clip
		};

		struct Item
		{
			ItemKind Kind = ItemKind::Clip;
			AssetID TrackID = AssetID(0);
			uint64_t ClipID = 0;

			int StartFrame = 0;
			int EndFrame = 1;
			int DurationFrames = 1;
			int LastCommittedStartFrame = 0;

			std::string Label;
			unsigned int Color = 0xFF9D6349;
		};

	public:
		explicit ParticleSequencerInterface(ParticleSystemEditorPanel* owner) : Owner(owner) {}

		int GetFrameMin() const override { return FrameMin; }
		int GetFrameMax() const override { return FrameMax; }
		int GetItemCount() const override { return static_cast<int>(Items.size()); }
		const char* GetCollapseFmt() const override { return "%d Frames / %d Particle Rows"; }

		const char* GetItemLabel(int index) const override
		{
			if (index < 0 || index >= static_cast<int>(Items.size()))
				return "";

			return Items[index].Label.c_str();
		}

		void Get(int index, int** start, int** end, int* type, unsigned int* color) override
		{
			Item& item = Items[index];

			if (start)
				*start = &item.StartFrame;
			if (end)
				*end = &item.EndFrame;
			if (type)
				*type = 0;
			if (color)
				*color = item.Color;
		}

		bool IsGroup(int index) const override
		{
			return index >= 0 && index < static_cast<int>(Items.size()) && Items[index].Kind == ItemKind::Group;
		}

		bool IsGroupOpen(int index) const override
		{
			if (!IsGroup(index))
				return true;

			auto it = GroupOpen.find(Items[index].TrackID);
			return it == GroupOpen.end() || it->second;
		}

		void ToggleGroup(int index) override
		{
			if (!IsGroup(index))
				return;

			const AssetID trackID = Items[index].TrackID;
			GroupOpen[trackID] = !IsGroupOpen(index);

			if (Owner)
				Owner->m_RebuildSequencerRequested = true;
		}

		void RemoveGroup(int index) override
		{
			if (Owner && IsGroup(index))
				Owner->RemoveTrack(Items[index].TrackID);
		}

		int GetItemIndent(int index) const override
		{
			return IsGroup(index) ? 0 : 1;
		}

		void BeginEdit(int index) override
		{
			if (IsGroup(index))
				return;

			EditedItem = index;

			if (index < 0 || index >= static_cast<int>(Items.size()))
				return;

			Items[index].LastCommittedStartFrame = Items[index].StartFrame;
		}

		void EndEdit() override
		{
			if (Owner && EditedItem >= 0)
				Owner->CommitSequencerEdit(EditedItem);

			EditedItem = -1;
		}

	public:
		ParticleSystemEditorPanel* Owner = nullptr;
		std::vector<Item> Items;
		std::unordered_map<AssetID, bool> GroupOpen;

		int FrameMin = 0;
		int FrameMax = 300;
		int EditedItem = -1;
	};

	ParticleSystemEditorPanel::ParticleSystemEditorPanel()
		: AssetEditor("ParticleSystemEditorPanel"), m_Sequencer(std::make_unique<ParticleSequencerInterface>(this))
	{
	}

	ParticleSystemEditorPanel::~ParticleSystemEditorPanel() = default;

	int ParticleSystemEditorPanel::TimeToFrame(float time)
	{
		return static_cast<int>(std::round(time * TimelineFramesPerSecond));
	}

	float ParticleSystemEditorPanel::FrameToTime(int frame)
	{
		return static_cast<float>(frame) / TimelineFramesPerSecond;
	}

	std::string ParticleSystemEditorPanel::GetTrackName(AssetID trackID) const
	{
		if (trackID == AssetID(0))
			return "Missing Emitter";

		return AssetManager::GetAssetInfo(trackID).GetName();
	}

	float ParticleSystemEditorPanel::GetEmitterDuration(const ParticleSystemTrack& track) const
	{
		const Count<ParticleEmitter> emitter = track.Emitter.GetAsset<ParticleEmitter>();

		if (!emitter)
			return 1.0f / TimelineFramesPerSecond;

		return std::max(emitter->ParticleInitialState.Duration, 1.0f / TimelineFramesPerSecond);
	}

	float ParticleSystemEditorPanel::GetParticleSystemDuration() const
	{
		if (!m_ParticleSystem)
			return MinimumTimelineDuration;

		const Count<ParticleSystemTimeline> timeline = m_ParticleSystem->GetTimeline();
		if (!timeline)
			return MinimumTimelineDuration;

		float duration = 0.0f;

		for (const auto& [trackID, track] : timeline->GetTracks())
		{
			const float emitterDuration = GetEmitterDuration(track);

			for (const ParticleSystemEmitterClip& clip : track.Clips)
				duration = std::max(duration, clip.StartTime + emitterDuration);
		}

		return std::max(duration, MinimumTimelineDuration);
	}

	ParticleSystemTrack* ParticleSystemEditorPanel::FindTrack(AssetID trackID)
	{
		if (!m_ParticleSystem)
			return nullptr;

		const Count<ParticleSystemTimeline> timeline = m_ParticleSystem->GetTimeline();
		if (!timeline)
			return nullptr;

		return timeline->GetTrack(trackID);
	}

	ParticleSystemEmitterClip* ParticleSystemEditorPanel::FindClip(ParticleSystemTrack& track, uint64_t clipID)
	{
		for (ParticleSystemEmitterClip& clip : track.Clips)
		{
			if (clip.ID == clipID)
				return &clip;
		}

		return nullptr;
	}


	bool ParticleSystemEditorPanel::DoesClipOverlap(const ParticleSystemTrack& track, uint64_t ignoredClipID, float startTime) const
	{
		const float duration = GetEmitterDuration(track);
		const float endTime = startTime + duration;

		for (const ParticleSystemEmitterClip& other : track.Clips)
		{
			if (other.ID == ignoredClipID)
				continue;

			const float otherStart = other.StartTime;
			const float otherEnd = other.StartTime + duration;

			if (startTime < otherEnd && endTime > otherStart)
				return true;
		}

		return false;
	}

	float ParticleSystemEditorPanel::FindAvailableStartTime(const ParticleSystemTrack& track, float requestedStartTime) const
	{
		const float duration = GetEmitterDuration(track);
		float candidate = std::max(requestedStartTime, 0.0f);

		std::vector<const ParticleSystemEmitterClip*> clips;
		clips.reserve(track.Clips.size());

		for (const ParticleSystemEmitterClip& clip : track.Clips)
			clips.emplace_back(&clip);

		std::sort(clips.begin(), clips.end(), [](const ParticleSystemEmitterClip* left, const ParticleSystemEmitterClip* right)
		{
			return left->StartTime < right->StartTime;
		});

		for (const ParticleSystemEmitterClip* clip : clips)
		{
			const float clipStart = clip->StartTime;
			const float clipEnd = clip->StartTime + duration;

			if (candidate + duration <= clipStart)
				break;

			if (candidate < clipEnd)
				candidate = clipEnd;
		}

		return candidate;
	}

	

	void ParticleSystemEditorPanel::RebuildSequencer()
	{
		m_RebuildSequencerRequested = false;

		if (!m_Sequencer)
			m_Sequencer = std::make_unique<ParticleSequencerInterface>(this);

		uint64_t selectedClipID = 0;
		AssetID selectedTrackID = AssetID(0);

		if (m_SelectedEntry >= 0 && m_SelectedEntry < static_cast<int>(m_Sequencer->Items.size()))
		{
			const auto& selectedItem = m_Sequencer->Items[m_SelectedEntry];

			if (selectedItem.Kind == ParticleSequencerInterface::ItemKind::Clip)
			{
				selectedClipID = selectedItem.ClipID;
				selectedTrackID = selectedItem.TrackID;
			}
		}

		m_Sequencer->Items.clear();
		m_Sequencer->FrameMin = 0;
		m_Sequencer->FrameMax = TimeToFrame(MinimumTimelineDuration);
		m_SelectedEntry = -1;

		if (!m_ParticleSystem)
			return;

		const Count<ParticleSystemTimeline> timeline = m_ParticleSystem->GetTimeline();
		if (!timeline)
			return;

		struct TrackView
		{
			AssetID ID = AssetID(0);
			ParticleSystemTrack* Track = nullptr;
			std::string Name;
		};

		std::vector<TrackView> tracks;
		tracks.reserve(timeline->GetTracks().size());

		for (auto& [trackID, track] : timeline->GetTracks())
			tracks.emplace_back(TrackView{ .ID = trackID, .Track = &track, .Name = GetTrackName(trackID) });

		std::sort(tracks.begin(), tracks.end(), [](const TrackView& left, const TrackView& right)
		{
			return left.Name < right.Name;
		});

		for (TrackView& trackView : tracks)
		{
			ParticleSystemTrack& track = *trackView.Track;
			m_Sequencer->GroupOpen.try_emplace(trackView.ID, true);

			ParticleSequencerInterface::Item group;
			group.Kind = ParticleSequencerInterface::ItemKind::Group;
			group.TrackID = trackView.ID;
			group.Label = trackView.Name;
			m_Sequencer->Items.emplace_back(std::move(group));

			if (!m_Sequencer->GroupOpen[trackView.ID])
				continue;

			const int durationFrames = std::max(TimeToFrame(GetEmitterDuration(track)), 1);

			for (size_t clipIndex = 0; clipIndex < track.Clips.size(); clipIndex++)
			{
				ParticleSystemEmitterClip& clip = track.Clips[clipIndex];
				const int startFrame = std::max(TimeToFrame(clip.StartTime), 0);

				ParticleSequencerInterface::Item item;
				item.Kind = ParticleSequencerInterface::ItemKind::Clip;
				item.TrackID = trackView.ID;
				item.ClipID = clip.ID;
				item.StartFrame = startFrame;
				item.EndFrame = startFrame + durationFrames;
				item.DurationFrames = durationFrames;
				item.LastCommittedStartFrame = startFrame;
				item.Label = fmt::format("Track {}", clipIndex + 1);

				m_Sequencer->Items.emplace_back(std::move(item));

				const int itemIndex = static_cast<int>(m_Sequencer->Items.size()) - 1;

				if (clip.ID == selectedClipID && trackView.ID == selectedTrackID)
					m_SelectedEntry = itemIndex;

				m_Sequencer->FrameMax = std::max(m_Sequencer->FrameMax, startFrame + durationFrames);
			}
		}

		m_Sequencer->FrameMax += TimeToFrame(2.0f);
		m_CurrentFrame = std::clamp(m_CurrentFrame, m_Sequencer->FrameMin, m_Sequencer->FrameMax);
		m_FirstVisibleFrame = std::clamp(m_FirstVisibleFrame, m_Sequencer->FrameMin, m_Sequencer->FrameMax);
	}

	void ParticleSystemEditorPanel::CommitSequencerEdit(int itemIndex)
	{
		if (!m_Sequencer || itemIndex < 0 || itemIndex >= static_cast<int>(m_Sequencer->Items.size()))
			return;

		ParticleSequencerInterface::Item& item = m_Sequencer->Items[itemIndex];
		if (item.Kind != ParticleSequencerInterface::ItemKind::Clip)
			return;

		const int newStartFrame = std::max(item.StartFrame, 0);

		// ImSequencer allows edge resizing. Particle duration is fixed,
		// so always restore the emitter's real duration.
		item.StartFrame = newStartFrame;
		item.EndFrame = newStartFrame + item.DurationFrames;

		ParticleSystemTrack* track = FindTrack(item.TrackID);
		if (!track)
		{
			m_RebuildSequencerRequested = true;
			return;
		}

		ParticleSystemEmitterClip* clip = FindClip(*track, item.ClipID);
		if (!clip)
		{
			m_RebuildSequencerRequested = true;
			return;
		}

		const float newStartTime = FrameToTime(newStartFrame);

		if (clip->StartTime == newStartTime)
			return;

		clip->StartTime = newStartTime;
		m_SelectedTrackID = item.TrackID;
		m_SelectedEntry = itemIndex;

		MarkTimelineChanged();
		m_RebuildSequencerRequested = true;
	}

	void ParticleSystemEditorPanel::AddTrack()
	{
		if (!m_ParticleSystem || m_NewTrackEmitterID == AssetID(0))
			return;

		const Count<ParticleSystemTimeline> timeline = m_ParticleSystem->GetTimeline();
		if (!timeline || timeline->HasTrack(m_NewTrackEmitterID))
			return;

		AssetKey<AssetType::ParticleEmitter> emitter;
		if (!emitter.SetAssetID(m_NewTrackEmitterID) || !emitter.IsValid())
			return;

		if (!timeline->AddTrack(emitter))
			return;

		if (m_Sequencer)
			m_Sequencer->GroupOpen[m_NewTrackEmitterID] = true;

		m_SelectedTrackID = m_NewTrackEmitterID;
		m_NewTrackEmitterID = AssetID(0);
		m_SelectedEntry = -1;

		MarkTimelineChanged();
		m_RebuildSequencerRequested = true;
	}

	void ParticleSystemEditorPanel::RemoveTrack(AssetID trackID)
	{
		if (!m_ParticleSystem || !m_ParticleSystem->GetTimeline())
			return;

		auto timeline = m_ParticleSystem->GetTimeline();
		if (!timeline->RemoveTrack(trackID))
			return;

		if (m_Sequencer)
			m_Sequencer->GroupOpen.erase(trackID);

		m_SelectedEntry = -1;
		m_SelectedTrackID = timeline->GetTracks().empty() ? AssetID(0) : timeline->GetTracks().begin()->first;

		MarkTimelineChanged();
		m_RebuildSequencerRequested = true;
	}

	void ParticleSystemEditorPanel::DrawAddTrackPopup()
	{
		if (!ImGui::BeginPopup("Add Particle Emitter Track"))
			return;

		UI::BeginPropertyGrid();
		UI::AttributeAssetReference("Emitter", AssetType::ParticleEmitter, m_NewTrackEmitterID);
		UI::EndPropertyGrid();

		const bool alreadyAdded = m_ParticleSystem && m_ParticleSystem->GetTimeline() && m_NewTrackEmitterID != AssetID(0) &&
			m_ParticleSystem->GetTimeline()->HasTrack(m_NewTrackEmitterID);

		if (alreadyAdded)
			ImGui::TextDisabled("This emitter already has a track.");

		ImGui::BeginDisabled(m_NewTrackEmitterID == AssetID(0) || alreadyAdded);

		if (ImGui::Button("Add Track"))
		{
			AddTrack();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndDisabled();
		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			m_NewTrackEmitterID = AssetID(0);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	void ParticleSystemEditorPanel::AddClipToSelectedTrack()
	{
		ParticleSystemTrack* track = FindTrack(m_SelectedTrackID);
		if (!track)
			return;

		float startTime = 0.0f;
		const float emitterDuration = GetEmitterDuration(*track);

		for (const ParticleSystemEmitterClip& clip : track->Clips)
			startTime = std::max(startTime, clip.StartTime + emitterDuration);

		ParticleSystemEmitterClip clip(track->Emitter);
		clip.StartTime = startTime;

		track->Clips.emplace_back(std::move(clip));

		MarkTimelineChanged();
		m_RebuildSequencerRequested = true;
	}

	void ParticleSystemEditorPanel::DeleteSelectedClip()
	{
		if (!m_Sequencer || m_SelectedEntry < 0 || m_SelectedEntry >= static_cast<int>(m_Sequencer->Items.size()))
			return;

		const ParticleSequencerInterface::Item item = m_Sequencer->Items[m_SelectedEntry];
		if (item.Kind != ParticleSequencerInterface::ItemKind::Clip)
			return;

		ParticleSystemTrack* track = FindTrack(item.TrackID);
		if (!track)
			return;

		std::erase_if(track->Clips, [clipID = item.ClipID](const ParticleSystemEmitterClip& clip)
		{
			return clip.ID == clipID;
		});

		m_SelectedTrackID = item.TrackID;
		m_SelectedEntry = -1;

		MarkTimelineChanged();
		m_RebuildSequencerRequested = true;
	}

	void ParticleSystemEditorPanel::MarkTimelineChanged()
	{
		m_NeedsSaving = true;
		SyncPreviewWithAsset();
	}

	void ParticleSystemEditorPanel::SyncPreviewWithAsset()
	{
		if (!m_ParticleSystemInstance)
			return;

		m_ParticleSystemInstance->SyncWithParticleSystem();
		RestartPreview();
	}

	void ParticleSystemEditorPanel::RestartPreview()
	{
		m_CurrentTime = 0.0f;
		m_CurrentFrame = 0;

		if (!m_ParticleSystemInstance)
			return;

		m_ParticleSystemInstance->Play(true);

		if (!m_IsPlaying)
			m_ParticleSystemInstance->Pause();
	}

	void ParticleSystemEditorPanel::DrawSequencerToolbar()
	{
		ImGui::BeginChild("ParticleSystemToolbar", ImVec2(0.0f, 42.0f), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (ImGui::Button(m_IsPlaying ? "Pause" : "Play"))
		{
			m_IsPlaying = !m_IsPlaying;

			if (m_ParticleSystemInstance)
			{
				if (m_IsPlaying)
					m_ParticleSystemInstance->Play(false);
				else
					m_ParticleSystemInstance->Pause();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Restart"))
			RestartPreview();

		ImGui::SameLine();
		ImGui::Text("%.2f s / %.2f s", FrameToTime(m_CurrentFrame), GetParticleSystemDuration());
		ImGui::SameLine();

		if (ImGui::Button("+ Track"))
		{
			m_NewTrackEmitterID = AssetID(0);
			ImGui::OpenPopup("Add Particle Emitter Track");
		}

		DrawAddTrackPopup();
		ImGui::SameLine();

		std::string selectedTrackName = "No Emitter Track";
		if (m_SelectedTrackID != AssetID(0))
			selectedTrackName = GetTrackName(m_SelectedTrackID);

		ImGui::SetNextItemWidth(220.0f);

		if (ImGui::BeginCombo("##ParticleTrack", selectedTrackName.c_str()))
		{
			if (m_ParticleSystem && m_ParticleSystem->GetTimeline())
			{
				for (const auto& [trackID, track] : m_ParticleSystem->GetTimeline()->GetTracks())
				{
					const bool selected = trackID == m_SelectedTrackID;
					const std::string name = GetTrackName(trackID);

					if (ImGui::Selectable(name.c_str(), selected))
						m_SelectedTrackID = trackID;

					if (selected)
						ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::BeginDisabled(m_SelectedTrackID == AssetID(0));

		if (ImGui::Button("+ Clip"))
			AddClipToSelectedTrack();

		ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::BeginDisabled(m_SelectedEntry < 0);

		if (ImGui::Button("Delete Clip"))
			DeleteSelectedClip();

		ImGui::EndDisabled();
		ImGui::EndChild();
	}

	void ParticleSystemEditorPanel::DrawDetailsPanel()
	{
		ImGui::TextUnformatted("Clip Details");
		ImGui::Separator();

		if (!m_Sequencer || m_SelectedEntry < 0 || m_SelectedEntry >= static_cast<int>(m_Sequencer->Items.size()))
		{
			ImGui::TextDisabled("Select a particle clip.");
			return;
		}

		const auto& item = m_Sequencer->Items[m_SelectedEntry];
		if (item.Kind != ParticleSequencerInterface::ItemKind::Clip)
		{
			ImGui::TextDisabled("Select a particle track.");
			return;
		}

		ParticleSystemTrack* track = FindTrack(item.TrackID);
		if (!track)
			return;

		ParticleSystemEmitterClip* clip = FindClip(*track, item.ClipID);
		if (!clip)
			return;

		ImGui::Text("Emitter: %s", GetTrackName(item.TrackID).c_str());
		ImGui::Text("Clip ID: %llu", static_cast<unsigned long long>(clip->ID));
		ImGui::Separator();

		bool changed = false;
		int startFrame = TimeToFrame(clip->StartTime);

		UI::BeginPropertyGrid();

		if (UI::AttributeDrag("Start Frame", startFrame, 1, 0))
		{
			const float startTime = FrameToTime(std::max(startFrame, 0));

			if (clip->StartTime != startTime)
			{
				clip->StartTime = startTime;
				changed = true;
			}
		}


		UI::EndPropertyGrid();

		ImGui::Text("Start Time: %.2f s", clip->StartTime);
		ImGui::Text("Duration: %.2f s", GetEmitterDuration(*track));
		ImGui::Text("Duration Frames: %d", TimeToFrame(GetEmitterDuration(*track)));

		if (changed)
		{
			MarkTimelineChanged();
			m_RebuildSequencerRequested = true;
		}
	}


	void ParticleSystemEditorPanel::DrawSequencer()
	{
		if (!m_Sequencer)
			return;

		DrawSequencerToolbar();
		ImGui::Separator();

		const int options = ImSequencer::SEQUENCER_EDIT_STARTEND;
		ImSequencer::Sequencer(m_Sequencer.get(), &m_CurrentFrame, &m_SequencerExpanded, &m_SelectedEntry, &m_FirstVisibleFrame, options);

		if (m_SelectedEntry >= 0 && m_SelectedEntry < static_cast<int>(m_Sequencer->Items.size()))
		{
			const auto& item = m_Sequencer->Items[m_SelectedEntry];

			if (item.Kind == ParticleSequencerInterface::ItemKind::Clip)
				m_SelectedTrackID = item.TrackID;
		}

		if (m_RebuildSequencerRequested)
			RebuildSequencer();
	}

	void ParticleSystemEditorPanel::OnUpdate(FrameTime deltaTime)
	{
		if (!m_World || !m_WorldRenderer)
			return;

		m_Camera.SetViewportSize(std::max(m_ViewportSize.x, 1.0f), std::max(m_ViewportSize.y, 1.0f));
		m_Camera.SetActive(m_IsViewportFocused);
		m_Camera.OnUpdate(deltaTime);

		m_World->OnUpdateEditor(deltaTime);

		if (m_IsPlaying && m_ParticleSystemInstance)
		{
			m_CurrentTime += static_cast<float>(deltaTime);
			m_CurrentFrame = TimeToFrame(m_CurrentTime);

			if (m_CurrentTime >= GetParticleSystemDuration())
				RestartPreview();
		}

		m_World->OnRenderEditor(m_WorldRenderer, deltaTime, m_Camera, [this](Count<WorldRenderer> renderer)
		{
			if (!m_ParticleSystemInstance)
				return;

			const Count<ParticleSystemTimelineInstance> timeline = m_ParticleSystemInstance->GetTimeline();
			if (!timeline)
				return;

			for (const auto& [trackID, track] : timeline->GetTracks())
			{
				for (const auto& clip : track.EmitterClips)
				{
					if (clip.Emitter)
						renderer->SubmitParticleEmitter(clip.Emitter);
				}
			}
		});
	}

	void ParticleSystemEditorPanel::OnImGuiRender()
	{
		UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Main Window");

		if (!m_ParticleSystem || !m_WorldRenderer)
		{
			ImGui::TextDisabled("No particle system asset loaded.");
			ImGui::End();
			return;
		}

		const ImVec2 available = ImGui::GetContentRegionAvail();
		const float spacing = ImGui::GetStyle().ItemSpacing.x;
		const float timelineHeight = std::min(TimelinePanelHeight, std::max(220.0f, available.y * 0.42f));
		const float topHeight = std::max(available.y - timelineHeight - spacing, 120.0f);
		const float detailsWidth = std::min(DetailsPanelWidth, std::max(280.0f, available.x * 0.34f));
		const float viewportWidth = std::max(available.x - detailsWidth - spacing, 120.0f);

		{
			UI::ScopedStyleColor backgroundColor(ImGuiCol_ChildBg, { 0.0f, 0.0f, 0.0f, 1.0f });

			ImGui::BeginChild("ParticleSystemViewport", ImVec2(viewportWidth, topHeight));

			const ImVec2 viewportSize = ImGui::GetContentRegionAvail();

			m_ViewportSize = glm::vec2(viewportSize.x, viewportSize.y);
			m_WorldRenderer->SetViewportSize(viewportSize.x, viewportSize.y);
			m_IsViewportFocused = ImGui::IsWindowFocused();

			UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::EndChild();
		}

		ImGui::SameLine();

		ImGui::BeginChild("ParticleSystemDetails", ImVec2(0.0f, topHeight), true);
		DrawDetailsPanel();
		ImGui::EndChild();

		ImGui::BeginChild("ParticleSystemSequencer", ImVec2(0.0f, 0.0f), true);
		DrawSequencer();
		ImGui::EndChild();

		ImGui::End();
	}

	void ParticleSystemEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (!asset || asset->GetAssetType() != AssetType::ParticleSystem)
		{
			PF_ENGINE_ERROR("Cannot pass invalid Asset to ParticleSystemEditorPanel {}", m_TitleAndId);
			return;
		}

		m_ParticleSystem = asset.As<ParticleSystem>();
		m_World = Count<World>::Create();

		Entity particleEntity = m_World->CreateEntity("Particle");

		m_ParticleSystemInstance = Count<ParticleEffect>::Create(m_ParticleSystem);
		particleEntity.AddComponent<ParticleEffectComponent>().ParticleEffect = m_ParticleSystemInstance;

		Entity light = m_World->CreateEntity("Light");

		light.AddComponent<SkyLightComponent>();
		light.AddComponent<DirectionalLightComponent>();
		light.GetComponent<TransformComponent>().SetRotationEuler(glm::vec3(80.0f, 10.0f, 0.0f));

		m_Camera.SetPosition(glm::vec3(0.0f, 2.5f, 13.5f));
		m_WorldRenderer = Count<WorldRenderer>::Create();

		m_CurrentTime = 0.0f;
		m_CurrentFrame = 0;
		m_FirstVisibleFrame = 0;
		m_SelectedEntry = -1;
		m_SequencerExpanded = true;
		m_IsPlaying = true;
		m_NeedsSaving = false;
		m_SelectedTrackID = AssetID(0);
		m_NewTrackEmitterID = AssetID(0);

		if (m_ParticleSystem->GetTimeline() && !m_ParticleSystem->GetTimeline()->GetTracks().empty())
			m_SelectedTrackID = m_ParticleSystem->GetTimeline()->GetTracks().begin()->first;

		m_ParticleSystemInstance->Play(true);
		RebuildSequencer();
	}

	void ParticleSystemEditorPanel::Save()
	{
		if (!m_ParticleSystem)
			return;

		AssetManager::SaveAsset(m_ParticleSystem->GetID());
		m_NeedsSaving = false;

		for (auto worldWeak : World::GetAllActiveWorlds())
		{
			if (!worldWeak.IsValid())
				continue;

			const Count<World> world = worldWeak.Lock();
			if (!world || world->GetState() != WorldState::Edit)
				continue;

			world->ForEachEnitityWith<ParticleEffectComponent>([&](Entity entity)
			{
				auto& component = entity.GetComponent<ParticleEffectComponent>();

				if (!component.ParticleEffect)
					return;

				if (component.ParticleEffect->GetParticleSystem() != m_ParticleSystem)
					return;

				component.ParticleEffect->SyncWithParticleSystem();
			});
		}
	}
}