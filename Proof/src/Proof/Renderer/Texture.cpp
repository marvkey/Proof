#include "Proofprch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/Vulkan/VulkanTexutre.h"
//#include "Platform/OpenGL/OpenGLCubeMap.h"
namespace Proof {
	Count<Texture2D> Texture2D::Create(uint32_t width,uint32_t height,DataFormat dataFormat,InternalFormat internalFormat,TextureBaseTypes WrapS,TextureBaseTypes WrapT,TextureBaseTypes MinFilter,TextureBaseTypes MagFilter,type baseType,bool usWrap ) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLTexture2D>(width,height,dataFormat,internalFormat,WrapS,WrapT,MinFilter,MagFilter,baseType,usWrap );
		}
	}
	Count<Texture2D> Texture2D::Create(const std::string& Path,TextureType _TextureType) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLTexture2D>(Path,_TextureType);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanTexture2D>(Path);
		}
	}

	Count<Texture2D> Texture2D::Create(uint32_t ImageWidth,uint32_t ImageHeight,TextureType _TextureType) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLTexture2D>(ImageWidth,ImageHeight,_TextureType);
		}
	}
	Count<CubeMap> CubeMap::Create(const std::vector<std::string>& Paths) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount<OpenGLCubeMap>(Paths);
		}
	}
	Count<CubeMap> CubeMap::Create(const std::string& Path) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount<OpenGLCubeMap>(Path);
		}
	}
	Count<CubeMap> CubeMap::Create(uint32_t textureWidht,uint32_t textureHeight,bool generateMipMap) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLCubeMap>(textureWidht,textureHeight,generateMipMap);
		}
	}
	Count<HDRTexture> HDRTexture::Create(const std::string& path) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLHDRTexture>(path);
		}
	}
}