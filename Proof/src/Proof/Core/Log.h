
#pragma once
#include "Core.h"
#include <memory>
#include <Log/Loger.h>
#include <unordered_map>
namespace Proof
{
	class Proof_API Log {
	public:
		static void Init();
		static const Count<Logger::Log>& GetEngineLogger() {
			return EngineLogger;
		}
		static const Count<Logger::Log>& GetClientLogger() {
			return ClientLogger;
		}
		static enum LogType {
			ERROR,
			WARN,
			INFO,
			TRACE,
			CRITICAL
		};
		static void AppendString(uint32_t type,const std::string& temp) {
			if(m_PauseLog)return; 
			Logs.insert({Inputposition(),{(LogType)type,temp}});
			NewLog = true;
		}
		static std::unordered_map<uint16_t,std::pair<LogType,std::string>> Logs;
		static bool NewLog;
		static bool m_PauseLog;
	private:
		static Count<Logger::Log>EngineLogger;
		static Count<Logger::Log>ClientLogger;
		static uint16_t Inputposition() {
			return Logs.size() + 1;
		}
	};
}

#ifdef PF_ENABLE_DEBUG 

//#define USING_PROOF_LOGGER
#ifdef USING_PROOF_LOGGER
#define  PF_ENGINE_ERROR(...) ::Proof::Log::AppendString(0,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define  PF_ENGINE_WARN(...)  ::Proof::Log::AppendString(1,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define  PF_ENGINE_INFO(...)  Proof::Log::AppendString(2,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define  PF_ENGINE_TRACE(...) ::Proof::Log::AppendString(3,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define	 PF_ENGINE_CRITICAL(...)::Proof::Log::AppendString(4,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))

		// Client Logging
#define  PF_ERROR(...) ::Proof::Log::AppendString(0,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define  PF_WARN(...)  ::Proof::Log::AppendString(1,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define  PF_INFO(...)  ::Proof::Log::AppendString(2,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define  PF_TRACE(...) ::Proof::Log::AppendString(3,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#define	 PF_CRITICAL(...)::Proof::Log::AppendString(4,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
//#define	 PF_ENGINE_TEST(...)::Proof::Log::AppendString(Proof::Log::LogType::CRITICAL,::Proof::Log::GetEngineLogger()->GetLogString(__VA_ARGS__))
#else
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
#endif // USING_PROOF_LOGGER



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
