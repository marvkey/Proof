#include "Proofprch.h"
#include "FileSystem.h"
namespace Proof
{
	static std::unordered_map<std::string, std::string> s_EnvironmentVariables;
	void FileSystem::SetAEnvironmentVariable(const std::string& ID, const std::string& value) {
		//PF_CORE_ASSERT(s_EnvironmentVariables.contains(ID)==true, "Envrionent variable already exist");

		s_EnvironmentVariables.insert({ ID,value });
	}
	std::string FileSystem::GetEnvironmentValue(const std::string& ID) {
		//PF_CORE_ASSERT(s_EnvironmentVariables.contains(ID) == false, "Envrionent variable does not exist");
		return s_EnvironmentVariables.at(ID);
	}
}
