#include "Proofprch.h"
#include "Sound.h"

#include "AudioEngine.h"
#include "Audio.h"
#include "Proof/Asset/AssetManager.h"
#include "AudioMixer.h"

namespace Proof
{
	static ma_attenuation_model GetMiniAudioAttenuationModel(AttenuationModel attenuationModel)
	{
		switch (attenuationModel)
		{
			case AttenuationModel::Inverse: return ma_attenuation_model_inverse;
			case AttenuationModel::Linear: return ma_attenuation_model_linear;
			case AttenuationModel::Exponential: return ma_attenuation_model_exponential;
			case AttenuationModel::None:
			default: return ma_attenuation_model_none;
		}
	}

	Sound::Sound(const SoundConfiguration& soundConfig)
	{
		UpdateDataSource(soundConfig);
	}

	Sound::~Sound()
	{
		Release();
	}

	void Sound::Release()
	{
		if (m_Initialized)
			ma_sound_uninit(&m_Sound);

		m_Initialized = false;
		m_IsFinished = false;
		m_TotalLength = 0;
		m_State = SoundState::None;
	}

	void Sound::SetMixerGroup(const Count<class AudioMixerGroup> group)
	{
		if (!m_Initialized)
			return;

		if (!group)
		{
			ma_node_attach_output_bus(&m_Sound, 0, ma_engine_get_endpoint(&AudioEngine::GetEngine()), 0);
			return;
		}

		ma_node_attach_output_bus(&m_Sound, 0, group->GetNativeGroup(), 0);
	}

	bool Sound::Play()
	{
		if (!IsReadyToPlay())
			return false;

		if (m_State == SoundState::Play)
		{
			ma_sound_stop(&m_Sound);
			ma_sound_seek_to_pcm_frame(&m_Sound, 0);
		}

		if (m_State == SoundState::End)
			ma_sound_seek_to_pcm_frame(&m_Sound, 0);
		// doign this little faid to avoid  a burp soudn that happens sometiems
		ma_sound_set_fade_in_milliseconds(&m_Sound, 0.0f, 1.0f, 25);

		ma_result result = ma_sound_start(&m_Sound);

		if (result != MA_SUCCESS)
			return false;

		m_State = SoundState::Play;
		m_IsFinished = false;
		return true;
	}

	bool Sound::Pause()
	{
		if (!IsReadyToPlay())
			return false;

		if (m_State != SoundState::Play)
			return false;

		ma_result result = ma_sound_stop(&m_Sound);

		if (result != MA_SUCCESS)
			return false;

		m_State = SoundState::Pause;
		return true;
	}

	bool Sound::Stop()
	{
		if (!IsReadyToPlay())
			return false;

		if (m_State == SoundState::End || m_State == SoundState::None)
			return false;

		ma_result result = ma_sound_stop(&m_Sound);

		if (result != MA_SUCCESS)
			return false;

		ma_sound_seek_to_pcm_frame(&m_Sound, 0);

		m_State = SoundState::End;
		m_IsFinished = true;
		return true;
	}

	bool Sound::Restart()
	{
		if (!IsReadyToPlay())
			return false;

		ma_sound_stop(&m_Sound);
		ma_sound_seek_to_pcm_frame(&m_Sound, 0);

		// doign this little faid to avoid  a burp soudn that happens sometiems
		ma_sound_set_fade_in_milliseconds(&m_Sound, 0.0f, 1.0f, 25);

		ma_result result = ma_sound_start(&m_Sound);

		if (result != MA_SUCCESS)
			return false;

		m_State = SoundState::Play;
		m_IsFinished = false;
		return true;
	}

	bool Sound::IsPlaying() const
	{
		return m_State == SoundState::Play;
	}

	void Sound::Update(float ts)
	{
		
		if (!m_Initialized)
			return;

		if (m_State != SoundState::Play)
			return;

		if (m_Config.Looping)
			return;

		if (ma_sound_at_end(&m_Sound) == MA_TRUE)
		{
			m_State = SoundState::End;
			m_IsFinished = true;

			ma_sound_seek_to_pcm_frame(&m_Sound, 0);

			if (m_OnPlaybackComplete)
				m_OnPlaybackComplete();
		}
	}

