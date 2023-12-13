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
	class Texture : public RendererResource
	{
	public:
		Texture()
		{

		}
		// if mempty means string was constructed with no path
		virtual ~Texture() {};

		ASSET_CLASS_TYPE(Texture);

	};
	
	class Texture2D : public Texture {
	public:
		virtual ~Texture2D() {};
		RENDER_VIEW_RESOURCE_CLASS_TYPE(Texture2D);

		virtual const TextureConfiguration& GetSpecification()const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Resize(uint32_t width, uint32_t height, Buffer buffer) = 0;
		virtual void Resize(Vector2U size) = 0;
		virtual uint32_t GetMipLevelCount() = 0;

		virtual uint32_t GetWidth()const = 0;
		virtual float GetAspectRatio()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual Vector2U GetSize()const = 0;
		virtual const std::filesystem::path& GetPath()const = 0;
		virtual void SetData(Buffer buffer)= 0;

		static Count<Texture2D> Create(const TextureConfiguration& config, const std::filesystem::path& path);
		static Count<Texture2D> Create(const TextureConfiguration& config, Buffer data);
		virtual void GenerateMips() = 0;

		static Count<Texture2D> Create(const TextureConfiguration& config);
		virtual Count<Image2D> GetImage() = 0;
		virtual Count<ImageView> GetImageMip(uint32_t mip, uint32_t layer=0) = 0;

	};

	class TextureCube : public Texture {
	public:
		RENDER_VIEW_RESOURCE_CLASS_TYPE(TextureCube);

		virtual ~TextureCube() {};
		virtual uint32_t GetWidth()const = 0;
		virtual float GetAspectRatio()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual Vector2U GetSize()const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Resize(Vector2U size) = 0;
		virtual uint32_t GetMipLevelCount() = 0;
		virtual void GenerateMips() = 0;
		virtual Count<Image2D> GetImage()const =0 ;
		static Count<TextureCube> Create(const TextureConfiguration& config, const std::filesystem::path& path);
		static Count<TextureCube> Create(const TextureConfiguration& config);
		// (TODO)make sure its hdr format
		static Count<TextureCube> Create(const TextureConfiguration& config, Count<Texture2D> texture);
		static Count<TextureCube> Create(const void* data,const TextureConfiguration& config);
	};

	namespace Utils 
	{
		inline uint32_t GetMipLevelCount(uint32_t width, uint32_t height)
		{
			return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
		}
	}

	struct PreethamSkyData
	{
		float Turbidity = 2.0f; // min(1.8f)
		float Azimuth = 0;
		float Inclination = 0;

		// Equality overload
		bool operator==(const PreethamSkyData& other) const {
			return Turbidity == other.Turbidity &&
				Azimuth == other.Azimuth &&
				Inclination == other.Inclination;
		}

		// Inequality overload
		bool operator!=(const PreethamSkyData& other) const {
			return !(*this == other);
		}
	};

	struct HosekWilkieSkyData
	{
		glm::vec3 SunDirection = { 0, 0, 0 };
		float Turbidity = 4.0f, GroundReflectance = 0.1f;

		// Equality overload
		bool operator==(const HosekWilkieSkyData& other) const {
			return SunDirection == other.SunDirection &&
				Turbidity == other.Turbidity &&
				GroundReflectance == other.GroundReflectance;
		}

		// Inequality overload
		bool operator!=(const HosekWilkieSkyData& other) const {
			return !(*this == other);
		}
	};

	struct EnvironmentTextureData
	{
		AssetID Image = 0;

		// Equality overload
		bool operator==(const EnvironmentTextureData& other) const {
			return Image == other.Image;
		}

		// Inequality overload
		bool operator!=(const EnvironmentTextureData& other) const {
			return !(*this == other);
		}
	};
	enum class EnvironmentState
	{
		HosekWilkie = 0,
		PreethamSky,
		EnvironmentTexture
	};
	class Environment : public RefCounted
	{
	public:
		Environment();
		Environment(HosekWilkieSkyData data);
		Environment(PreethamSkyData data);
		Environment(EnvironmentTextureData data);

		void Update(HosekWilkieSkyData data);
		void Update(PreethamSkyData data);
		void Update(EnvironmentTextureData data);
		~Environment();

		Count<TextureCube> GetIrradianceMap() { return m_IrradianceMap; }
		Count<TextureCube> GetPrefilterMap() { return m_PrefilterMap; }
		EnvironmentTextureData GetTextureData() { return m_EnvironmentTexture; }
		PreethamSkyData GetPreethamSkyData() { return m_PreethamSky; }
		HosekWilkieSkyData GetHosekWilkieDataSkyData() { return m_HosekWilkieSky; }
		EnvironmentState GetEnvironmentState() { return m_EnvironmentState; }
		bool IsDynamic() { return m_EnvironmentState != EnvironmentState::EnvironmentTexture; }
	private:
		Count<TextureCube> m_IrradianceMap = nullptr;
		Count<TextureCube> m_PrefilterMap = nullptr;
		EnvironmentState m_EnvironmentState = EnvironmentState::HosekWilkie;
		PreethamSkyData m_PreethamSky;
		HosekWilkieSkyData m_HosekWilkieSky;
		EnvironmentTextureData m_EnvironmentTexture;
		bool m_IsUpdated = false;
		static inline std::vector<WeakCount<Environment>> s_Instances;

		friend class Renderer;
	};
	struct Buffer;
	class TextureImporter
	{
	public:
		static Buffer ToBufferFromFile(const std::filesystem::path& path, ImageFormat& outFormat, uint32_t& width, uint32_t& height);
		//static Buffer ToBufferFromMemory(const void* data, ImageFormat format, uint32_t width, uint32_t height);
		static Buffer ToBufferFromMemory(Buffer buffer,ImageFormat& outFormat, uint32_t& outwidth, uint32_t& outheight);
	};

}
