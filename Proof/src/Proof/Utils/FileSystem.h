#pragma once


#ifdef CreateDirectory
#undef CreateDirectory
#undef DeleteFile
#undef MoveFile
#undef CopyFile
#undef SetEnvironmentVariable
#undef GetEnvironmentVariable
#endif

#include <string>
#include <filesystem>
namespace Proof
{
	namespace  Utils 
	{
		inline std::string RemoveExtension(const std::string& filename)
		{
			return filename.substr(0, filename.find_last_of('.'));
		}
	}
	class FileSystem
	{
	public:
		// RETURN EMPTY STRING IF CANCELED
		static std::filesystem::path OpenFileDialog(const char* filter = "All\0*.*\0");
		static std::filesystem::path OpenFolderDialog(const char* initialFolder = "");
		static std::filesystem::path SaveFileDialog(const char* filter = "All\0*.*\0");

		// get the file name removing any extension
		static std::string GetFileName(const std::filesystem::path& path)
		{
			std::string temp = path.filename().string();
			temp = temp.substr(0, temp.find_first_of(".")); // NEED TO DO A CHECK
			return temp;
		}
		// includes the extension
		static std::string GetFullFileName(const std::filesystem::path& path)
		{
			return path.filename().string();
		}
		//Returns the extension of the last .found in a file name
		static std::string GetFileExtension(const std::filesystem::path& path)
		{
			std::string temp = path.filename().string();
			temp = temp.substr(temp.find_last_of(".") + 1); // storing all the text after the last .
			return temp;
		}
		// Returns the extension of the first . found in a file name
		static std::string GetFullFileExtension(const std::filesystem::path& path)
		{
			std::string temp = path.filename().string();
			if (temp.find_first_of(".") != std::string::npos)
			{ // means that we can find a . in the string
				temp = temp.substr(temp.find_first_of(".") + 1); // storing all the text after the first .
				return temp;
			}
			return "";
		}

		static std::string GetFileFullNameWithoutExtension(const std::filesystem::path& path)
		{
			std::string temp = path.string();
				temp = temp.substr(0, temp.find_first_of(".")); 
				return temp;
		}
	public:
		static bool HasEnvironmentVariable(const std::string& key);
		static bool SetEnvironmentVariable(const std::string& key, const std::string& value);
		static std::string GetEnvironmentVariable(const std::string& key);

		static bool ShowFileInExplorer(const std::filesystem::path& path);
		static bool OpenDirectoryInExplorer(const std::filesystem::path& path);
		static bool OpenExternally(const std::filesystem::path& path);

		static bool WriteBytes(const std::filesystem::path& filepath, const struct Buffer& buffer);
		static struct Buffer ReadBytes(const std::filesystem::path& filepath);

		static std::filesystem::path GenerateUniqueFileName(const std::filesystem::path& filepath);

		static bool CreateDirectory(const std::filesystem::path& directory);
		static bool CreateDirectory(const std::string& directory);
		static bool Exists(const std::filesystem::path& filepath);
		static bool Exists(const std::string& filepath);
		static bool DeleteFile(const std::filesystem::path& filepath);
		static bool MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest);
		static bool CopyFile(const std::filesystem::path& filepath, const std::filesystem::path& dest);
		static bool IsDirectory(const std::filesystem::path& filepath);

		static bool IsNewer(const std::filesystem::path& fileA, const std::filesystem::path& fileB);

		static bool Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
		static bool Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
		static bool Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);

		// extension is already added
		static bool RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newName);
	};

	class ShortCutDialogs 
	{
	public:
		static void Copy(const std::string& text);
	};
}
