#pragma once
#include "Proofprch.h"
#include "Log.h"
namespace Proof {
	std::shared_ptr<Logger::Log>Log::EngineLogger;
	std::shared_ptr<Logger::Log>Log::ClientLogger;
	bool Log::m_PauseLog=false;
	bool Log::NewLog=false;
	std::unordered_map<uint32_t,std::pair<Log::LogType,std::string>> Log::Logs;
	void Log::Init() {
		EngineLogger = CreateCount<Logger::Log>("Proof");
		ClientLogger= CreateCount<Logger::Log>("App");
	}
}