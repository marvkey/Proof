#include "Proofprch.h"
#include "AudioEngine.h"

#include "Sound.h"
#include "Audio.h"
#include "Proof/Asset/AssetManager.h"

namespace Proof
{
	void AudioLogCallBack(void* pUserData, ma_uint32 level, const char* pMessage)
	{
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
	};

	AudioEngineData* s_Data = nullptr;

	void AudioEngine::PlaySoundByPath(const std::filesystem::path& path)
	{
		ma_engine_play_sound(&s_Data->Engine, path.string().c_str(), NULL);
	}

	void AudioEngine::PlaySoundByPath(Count<Sound> sound)
	{
		AssetInfo info = AssetManager::GetAssetInfo(sound->m_Config.Aduio);
		PlaySoundByPath(AssetManager::GetAssetFileSystemPath(info.Path));
	}

	ma_engine& AudioEngine::GetEngine()
	{
		return s_Data->Engine;
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

		ma_engine_set_volume(&s_Data->Engine, 0.5f); // just hafl teh iniital volueme
	}

	void AudioEngine::ShutDown()
	{
		ScopeTimer timer(__FUNCTION__);

		ma_engine_uninit(&s_Data->Engine);
		pdelete s_Data;
	}
}