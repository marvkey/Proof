#pragma once

#include <string>
#include<filesystem>
namespace Proof{
	namespace Utils{
		class FileDialogs{
		public:
			// RETURN EMPTY STRING IF CANCELED
			static std::string OpenFile(const char* filter);
			// RETURN EMPTY STRING IF CANCELED
			static std::string SaveFile(const char* filter);

			// get the file name removing any extension
			static std::string GetFileName(const std::filesystem::path& path) {
				std::string temp = path.filename().string();
				temp = temp.substr(0,temp.find_first_of("."));
				return temp;
			}
			// includes the extension
			static std::string GetFullFileName(const std::filesystem::path& path) {
				return path.filename().string();
			}
			//Returns the extension of the last .found in a file name
			static std::string GetFileExtension(const std::filesystem::path& path) {
				std::string temp = path.filename().string();
				temp = temp.substr(temp.find_last_of(".") + 1); // storing all the text after the first .
				return temp;
			}
			// Returns the extension of the first . found in a file name
			static std::string GetFullFileExtension(const std::filesystem::path& path) {
				std::string temp = path.filename().string();
				temp = temp.substr(temp.find_first_of(".")+1); // storing all the text after the first .
				return temp;
			}
		};

		class ShortCutDialogs{
		public:
			static void Copy(const std::string& text);
		};
	}
}