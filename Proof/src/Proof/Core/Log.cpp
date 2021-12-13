#pragma once
#include "Proofprch.h"
#include "Log.h"
namespace Proof {
	Count<Logger::Log>Log::EngineLogger;
	Count<Logger::Log>Log::ClientLogger;
	bool Log::m_PauseLog=false;
	bool Log::NewLog=false;
	std::unordered_map<uint16_t,std::pair<Log::LogType,std::string>> Log::Logs;
	void Log::Init() {
		EngineLogger = CreateCount<Logger::Log>("Proof");
		ClientLogger= CreateCount<Logger::Log>("Application");
	}
}