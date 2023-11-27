#include "ApplicationSettings.h"
#include <filesystem>
namespace Proof {
	//static std::filesystem::path s_EditorSettingsPath;

	ApplicationSettings& ApplicationSettings::Get()
	{
		static ApplicationSettings s_Settings;
		return s_Settings;
	}
}