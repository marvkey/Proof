#include "Proofprch.h"
#include "Texture.h"
#include "Renderer.h"
//#include "Platform/OpenGL/OpenGLTexture.h"
#include "Proof/Platform/Vulkan/VulkanTexutre.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
#include "Proof/Asset/AssetManager.h"
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"../vendor/stb_image.h"
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Renderer/RendererSampler.h"
#include "Proof/Renderer/SamplerFactory.h"
namespace Proof 
{

	SamplerResourceConfig texture2DConfig("Texture2DCustomSampler", SamplerWrap::Repeat, SamplerFilter::Linear, SamplerBorderColor::OpaqueWhiteInt, SamplerMipMapMode::Linear);
	SamplerResourceConfig textureCubeConfig("TextureCubeCustomSampler", SamplerWrap::ClampEdge, SamplerFilter::Linear, SamplerBorderColor::OpaqueWhiteInt, SamplerMipMapMode::Linear);
	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, const std::filesystem::path& path, SamplerWrap wrap, SamplerFilter filter)
	{
		texture2DConfig.Wrap = wrap;
		texture2DConfig.Filter = filter;

		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(path,config,SamplerFactory::GetOrAddSamplerToFactory(texture2DConfig));
		}
		return nullptr;
	}

	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, const std::filesystem::path& path, Count<class RenderSampler> sampler)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(path, config, sampler);
		}
		return nullptr;
	}

	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, Buffer data, SamplerWrap wrap, SamplerFilter filter)
	{
		texture2DConfig.Wrap = wrap;
		texture2DConfig.Filter = filter;
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(config,data, SamplerFactory::GetOrAddSamplerToFactory(texture2DConfig));
		}
		return nullptr;
	}

	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, Buffer data, Count<class RenderSampler> sampler)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(config, data, sampler);
		}
		return nullptr;
	}

	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, SamplerWrap wrap, SamplerFilter filter)
	{

		texture2DConfig.Wrap = wrap;
		texture2DConfig.Filter = filter;

		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(config, SamplerFactory::GetOrAddSamplerToFactory(texture2DConfig));
		}
		return nullptr;
	}

	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, Count<class RenderSampler> sampler)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(config, sampler);
		}
		return nullptr;
	}


	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, const std::filesystem::path& path, Count<class RenderSampler> sampler)
	{

		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, path,sampler);
		}
		return nullptr;
	}

	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, const std::filesystem::path& path, SamplerWrap wrap, SamplerFilter filter)
	{

		textureCubeConfig.Wrap = wrap;
		textureCubeConfig.Filter = filter;

		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, path, SamplerFactory::GetOrAddSamplerToFactory(textureCubeConfig));
		}
		return nullptr;
	}

	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, Count<class RenderSampler> sampler)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config,sampler);
		}
		return nullptr;
	}

	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, SamplerWrap wrap, SamplerFilter filter)
	{

		textureCubeConfig.Wrap = wrap;
		textureCubeConfig.Filter = filter;
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, SamplerFactory::GetOrAddSamplerToFactory(textureCubeConfig));
		}
		return nullptr;
	}

	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, Count<Texture2D> texture, Count<class RenderSampler> sampler)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, texture,sampler);
		}
		return nullptr;
	}

	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, Count<Texture2D> texture, SamplerWrap wrap, SamplerFilter filter)
	{
		textureCubeConfig.Wrap = wrap;
		textureCubeConfig.Filter = filter;
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, texture, SamplerFactory::GetOrAddSamplerToFactory(textureCubeConfig));
		}
		return nullptr;
	}

	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, Buffer data, Count<class RenderSampler> sampler)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config,data,sampler);
		}
		return nullptr;
	}
	
	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, Buffer data, SamplerWrap wrap, SamplerFilter filter)
	{
		textureCubeConfig.Wrap = wrap;
		textureCubeConfig.Filter = filter;
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, data, SamplerFactory::GetOrAddSamplerToFactory(textureCubeConfig));
		}
		return nullptr;
	}

	static bool IsImageFormatFloat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R16F:
			case ImageFormat::R32F:
			case ImageFormat::R64F:
			case ImageFormat::RG16F:
			case ImageFormat::RG32F:
			case ImageFormat::RG64F:
			case ImageFormat::RGB16F:
			case ImageFormat::RGB32F:
			case ImageFormat::RGB64F:
			case ImageFormat::RGBA16F:
			case ImageFormat::RGBA32F:
			case ImageFormat::RGBA64F:
				return true;
			default: return false;
		}
		PF_CORE_ASSERT(false);
	}
	Buffer TextureImporter::ToBufferFromFile(const std::filesystem::path& path, ImageFormat& outFormat, uint32_t& outWidth, uint32_t& outHeight)
	{
		Buffer imageBuffer;
		std::string pathString = path.string();
		if (!FileSystem::Exists(pathString))
		{
			PF_ENGINE_ERROR("Texture File {} does not exist", pathString);
			return {};
		}

		int width, height, channels;

		if (stbi_is_hdr(pathString.c_str()))
		{
			stbi_set_flip_vertically_on_load(0);

			outFormat = ImageFormat::RGBA32F;
			imageBuffer.Data = (byte*)stbi_loadf(pathString.c_str(), &width, &height, &channels, 4);
			//imageBuffer.Size = width * height * 4 * sizeof(float);
			imageBuffer.Size = width * height * Utils::BytesPerPixel(outFormat);

		}
		else 
		{
			stbi_set_flip_vertically_on_load(0);

			outFormat = ImageFormat::RGBA;
			imageBuffer.Data = stbi_load(pathString.c_str(), &width, &height, &channels, 4);
			imageBuffer.Size = width * height * Utils::BytesPerPixel(outFormat);
		}

		if (!imageBuffer.Data)
			return {};

		outWidth = width;
		outHeight = height;
		return imageBuffer;
	}
	//Buffer TextureImporter::ToBufferFromMemory(const void* data, ImageFormat format, uint32_t width, uint32_t height)
	//{
	//	size_t bufferSize = width * height * Utils::BytesPerPixel(format);
	//	return Buffer((uint8_t*)data,bufferSize,true);
	//}

	Buffer TextureImporter::ToBufferFromMemory(Buffer buffer, ImageFormat& outFormat, uint32_t& outWidth, uint32_t& outHeight)
	{
		Buffer imageBuffer;

		int width, height, channels;
		if (stbi_is_hdr_from_memory((const stbi_uc*)buffer.Get(), (int)buffer.GetSize()))
		{
			const void* rawData = (byte*)stbi_loadf_from_memory((const stbi_uc*)buffer.Get(), (int)buffer.GetSize(), &width, &height, &channels, STBI_rgb_alpha);
			const auto size= width * height * 4 * sizeof(float);
			outFormat = ImageFormat::RGBA32F;
			imageBuffer.SetData(rawData, size,0);
		}
		else
		{
			imageBuffer.Data = stbi_load_from_memory((const stbi_uc*)buffer.Data, (int)buffer.Size, &width, &height, &channels, STBI_rgb_alpha);
			imageBuffer.Size = width * height * 4;
			outFormat = ImageFormat::RGBA;
		}

		if (!imageBuffer.Data)
			return {};

		outWidth = width;
		outHeight = height;
		return imageBuffer;
	}

	Environment::Environment()
	{
		s_Instances.push_back(this);
		m_EnvironmentState = EnvironmentState::PreethamSky;
		m_IsUpdated = true;
		m_IrradianceMap = Renderer::GetBlackTextureCube();
		m_PrefilterMap = Renderer::GetBlackTextureCube();
	}

	Environment::Environment(HosekWilkieSkyData data)
	{
		s_Instances.push_back(this);
		m_EnvironmentState = EnvironmentState::HosekWilkie;
		m_IsUpdated = true;
		m_HosekWilkieSky = data;
		m_IrradianceMap = Renderer::GetBlackTextureCube();
		m_PrefilterMap = Renderer::GetBlackTextureCube();
	}

	Environment::Environment(PreethamSkyData data)
	{
		s_Instances.push_back(this);
		m_EnvironmentState = EnvironmentState::PreethamSky;
		m_IsUpdated = true;
		m_PreethamSky = data;
		m_IrradianceMap = Renderer::GetBlackTextureCube();
		m_PrefilterMap = Renderer::GetBlackTextureCube();
	}

	Environment::Environment(EnvironmentTextureData data)
	{
		s_Instances.push_back(this);
		m_EnvironmentState = EnvironmentState::EnvironmentTexture;
		m_EnvironmentTexture = data;

		if (AssetManager::HasAsset(m_EnvironmentTexture.Image))
		{
			m_IsUpdated = true;
		}
		else
		{
			m_EnvironmentTexture.Image = 0;
		}
		m_IrradianceMap = Renderer::GetBlackTextureCube();
		m_PrefilterMap = Renderer::GetBlackTextureCube();
	}

	Environment::~Environment()
	{
		PF_CORE_ASSERT(Utils::Remove(s_Instances, WeakCount<Environment>(this)),"This should exist");
	}

	void Environment::Update(HosekWilkieSkyData data)
	{
		if (m_EnvironmentState == EnvironmentState::HosekWilkie)
		{
			if (m_HosekWilkieSky != data)
			{
				m_HosekWilkieSky = data;
				m_IsUpdated = true;
			}
			return;
		}
		m_IsUpdated = true;
		m_EnvironmentState = EnvironmentState::HosekWilkie;
		m_HosekWilkieSky = data;
	}
	void Environment::Update(PreethamSkyData data) 
	{
		if (m_EnvironmentState == EnvironmentState::PreethamSky)
		{
			if (m_PreethamSky != data)
			{
				m_PreethamSky = data;
				m_IsUpdated = true;
			}
			return;
		}
		m_IsUpdated = true;
		m_EnvironmentState = EnvironmentState::PreethamSky;
		m_PreethamSky = data;
	}
	void Environment::Update(EnvironmentTextureData data)
	{
		if (m_EnvironmentState == EnvironmentState::EnvironmentTexture)
		{
			if (m_EnvironmentTexture != data)
			{
				m_EnvironmentTexture = data;
				if (AssetManager::HasAsset(m_EnvironmentTexture.Image))
				{
					m_EnvironmentTexture = data;
					m_IsUpdated = true;
				}
				else
				{
					m_EnvironmentTexture.Image = 0;
				}
			}
			return;
		}
		m_IsUpdated = true;
		m_EnvironmentState = EnvironmentState::EnvironmentTexture;
		m_EnvironmentTexture = data;
	}

}