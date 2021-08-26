#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <ctime>
#include<iostream>
#include <cstdlib>
namespace Logger {
	class  Log {
	public:
		Log(const std::string& _LoggerName = "Log")
			:LoggerName(_LoggerName) 		{
			CurrentTime = time(NULL);
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
		void LogWarn(const char* Msg,Args&&... args);
	public:
		std::string LoggerName;
	private:
		time_t CurrentTime;
		tm* LocalTime = nullptr;
		std::string ColorOutput;
	};
	template<typename ...Args>
	inline void Log::LogError(const char* Msg,Args&& ...args) {
		LocalTime = localtime(&CurrentTime);
		printf("\x1b[31m[%i:%i:%i]",LocalTime->tm_hour,LocalTime->tm_min,LocalTime->tm_sec);
		ColorOutput = "\x1b[31m";
		ColorOutput += Msg;
		printf("\x1b[31m %s: ",LoggerName.c_str());
		printf(Msg,args...);
		printf("\x1b[37m\n");
	}
	template<typename ...Args>
	inline void Log::LogInfo(const char* Msg,Args&& ...args) {
		LocalTime = localtime(&CurrentTime);
		if(LocalTime !=nullptr){
			printf("\x1b[32m[%i:%i:%i]",LocalTime->tm_hour,LocalTime->tm_min,LocalTime->tm_sec);
		}
		ColorOutput = "\x1b[32m";
		ColorOutput += Msg;
		printf("\x1b[32m %s: ",LoggerName.c_str());
		printf(ColorOutput.c_str(),args...);
		printf("\x1b[37m\n");
	}
	template<typename ...Args>
	inline void Log::LogTrace(const char* Msg,Args&& ...args) {
		LocalTime = localtime(&CurrentTime);
		printf("\x1b[37m[%i:%i:%i]",LocalTime->tm_hour,LocalTime->tm_min,LocalTime->tm_sec);
		ColorOutput = "\x1b[37m";
		ColorOutput += Msg;
		printf("\x1b[37m %s: ",LoggerName.c_str());
		printf(ColorOutput.c_str(),args...);
		printf("\x1b[37m\n"); // changes thecolor and background
	}
	template<typename ...Args>
	inline void Log::LogCritical(const char* Msg,Args&& ...args) {
		LocalTime = localtime(&CurrentTime);
		printf("\x1b[31;43m[%i:%i:%i]",LocalTime->tm_hour,LocalTime->tm_min,LocalTime->tm_sec);
		ColorOutput = "\x1b[31;43m ";
		ColorOutput += Msg;
		printf("\x1b[31;43m %s:",LoggerName.c_str());
		printf(ColorOutput.c_str(),args...);
		printf("\x1b[37;40m\n"); // changes thecolor and background
	}
	template<typename ...Args>
	inline void Log::LogWarn(const char* Msg,Args&& ...args) {
		LocalTime = localtime(&CurrentTime);
		printf("\x1b[33m[%i:%i:%i]",LocalTime->tm_hour,LocalTime->tm_min,LocalTime->tm_sec);
		ColorOutput = "\x1b[33m";
		ColorOutput += Msg;
		printf("\x1b[33m %s: ",LoggerName.c_str());
		printf(Msg,args...);
		printf("\x1b[37m\n");
	}
}
#undef _CRT_SECURE_NO_WARNINGS