	void Sound::UpdateDataSource(const SoundConfiguration& config)
	{
		bool audioChanged = m_Config.Aduio != config.Aduio;
		bool mixerChange = m_Config.Mixer != config.Mixer;
		if (audioChanged)
		{
			if (m_Initialized)
				Release();

			if (config.Aduio == nullptr)
			{
				m_Config = config;
				return;
			}

			AssetInfo info = AssetManager::GetAssetInfo(config.Aduio->GetID());
			std::filesystem::path path = AssetManager::GetAssetFileSystemPath(info.Path);

			bool streaming = false;
			ma_uint32 flags = MA_SOUND_FLAG_DECODE | (streaming ? MA_SOUND_FLAG_STREAM : 0);

			// has a resourc emanager that handles this so we dont always be reloading saame memory
			ma_result result = ma_sound_init_from_file(&AudioEngine::GetEngine(), path.string().c_str(), flags, NULL, NULL, &m_Sound);
			PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to load sound from filepath");

			if (result != MA_SUCCESS)
				return;

			ma_sound_get_length_in_pcm_frames(&m_Sound, &m_TotalLength);

			m_Initialized = true;
			m_IsFinished = false;
			m_State = SoundState::None;
		}

		if (!m_Initialized)
			return;

		if (!audioChanged && m_Config == config)
			return;

		ma_sound_set_volume(&m_Sound, config.VolumeMultiplier);
		ma_sound_set_pitch(&m_Sound, config.PitchMultiplier);
		ma_sound_set_looping(&m_Sound, config.Looping);
		ma_sound_set_spatialization_enabled(&m_Sound, config.SpatializationEnabled);

		if (mixerChange)
			SetMixerGroup(config.Mixer);
		if (config.SpatializationEnabled)
		{
			ma_sound_set_attenuation_model(&m_Sound, GetMiniAudioAttenuationModel(config.AttenuationMod));
			ma_sound_set_min_gain(&m_Sound, config.MinGain);
			ma_sound_set_max_gain(&m_Sound, config.MaxGain);
			ma_sound_set_min_distance(&m_Sound, config.MinDistance);
			ma_sound_set_max_distance(&m_Sound, config.MaxDistance);
			ma_sound_set_rolloff(&m_Sound, config.Rolloff);
			ma_sound_set_cone(&m_Sound, config.ConeInnerAngleInRadians, config.ConeOuterAngleInRadians, config.ConeOuterGain);
			ma_sound_set_doppler_factor(&m_Sound, config.DopplerFactor);
		}

		m_Config = config;
		m_IsLooping = config.Looping;
	}

	float Sound::GetPlaybackPercentage()
	{
		if (!m_Initialized || m_TotalLength == 0)
			return 0.0f;

		ma_uint64 currentFrame = 0;
		ma_sound_get_cursor_in_pcm_frames(&m_Sound, &currentFrame);
		return (float)currentFrame / (float)m_TotalLength;
	}

	uint64_t Sound::GetCurrentPCMFrame()
	{
		if (!m_Initialized)
			return 0;

		ma_uint64 currentFrame = 0;
		ma_sound_get_cursor_in_pcm_frames(&m_Sound, &currentFrame);
		return currentFrame;
	}

	uint64_t Sound::GetTotalPCmFrame()
	{
		return m_TotalLength;
	}

	void Sound::SetTransform(const AudioTransform& transform)
	{
		if (!m_Initialized)
			return;

		if (!m_Config.SpatializationEnabled)
			return;

		ma_sound_set_position(&m_Sound, transform.Position.x, transform.Position.y, transform.Position.z);
		ma_sound_set_direction(&m_Sound, transform.Orientation.x, transform.Orientation.y, transform.Orientation.z);
	}

	void Sound::SetVelocity(const glm::vec3& velocity)
	{
		if (!m_Initialized)
			return;

		ma_sound_set_velocity(&m_Sound, velocity.x, velocity.y, velocity.z);
	}
}