#include "Proofprch.h"
#include "FileSystem.h"
namespace Proof
{
	static std::unordered_map<std::string, std::string> s_EnvironmentVariables;
	void FileSystem::SetAnEnvironmentVariable(const std::string& ID, const std::string& value) {
		s_EnvironmentVariables.insert({ ID,value });
	}
	std::string FileSystem::GetEnvironmentValue(const std::string& ID) {
		return s_EnvironmentVariables.at(ID);
	}
	std::string FileSystem::GetProjectDir() {
		return s_EnvironmentVariables.at("PROOF_PROJECT_DIR");
	}
}
