#pragma once
#include "Proofprch.h"
#include "Log.h"
#include "Proof/Core/Core.h"
namespace Proof {
	std::shared_ptr<Logger::Log>Log::ClientLogger;
	std::shared_ptr<Logger::Log>Log::EngineLogger;
	bool Log::m_PauseLog=false;
	bool Log::NewLog=false;
	std::unordered_map<uint32_t,std::pair<Log::LogType,std::string>> Log::Logs;
	void Log::Init() {
		
		EngineLogger = std::make_shared<Logger::Log>("Proof");
		ClientLogger = std::make_shared<Logger::Log>("App");
	}
}