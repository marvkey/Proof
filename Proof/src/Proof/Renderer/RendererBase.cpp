#include "Proofprch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "3DRenderer/Renderer3D.h"
#include "3DRenderer/Renderer3DPBR.h"
#include "Platform/Vulkan/VulkanRenderer/VulkanRenderer.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/GraphicsContext.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Renderer.h"
#include "Shader.h"
#include "platform/Vulkan/VulkanRendererAPI.h"
namespace Proof {

	ShaderLibrary* RendererBase::AllShaders = new ShaderLibrary;
	std::string RendererBase::s_RenderCompany;
	std::string RendererBase::s_GraphicsCard;
	std::string RendererBase::s_GraphicsCardVersion;
	BaseTextures* RendererBase::s_BaseTextures = nullptr;
	Count<class GraphicsContext>RendererBase::m_GraphicsContext = nullptr;

	void RendererBase::Init(Window* window) {
		PF_PROFILE_FUNC();
		if (RendererAPI::ActiveAPI == RendererAPI::API::Vulkan) {
			Renderer::s_RendererAPI = new VulkanRendererAPI();
		}
		m_GraphicsContext = GraphicsContext::Create(window);
		window->m_SwapChain = SwapChain::Create(ScreenSize{ Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight() });
		Renderer::s_RendererAPI->Init();

		s_BaseTextures = new  BaseTextures();
		MeshWorkShop::Init();
		PF_ENGINE_TRACE("Renderer Initilized");
	}

	void RendererBase::Destroy() {
		PF_PROFILE_FUNC();
		// delete the graphics conttext tehn destroy render api
		delete AllShaders;
		delete s_BaseTextures;
		Renderer::s_RendererAPI->Destroy();
		m_GraphicsContext = nullptr;
	}
	BaseTextures::BaseTextures() {
		uint32_t whiteTexturedata = 0xffffffff;
		WhiteTexture = Texture2D::Create(1, 1, ImageFormat::RGBA, &whiteTexturedata);

		uint32_t blackTexturedata = 0xFF000000;
		BlackTexture = Texture2D::Create(1, 1, ImageFormat::RGBA, &blackTexturedata);
	}
}