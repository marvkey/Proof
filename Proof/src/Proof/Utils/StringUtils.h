#pragma once
#include <string>
#include<map>
#include <chrono>
#include <algorithm>

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

	static inline std::string ReplaceUnderscoresWithPeriod(const std::string& str) {
		std::string result = str;
		std::replace(result.begin(), result.end(), '_', '.');
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

	template <typename T>
	static inline std::string GenerateUniqueName(const std::string& baseName,
		const T& container) {
		int counter = 0;

		// Generate name with padded counter if less than 10
		auto generateName = [&baseName, &counter]() {
			std::string counterStr = (counter < 10)
				? "0" + std::to_string(counter)
				: std::to_string(counter);
			return baseName + "_" + counterStr;
		};

		// Check if the generated name is unique within the container
		auto isUnique = [&container](const std::string& name) {
			if constexpr (std::is_same_v<T, std::unordered_set<std::string>> ||
				(std::is_same_v<T, std::unordered_map<std::string, typename T::mapped_type>>)) {
				return container.find(name) == container.end();
			}
			else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
				return std::find(container.begin(), container.end(), name) == container.end();
			}
		};

		std::string uniqueName = baseName;
		while (!isUnique(uniqueName)) {
			uniqueName = generateName();
			counter++;
		}

		return uniqueName;
	}
}