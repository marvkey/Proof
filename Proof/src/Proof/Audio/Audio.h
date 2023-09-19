#pragma once
#include "Proof/Asset/Asset.h"
namespace Proof
{
	class Audio : public Asset
	{
	public:
		Audio(const std::filesystem::path& path) :
			m_Path(path)
		{

		}
		Audio() {};
		ASSET_CLASS_TYPE(Audio);
		const std::filesystem::path& GetPath()const { return m_Path; }
	private:
		std::filesystem::path m_Path;
	};
}