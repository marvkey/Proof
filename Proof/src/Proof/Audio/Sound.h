#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include <MiniAudio.h>
#include "Proof/Core/UUID.h"
#include "AudioTools.h"
namespace Proof
{
    
	class Sound : public RefCounted
	{
    public:
        Sound(const SoundConfiguration& soundConfig);
        Sound() {};
        ~Sound();
        virtual bool Play();
        virtual bool Stop();
        virtual bool Pause();
        virtual bool IsPlaying() const;
        virtual bool IsStopping() const;
        SoundState GetState() const { return m_State; };
        void Update(float ts);

        bool IsReadyToPlay() const { return m_Initialized; }
        void SetTransform(const AudioTransform& transform);
        void SetVelocity(const glm::vec3& velocity = { 0.0f, 0.0f, 0.0f });
        void UpdateDataSource(const SoundConfiguration& soundConfig);
        virtual bool IsLooping() const { return m_IsLooping; };

        float GetPlaybackPercentage();

        uint64_t GetCurrentPCMFrame();
        uint64_t GetTotalPCmFrame();
    private:

        void Release();
        bool StopFade(uint64_t numSamples);

       /* Stop playback with short fade-out to prevent click.
          @param milliseconds - length of the fade-out in milliseconds

          @returns true - if successfully initialized fade
       */
        bool StopFade(int milliseconds);

        void StopNow(bool notifyPlaybackComplete = true, bool resetPlaybackPosition = true);
    private:

        UUID m_UUID = { 0 };
        std::function<void()> m_OnPlaybackComplete;
        ma_sound m_Sound;
            
        SoundState m_State{ SoundState::Stopped };
        SoundConfiguration m_Config;
        bool m_IsLooping = false;
        bool m_IsFinished = false;

        bool m_Initialized = false;

        float m_StoredFaderValue = 1.0f;
        float m_LastFadeOutDuration = 0.0f;

           /* Stop-fade counter. Used to stop the sound after "stopping-fade" has finished. */
        float m_StopFadeTime = 0.0f;
        uint64_t m_TotalLength;

        friend class AudioEngine;
	};
}