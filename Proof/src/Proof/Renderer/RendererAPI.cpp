#include "Proofprch.h"
#include "RendererAPI.h"
#include "FrameBuffer.h"
namespace Proof {
	Renderer::API RendererAPI::ActiveAPI = Renderer::API::Vulkan;

	RendererAPI::~RendererAPI() {}

}