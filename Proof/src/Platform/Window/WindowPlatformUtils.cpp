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
			ZeroMemory(&ofn,sizeof(OPENFILENAME));
			ofn.lStructSize =sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(CurrentWindow::GetWindow()); // thw owning application
			ofn.lpstrFile =szFile;
			ofn.nMaxFile =sizeof(szFile);
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
			ZeroMemory(&ofn,sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(CurrentWindow::GetWindow()); // thw owning application
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = Filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; // IF WE DO NOT DO THIS IT WILL CHAGNE THE working directory of our application
			if (GetSaveFileNameA(&ofn) == TRUE) {
				return ofn.lpstrFile;
			}
			return std::string();
		};
	};
}