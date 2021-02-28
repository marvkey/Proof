#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <memory>
#include "Application.h"

namespace Proof {
	class Proof_API Log{

	public:
		static void Init();
		static	std::shared_ptr <spdlog::logger>& GetEngineLogger() {
			return EngineLogger;
		}
		static	std::shared_ptr <spdlog::logger>& GetClientLogger() {
			return ClientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger>EngineLogger;
		static std::shared_ptr<spdlog::logger>ClientLogger;
	};
}

// Engine Logging
#define  PF_ENGINE_ERROR(...) ::Proof::Log::GetEngineLogger()->error(__VA_ARGS__)
#define  PF_ENGINE_WARN(...)  ::Proof::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define  PF_ENGINE_INFO(...)  ::Proof::Log::GetEngineLogger()->info(__VA_ARGS__)
#define  PF_ENGINE_TRACE(...) ::Proof::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define  PF_ENGINE_FATAL(...) ::Proof::Log::GetEngineLogger()->fatal(__VA_ARGS__)


// Client Logging
#define  PF_ERROR(...) ::Proof::Log::GetClientLogger()->error(__VA_ARGS__)
#define  PF_WARN(...)  ::Proof::Log::GetClientLogger()->warn(__VA_ARGS__)
#define  PF_INFO(...)  ::Proof::Log::GetClientLogger()->info(__VA_ARGS__)
#define  PF_TRACE(...) ::Proof::Log::GetClientLogger()->trace(__VA_ARGS__)
#define  PF_FATAL(...) ::Proof::Log::GetClientLogger(->fatal(__VA_ARGS__)