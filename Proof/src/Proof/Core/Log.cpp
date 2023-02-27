#pragma once
#include "Proofprch.h"
#include "Log.h"
#include "Proof/Core/Core.h"
namespace Proof {
	Count<Logger::Log>Log::ClientLogger;
	Count<Logger::Log>Log::EngineLogger;
	bool Log::m_PauseLog=false;
	bool Log::NewLog=false;
	std::unordered_map<uint32_t,std::pair<Log::LogType,std::string>> Log::Logs;
	void Log::Init() {
		EngineLogger = Count<Logger::Log>::Create("Proof");
		ClientLogger = Count<Logger::Log>::Create("App");
	}
}