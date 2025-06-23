#include "Proofprch.h"
#include "StringUtils.h"
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <regex>
namespace Proof::Utils::String
{
	std::string ToLower(const std::string_view& string)
	{
		std::string result;
		for (const auto& character : string)
		{
			result += std::tolower(character);
		}

		return result;
	}
	std::string ToUpper(const std::string_view& string)
	{
		std::string result;
		for (const auto& character : string)
		{
			result += std::toupper(character);
		}

		return result;
	}
	std::string SubStr(const std::string& string, size_t offset, size_t count)
	{
		if (offset == std::string::npos)
			return string;

		if (offset >= string.length())
			return string;

		return string.substr(offset, count);
	}
	void Erase(std::string& str, const char* chars)
	{
		for (size_t i = 0; i < strlen(chars); i++)
			str.erase(std::remove(str.begin(), str.end(), chars[i]), str.end());
	}
	void Erase(std::string& str, const std::string& chars)
	{
		Erase(str, chars.c_str());
	}
	std::string BytesToString(uint64_t bytes)
	{
		constexpr uint64_t GB = 1024 * 1024 * 1024;
		constexpr uint64_t MB = 1024 * 1024;
		constexpr uint64_t KB = 1024;

		char buffer[32];

		if (bytes >= GB)
			sprintf_s(buffer, "%.2f GB", (float)bytes / (float)GB);
		else if (bytes >= MB)
			sprintf_s(buffer, "%.2f MB", (float)bytes / (float)MB);
		else if (bytes >= KB)
			sprintf_s(buffer, "%.2f KB", (float)bytes / (float)KB);
		else
			sprintf_s(buffer, "%.2f bytes", (float)bytes);

		return std::string(buffer);
	}
	/*
	template<class...Durations, class DurationIn>
	std::tuple<Durations...> BreakDownDuration(DurationIn d) {
		std::tuple<Durations...> retval;
		using discard = int[];
		(void)discard {
			0, (void((
				(std::get<Durations>(retval) = std::chrono::duration_cast<Durations>(d)),
				(d -= std::chrono::duration_cast<DurationIn>(std::get<Durations>(retval)))
				)), 0)...
		};
		return retval;
	}
	std::string DurationToStringFormat(std::chrono::duration<double> duration)
	{
		const auto durations = BreakDownDuration<std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds>(duration);

		std::stringstream durSs;
		durSs << std::setfill('0') << std::setw(1) << std::get<0>(durations).count() << ':'
			<< std::setfill('0') << std::setw(2) << std::get<1>(durations).count() << '.'
			<< std::setfill('0') << std::setw(3) << std::get<2>(durations).count();
		return durSs.str();


		// retuns [][][]
		//min seconds miliseconds
	}
	*/
	std::string DurationToString(float durationMilisecond)
	{

		constexpr float minutes = 1000 * 60;
		constexpr float seconds = 1000;

		char buffer[32];

		if (durationMilisecond > minutes)
			sprintf_s(buffer, "%.2fmin", (float)durationMilisecond / (float)minutes);
		else if (durationMilisecond > seconds)
			sprintf_s(buffer, "%.2fs", (float)durationMilisecond / (float)seconds);
		else
			sprintf_s(buffer, "%.2fm/s", (float)durationMilisecond);

		return std::string(buffer);
	}
	std::string ReplaceInString(const std::string& templateScript, const std::map<std::string, std::string>& replacements)
	{
		std::string replacedText = templateScript;

		for (const auto& replacement : replacements)
		{
			size_t found = replacedText.find(replacement.first);
			while (found != std::string::npos)
			{
				replacedText.replace(found, replacement.first.length(), replacement.second);
				found = replacedText.find(replacement.first, found + replacement.second.length());
			}
		}
		return replacedText;
	}
}

