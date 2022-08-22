#include "Proofprch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "3DRenderer/Renderer3D.h"
#include "3DRenderer/Renderer3DPBR.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/MeshWorkShop.h"
namespace Proof {

	ShaderLibrary* Renderer::AllShaders = new ShaderLibrary;
	std::string Renderer::s_RenderCompany;
	std::string Renderer::s_GraphicsCard;
	std::string Renderer::s_GraphicsCardVersion;
	Count<class GraphicsContext>Renderer::m_GraphicsContext = nullptr;
	uint32_t Renderer::s_CurrentFrame = 0;
	void Renderer::Init(Window* window) {
		PF_PROFILE_FUNC();

		m_GraphicsContext = GraphicsContext::Create(window);
		Renderer3DCore::Init();
		Renderer3DPBR::Init();
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)return;

		Renderer2D::Init();
		Renderer2D::Reset();
		MeshWorkShop::Init();

		PF_ENGINE_TRACE("Renderer Initilized");
	}
	void Renderer::Destroy() {
		PF_PROFILE_FUNC();

		Renderer3DPBR::Destroy();

	}
}