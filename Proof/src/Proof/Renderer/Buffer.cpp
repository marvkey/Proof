#include "Proofprch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "RendererAPI.h"
namespace Proof {
	Count<VertexBuffer>VertexBuffer::Create(const void* Data,uint32_t Size) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Vertex Buffer None it needs an api"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount< OpenGLVertexBuffer>(Data,Size);
		}
	}

	Count<VertexBuffer> VertexBuffer::Create(uint32_t Size) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Vertex Buffer None it needs an api"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount< OpenGLVertexBuffer>(Size);
		}
	}

	Count<IndexBuffer>IndexBuffer::Create(const void* Data,uint32_t Count) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Index Buffer None it needs an api "); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount< OpenGLIndexBuffer>(Data,Count);
		}
	}
}