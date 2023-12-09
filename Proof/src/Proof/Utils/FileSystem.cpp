#include "Proofprch.h"
#include "FileSystem.h"
#include "Proof/Core/Application.h"
#include "Proof/Core/Buffer.h"
#include "Proof/Project/Project.h"

#include <GLFW/glfw3.h> // needs to know hte parent
#define GLFW_EXPOSE_NATIVE_WIN32 // lets us get win 32 stuff
#include <GLFW/glfw3native.h>
#include <commdlg.h> // API TO OPEN FILE
#include <windows.h>
#include <Shlobj.h>
#include <shellapi.h>
#include <filesystem>
#include <fstream>
#ifdef CreateDirectory
#undef CreateDirectory
#undef DeleteFile
#undef MoveFile
#undef CopyFile
#undef SetEnvironmentVariable
#undef GetEnvironmentVariable
#endif
namespace Proof
{
	std::vector<FileSystem::FileSystemChangedCallbackFn> FileSystem::s_Callbacks;

	bool FileSystem::CreateDirectory(const std::filesystem::path& directory)
	{
		return std::filesystem::create_directories(directory);
	}

	bool FileSystem::CreateDirectory(const std::string& directory)
	{
		return CreateDirectory(std::filesystem::path(directory));
	}

	bool FileSystem::Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
	{
		if (FileSystem::Exists(newFilepath))
			return false;

		std::filesystem::rename(oldFilepath, newFilepath);
		return true;
	}

	bool FileSystem::Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
	{
		if (FileSystem::Exists(newFilepath))
			return false;

		std::filesystem::copy(oldFilepath, newFilepath);
		return true;
	}

	bool FileSystem::MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
	{
		return Move(filepath, dest / filepath.filename());
	}

	bool FileSystem::CopyFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
	{
		return Copy(filepath, dest / filepath.filename());
	}

	bool FileSystem::Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
	{
		return Move(oldFilepath, newFilepath);
	}

