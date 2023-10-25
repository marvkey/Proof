#include "Proofprch.h"
#include "AudioEngine.h"

#include "Sound.h"
#include "Audio.h"
#include "Audio.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "AudioListenerActor.h"
#include "Proof/Scene/Component.h"

#include "Proof/Asset/AssetManager.h"
#include "AudioUtils.h"
namespace Proof
{
    void AudioLogCallBack(void* pUserData, ma_uint32 level, const char* pMessage) {
        std::string message = fmt::format("{0}: {1}", std::string(ma_log_level_to_string(level)), pMessage);
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
        switch (level)
        {
            case MA_LOG_LEVEL_INFO:
                PF_ENGINE_INFO("[miniaudio] {}", message);
                break;
            case MA_LOG_LEVEL_WARNING:
                PF_ENGINE_WARN("[miniaudio] {}", message);
                break;
            case MA_LOG_LEVEL_ERROR:
                PF_ENGINE_ERROR("[miniaudio] {}", message);
                break;
            default:
                PF_ENGINE_TRACE("[miniaudio] {}", message);
        }
    }
    struct AudioEngineData 
    {
        ma_engine Engine;
        ma_log MaLog;

        Count<World> WorldContext = nullptr;
        std::unordered_map<UUID, Count<Sound>> WorldSounds;
        std::array< Count<AudioListenerActor>, MA_ENGINE_MAX_LISTENERS> AudioListeners;
        uint32_t AudioListenersCount = 0;
    };
    AudioEngineData* s_Data = nullptr;
    void AudioEngine::PlaySoundByPath(const std::filesystem::path& path)
    {
        ma_engine_play_sound(&s_Data->Engine, path.string().c_str(), NULL);
    }
    ma_engine& AudioEngine::GetEngine() 
    {
        return s_Data->Engine;
    }
    
    static Count<Sound> AddAudio(Entity entity)
    {
        //PF_CORE_ASSERT(s_Data->WorldSounds.contains(entity.GetUUID()), "Cannot have multiple sounds with the same UUID");
        auto& audioComp = entity.GetComponent<AudioComponent>();
        Count<Sound> sound;
        if (AssetManager::HasAsset(audioComp.AudioAsset))
        {
           SoundConfiguration soundConfig = Utils::AudioComponentToSoundConfig(audioComp);
            sound = Count<Sound>::Create(soundConfig);

            if (audioComp.PlayOnAwake)
                sound->Play();
        }
        else
            sound = Count<Sound>::Create();

        s_Data->WorldSounds[entity.GetUUID()] = sound;
        return sound;
    }

    void AudioEngine::BeginContext(Count<World> world)
    {
        s_Data->WorldContext = world;

        auto view = world->GetAllEntitiesWith<AudioComponent>();

        for (auto e : view)
        {
            Entity entity{ e,world.Get() };

            AddAudio(entity)->m_UUID = entity.GetUUID();
        }
        s_Data->AudioListeners[0] = Count<AudioListenerActor>::Create();
        s_Data->AudioListeners[0]->m_IndexPosition = 0;

        s_Data->AudioListenersCount++;
    }
    void AudioEngine::OnUpdate(float deltaTime)
    {
        PF_PROFILE_FUNC();

        if (s_Data->WorldContext)
        {

            for (auto& [id, sound] : s_Data->WorldSounds)
            {
                sound->Update(deltaTime);
            }
        }
    }
    void AudioEngine::EndContext()
    {
        s_Data->AudioListeners = {};
        s_Data->AudioListenersCount = 0;
        s_Data->WorldSounds.clear();

        s_Data->WorldContext = nullptr;
    }
    void AudioEngine::Init()
	{
        ScopeTimer timer(__FUNCTION__);
        s_Data = pnew AudioEngineData();
        ma_result result;

        ma_engine_config engineConfig;

        ma_log_callback logCallback = ma_log_callback_init(&AudioLogCallBack, nullptr);
        result = ma_log_register_callback(&s_Data->MaLog, logCallback);
        PF_CORE_ASSERT(result == MA_SUCCESS, "Failed to register miniaudio log callback.");

        engineConfig.pLog = &s_Data->MaLog;
        engineConfig.channels = 2;

        engineConfig = ma_engine_config_init();
        engineConfig.listenerCount = 1; // a device typically has one sound source
        result = ma_engine_init(&engineConfig, &s_Data->Engine);

        if (result != MA_SUCCESS)
        {
            PF_CORE_ASSERT(false);
        }
	}
	void AudioEngine::ShutDown()
	{
        ScopeTimer timer(__FUNCTION__);
        ma_engine_uninit(&s_Data->Engine);
        pdelete s_Data;
	}


    void AudioEngine::UpdateListenerPosition(const AudioTransform& transform)
    {
        PF_CORE_ASSERT(s_Data->WorldContext);
        s_Data->AudioListeners[0]->UpdateTransform(transform);
    }
    void AudioEngine::UpdateListenerConeAttenuation(float innerAngleRadians, float outerAngleRadians, float outerGrain)
    {
        PF_CORE_ASSERT(s_Data->WorldContext);
        s_Data->AudioListeners[0]->UpdateConeListener(innerAngleRadians,outerAngleRadians,outerGrain);
    }
    void AudioEngine::UpdateAudioListenerVelocity(const glm::vec3& velocity)
    {
        PF_CORE_ASSERT(s_Data->WorldContext);
        s_Data->AudioListeners[0]->UpdateVelocity(velocity);
    }
    void AudioEngine::UpdateAudio(UUID soundId, const SoundConfiguration& soundConfiguration)
    {
        PF_CORE_ASSERT(s_Data->WorldContext);

        if (!s_Data->WorldSounds.contains(soundId))
        {
            PF_ENGINE_ERROR("Trying to update entity {} doesnt have sound ", s_Data->WorldContext->GetEntity(soundId).GetName());
            return;
        }

        auto sound = s_Data->WorldSounds[soundId];
        sound->UpdateDataSource(soundConfiguration);
    }
    void AudioEngine::UpdateAudioTransform(UUID soundId, const AudioTransform& transform)
    {
        PF_CORE_ASSERT(s_Data->WorldContext);

        if (!s_Data->WorldSounds.contains(soundId))
        {
            PF_ENGINE_ERROR("Trying to update entity {} doesnt have sound ", s_Data->WorldContext->GetEntity(soundId).GetName());
            return;
        }

        auto sound = s_Data->WorldSounds[soundId];
        sound->SetTransform(transform);
    }
    void AudioEngine::UpdateAudioVelocity(UUID soundId, const glm::vec3& velocity)
    {
        PF_CORE_ASSERT(s_Data->WorldContext);

        if (!s_Data->WorldSounds.contains(soundId))
        {
            PF_ENGINE_ERROR("Trying to update entity {} doesnt have sound ", s_Data->WorldContext->GetEntity(soundId).GetName());
            return;
        }
        auto sound = s_Data->WorldSounds[soundId];
        sound->SetVelocity(velocity);
    }
}

