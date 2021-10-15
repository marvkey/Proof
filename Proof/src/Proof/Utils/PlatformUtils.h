#pragma once

#include <string>
namespace Proof{
	namespace Utils{
		class FileDialogs{
		public:
			// RETURN EMPTY STRING IF CANCELED
			static std::string OpenFile(const char* Filter);
			// RETURN EMPTY STRING IF CANCELED
			static std::string SaveFile(const char* Filter);
		};

		class ShortCutDialogs{
		public:
			static void Copy(const std::string& text);
		};
	}
}