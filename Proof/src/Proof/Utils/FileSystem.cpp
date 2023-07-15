#include "Proofprch.h"
#include "FileSystem.h"
#include "Proof/Core/Buffer.h"

namespace Proof
{
	static std::unordered_map<std::string, std::string> s_EnvironmentVariables;
	void FileSystem::SetAnEnvironmentVariable(const std::string& ID, const std::string& value) {
		PF_CORE_ASSERT(!s_EnvironmentVariables.contains(ID), "Already contains variable use override");
		s_EnvironmentVariables.insert({ ID,value });
	}
	std::string FileSystem::GetEnvironmentValue(const std::string& ID) {
		return s_EnvironmentVariables.at(ID);
	}
	std::string FileSystem::GetProjectDir() {
		return s_EnvironmentVariables.at("PROOF_PROJECT_DIR");
	}
	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return {};
		}


		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), size);
		stream.close();
		return buffer;
	}
	void FileSystem::ClearEnvironmentVariables()
	{
		s_EnvironmentVariables.clear();
	}
}
