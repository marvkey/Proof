#pragma once
#include "Core.h"
#include <memory>
#include <Log/Loger.h>

namespace Proof {
	class Proof_API Log {
	public:
		static void Init();
		static Count<Logger::Log>& GetEngineLogger() {
			return EngineLogger;
		}
		static	Count<Logger::Log>& GetClientLogger() {
			return ClientLogger;
		}
	private:
		static Count<Logger::Log>EngineLogger;
		static Count<Logger::Log>ClientLogger;
	};
}
#ifdef PF_ENABLE_DEBUG
	// Engine Logging
	#define  PF_ENGINE_ERROR(...) ::Proof::Log::GetEngineLogger()->LogError(__VA_ARGS__)
	#define  PF_ENGINE_WARN(...)  ::Proof::Log::GetEngineLogger()->LogWarn(__VA_ARGS__)
	#define  PF_ENGINE_INFO(...)  ::Proof::Log::GetEngineLogger()->LogInfo(__VA_ARGS__)
	#define  PF_ENGINE_TRACE(...) ::Proof::Log::GetEngineLogger()->LogTrace(__VA_ARGS__)
	#define	 PF_ENGINE_CRITICAL(...)::Proof::Log::GetEngineLogger()->LogCritical(__VA_ARGS__)
/*
	// Client Logging
	#define  PF_ERROR(...) ::Proof::Loger::GetClientLogger()->error(__VA_ARGS__)
	#define  PF_WARN(...)  ::Proof::Loger::GetClientLogger()->warn(__VA_ARGS__)
	#define  PF_INFO(...)  ::Proof::Loger::GetClientLogger()->info(__VA_ARGS__)
	#define  PF_TRACE(...) ::Proof::Loger::GetClientLogger()->trace(__VA_ARGS__)
*/
#else
	#define  PF_ENGINE_ERROR(...) ::Proof::Log::GetEngineLogger()
	#define  PF_ENGINE_WARN(...)  ::Proof::Log::GetEngineLogger()
	#define  PF_ENGINE_INFO(...)  ::Proof::Log::GetEngineLogger()
	#define  PF_ENGINE_TRACE(...) ::Proof::Log::GetEngineLogger()
	#define	 PF_ENGINE_CRITICAL(...)::Proof::Log::GetEngineLogger()
#endif