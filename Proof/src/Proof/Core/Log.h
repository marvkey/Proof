#pragma once
#include "Core.h"
#include <memory>
#include <Log/Loger.h>
#include <unordered_map>
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
		static enum LogType{
			ERROR,
			WARN,
			INFO,
			TRACE,
			CRITICAL
		};
	private:
		static Count<Logger::Log>EngineLogger;
		static Count<Logger::Log>ClientLogger;
		static std::unordered_map<LogType,const std::string&> Logs;
	};
}
#ifdef PF_ENABLE_DEBUG 
	// Engine Logging
	#define  PF_ENGINE_ERROR(...) ::Proof::Log::GetEngineLogger()->LogError(__VA_ARGS__)
	#define  PF_ENGINE_WARN(...)  ::Proof::Log::GetEngineLogger()->LogWarn(__VA_ARGS__)
	#define  PF_ENGINE_INFO(...)  ::Proof::Log::GetEngineLogger()->LogInfo(__VA_ARGS__)
	#define  PF_ENGINE_TRACE(...) ::Proof::Log::GetEngineLogger()->LogTrace(__VA_ARGS__)
	#define	 PF_ENGINE_CRITICAL(...)::Proof::Log::GetEngineLogger()->LogCritical(__VA_ARGS__)
	
	// Client Logging
	#define  PF_ERROR(...) ::Proof::Log:: GetClientLogger()->LogError(__VA_ARGS__)
	#define  PF_WARN(...)  ::Proof::Log:: GetClientLogger()->LogWarn(__VA_ARGS__)
	#define  PF_INFO(...)  ::Proof::Log:: GetClientLogger()->LogInfo(__VA_ARGS__)
	#define  PF_TRACE(...) ::Proof::Log:: GetClientLogger()->LogTrace(__VA_ARGS__)
	#define	 PF_CRITICAL(...)::Proof::Log::GetClientLogger()->LogCritical(__VA_ARGS__)

#else
	#define  PF_ENGINE_ERROR(...)
	#define  PF_ENGINE_WARN(...) 
	#define  PF_ENGINE_INFO(...) 
	#define  PF_ENGINE_TRACE(...)
	#define	 PF_ENGINE_CRITICAL(...)

	#define  PF_ERROR(...) 
	#define  PF_WARN(...)  
	#define  PF_INFO(...)  
	#define  PF_TRACE(...) 
	#define	 PF_ENGINE_CRITICAL(...)
#endif
