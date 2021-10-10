#include "Proofprch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "platform/OpenGL/OpenGLFrameBuffer.h"
namespace Proof
{
	Count<FrameBuffer> FrameBuffer::Create() {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"FrameBuffer None it needs an api"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount<OpenGLFrameBuffer>();
		}
	}
}