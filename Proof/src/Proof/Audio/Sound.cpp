#include "Proofprch.h"
#include "Sound.h"
#include "AudioEngine.h"
#include "Audio.h"
#include "Proof/Asset/AssetManager.h"
namespace Proof
{

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
	}
	bool Sound::Play()
	{
		if (!IsReadyToPlay())return false;
		//PF_ENGINE_TRACE("Old state: {} ",EnumReflection::EnumString( m_State));
		ma_result result = MA_ERROR;
		switch (m_State)
		{
			case Proof::SoundState::Stopped:
				{
					m_IsFinished = false;
					result = ma_sound_start(&m_Sound);
					m_State = SoundState::Starting;
				}
				break;
			case Proof::SoundState::Starting:
				{
					PF_CORE_ASSERT(false);
				}
				break;
			case Proof::SoundState::Playing:
				{
					StopNow(false, true);
					m_IsFinished = false;
					result = ma_sound_start(&m_Sound);
					m_State = SoundState::Starting;
				}
				break;
			case Proof::SoundState::Pausing:
				{
					StopNow(false, false);
					result = ma_sound_start(&m_Sound);
					m_State = SoundState::Starting;
				}
				break;
			case Proof::SoundState::Paused:
				{
					// Prepare fade-in for un-pause
					ma_sound_set_fade_in_milliseconds(&m_Sound, 0.0f, m_StoredFaderValue, STOPPING_FADE_MS / 2);
					result = ma_sound_start(&m_Sound);
					m_State = SoundState::Starting;
				}
				break;
			case Proof::SoundState::Stopping:
				{
					StopNow(true, true);
					result = ma_sound_start(&m_Sound);
					m_State = SoundState::Starting;
				}
				break;
			case Proof::SoundState::FadingOut:
				break;
			case Proof::SoundState::FadingIn:
				break;
			default:
				break;
		}
		PF_CORE_ASSERT(result == MA_SUCCESS);
		return result == MA_SUCCESS;
	}
	bool Sound::Stop()
	{
		bool result = true;
		switch (m_State)
		{
			case SoundState::Stopped:
				m_IsFinished = true;
				result = false;
				break;
			case SoundState::Starting:
				StopNow(false, true); // consider stop-fading
				m_State = SoundState::Stopping;
				break;
			case SoundState::Playing:
				result = StopFade(STOPPING_FADE_MS);
				m_State = SoundState::Stopping;
				break;
			case SoundState::Pausing:
				StopNow(true, true);
				m_State = SoundState::Stopping;
				break;
			case SoundState::Paused:
				StopNow(true, true);
				m_State = SoundState::Stopping;
				break;
			case SoundState::Stopping:
				StopNow(true, true);
				break;
			case SoundState::FadingOut:
				break;
			case SoundState::FadingIn:
				break;
			default:
				break;
		}
		m_LastFadeOutDuration = (float)STOPPING_FADE_MS / 1000.0f;
		return result;
	}
	bool Sound::Pause()
	{

		bool result = true;

		switch (m_State)
		{
			case SoundState::Playing:
				result = StopFade(STOPPING_FADE_MS);
				m_State = SoundState::Pausing;
				break;
			case SoundState::FadingOut:
				break;
			case SoundState::FadingIn:
				break;
			default:
				// If was called to Pause while not playing
				m_State = SoundState::Paused;
				result = true;
				break;
		}
		return result;
	}
	bool Sound::IsPlaying() const
	{
		return m_State != SoundState::Stopped && m_State != SoundState::Paused;
	}
	bool Sound::IsStopping() const
	{
		return m_State == SoundState::Stopping;
	}
	void Sound::Update(float ts)
	{
		auto notifyIfFinished = [&]
		{
			if (ma_sound_at_end(&m_Sound) == MA_TRUE && m_OnPlaybackComplete)
				m_OnPlaybackComplete();
		};

		auto isNodePlaying = [&]
		{
			return ma_sound_is_playing(&m_Sound) == MA_TRUE;
		};

		auto isFadeFinished = [&]
		{
			return m_StopFadeTime <= 0.0;
		};

		m_StopFadeTime = std::max(0.0f, m_StopFadeTime - ts);

		switch (m_State)
		{
			case SoundState::Stopped:
				break;
			case SoundState::Starting:
				if (isNodePlaying())
				{
					m_State = SoundState::Playing;
				}
				break;
			case SoundState::Playing:
				if (ma_sound_is_playing(&m_Sound) == MA_FALSE)
				{
					m_State = SoundState::Stopped;
					m_IsFinished = true;
					notifyIfFinished();
				}
				break;
			case SoundState::Pausing:
				if (isFadeFinished())
				{
					StopNow(false, false);
					m_State = SoundState::Paused;
				}
				break;
			case SoundState::Paused:
				break;
			case SoundState::Stopping:
				if (isFadeFinished())
				{
					StopNow(true, true);
					m_State = SoundState::Stopped;
				}
				break;
			case SoundState::FadingOut:
				break;
			case SoundState::FadingIn:
				break;
			default:
				break;
		}
	}

	void Sound::UpdateDataSource(const SoundConfiguration& config)
	{
		

		if (m_Config.Aduio != config.Aduio )
		{
			if (m_Initialized)
				Release();
			ma_result result;
			std::filesystem::path path = config.Aduio->GetPath();
			//PF_ENGINE_INFO("{}", path.string());
			result = ma_sound_init_from_file(&AudioEngine::GetEngine(), path.string().c_str(), 0, NULL, NULL, &m_Sound);
			PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to load sound from filepath");


			ma_sound_set_volume(&m_Sound, config.VolumeMultiplier);
			ma_sound_set_pitch(&m_Sound, config.PitchMultiplier);

			ma_sound_set_looping(&m_Sound, config.Looping);
			ma_sound_set_spatialization_enabled(&m_Sound, config.SpatializationEnabled);
			{
				ma_sound_get_length_in_pcm_frames(&m_Sound, &m_TotalLength);
			}


			m_Config = config;

			m_Initialized = true;
		}
		if (m_Initialized == false)return;
		if (m_Config == config)return;
		m_Config = config;

		ma_sound_set_volume(&m_Sound, config.VolumeMultiplier);
		ma_sound_set_pitch(&m_Sound, config.PitchMultiplier);

		ma_sound_set_looping(&m_Sound, config.Looping);
		ma_sound_set_spatialization_enabled(&m_Sound, config.SpatializationEnabled);

		if (config.SpatializationEnabled == false)
			return;

		ma_sound_set_min_gain(&m_Sound, config.MinGain);
		ma_sound_set_max_gain(&m_Sound, config.MaxGain);

		ma_sound_set_min_distance(&m_Sound, config.MinDistance);
		ma_sound_set_max_distance(&m_Sound, config.MaxDistance);

		ma_sound_set_cone(&m_Sound, config.ConeInnerAngleInRadians, config.ConeOuterAngleInRadians, config.ConeOuterGain);
		ma_sound_set_doppler_factor(&m_Sound, config.DopplerFactor);

		ma_sound_set_rolloff(&m_Sound, config.Rolloff);


	}
	float Sound::GetPlaybackPercentage()
	{
		ma_uint64 currentFrame;
		ma_sound_get_cursor_in_pcm_frames(&m_Sound, &currentFrame);

		return (float)currentFrame / (float)m_TotalLength;
	}
	uint64_t Sound::GetCurrentPCMFrame()
	{
		ma_uint64 currentFrame;
		ma_sound_get_cursor_in_pcm_frames(&m_Sound, &currentFrame);

		return currentFrame;
	}
	uint64_t Sound::GetTotalPCmFrame()
	{
		return m_TotalLength;
	}
	
	bool Sound::StopFade(uint64_t numSamples)
	{
		constexpr double stopFadeTime = (double)STOPPING_FADE_MS * 1.1 / 1000.0;
		m_StopFadeTime = (float)stopFadeTime;

		// Negative volumeBeg starts the fade from the current volume
		ma_sound_set_fade_in_pcm_frames(&m_Sound, -1.0f, 0.0f, numSamples);

		return true;
	}
	bool Sound::StopFade(int milliseconds)
	{
		PF_CORE_ASSERT(milliseconds > 0);

		const uint64_t fadeInFrames = (milliseconds * m_Sound.engineNode.fader.config.sampleRate) / 1000;

		return StopFade(fadeInFrames);
	}
	void Sound::StopNow(bool notifyPlaybackComplete, bool resetPlaybackPosition)
	{
		ma_sound_stop(&m_Sound);

		if (resetPlaybackPosition)
		{
			// Reset data source read position to the beginning of the data
			ma_sound_seek_to_pcm_frame(&m_Sound, 0);

			// Mark this voice to be released.
			m_IsFinished = true;

			PF_CORE_ASSERT(m_State != SoundState::Starting);
			m_State = SoundState::Stopped ;
		}
		m_Sound.engineNode.fader.volumeEnd = 1.0f;

		// Need to notify AudioEngine of completion,
		// if this is one shot, AudioComponent needs to be destroyed.
		if (notifyPlaybackComplete && m_OnPlaybackComplete)
			m_OnPlaybackComplete();
	}
	void Sound::SetTransform(const AudioTransform& transform)
	{
		if (!m_Config.SpatializationEnabled)
		{
			return;
		}

		ma_sound_set_position(&m_Sound, transform.Position.x, transform.Position.y, transform.Position.z);
		ma_sound_set_direction(&m_Sound,transform.Orientation.x, transform.Orientation.y, transform.Orientation.z);
		//ma_sound_set_up(&AudioEngine::GetEngine(), 0, transform.Up.x, transform.Up.y, transform.Up.z);
	}
	void Sound::SetVelocity(const glm::vec3& velocity)
	{
		if (m_Initialized == false)
			return;

		ma_sound_set_velocity(&m_Sound, velocity.x, velocity.y, velocity.z);
	}
}

