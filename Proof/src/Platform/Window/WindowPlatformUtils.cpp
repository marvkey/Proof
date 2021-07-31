#include "Proofprch.h"
#include "Proof/Utils/PlatformUtils.h"
#include <commdlg.h> // API TO OPEN FILE
#include <GLFW/glfw3.h> // needs to know hte parent
#define GLFW_EXPOSE_NATIVE_WIN32 // lets us get win 32 stuff
#include <GLFW/glfw3native.h>
#include "Proof/Core/Application.h"

namespace Proof{
	namespace Utils{
		std::string FileDialogs::OpenFile(const char* Filter){
			OPENFILENAMEA ofn;
			CHAR szFile[260] ={0};
			CHAR currentDir[256] = {0};
			ZeroMemory(&ofn,sizeof(OPENFILENAME));
			ofn.lStructSize =sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(CurrentWindow::GetWindow()); // the owning application
			ofn.lpstrFile =szFile;
			ofn.nMaxFile =sizeof(szFile);
			if (GetCurrentDirectoryA(256,currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter =Filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // IF WE DO NOT DO THIS IT WILL CHAGNE THE working directory of our application
			if(GetOpenFileNameA(&ofn) == TRUE){
				return ofn.lpstrFile;
			}
			return std::string(); // empty string
		}
		std::string FileDialogs::SaveFile(const char* Filter){
			OPENFILENAMEA ofn;
			CHAR szFile[260] = {0};
			CHAR currentDir[256] = {0};
			ZeroMemory(&ofn,sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(CurrentWindow::GetWindow()); // the owning application
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256,currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = Filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // IF WE DO NOT DO THIS IT WILL CHAGNE THE working directory of our application
			
			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(Filter,'\0') + 1;
			if (GetSaveFileNameA(&ofn) == TRUE) {
				return ofn.lpstrFile;
			}
			return std::string();
		};
	};
}