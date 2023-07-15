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
	Count<Texture2D> Texture2D::Create(const void* data,const TextureConfiguration& config)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanTexture2D>::Create(config,data);
		}
		return nullptr;
	}
	Count<TextureCube> TextureCube::Create(const std::filesystem::path& Path, uint32_t dimension, bool generateMips) {
		switch (Renderer::GetAPI()) {
		case Renderer::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
		case Renderer::API::OpenGL: return nullptr;
		//case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(Path,dimension,generateMips);
		}
		return nullptr;

	}

	Count<TextureCube> TextureCube::Create(Count<TextureCube>map, Count<class Shader> shader, uint32_t dimension, bool generateMips) {
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			//case Renderer::API::Vulkan: return Count<VulkanTextureCube>::Create(map, shader,dimension, generateMips);
		}
		return nullptr;

	}
	Count<TextureCube> TextureCube::GeneratePrefiltered(Count<TextureCube>map, uint32_t dimenison , uint32_t numSamples)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			//case Renderer::API::Vulkan: return VulkanTextureCube::GeneratePreFilterMap(map, dimenison,numSamples);
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
	Buffer TextureImporter::ToBufferFromFile(const std::filesystem::path& path, ImageFormat format, uint32_t& width,  uint32_t& height)
	{
		int channels;
		int widthint;
		int heightInt;

		stbi_set_flip_vertically_on_load(true);
		if (IsImageFormatFloat(format))
		{
			float* data = stbi_loadf(path.string().c_str(), &widthint, &heightInt, &channels, 0);
			if (data == nullptr)
			{
				PF_ENGINE_ERROR("Texture passed is Invalid {}", path.string().c_str());
				return Buffer();
			}
			// prrety sure flaot is already 4 so it is the smae as width * height * 4 *4 im guessing i forgot
			float* data_rgba = new float[size_t(width) * size_t(height) * 4];
			if (channels == 3)
			{
				for (size_t i = 0; i < size_t(width) * size_t(height); i++)
				{
					for (size_t c = 0; c < 3; c++)
					{
						data_rgba[4 * i + c] = data[3 * i + c];
					}
					data_rgba[4 * i + 3] = 1.0f;
				}
			}
			else
			{
				memcpy(data_rgba, data, size_t(width) * size_t(height) * 4 * 4);
			}
			stbi_image_free(data);
			width = widthint;
			height = heightInt;
			return Buffer((uint8_t*)data_rgba, size_t(width) * size_t(height) * 4 * 4);
		}

		uint8_t* data = stbi_load(path.string().c_str(), &widthint, &heightInt, &channels, 4);
		if (data == nullptr)
		{
			PF_ENGINE_ERROR("Texture passed is Invalid {}", path.string().c_str());
			return Buffer();
		}
		int imageSize = widthint * heightInt * Utils::BytesPerPixel(format);
		Buffer buffer(data, imageSize,true);
		stbi_image_free(data);
		width = widthint;
		height = heightInt;
		return buffer;
	}
	Buffer TextureImporter::ToBufferFromMemory(const void* data, ImageFormat format, uint32_t width, uint32_t height)
	{
		size_t bufferSize = width * height * Utils::BytesPerPixel(format);
		return Buffer((uint8_t*)data,bufferSize,true);
	}
	
}