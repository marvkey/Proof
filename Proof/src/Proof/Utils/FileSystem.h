#pragma once
#include <string>

namespace Proof{
	class FileSystem {
	public:
		static void SetAEnvironmentVariable(const std::string& ID, const std::string& value);
		static std::string GetEnvironmentValue(const std::string& ID);
	};
}
