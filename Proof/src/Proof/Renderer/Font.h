#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Asset/Asset.h"
namespace Proof{
	struct MSDFData;
	class Font : public Asset
	{
	public:
		Font(const std::filesystem::path& fullPath);
		~Font();
		const MSDFData* GetMSDFData() const { return m_Data; }
		Count<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Count<Font> GetDefault();
		ASSET_CLASS_TYPE(Font);
	private:
		MSDFData* m_Data;

		Count<Texture2D> m_AtlasTexture;
	};
}
