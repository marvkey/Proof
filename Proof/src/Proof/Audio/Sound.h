#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include "AudioTools.h"

#include <glm/glm.hpp>
#include <MiniAudio.h>

namespace Proof
{


	class Sound : public RefCounted
	{
	public:
		Sound(const SoundConfiguration& soundConfig);
		Sound() {};
		~Sound();

		bool Play();
		bool Pause();
		bool Stop();
		bool Restart();

		bool IsPlaying() const;
		SoundState GetState() const { return m_State; }

		void Update(float ts);

		bool IsReadyToPlay() const { return m_Initialized; }
		bool IsFinished() const { return m_IsFinished; }

		void SetTransform(const AudioTransform& transform);
		void SetVelocity(const glm::vec3& velocity = { 0.0f, 0.0f, 0.0f });

		void UpdateDataSource(const SoundConfiguration& soundConfig);

		float GetPlaybackPercentage();

		uint64_t GetCurrentPCMFrame();
		uint64_t GetTotalPCmFrame();

	private:
		void Release();

	private:
		UUID m_UUID = { 0 };

		std::function<void()> m_OnPlaybackComplete;

		ma_sound m_Sound{};

		SoundState m_State = SoundState::None;
		SoundConfiguration m_Config;

		bool m_IsLooping = false;
		bool m_IsFinished = false;
		bool m_Initialized = false;

		uint64_t m_TotalLength = 0;

		friend class AudioEngine;
		friend class AudioWorld;
	};
}