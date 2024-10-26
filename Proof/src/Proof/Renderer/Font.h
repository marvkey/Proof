#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Asset/Asset.h"
namespace Proof {
	struct MSDFData;
	class Font : public Asset
	{
	public:
		Font(const std::filesystem::path& fullPath);
		Font(const std::string& name, Buffer buffer);
		virtual ~Font();
		const MSDFData* GetMSDFData() const { return m_MSDFData; }
		Count<Texture2D> GetTextureAtlas() const { return m_TextureAtlas; }

		static Count<Font> GetDefault();
		ASSET_CLASS_TYPE(Font);

	private:
		static void Init();
		static void ShutDown();
		void CreateAtlas(Buffer buffer);
	private:

		MSDFData* m_MSDFData;
		Count<Texture2D> m_TextureAtlas;

		std::string m_Name;
		static Count<Font> s_DefaultFont;

		friend class Application;
	};
}
