#include "Proofprch.h"
#include "AudioMixerEditorPanel.h"
#include "Proof/ImGui/UI.h"

#include "Proof/Asset/AssetManager.h"

#include "Proof/Audio/AudioEffects.h"

#include <imgui.h>
#include <glm/common.hpp>

namespace Proof
{
	static constexpr float AudioMixerMinimumDB = -80.0f;
	static constexpr float AudioMixerMaximumDB = 20.0f;


	AudioMixerEditorPanel::AudioMixerEditorPanel()
		: AssetEditor("AudioMixerEditorPanel")
	{
	}

	void AudioMixerEditorPanel::SetAsset(const Count<class Asset>& asset)
	{
		if (!asset)
			return;

		if (asset->GetAssetType() != AssetType::AudioMixer)
		{
			PF_ENGINE_ERROR("Trying to open invalid asset type in AudioMixerEditorPanel");
			return;
		}

		m_AudioMixer = asset.As<AudioMixer>();

		if (!m_AudioMixer)
			return;

		SelectGroup(m_AudioMixer->GetMasterGroupID());
		m_NeedsSaving = false;
	}

	void AudioMixerEditorPanel::OnUpdate(FrameTime ts)
	{
		if (!m_AudioMixer)
			return;

		AssetEditor::OnUpdate(ts);
	}

	void AudioMixerEditorPanel::OnImGuiRender()
	{
		if (!m_AudioMixer)
			return;
    	ImGui::Begin("Audio Mixer");

		DrawToolbar();

		ImGui::Separator();

		ImVec2 available = ImGui::GetContentRegionAvail();
		float hierarchyWidth = available.x * 0.32f;

		ImGui::BeginChild("##AudioMixerHierarchy", ImVec2(hierarchyWidth, 0.0f), true);

		DrawGroupHierarchy();

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("##AudioMixerInspector", ImVec2(0.0f, 0.0f), true);

		DrawGroupInspector();

		ImGui::EndChild();

		ImGui::End();
	}

	void AudioMixerEditorPanel::DrawToolbar()
	{
		Count<AudioMixerGroup> selectedGroup = m_AudioMixer->GetGroup(m_SelectedGroupID);

		if (ImGui::Button("Add Group"))
			CreateGroup(m_AudioMixer->GetMasterGroupID());

		ImGui::SameLine();

		if (!selectedGroup)
			ImGui::BeginDisabled();

		if (ImGui::Button("Add Child"))
			CreateGroup(m_SelectedGroupID);

		if (!selectedGroup)
			ImGui::EndDisabled();

		ImGui::SameLine();

		bool canDelete = selectedGroup && selectedGroup->GetID() != m_AudioMixer->GetMasterGroupID();

		if (!canDelete)
			ImGui::BeginDisabled();

		if (ImGui::Button("Delete"))
			DeleteSelectedGroup();

		if (!canDelete)
			ImGui::EndDisabled();

		ImGui::SameLine();

		if (ImGui::Button("Save"))
			Save();

		if (m_NeedsSaving)
		{
			ImGui::SameLine();
			ImGui::TextUnformatted("*");
		}
	}

