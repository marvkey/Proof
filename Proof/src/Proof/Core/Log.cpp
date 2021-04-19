#pragma once
#include "Proofprch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Proof {
	std::shared_ptr<spdlog::logger>Log::EngineLogger;
	std::shared_ptr<spdlog::logger>Log::ClientLogger;
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		EngineLogger = spdlog::stdout_color_mt("Proof");
		EngineLogger->set_level(spdlog::level::trace);
		
		ClientLogger = spdlog::stdout_color_mt("SandBox");
		ClientLogger->set_level(spdlog::level::trace);
		PF_ENGINE_TRACE("Log initilized");
	}
}