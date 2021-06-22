#include "Proofprch.h"
#include "FrameBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
namespace Proof {
	Count<FrameBuffer> FrameBuffer::Create(uint32_t Width,uint32_t Height) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLFrameBuffer>(Width,Height);
		}
	}
}