	void AudioMixerEditorPanel::DrawGroupHierarchy()
	{
		ImGui::TextUnformatted("Groups");
		ImGui::Separator();

		DrawGroupNode(m_AudioMixer->GetMasterGroupID());

		if (ImGui::BeginPopupContextWindow("##AudioMixerHierarchyContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Add Group"))
				CreateGroup(m_AudioMixer->GetMasterGroupID());

			ImGui::EndPopup();
		}
	}

	void AudioMixerEditorPanel::DrawGroupNode(UUID groupID)
	{
		Count<AudioMixerGroup> group = m_AudioMixer->GetGroup(groupID);

		if (!group)
			return;

		bool selected = m_SelectedGroupID == groupID;
		bool hasChildren = !group->GetChildren().empty();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		if (!hasChildren)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (groupID == m_AudioMixer->GetMasterGroupID())
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		ImGui::PushID((int)groupID.Get());

		bool opened = ImGui::TreeNodeEx("##AudioMixerGroup", flags, "%s", group->GetName().c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			SelectGroup(groupID);

		if (groupID != m_AudioMixer->GetMasterGroupID())
		{
			if (ImGui::BeginDragDropSource())
			{
				uint64_t id = groupID.Get();

				ImGui::SetDragDropPayload("AUDIO_MIXER_GROUP", &id, sizeof(uint64_t));
				ImGui::TextUnformatted(group->GetName().c_str());

				ImGui::EndDragDropSource();
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AUDIO_MIXER_GROUP"))
			{
				UUID droppedID = *(const uint64_t*)payload->Data;

				if (droppedID != groupID && m_AudioMixer->ReparentGroup(droppedID, groupID))
					m_NeedsSaving = true;
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem("##AudioMixerGroupContext"))
		{
			if (ImGui::MenuItem("Add Child"))
				CreateGroup(groupID);

			if (groupID != m_AudioMixer->GetMasterGroupID())
			{
				if (ImGui::MenuItem("Delete"))
				DeleteSelectedGroup();
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			std::vector<UUID> children = group->GetChildren();

			for (UUID childID : children)
				DrawGroupNode(childID);

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void AudioMixerEditorPanel::DrawGroupInspector()
	{
		Count<AudioMixerGroup> group = m_AudioMixer->GetGroup(m_SelectedGroupID);

		if (!group)
		{
			ImGui::TextUnformatted("No Audio Mixer Group Selected");
			return;
		}

		ImGui::Text("%s", group->GetName().c_str());
		ImGui::Separator();

		bool isMaster = group->GetID() == m_AudioMixer->GetMasterGroupID();

		UI::PushModified(m_NeedsSaving);

		UI::BeginPropertyGrid();

		std::string name = group->GetName();

		UI::PushItemDisabled(isMaster);

		if (UI::AttributeInputText("Name", name))
			group->SetName(name);

		UI::PopItemDisabled();

		float volumeDB = VolumeToDecibels(group->GetVolume());

		if (UI::AttributeSlider("Volume", volumeDB, AudioMixerMinimumDB, AudioMixerMaximumDB,
			"Controls the volume of this mixer group and everything routed through it.",
			ImGuiSliderFlags_AlwaysClamp, "%.1f dB"))
		{
			group->SetVolume(DecibelsToVolume(volumeDB));
		}

		UI::EndPropertyGrid();

		UI::PopModified();


		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextUnformatted("Routing");

		UI::BeginPropertyGrid();

		if (isMaster)
		{
			UI::AttributeTextBar("Output", "Audio Device");
		}
		else
		{
			Count<AudioMixerGroup> parent = m_AudioMixer->GetGroup(group->GetParentID());

			if (parent)
				UI::AttributeTextBar("Output", parent->GetName());
			else
				UI::AttributeTextBar("Output", "Invalid");
		}

		UI::AttributeTextBar("Children", fmt::format("{}", group->GetChildren().size()));

		UI::EndPropertyGrid();


		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		DrawEffectTable(group);


		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextUnformatted("Volume Preview");

		float gain = group->GetVolume();
		float displayDB = VolumeToDecibels(gain);

		UI::BeginPropertyGrid();

		UI::AttributeTextBar("Linear Gain", fmt::format("{:.4f}", gain));
		UI::AttributeTextBar("Decibels", fmt::format("{:.2f} dB", displayDB));

		UI::EndPropertyGrid();

		float meter = glm::clamp((displayDB - AudioMixerMinimumDB) / (0.0f - AudioMixerMinimumDB), 0.0f, 1.0f);
		ImGui::ProgressBar(meter, ImVec2(-1.0f, 18.0f), "");
	}

	void AudioMixerEditorPanel::DrawEffectTable(const Count<AudioMixerGroup>& group)
	{
		Count<AudioEffectTable> effectTable = group->GetEffectTable();

		if (!effectTable)
			return;

		ImGui::TextUnformatted("Effects");

		static AudioEffectType selectedEffectType = AudioEffectType::LowPass;

		UI::BeginPropertyGrid();

		UI::EnumCombo("Effect", selectedEffectType, { AudioEffectType::None },
			{},
			"Select the effect type that will be added to this mixer group.");

		UI::PushModified(m_NeedsSaving);

		bool addEffect = UI::AttributeButton("Add", "Add Effect");

		UI::PopModified();

		UI::EndPropertyGrid();

		if (addEffect)
		{
			Count<AudioEffect> effect = CreateAudioEffect(selectedEffectType);

			if (effect)
				effectTable->AddEffect(effect);
		}

		if (effectTable->Empty())
		{
			ImGui::TextDisabled("No Effects");
			return;
		}

		ImGui::Spacing();

		int removeIndex = -1;
		int moveFrom = -1;
		int moveTo = -1;

		for (uint32_t i = 0; i < effectTable->GetEffectCount(); i++)
		{
			Count<AudioEffect> effect = effectTable->GetEffect(i);

			if (!effect)
				continue;

			ImGui::PushID((int)i);

			std::string effectName = fmt::format("{}##AudioEffect{}", EnumReflection::EnumString(effect->GetType()), i);

			bool opened = UI::AttributeTreeNode(effectName, true, 6.0f, 6.0f, false, false);

			if (opened)
			{
				UI::PushModified(m_NeedsSaving);

				UI::BeginPropertyGrid();

				bool enabled = effect->IsEnabled();

				if (UI::AttributeBool("Enabled", enabled, "Controls whether this effect is enabled in the mixer effect chain."))
					effect->SetEnabled(enabled);

				UI::EndPropertyGrid();

				UI::PopModified();


				DrawEffect(effect, i);


				UI::PushModified(m_NeedsSaving);

				UI::BeginPropertyGrid();

				bool moveUp = false;
				bool moveDown = false;
				bool remove = false;

				if (i > 0)
					moveUp = UI::AttributeButton("Move Up", "Up");
				else
				{
					UI::PushItemDisabled();
					UI::AttributeButton("Move Up", "Up");
					UI::PopItemDisabled();
				}

				if (i + 1 < effectTable->GetEffectCount())
					moveDown = UI::AttributeButton("Move Down", "Down");
				else
				{
					UI::PushItemDisabled();
					UI::AttributeButton("Move Down", "Down");
					UI::PopItemDisabled();
				}

				remove = UI::AttributeButton("Remove", "Remove");

				UI::EndPropertyGrid();

				UI::PopModified();

				if (moveUp)
				{
					moveFrom = i;
					moveTo = i - 1;
				}

				if (moveDown)
				{
					moveFrom = i;
					moveTo = i + 1;
				}

				if (remove)
					removeIndex = i;

				UI::EndTreeNode();
			}

			ImGui::PopID();
		}

		if (moveFrom != -1 && moveTo != -1)
			effectTable->MoveEffect(moveFrom, moveTo);

		if (removeIndex != -1)
			effectTable->RemoveEffect(removeIndex);
	}

	void AudioMixerEditorPanel::DrawEffect(const Count<AudioEffect>& effect, uint32_t index)
	{
		if (!effect)
			return;

		UI::PushModified(m_NeedsSaving);

		UI::BeginPropertyGrid();

		switch (effect->GetType())
		{
			case AudioEffectType::LowPass:
			{
				auto lowPass = effect.As<AudioLowPassEffect>();

				float cutoff = lowPass->GetCutoffFrequency();
				uint32_t order = lowPass->GetOrder();

				if (UI::AttributeSlider("Cutoff Frequency", cutoff, 20.0f, 20000.0f,
					"Frequencies above this value are reduced. Lower values make the audio sound more muffled.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.0f Hz"))
				{
					lowPass->SetCutoffFrequency(cutoff);
				}

				if (UI::AttributeSlider("Order", order, 1u, 8u,
					"Controls how steep the filter is after the cutoff. 1 is gentle, 2 is a good default, and 8 is extremely steep."))
				{
					lowPass->SetOrder(order);
				}

				break;
			}

			case AudioEffectType::HighPass:
			{
				auto highPass = effect.As<AudioHighPassEffect>();

				float cutoff = highPass->GetCutoffFrequency();
				uint32_t order = highPass->GetOrder();

				if (UI::AttributeSlider("Cutoff Frequency", cutoff, 20.0f, 20000.0f,
					"Frequencies below this value are reduced. Higher values remove more bass and low-frequency content.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.0f Hz"))
				{
					highPass->SetCutoffFrequency(cutoff);
				}

				if (UI::AttributeSlider("Order", order, 1u, 8u,
					"Controls how steep the filter is below the cutoff. 1 is gentle, 2 is a good default, and 8 is extremely steep."))
				{
					highPass->SetOrder(order);
				}

				break;
			}

			case AudioEffectType::Echo:
			{
				auto echo = effect.As<AudioEchoEffect>();

				float wet = echo->GetWet();
				float dry = echo->GetDry();
				float decay = echo->GetDecay();

				UI::AttributeTextBar("Delay", fmt::format("{:.2f} sec", echo->GetDelaySeconds()));

				if (UI::AttributeSlider("Wet", wet, 0.0f, 1.0f,
					"Controls how much of the delayed echo signal is heard."))
				{
					echo->SetWet(wet);
				}

				if (UI::AttributeSlider("Dry", dry, 0.0f, 1.0f,
					"Controls how much of the original unprocessed signal is heard."))
				{
					echo->SetDry(dry);
				}

				if (UI::AttributeSlider("Decay", decay, 0.0f, 1.0f,
					"Controls how quickly repeated echoes lose volume."))
				{
					echo->SetDecay(decay);
				}

				break;
			}

			case AudioEffectType::Equalizer:
			{
				auto equalizer = effect.As<AudioEqualizerEffect>();

				float frequency = equalizer->GetFrequency();
				float gainDB = equalizer->GetGainDB();
				float q = equalizer->GetQ();

				if (UI::AttributeSlider("Frequency", frequency, 20.0f, 20000.0f,
					"The center frequency that this EQ band boosts or cuts.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.0f Hz"))
				{
					equalizer->SetFrequency(frequency);
				}

				if (UI::AttributeSlider("Gain", gainDB, -24.0f, 24.0f,
					"Boosts or cuts the selected frequency range.",
					ImGuiSliderFlags_AlwaysClamp, "%.1f dB"))
				{
					equalizer->SetGainDB(gainDB);
				}

				if (UI::AttributeSlider("Q", q, 0.1f, 20.0f,
					"Controls how narrow the affected frequency range is. Higher Q affects a narrower range.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.2f"))
				{
					equalizer->SetQ(q);
				}

				break;
			}

			case AudioEffectType::LowShelf:
			{
				auto lowShelf = effect.As<AudioLowShelfEffect>();

				float frequency = lowShelf->GetFrequency();
				float gainDB = lowShelf->GetGainDB();
				float q = lowShelf->GetQ();

				if (UI::AttributeSlider("Frequency", frequency, 20.0f, 20000.0f,
					"Sets the frequency where the low-frequency shelf begins.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.0f Hz"))
				{
					lowShelf->SetFrequency(frequency);
				}

				if (UI::AttributeSlider("Gain", gainDB, -24.0f, 24.0f,
					"Boosts or cuts the low frequencies below the shelf frequency.",
					ImGuiSliderFlags_AlwaysClamp, "%.1f dB"))
				{
					lowShelf->SetGainDB(gainDB);
				}

				if (UI::AttributeSlider("Q", q, 0.1f, 20.0f,
					"Controls the shape of the shelf transition.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.2f"))
				{
					lowShelf->SetQ(q);
				}

				break;
			}

			case AudioEffectType::HighShelf:
			{
				auto highShelf = effect.As<AudioHighShelfEffect>();

				float frequency = highShelf->GetFrequency();
				float gainDB = highShelf->GetGainDB();
				float q = highShelf->GetQ();

				if (UI::AttributeSlider("Frequency", frequency, 20.0f, 20000.0f,
					"Sets the frequency where the high-frequency shelf begins.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.0f Hz"))
				{
					highShelf->SetFrequency(frequency);
				}

				if (UI::AttributeSlider("Gain", gainDB, -24.0f, 24.0f,
					"Boosts or cuts the high frequencies above the shelf frequency.",
					ImGuiSliderFlags_AlwaysClamp, "%.1f dB"))
				{
					highShelf->SetGainDB(gainDB);
				}

				if (UI::AttributeSlider("Q", q, 0.1f, 20.0f,
					"Controls the shape of the shelf transition.",
					ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic, "%.2f"))
				{
					highShelf->SetQ(q);
				}

				break;
			}

			case AudioEffectType::None:
			default:
				break;
		}

		UI::EndPropertyGrid();

		UI::PopModified();
	}

	void AudioMixerEditorPanel::CreateGroup(UUID parentID)
	{
		if (!m_AudioMixer)
			return;

		Count<AudioMixerGroup> group = m_AudioMixer->CreateGroup("New Group", parentID);

		if (!group)
			return;

		SelectGroup(group->GetID());

		m_NeedsSaving = true;
	}

	void AudioMixerEditorPanel::DeleteSelectedGroup()
	{
		if (!m_AudioMixer)
			return;

		if (m_SelectedGroupID == m_AudioMixer->GetMasterGroupID())
			return;

		Count<AudioMixerGroup> group = m_AudioMixer->GetGroup(m_SelectedGroupID);

		if (!group)
			return;

		UUID parentID = group->GetParentID();

		if (m_AudioMixer->RemoveGroup(m_SelectedGroupID))
		{
			SelectGroup(parentID);
			m_NeedsSaving = true;
		}
	}

	void AudioMixerEditorPanel::SelectGroup(UUID groupID)
	{
		if (!m_AudioMixer)
			return;

		if (!m_AudioMixer->HasGroup(groupID))
			return;

		m_SelectedGroupID = groupID;
	}

	void AudioMixerEditorPanel::Save()
	{
		if (!m_AudioMixer)
			return;

		AssetManager::SaveAsset(m_AudioMixer->GetID());

		m_NeedsSaving = false;
	}

	float AudioMixerEditorPanel::DecibelsToVolume(float decibels)
	{
		if (decibels <= AudioMixerMinimumDB)
			return 0.0f;

		return std::pow(10.0f, decibels / 20.0f);
	}

	float AudioMixerEditorPanel::VolumeToDecibels(float volume)
	{
		if (volume <= 0.0001f)
			return AudioMixerMinimumDB;

		return 20.0f * std::log10(volume);
	}
}