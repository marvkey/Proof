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
	Count<Texture2D> Texture2D::Create(const std::string& Path) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount<OpenGLTexture2D>(Path, _TextureType);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanTexture2D>(Path);
		}
		return nullptr;
	}
	Count<CubeMap> CubeMap::Create(const std::filesystem::path& Path) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
		case RendererAPI::API::OpenGL: return nullptr;
		case RendererAPI::API::Vulkan: return CreateCount<VulkanCubeMap>(Path);
		}
		return nullptr;

	}

}