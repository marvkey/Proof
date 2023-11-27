#pragma once
namespace Proof
{
	struct ApplicationSettings
	{
		//---------- Scripting ------------
		bool ShowHiddenFields = false;
		int ScriptDebuggerListenPort = 2550;

		//---------- Content Browser ------------
		bool ContentBrowserShowAssetTypes = true;
		int ContentBrowserThumbnailSize = 128;

		//---------- AntPlay ------------
		bool AdvancedMode = false;

		static ApplicationSettings& Get();
	};
}