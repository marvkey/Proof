#pragma once
#include <string>
#include<map>
#include <chrono>
namespace Proof::Utils::String
{
	std::string ToLower(const std::string_view& string);
	std::string ToUpper(const std::string_view& string);
	std::string SubStr(const std::string& string, size_t offset, size_t count = std::string::npos);

	void Erase(std::string& str, const char* chars);
	void Erase(std::string& str, const std::string& chars);
	std::string BytesToString(uint64_t bytes);
	//std::string DurationToStringFormat(std::chrono::duration<double> duration);
	std::string DurationToString(float durationMilisecond);

	std::string ReplaceInString(const std::string& templateScript, const std::map<std::string, std::string>& replacements);
}