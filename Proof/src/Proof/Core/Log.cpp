#pragma once
#include "Proofprch.h"
#include "Log.h"
#include "Proof/Core/Core.h"
namespace Proof {
	//std::shared_ptr<Logger::Log>Log::ClientLogger;
	//std::shared_ptr<Logger::Log>Log::EngineLogger;
	//bool Log::m_PauseLog=false;
	//bool Log::NewLog=false;
	//std::unordered_map<uint32_t,std::pair<Log::LogType,std::string>> Log::Logs;
	void Log::Init() {
		
		EngineLogger = std::make_shared<Logger::Log>("Proof");
		ClientLogger = std::make_shared<Logger::Log>("App");
	}
	const std::shared_ptr<Logger::Log>& Log::GetEngineLogger()
	{
		return EngineLogger;
	}
	const std::shared_ptr<Logger::Log>& Log::GetClientLogger()
	{
		return ClientLogger;
	}
	void Log::AppendString(uint32_t type, const std::string& temp)
	{
		if (m_PauseLog)return;
		Logs.insert({ Inputposition(),{(LogType)type,temp} });
		NewLog = true;
	}
	uint32_t Log::Inputposition()
	{
		return Logs.size() + 1;
	}
}