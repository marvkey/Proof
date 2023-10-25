#include "Proofprch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/Vulkan/VulkanTexutre.h"
#include "Platform/Vulkan/VulkanImage.h"
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"../vendor/stb_image.h"
namespace Proof {
	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, const std::filesystem::path& path)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(path,config);
		}
		return nullptr;
	}
	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config, Buffer data)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(config,data);
		}
		return nullptr;
	}

	Count<Texture2D> Texture2D::Create(const TextureConfiguration& config)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(config);
		}
		return nullptr;
	}

	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, const std::filesystem::path& path)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, path);
		}
		return nullptr;
	}
	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config);
		}
		return nullptr;
	}
	Count<TextureCube> TextureCube::Create(const TextureConfiguration& config, Count<Texture2D> texture)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(config, texture);
		}
		return nullptr;
	}
	Count<TextureCube> TextureCube::Create(const void* data, const TextureConfiguration& config)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(data,config);
		}
		return nullptr;
	}
	Count<Image2D> Image2D::Create(const ImageConfiguration& specification)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImage2D>::Create(specification);
		}
		return nullptr;
	}

	Count<ImageView> ImageView::Create(const ImageViewConfiguration& specification)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImageView>::Create(specification);
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
			stbi_set_flip_vertically_on_load(1);

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
			imageBuffer.SetData(stbi_load_from_memory((const stbi_uc*)buffer.Get(), (int)buffer.GetSize(), &width, &height, &channels, STBI_rgb_alpha), width * height * 4,0);
		}

		if (!imageBuffer.Get())
			return {};

		outWidth = width;
		outHeight = height;
		return imageBuffer;
	}


}