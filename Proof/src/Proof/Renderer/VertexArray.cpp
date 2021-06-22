#include "Proofprch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "RendererAPI.h"
namespace Proof {
	Count<VertexArray> VertexArray::Create(uint32_t Size) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Vertex Array None it needs an api"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount<OpenGLVertexArray>(Size);
		}
	}
}