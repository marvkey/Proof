#pragma once
#include "Proof/Core/Core.h"
#include "RendererResouce.h"
#include "Proof/Asset/Asset.h"
#include "Image.h"
#include "Proof/Math/Vector.h"
#include<iostream>
#include <any>
namespace Proof {
	static uint32_t ConvertToBytes(const Vector& color)
	{
		uint8_t r = (uint8_t)(color.X * 255.0f);
		uint8_t g = (uint8_t)(color.Y * 255.0f);
		uint8_t b = (uint8_t)(color.Z * 255.0f);
		uint8_t a = (uint8_t)(1 * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

	static uint32_t ConvertToBytes(const Vector4& color)
	{
		uint8_t r = (uint8_t)(color.X * 255.0f);
		uint8_t g = (uint8_t)(color.Y * 255.0f);
		uint8_t b = (uint8_t)(color.Z * 255.0f);
		uint8_t a = (uint8_t)(color.W * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
	enum class TextureWrap
	{
		None = 0,
		Repeat,
		MirroredRepeat,
		ClampEdge,
		MirrorClampEdge,
		ClampBorder,
	};
	enum class TextureFilter
	{
		None = 0,
		Linear,
		Nearest,
		Cubic
	};
	struct TextureConfiguration
	{
		std::string DebugName;
		ImageFormat Format = ImageFormat::RGBA;
		uint32_t Width = 1, Height = 1;
		TextureWrap Wrap = TextureWrap::Repeat;
		TextureFilter Filter = TextureFilter::Linear;
		bool GenerateMips = false;
		bool Storage = false;
	};
	class Proof_API Texture : public Asset, public RendererViewResource
	{
	public:
		Texture()
		{

		}
		// if mempty means string was constructed with no path
		virtual ~Texture() {};

		ASSET_CLASS_TYPE(Texture);
	};
	
	class Proof_API Texture2D : public Texture {
	public:
		virtual ~Texture2D() {};

		virtual const TextureConfiguration& GetSpecification()const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Resize(Vector2U size) = 0;

		virtual uint32_t GetWidth()const = 0;
		virtual float GetAspectRatio()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual Vector2U GetSize()const = 0;
		virtual const std::filesystem::path& GetPath()const = 0;
		virtual void SetData(const void* data)= 0;

		static Count<Texture2D> Create(const TextureConfiguration& config, const std::filesystem::path& path);
		static Count<Texture2D> Create(const void* data,const TextureConfiguration& config);
		virtual Count<Image2D> GetImage() = 0;

	};

	class Proof_API TextureCube : public Texture {
	public:
		virtual ~TextureCube() {};

		virtual Count<Image2D> GetImage()const =0 ;

		static Count<TextureCube> Create(Count<Texture2D> textrure, uint32_t dimension = 512, bool generateMips = false);
		static Count<TextureCube> Create(const std::filesystem::path& Path, uint32_t dimension = 512, bool generateMips = false);
		static Count<TextureCube> Create(Count<TextureCube>map, Count<class Shader> shader, uint32_t dimension = 64, bool generateMips = false);
		static Count<TextureCube> GeneratePrefiltered(Count<TextureCube>map, uint32_t dimension = 128, uint32_t numSamples = 1024);
	};
	namespace Utils {
		inline uint32_t GetMipLevelCount(uint32_t width, uint32_t height)
		{
			return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
		}
	}
	struct Buffer;
	class TextureImporter
	{
	public:
		static Buffer ToBufferFromFile(const std::filesystem::path& path, ImageFormat format, uint32_t& width, uint32_t& height);
		static Buffer ToBufferFromMemory(const void* data,ImageFormat format, uint32_t width, uint32_t height);
	};

}
