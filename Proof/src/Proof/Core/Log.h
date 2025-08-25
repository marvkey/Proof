
#pragma once
#include "SmartPointer.h"
#include <Log/Loger.h>

#include <memory>
#include <unordered_map>
#include <utility>
namespace Proof
{
	class LogManager : public RefCounted {
	public:
		static void Init();
		static const std::shared_ptr<Logger::Log>& GetEngineLogger();
		static const std::shared_ptr<Logger::Log>& GetClientLogger();
		enum class LogType 
		{
			ERROR,
			WARN,
			INFO,
			TRACE,
			CRITICAL
		};
		static void AppendString(uint32_t type, const std::string& temp);
		static inline std::unordered_map<uint32_t,std::pair<LogManager::LogType,std::string>> Logs;
		static inline bool NewLog = false;
		static inline bool m_PauseLog = false;
	private:
		static inline std::shared_ptr<Logger::Log>EngineLogger;
		static inline std::shared_ptr<Logger::Log>ClientLogger;
		static uint32_t Inputposition();
	};
}

#ifdef PF_ENABLE_DEBUG 
#define  PF_ENGINE_ERROR(...) ::Proof::LogManager::GetEngineLogger()->LogError(__VA_ARGS__)
#define  PF_ENGINE_WARN(...)  ::Proof::LogManager::GetEngineLogger()->LogWarn(__VA_ARGS__)
#define  PF_ENGINE_INFO(...)  ::Proof::LogManager::GetEngineLogger()->LogInfo(__VA_ARGS__)
#define  PF_ENGINE_TRACE(...) ::Proof::LogManager::GetEngineLogger()->LogTrace(__VA_ARGS__)
#define	 PF_ENGINE_CRITICAL(...)::Proof::LogManager::GetEngineLogger()->LogCritical(__VA_ARGS__)
// Client Logging AND ENGINE
#define  PF_EC_ERROR(...) {::Proof::LogManager::AppendString(0,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::LogManager::GetEngineLogger()->LogError(__VA_ARGS__);}
#define  PF_EC_WARN(...)  {::Proof::LogManager::AppendString(1,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::LogManager::GetEngineLogger()->LogWarn(__VA_ARGS__);}
#define  PF_EC_INFO(...)  {::Proof::LogManager::AppendString(2,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::LogManager::GetEngineLogger()->LogInfo(__VA_ARGS__);}
#define  PF_EC_TRACE(...) {::Proof::LogManager::AppendString(3,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::LogManager::GetEngineLogger()->LogTrace(__VA_ARGS__);}
#define	 PF_EC_CRITICAL(...){::Proof::LogManager::AppendString(4,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__));::Proof::LogManager::GetEngineLogger()->LogCritical(__VA_ARGS__);}

#define  PF_ERROR(...) ::Proof::LogManager::AppendString(0,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__))
#define  PF_WARN(...)  ::Proof::LogManager::AppendString(1,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__))
#define  PF_INFO(...)  ::Proof::LogManager::AppendString(2,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__))
#define  PF_TRACE(...) ::Proof::LogManager::AppendString(3,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__))
#define	 PF_CRITICAL(...)::Proof::LogManager::AppendString(4,::Proof::LogManager::GetClientLogger()->GetLogString(__VA_ARGS__))
#else

#define  PF_ENGINE_ERROR(...)
#define  PF_ENGINE_WARN(...) 
#define  PF_ENGINE_INFO(...) 
#define  PF_ENGINE_TRACE(...)
#define	 PF_ENGINE_CRITICAL(...)

#define  PF_EC_ERROR(...)
#define  PF_EC_WARN(...)
#define  PF_EC_INFO(...)  
#define  PF_EC_TRACE(...) 
#define	 PF_EC_CRITICAL(...)

#define  PF_ERROR(...) 
#define  PF_WARN(...)  
#define  PF_INFO(...)  
#define  PF_TRACE(...) 
#define	 PF_CRITICAL(...)
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





