#include "Proofprch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/Vulkan/VulkanTexutre.h"
//#include "Platform/OpenGL/OpenGLCubeMap.h"
namespace Proof {

	Count<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, ImageFormat format, const void* data) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanTexture2D>(width,height,format, data);
		}
		return nullptr;
	}
	Count<Texture2D> Texture2D::GenerateBRDF(uint32_t dimension, uint32_t sampleCount)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount<OpenGLTexture2D>(Path, _TextureType);
			case RendererAPI::API::Vulkan: return VulkanTexture2D::GenerateBRDF(dimension,sampleCount);
		}
		return nullptr;
	}
	Count<Texture2D> Texture2D::Create(TextureConfig config)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount<OpenGLTexture2D>(Path, _TextureType);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanTexture2D>(config);
		}
		return nullptr;
	}
	Count<Texture2D> Texture2D::Create(const std::string& Path) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount<OpenGLTexture2D>(Path, _TextureType);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanTexture2D>(Path);
		}
		return nullptr;
	}
	Count<CubeMap> CubeMap::Create(const std::filesystem::path& Path, uint32_t dimension, bool generateMips) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return CreateCount<VulkanCubeMap>(Path,dimension,generateMips);
		}
		return nullptr;

	}

	Count<CubeMap> CubeMap::Create(Count<CubeMap>map, Count<class Shader> shader, uint32_t dimension, bool generateMips) {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanCubeMap>(map, shader,dimension, generateMips);
		}
		return nullptr;

	}
	Count<CubeMap> CubeMap::GeneratePrefiltered(Count<CubeMap>map, uint32_t dimenison , uint32_t numSamples)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return VulkanCubeMap::GeneratePreFilterMap(map, dimenison,numSamples);
		}
		return nullptr;
	}

}