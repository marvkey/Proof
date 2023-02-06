#pragma once
#include <string>
#include <filesystem>
namespace Proof{
	class FileSystem {
	public:
		static void SetAnEnvironmentVariable(const std::string& ID, const std::string& value);
		static std::string GetEnvironmentValue(const std::string& ID);
		static std::string GetProjectDir();
	private:
		static void ClearEnvironmentVariables();
		friend class Application;
	};

	struct FilePath {
		FilePath(const FilePath& path) = default;
		FilePath(const std::filesystem::path& path) {
			if (Path.root_directory() == path.root_directory()) {
				Path = path;
				return;
			}
			Path /= path.filename();
		}
		operator std::filesystem::path&() { return Path; };

		std::filesystem::path Path = FileSystem::GetProjectDir();
	};
}
