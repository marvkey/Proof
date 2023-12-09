#pragma once
#include <string>
#include<map>
#include <chrono>
namespace Proof::Utils::String
{
	static inline bool ContainsWhitespace(const std::string& str) {
		return std::any_of(str.begin(), str.end(), ::isspace);
	}

	// Split a string at whitespace and return a vector of tokens
	static inline std::vector<std::string> SplitAtWhitespace(const std::string& str) {
		std::vector<std::string> tokens;
		std::istringstream iss(str);
		std::copy(std::istream_iterator<std::string>(iss),
			std::istream_iterator<std::string>(),
			std::back_inserter(tokens));
		return tokens;
	}

	// Check if a string contains a substring
	static inline bool ContainsSubstring(const std::string& str, const std::string& substr) {
		return str.find(substr) != std::string::npos;
	}

	// Remove all whitespace characters from a string
	static inline std::string RemoveWhiteSpaces(const std::string& str) {
		std::string result = str;
		result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
		return result;
	}

	// Replace underscores with spaces in a string
	static inline std::string ReplaceUnderscoresWithSpaces(const std::string& str) {
		std::string result = str;
		std::replace(result.begin(), result.end(), '_', ' ');
		return result;
	}
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