	bool FileSystem::RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newName)
	{
		std::filesystem::path newPath = fmt::format("{0}\\{1}{2}", oldFilepath.parent_path().string(), newName, oldFilepath.extension().string());
		return Rename(oldFilepath, newPath);
	}

	bool FileSystem::Exists(const std::filesystem::path& filepath)
	{
		return std::filesystem::exists(filepath);
	}

	bool FileSystem::Exists(const std::string& filepath)
	{
		return std::filesystem::exists(std::filesystem::path(filepath));
	}

	bool FileSystem::DeleteFile(const std::filesystem::path& filepath)
	{
		if (!FileSystem::Exists(filepath))
			return false;

		if (std::filesystem::is_directory(filepath))
			return std::filesystem::remove_all(filepath) > 0;
		return std::filesystem::remove(filepath);
	}

	bool FileSystem::IsDirectory(const std::filesystem::path& filepath)
	{
		return std::filesystem::is_directory(filepath);
	}

	// returns true <=> fileA was last modified more recently than fileB
	bool FileSystem::IsNewer(const std::filesystem::path& fileA, const std::filesystem::path& fileB)
	{
		return std::filesystem::last_write_time(fileA) > std::filesystem::last_write_time(fileB);
	}

	bool FileSystem::ShowFileInExplorer(const std::filesystem::path& path)
	{
		auto absolutePath = std::filesystem::canonical(path);
		if (!Exists(absolutePath))
			return false;

		std::string cmd = fmt::format("explorer.exe /select,\"{0}\"", absolutePath.string());
		system(cmd.c_str());
		return true;
	}

	bool FileSystem::OpenDirectoryInExplorer(const std::filesystem::path& path)
	{
		auto absolutePath = std::filesystem::canonical(path);
		if (!Exists(absolutePath))
			return false;

		ShellExecute(NULL, L"explore", absolutePath.wstring().c_str(), NULL, NULL, SW_SHOWNORMAL);
		return true;
	}

	bool FileSystem::OpenExternally(const std::filesystem::path& path)
	{
		auto absolutePath = std::filesystem::canonical(path); // gives the abslute full path
		if (!Exists(absolutePath))
			return false;

		ShellExecute(NULL, L"open", absolutePath.wstring().c_str(), NULL, NULL, SW_SHOWNORMAL);
		return true;
	}

	std::filesystem::path FileSystem::GenerateUniqueFileName(const std::filesystem::path& filepath)
	{
		if (!FileSystem::Exists(filepath))
			return filepath;

		int counter = 0;
		auto checkID = [&counter, filepath](auto checkID) -> std::filesystem::path
		{
			++counter;
			const std::string counterStr = [&counter] {
				if (counter < 10)
					return "0" + std::to_string(counter);
				else
					return std::to_string(counter);
			}();  // Pad with 0 if < 10;

			std::string newFileName = fmt::format("{} ({})", Utils::RemoveExtension(filepath.filename().string()), counterStr);

			if (filepath.has_extension())
				newFileName = fmt::format("{}{}", newFileName, filepath.extension().string());

			if (std::filesystem::exists(filepath.parent_path() / newFileName))
				return checkID(checkID);
			else
				return filepath.parent_path() / newFileName;
		};

		return checkID(checkID);
	}

	std::filesystem::path FileSystem::OpenFileDialog(const char* filter)
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get()->GetWindow()->GetWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			std::string fp = ofn.lpstrFile;
			std::replace(fp.begin(), fp.end(), '\\', '/');
			return std::filesystem::path(fp);
		}

		return std::filesystem::path();
	}

	std::filesystem::path FileSystem::OpenFolderDialog(const char* initialFolder)
	{
		std::filesystem::path result = "";
		IFileOpenDialog* dialog;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&dialog)))
		{
			DWORD options;
			dialog->GetOptions(&options);
			dialog->SetOptions(options | FOS_PICKFOLDERS);
			if (SUCCEEDED(dialog->Show(NULL)))
			{
				IShellItem* selectedItem;
				if (SUCCEEDED(dialog->GetResult(&selectedItem)))
				{
					PWSTR pszFilePath;
					if (SUCCEEDED(selectedItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath)))
					{
						result = std::filesystem::path(pszFilePath, std::filesystem::path::generic_format);
						CoTaskMemFree(pszFilePath);
					}

					selectedItem->Release();
				}
			}

			dialog->Release();
		}
		if (result.empty())
			return result;

		std::string fp = result.string();
		std::replace(fp.begin(), fp.end(), '\\', '/');
		return fp;
	}

	std::filesystem::path FileSystem::SaveFileDialog(const char* filter)
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get()->GetWindow()->GetWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			std::string fp = ofn.lpstrFile;
			std::replace(fp.begin(), fp.end(), '\\', '/');
			return std::filesystem::path(fp);
		}

		return std::filesystem::path();
	}


	bool FileSystem::WriteBytes(const std::filesystem::path& filepath, const Buffer& buffer)
	{
		std::ofstream stream(filepath, std::ios::binary | std::ios::trunc);

		if (!stream)
		{
			stream.close();
			return false;
		}

		stream.write((char*)buffer.Get(), buffer.GetSize());
		stream.close();

		return true;
	}

	Buffer FileSystem::ReadBytes(const std::filesystem::path& filepath)
	{
		Buffer buffer;

		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		PF_CORE_ASSERT(stream);

		auto end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		auto size = end - stream.tellg();
		PF_CORE_ASSERT(size != 0);

		buffer.Allocate((uint64_t)size);
		stream.read((char*)buffer.Get(), buffer.GetSize());
		stream.close();

		return buffer;
	}

	bool FileSystem::HasEnvironmentVariable(const std::string& key)
	{
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		LSTATUS lOpenStatus = RegOpenKeyExA(HKEY_CURRENT_USER, keyPath, 0, KEY_ALL_ACCESS, &hKey);

		if (lOpenStatus == ERROR_SUCCESS)
		{
			lOpenStatus = RegQueryValueExA(hKey, key.c_str(), 0, NULL, NULL, NULL);
			RegCloseKey(hKey);
		}

		return lOpenStatus == ERROR_SUCCESS;
	}

	bool FileSystem::SetEnvironmentVariable(const std::string& key, const std::string& value)
	{
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		DWORD createdNewKey;
		LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);
		if (lOpenStatus == ERROR_SUCCESS)
		{
			LSTATUS lSetStatus = RegSetValueExA(hKey, key.c_str(), 0, REG_SZ, (LPBYTE)value.c_str(), (DWORD)(value.length() + 1));
			RegCloseKey(hKey);

			if (lSetStatus == ERROR_SUCCESS)
			{
				SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_BLOCK, 100, NULL);
				return true;
			}
		}

		return false;
	}

	std::string FileSystem::GetEnvironmentVariable(const std::string& key)
	{
		HKEY hKey;
		LPCSTR keyPath = "Environment";
		DWORD createdNewKey;
		LSTATUS lOpenStatus = RegCreateKeyExA(HKEY_CURRENT_USER, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &createdNewKey);
		if (lOpenStatus == ERROR_SUCCESS)
		{
			DWORD valueType;
			char* data = pnew char[512];
			DWORD dataSize = 512;
			LSTATUS status = RegGetValueA(hKey, NULL, key.c_str(), RRF_RT_ANY, &valueType, (PVOID)data, &dataSize);

			RegCloseKey(hKey);

			if (status == ERROR_SUCCESS)
			{
				std::string result(data);
				pdelete[] data;
				return result;
			}
		}

		return std::string{};
	}

	void ShortCutDialogs::Copy(const std::string& text) {
		OpenClipboard(glfwGetWin32Window((GLFWwindow*)Application::Get()->GetWindow()->GetWindow()));
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size());
		if (!hg)
		{
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg), text.c_str(), text.size());
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
	}
	static bool s_Watching = false;
	static bool s_IgnoreNextChange = false;
	static HANDLE s_WatcherThread;
	//static std::filesystem::path s_PersistentStoragePath;
	void FileSystem::StartWatching()
	{
		s_Watching = true;
		DWORD threadId;
		s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadId);
		PF_CORE_ASSERT(s_WatcherThread != NULL);
		SetThreadDescription(s_WatcherThread, L"Ant FileSystemWatcher");
	}

	void FileSystem::StopWatching()
	{
		if (!s_Watching)
			return;

		s_Watching = false;
		//DWORD result = WaitForSingleObject(s_WatcherThread, 5000);
		//if (result == WAIT_TIMEOUT)
		// NOTE: this is a little annoying, but it's a quick and dirty
		//       way to shutdown the file watching ASAP.
		TerminateThread(s_WatcherThread, 0);
		CloseHandle(s_WatcherThread);
	}
	void FileSystem::SkipNextFileSystemChange()
	{
		s_IgnoreNextChange = true;
	}

	void FileSystem::AddFileSystemChangedCallback(const FileSystemChangedCallbackFn& callback)
	{
		s_Callbacks.push_back(callback);
	}

	void FileSystem::ClearFileSystemChangedCallbacks()
	{
		s_Callbacks.clear();
	}

	unsigned long FileSystem::Watch(void* param)
	{
		auto assetDirectory = Project::GetActive()->GetAssetDirectory();
		std::wstring dirStr = assetDirectory.wstring();

		char buf[2048];
		DWORD bytesReturned;
		std::filesystem::path filepath;
		BOOL result = TRUE;

		HANDLE directoryHandle = CreateFile(
			dirStr.c_str(),
			GENERIC_READ | FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
		);

		if (directoryHandle == INVALID_HANDLE_VALUE)
		{
			PF_CORE_ASSERT(false, "Failed to open directory!");
			return 0;
		}

		OVERLAPPED pollingOverlap;
		pollingOverlap.OffsetHigh = 0;
		pollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		std::vector<FileSystemChangedEvent> eventBatch;
		eventBatch.reserve(10);

		while (s_Watching && result)
		{
			result = ReadDirectoryChangesW(
				directoryHandle,
				&buf,
				sizeof(buf),
				TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME |
				FILE_NOTIFY_CHANGE_SIZE,
				&bytesReturned,
				&pollingOverlap,
				NULL
			);

			WaitForSingleObject(pollingOverlap.hEvent, INFINITE);

			if (s_IgnoreNextChange)
			{
				s_IgnoreNextChange = false;
				eventBatch.clear();
				continue;
			}

			FILE_NOTIFY_INFORMATION* pNotify;
			int offset = 0;
			std::wstring oldName;

			do
			{
				pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
				size_t filenameLength = pNotify->FileNameLength / sizeof(wchar_t);

				FileSystemChangedEvent e;
				e.FilePath = std::filesystem::path(std::wstring(pNotify->FileName, filenameLength));
				e.IsDirectory = IsDirectory(e.FilePath);

				switch (pNotify->Action)
				{
				case FILE_ACTION_ADDED:
				{
					e.Action = FileSystemAction::Added;
					break;
				}
				case FILE_ACTION_REMOVED:
				{
					e.Action = FileSystemAction::Delete;
					break;
				}
				case FILE_ACTION_MODIFIED:
				{
					e.Action = FileSystemAction::Modified;
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME:
				{
					oldName = e.FilePath.filename();
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME:
				{
					e.OldName = oldName;
					e.Action = FileSystemAction::Rename;
					break;
				}
				}

				// NOTE: Fix for https://gitlab.com/chernoprojects/Ant-dev/-/issues/143
				bool hasAddedEvent = false;
				if (e.Action == FileSystemAction::Modified)
				{
					for (const auto& event : eventBatch)
					{
						if (event.FilePath == e.FilePath && event.Action == FileSystemAction::Added)
							hasAddedEvent = true;
					}
				}

				if (pNotify->Action != FILE_ACTION_RENAMED_OLD_NAME && !hasAddedEvent)
					eventBatch.push_back(e);

				offset += pNotify->NextEntryOffset;
			} while (pNotify->NextEntryOffset);

			if (eventBatch.size() > 0)
			{
				for (auto& callback : s_Callbacks)
					callback(eventBatch);
				eventBatch.clear();
			}
		}

		CloseHandle(directoryHandle);
		return 0;
	}

	#if 0
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
	#endif
}
