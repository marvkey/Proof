#pragma once
#include "Proof/Core/Core.h"
#include <filesystem>
namespace Proof{
	class Project {
	public:
		std::string Name;
	private:
		std::filesystem::path m_AssetDirectory;
		std::filesystem::path m_StartWorld;
	};

}
