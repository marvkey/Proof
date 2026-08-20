#include "Proofprch.h"
#include "AudioControllerEditorPanel.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/Audio/Sound.h"
#include "Proof/Audio/AudioController.h"
#include "Proof/Audio/AudioUtils.h"
#include "Proof/Imgui/UI.h"
#include "Proof/Imgui/UIHandlers.h"
#include "Proof/ImGui/UiUtilities.h"
#include "Proof/Scene/Component.h"

namespace  Proof
{
    AudioControllerEditorPanel::AudioControllerEditorPanel()
    :
        AssetEditor("AudioControllerEditorPanel")
    {
    }
	
	void AudioControllerEditorPanel::OnImGuiRender()
    {
    	UI::ScopedStyleVar padding(ImGuiStyleVar_WindowPadding, ImVec2{ 6,6 });

    	ImGui::Begin("Audio Controller");

    	if (!m_AudioController)
    	{
    		ImGui::TextDisabled("No audio controller loaded.");
    		ImGui::End();
    		return;
    	}

    	// ───────────── Playback ─────────────
    	ImGui::BeginChild("Audio Controller Playback", ImVec2(0.0f, 120.0f), true);
    	DrawPlaybackPanel();
    	ImGui::EndChild();

    	ImGui::Spacing();

    	// ───────────── Properties ─────────────
    	ImGui::BeginChild("Audio Controller Properties", ImVec2(0.0f, 0.0f), true);
    	DrawProperties();
    	ImGui::EndChild();

    	ImGui::End();
    }

	void AudioControllerEditorPanel::DrawPlaybackPanel()
    {
    	ImGui::TextUnformatted("Preview");
    	ImGui::Separator();

    	if (!m_Sound)
    	{
    		ImGui::TextDisabled("No audio preview available.");
    		return;
    	}

    	AudioComponent component;
    	component.AudioController = m_AudioController->GetID();
    	m_Sound->UpdateDataSource(Utils::AudioComponentToSoundConfig(component));

    	if (!m_Sound->IsReadyToPlay())
    	{
    		ImGui::TextDisabled("No audio preview available.");
    		return;
    	}

    	SoundState state = m_Sound->GetState();

    	bool isPlaying = state == SoundState::Play;
    	bool canStop = state == SoundState::Play || state == SoundState::Pause;

    	const float spacing = ImGui::GetStyle().ItemSpacing.x;
    	const float buttonWidth = (ImGui::GetContentRegionAvail().x - spacing * 2.0f) / 3.0f;

    	if (ImGui::Button(isPlaying ? "Pause" : "Play", ImVec2(buttonWidth, 0.0f)))
    	{
    		if (isPlaying)
    			m_Sound->Pause();
    		else
    			m_Sound->Play();
    	}

    	ImGui::SameLine();

    	if (ImGui::Button("Restart", ImVec2(buttonWidth, 0.0f)))
    		m_Sound->Restart();

    	ImGui::SameLine();

    	ImGui::BeginDisabled(!canStop);

    	if (ImGui::Button("Stop", ImVec2(buttonWidth, 0.0f)))
    		m_Sound->Stop();

    	ImGui::EndDisabled();

    	ImGui::Separator();

    	float playbackPercentage = m_Sound->GetPlaybackPercentage();

    	ImGui::ProgressBar(playbackPercentage, ImVec2(-1.0f, 0.0f));
    }

	void AudioControllerEditorPanel::RestartPreview()
    {
    	if (!m_Sound)
    		return;

    	m_Sound->Stop();
    	m_Sound->Play();
    }


