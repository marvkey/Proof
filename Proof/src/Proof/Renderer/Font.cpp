#include "Proofprch.h"
#include "Font.h"

// msdf has an infinte we need to disbale
#undef INFINITE
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"

#include "MSDFData.h"
namespace Proof {
	//https://github.com/TheCherno/Hazel/blob/text/Hazel/src/Hazel/Renderer/Font.cpp
	// thank you for the code

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Count<Texture2D> CreateTextureAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
		const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();


		Count<Texture2D> texture = Texture2D::Create(bitmap.width, bitmap.height, ImageFormat::RGB, nullptr);
		texture->SetData((void*)bitmap.pixels);
		return texture;
	}
	Font::Font(const std::filesystem::path& fullPath)
		: m_Data(new MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		PF_CORE_ASSERT(ft);
		msdfgen::FontHandle* font = msdfgen::loadFont(ft, fullPath.string().c_str());
		if (!font)
		{
			PF_EC_ERROR("Failed to load font: {}", fullPath.string());
			return;
		}

		struct CharsetRange
		{
			uint32_t Begin, End;
		};

		// From imgui_draw.cpp
		static const CharsetRange charsetRanges[] =
		{
			{ 0x0020, 0x00FF }
		};

		msdf_atlas::Charset charset;
		for (CharsetRange range : charsetRanges)
		{
			for (uint32_t c = range.Begin; c <= range.End; c++)
				charset.add(c);
		}

		double fontScale = 1.0;
		m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
		int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
		PF_ENGINE_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());


		double emSize = 40.0;

		msdf_atlas::TightAtlasPacker atlasPacker;
		// atlasPacker.setDimensionsConstraint()
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(emSize);
		int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
		PF_CORE_ASSERT(remaining == 0);

		int width, height;
		atlasPacker.getDimensions(width, height);
		emSize = atlasPacker.getScale();


		#define DEFAULT_ANGLE_THRESHOLD 3.0
		#define LCG_MULTIPLIER 6364136223846793005ull
		#define LCG_INCREMENT 1442695040888963407ull
		#define THREAD_COUNT 8

		uint64_t coloringSeed = 0;
		bool expensiveColoring = false;
		if (expensiveColoring)
		{
			msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
				unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				return true;
			}, m_Data->Glyphs.size()).finish(THREAD_COUNT);
		}
		else
		{
			unsigned long long glyphSeed = coloringSeed;
			for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
			{
				glyphSeed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
			}
		}


		m_AtlasTexture = CreateTextureAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", (float)emSize, m_Data->Glyphs, m_Data->FontGeometry, width, height);
		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}
	Font::~Font() {
		delete m_Data;	
	}
	Count<Font> Font::GetDefault()
	{
		static Count<Font> font;
		if (!font)
			font = Count<Font>::Create("Assets/Fonts/Poppins/Poppins-Regular.ttf");

		return font;
	}
}
