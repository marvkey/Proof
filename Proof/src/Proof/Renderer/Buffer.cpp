#include "Proofprch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "RendererAPI.h"
namespace Proof {
	Count<VertexBuffer>VertexBuffer::Create(const void* Data,uint32_t amount) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Vertex Buffer None it needs an api"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount< OpenGLVertexBuffer>(Data,amount);
		}
	}

	Count<VertexBuffer> VertexBuffer::Create(uint32_t amount) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Vertex Buffer None it needs an api"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount< OpenGLVertexBuffer>(amount);
		}
	}

	Count<IndexBuffer>IndexBuffer::Create(const void* Data,uint32_t Count) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Index Buffer None it needs an api "); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount< OpenGLIndexBuffer>(Data,Count);
		}
	}
	Count<RenderBuffer> RenderBuffer::Create(RenderBufferAttachment type,uint32_t width,uint32_t height) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false,"RenderBuffer None it needs an api "); return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLRenderBuffer>(type,width,height);
		}
	}
}