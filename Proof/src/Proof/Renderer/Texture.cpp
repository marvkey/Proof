#include "Proofprch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"
namespace Proof {
	Count<Texture2D> Texture2D::Create(const std::string& Path,TextureType _TextureType) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLTexture2D>(Path,_TextureType);
		
		}
	}

	Count<Texture2D> Texture2D::Create(uint32_t ImageWidth,uint32_t ImageHeight,TextureType _TextureType) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLTexture2D>(ImageWidth,ImageHeight,_TextureType);
		}
	}
}