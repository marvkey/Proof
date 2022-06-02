#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <ctime>
#include<iostream>
#include <cstdlib>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#define FMT_HEADER_ONLY
	
#include <fmt/core.h>
#include <fmt/color.h>
namespace Logger {
	
	class  Log {
	public:
		char* buffer;
		//char * bufferbasee;
		Log(const std::string& _LoggerName = "Log")
			:LoggerName(_LoggerName) 		{
			buffer=new char;
			//bufferbasee = new char;
		}
		template<typename... Args>
		void LogError(const char* Msg,Args&&... args);
		template<typename... Args>
		void LogInfo(const char* Msg,Args&&... args);
		template<typename... Args>
		void LogTrace(const char* Msg,Args&&... args);
		template<typename... Args>
		void LogCritical(const char* Msg,Args&&... args);
		template<typename... Args>
		void LogWarn(const char* Msg, Args&&... args);

		template<typename... Args>
		std::string GetLogString(const char* Msg,Args&&... args);
	public:
		std::string LoggerName;
	private:
		time_t CurrentTime;
		tm* LocalTime = nullptr;
		std::string ColorOutput;
	};

	template<typename ...Args>
	inline void Log::LogError(const char* Msg,Args&& ...args) {
		CurrentTime = time(NULL);
		LocalTime = localtime(&CurrentTime);
		fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
			"[{}:{}:{}]", LocalTime->tm_hour, LocalTime->tm_min, LocalTime->tm_sec);
		fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, "{}: ", LoggerName.c_str());
		fmt::print(fg(fmt::color::red) | fmt::emphasis::bold, Msg, args...);
		fmt::print("\n");
	}
	template<typename ...Args>
	inline void Log::LogInfo(const char* Msg,Args&& ...args) {
		CurrentTime = time(NULL);
		LocalTime = localtime(&CurrentTime);
		fmt::print(fg(fmt::color::green),
			"[{}:{}:{}]", LocalTime->tm_hour, LocalTime->tm_min, LocalTime->tm_sec);
		fmt::print(fg(fmt::color::green), "{}: ", LoggerName.c_str());
		fmt::print(fg(fmt::color::green),Msg, args...);
		fmt::print("\n");
	}
	template<typename ...Args>
	inline void Log::LogTrace(const char* Msg,Args&& ...args) {
		CurrentTime = time(NULL);
		LocalTime = localtime(&CurrentTime);
		fmt::print(fg(fmt::color::white),
			"[{}:{}:{}]", LocalTime->tm_hour, LocalTime->tm_min, LocalTime->tm_sec);
		fmt::print(fg(fmt::color::white), "{}: ", LoggerName.c_str());
		fmt::print(fg(fmt::color::white),Msg, args...);
		fmt::print("\n");
	}
	template<typename ...Args>
	inline void Log::LogCritical(const char* Msg,Args&& ...args) {
		CurrentTime = time(NULL);
		LocalTime = localtime(&CurrentTime);
		fmt::print(fg(fmt::color::red) | fmt::emphasis::bold | bg(fmt::color::yellow),
			"[{}:{}:{}]", LocalTime->tm_hour, LocalTime->tm_min, LocalTime->tm_sec);
		fmt::print(fg(fmt::color::red) | fmt::emphasis::bold | bg(fmt::color::yellow), "{}: ", LoggerName.c_str());
		fmt::print(fg(fmt::color::red) | fmt::emphasis::bold | bg(fmt::color::yellow),Msg, args...);
		fmt::print("\n");
	}
	template<typename ...Args>
	inline void Log::LogWarn(const char* Msg,Args&& ...args) {
		CurrentTime = time(NULL);
		LocalTime = localtime(&CurrentTime);
		fmt::print(fg(fmt::color::yellow),
			"[{}:{}:{}]", LocalTime->tm_hour, LocalTime->tm_min, LocalTime->tm_sec);
		fmt::print(fg(fmt::color::yellow), "{}: ", LoggerName.c_str());
		fmt::print(Msg, args...);
		fmt::print("\n");
	}

	template<typename ...Args>
	inline std::string Log::GetLogString(const char* Msg, Args && ...args) {

		CurrentTime = time(NULL);
		LocalTime = localtime(&CurrentTime);
		std::string temp = fmt::format("[{}:{}:{}]{}",LocalTime->tm_hour, LocalTime->tm_min, LocalTime->tm_sec, LoggerName);
		//sprintf(buffer, Msg, args...);
		std::string temp2 = fmt::format(Msg, args...);
		return temp+ temp2;
	}
}
#undef _CRT_SECURE_NO_WARNINGS


