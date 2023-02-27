
#pragma once
#include <memory>
#include <Log/Loger.h>
#include <unordered_map>
#include "SmartPointer.h"
namespace Proof
{
	class Log {
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
		static std::unordered_map<uint32_t,std::pair<LogType,std::string>> Logs;
		static bool NewLog;
		static bool m_PauseLog;
	private:
		static Count<Logger::Log>EngineLogger;
		static Count<Logger::Log>ClientLogger;
		static uint32_t Inputposition() {
			return Logs.size() + 1;
		}
	};
}

#ifdef PF_ENABLE_DEBUG 
#define  PF_ENGINE_ERROR(...) ::Proof::Log::GetEngineLogger()->LogError(__VA_ARGS__)
#define  PF_ENGINE_WARN(...)  ::Proof::Log::GetEngineLogger()->LogWarn(__VA_ARGS__)
#define  PF_ENGINE_INFO(...)  ::Proof::Log::GetEngineLogger()->LogInfo(__VA_ARGS__)
#define  PF_ENGINE_TRACE(...) ::Proof::Log::GetEngineLogger()->LogTrace(__VA_ARGS__)
#define	 PF_ENGINE_CRITICAL(...)::Proof::Log::GetEngineLogger()->LogCritical(__VA_ARGS__)
// Client Logging AND ENGINE
#define  PF_EC_ERROR(...) {::Proof::Log::AppendString(0,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogError(__VA_ARGS__);}
#define  PF_EC_WARN(...)  {::Proof::Log::AppendString(1,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogWarn(__VA_ARGS__);}
#define  PF_EC_INFO(...)  {::Proof::Log::AppendString(2,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogInfo(__VA_ARGS__);}
#define  PF_EC_TRACE(...) {::Proof::Log::AppendString(3,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogTrace(__VA_ARGS__);}
#define	 PF_EC_CRITICAL(...){::Proof::Log::AppendString(4,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogCritical(__VA_ARGS__);}

#define  PF_ERROR(...) ::Proof::Log::AppendString(0,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__))
#define  PF_WARN(...)  ::Proof::Log::AppendString(1,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__))
#define  PF_INFO(...)  ::Proof::Log::AppendString(2,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__))
#define  PF_TRACE(...) ::Proof::Log::AppendString(3,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__))
#define	 PF_CRITICAL(...)::Proof::Log::AppendString(4,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__))
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
#endif // USING_PROOF_LOGGER

/*
* #define  PF_ENGINE_ERROR(...) ::Proof::Log::GetEngineLogger()->LogError(__VA_ARGS__)
#define  PF_ENGINE_WARN(...)  ::Proof::Log::GetEngineLogger()->LogWarn(__VA_ARGS__)
#define  PF_ENGINE_INFO(...)  ::Proof::Log::GetEngineLogger()->LogInfo(__VA_ARGS__)
#define  PF_ENGINE_TRACE(...) ::Proof::Log::GetEngineLogger()->LogTrace(__VA_ARGS__)
#define	 PF_ENGINE_CRITICAL(...)::Proof::Log::GetEngineLogger()->LogCritical(__VA_ARGS__)
		// Client Logging
#define  PF_ERROR(...) {::Proof::Log::AppendString(0,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogError(__VA_ARGS__);}
#define  PF_WARN(...)  {::Proof::Log::AppendString(1,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogWarn(__VA_ARGS__);}
#define  PF_INFO(...)  {::Proof::Log::AppendString(2,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogInfo(__VA_ARGS__);}
#define  PF_TRACE(...) {::Proof::Log::AppendString(3,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogTrace(__VA_ARGS__);}
#define	 PF_CRITICAL(...){::Proof::Log::AppendString(4,::Proof::Log::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::Log::GetEngineLogger()->LogCritical(__VA_ARGS__);}
*/





