#include "Proofprch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "RendererBase.h"
#include "Texture.h"
namespace Proof {
	RendererAPI* Renderer::s_RendererAPI;
	Count<Texture2D>Renderer::GetWhiteTexture(){
		return RendererBase::s_BaseTextures->WhiteTexture;
	}

}