	void AudioControllerEditorPanel::DrawProperties()
	{
		bool changed = false;

				// ───────────── Source ─────────────
		{
			UI::BeginPropertyGrid();

			AssetID audioSource = m_AudioController->AudioSource.GetAssetID();

			UI::AttributeAssetReference("Audio Source", AssetType::Audio, audioSource);
			UI::AttributeAudioMixerGroupKeyReference("Audio Mixer",m_AudioController->MixerKey);

			if (audioSource != m_AudioController->AudioSource.GetAssetID())
			{
				m_AudioController->AudioSource.SetAssetID(audioSource);
				changed = true;
			}

			UI::EndPropertyGrid();
		}

		// ───────────── Pitch ─────────────
		if (UI::AttributeTreeNode("Pitch", true, 1, 1))
		{
			UI::BeginPropertyGrid();

			changed |= UI::AttributeDrag("Min Pitch", m_AudioController->MinPitch, 0.01f, 0.0f, 0.0f,
				"The minimum pitch multiplier that can be used. 1.0 is the original pitch, values below 1.0 lower the pitch, and values above 1.0 raise it.");

			changed |= UI::AttributeDrag("Max Pitch", m_AudioController->MaxPitch, 0.01f, 0.0f, 0.0f,
				"The maximum pitch multiplier that can be used. If Min Pitch and Max Pitch are different, a pitch can be selected between the two values.");

			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}

		// ───────────── Effects ─────────────
		if (UI::AttributeTreeNode("Effects", true, 1, 1))
		{
			UI::BeginPropertyGrid();

			changed |= UI::AttributeSlider("Master Reverb Send", m_AudioController->MasterReverbSend, 0.0f, 1.0f,
				"Controls how much of this sound is sent to the master reverb effect. 0.0 sends no signal and 1.0 sends the maximum amount.");

			changed |= UI::AttributeSlider("Low Pass Filter", m_AudioController->LowPassFilter, 0.0f, 1.0f,
				"Controls the low-pass filter applied to the sound. Lower values remove more high-frequency detail, making the sound more muffled.");

			changed |= UI::AttributeSlider("High Pass Filter", m_AudioController->HighPassFilter, 0.0f, 1.0f,
				"Controls the high-pass filter applied to the sound. Higher values remove more low-frequency content from the sound.");

			UI::EndPropertyGrid();
			UI::EndTreeNode();
		}

		// ───────────── Spatialization ─────────────
		if (UI::AttributeTreeNode("Spatialization", true, 1, 1))
		{
			UI::BeginPropertyGrid();

			changed |= UI::AttributeBool("Enabled", m_AudioController->SpatializationEnabled);

			UI::EndPropertyGrid();

			if (m_AudioController->SpatializationEnabled)
			{
				UI::BeginPropertyGrid();

				AttenuationModel oldAttenuationModel = m_AudioController->AttenuationMod;

				UI::EnumCombo("Attenuation Model", m_AudioController->AttenuationMod);

				if (oldAttenuationModel != m_AudioController->AttenuationMod)
					changed = true;

				changed |= UI::AttributeSlider("Min Gain", m_AudioController->MinGain, 0.0f, 1.0f,
					"The minimum volume the sound can reach after distance attenuation is applied.");

				changed |= UI::AttributeSlider("Max Gain", m_AudioController->MaxGain, 0.0f, 1.0f,
					"The maximum volume the sound can reach after distance attenuation is applied.");

				changed |= UI::AttributeDrag("Min Distance", m_AudioController->MinDistance, 0.1f, 0.0f, 0.0f,
					"The distance from the listener where attenuation begins. Inside this distance the sound remains at its maximum gain.");

				changed |= UI::AttributeDrag("Max Distance", m_AudioController->MaxDistance, 0.1f, 0.0f, 0.0f,
					"The distance used as the far range of the sound's attenuation.");

				changed |= UI::AttributeSlider("Doppler Factor", m_AudioController->DopplerFactor, 0.0f, 1.0f,
					"Controls the strength of the Doppler effect caused by movement between the sound and listener. 0.0 disables the effect.");

				changed |= UI::AttributeSlider("Rolloff", m_AudioController->Rolloff, 0.0f, 1.0f,
					"Controls how quickly the sound becomes quieter as distance from the listener increases. Higher values cause stronger distance attenuation.");

				UI::EndPropertyGrid();

				// ───────────── Cone ─────────────
				if (UI::AttributeTreeNode("Cone", false, 1, 1))
				{
					UI::BeginPropertyGrid();

					float degrees = Math::Degrees(m_AudioController->ConeInnerAngleInRadians);

					changed |= UI::AttributeSlider("Inner Angle", degrees, 0.0f, 360.0f,
						"The inner angle of the directional sound cone. Inside this cone the sound plays at its normal gain.");

					m_AudioController->ConeInnerAngleInRadians = Math::Radian(degrees);

					float degrees2 = Math::Degrees(m_AudioController->ConeOuterAngleInRadians);

					changed |= UI::AttributeSlider("Outer Angle", degrees2, 0.0f, 360.0f,
						"The outer angle of the directional sound cone. Between the inner and outer angles the sound transitions toward the Outer Gain.");

					m_AudioController->ConeOuterAngleInRadians = Math::Radian(degrees2);

					changed |= UI::AttributeSlider("Outer Gain", m_AudioController->ConeOuterGain, 0.0f, 1.0f,
						"The volume multiplier used when the listener is outside the sound's outer cone. 0.0 is silent and 1.0 keeps full volume.");

					UI::EndPropertyGrid();
					UI::EndTreeNode();
				}
			}

			UI::EndTreeNode();
		}

		if (changed)
			m_NeedsSaving = true;
	}

	void AudioControllerEditorPanel::OnUpdate(FrameTime ts)
	{
		AssetEditor::OnUpdate(ts);
	}

	void AudioControllerEditorPanel::SetAsset(const Count<class Asset>& asset)
    {
        if (asset->GetAssetType() != AssetType::AudioController)
        {
            PF_ENGINE_ERROR("Cannot pass {} Asset to MaterialEditorPanel {}", EnumReflection::EnumString(asset->GetAssetType()), m_TitleAndId);
            return;
        }

        m_AudioController = asset.As<AudioController>();
        AudioComponent component;
        component.AudioController = m_AudioController->GetID();
        m_Sound = Count<Sound>::Create(Utils::AudioComponentToSoundConfig(component));
    }

    void AudioControllerEditorPanel::Save()
    {
        if (!m_AudioController)
            return;

        AssetManager::SaveAsset(m_AudioController->GetID());
        m_NeedsSaving = false;
    }
}
