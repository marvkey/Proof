#include "Proofprch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "3DRenderer/Renderer3D.h"
#include "3DRenderer/Renderer3DPBR.h"
#include "Platform/Vulkan/VulkanRenderer/VulkanRenderer.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/GraphicsContext.h"
#include "Proof/Renderer/MeshWorkShop.h"
namespace Proof {

	ShaderLibrary* Renderer::AllShaders = new ShaderLibrary;
	std::string Renderer::s_RenderCompany;
	std::string Renderer::s_GraphicsCard;
	std::string Renderer::s_GraphicsCardVersion;
	Count<class GraphicsContext>Renderer::m_GraphicsContext = nullptr;

	void Renderer::Init(Window* window) {
		PF_PROFILE_FUNC();

		m_GraphicsContext = GraphicsContext::Create(window);
		Renderer3DCore::Init();
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
			VulkanRenderer::Init();
			window->m_SwapChain = VulkanRenderer::s_Pipeline->SwapChain;
		}
		//Renderer2D::Init();
		//Renderer2D::Reset();
		MeshWorkShop::Init();
		Renderer3DPBR::Init();

		PF_ENGINE_TRACE("Renderer Initilized");
	}
	void Renderer::BeginFrame() {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			VulkanRenderer::BeginFrame();
	}

	void Renderer::EndFrame() {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			VulkanRenderer::EndFrame();
	}
	CurrentFrame Renderer::GetCurrentFrame() {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			return VulkanRenderer::s_CurrentFrame;

		PF_CORE_ASSERT(false, "Need a renderpass");
		return CurrentFrame();
	}
	void Renderer::Destroy() {
		PF_PROFILE_FUNC();
		Renderer3DPBR::Destroy();
		m_GraphicsContext = nullptr;
		delete AllShaders;
	}
	const RendererConfig Renderer::GetConfig() {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			return VulkanRenderer::s_RendererConfig;
	}
	void Renderer::OnWindowResize(WindowResizeEvent& e) {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			VulkanRenderer::OnWindowResize(e);
	}
}