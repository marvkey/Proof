#pragma once
#include "Proof/Core/Core.h"
#include <filesystem>
#include <MiniAudio.h>

namespace Proof
{
	class Sound;

	class AudioEngine
	{
	public:
		static void PlaySoundByPath(const std::filesystem::path& path); // mainly for assetManager
		static void PlaySoundByPath(Count<Sound> sound); // mainly for testuigb

		static ma_engine& GetEngine();

	private:
		static void Init();
		static void ShutDown();

		friend class Application;
	};
}