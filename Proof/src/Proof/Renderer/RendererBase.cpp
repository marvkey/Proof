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
#include "SwapChain.h"
namespace Proof {

	ShaderLibrary* RendererBase::AllShaders = new ShaderLibrary;
	std::string RendererBase::s_RenderCompany;
	std::string RendererBase::s_GraphicsCard;
	std::string RendererBase::s_GraphicsCardVersion;
	BaseTextures* RendererBase::s_BaseTextures = nullptr;
	Count<class GraphicsContext> RendererBase::m_GraphicsContext = nullptr;

	void RendererBase::Init(Window* window) {
		PF_PROFILE_FUNC();
		if (RendererAPI::ActiveAPI == Renderer::API::Vulkan) {
			Renderer::s_RendererAPI = new VulkanRendererAPI();
		}
		m_GraphicsContext = GraphicsContext::Create(window);
		Renderer::s_RendererAPI->SetGraphicsContext(m_GraphicsContext);

		window->m_SwapChain = SwapChain::Create(ScreenSize{ Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight() });
		Renderer::s_RendererAPI->Init();


		AllShaders->LoadShader("ProofPBR_Static", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/ProofPBR_Static.shader");
		AllShaders->LoadShader("BRDFLUT", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/BRDFLut.glsl");
		AllShaders->LoadShader("EquirectangularToCubemap", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EquirectangularToCubemap.glsl");
		AllShaders->LoadShader("SkyBox", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/SkyBox.glsl");
		AllShaders->LoadShader("EnvironmentIrradiance", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentIrradiance.glsl");
		AllShaders->LoadShader("EnvironmentPrefilter", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentPrefilter.glsl");

		s_BaseTextures = new  BaseTextures();
		PF_ENGINE_TRACE("Renderer Initilized");
	}

	void RendererBase::Destroy() {
		PF_PROFILE_FUNC();
		// delete the graphics conttext tehn destroy render api
		delete AllShaders;
		delete s_BaseTextures;
		m_GraphicsContext = nullptr;
		Renderer::s_RendererAPI->Destroy();
		delete Renderer::s_RendererAPI;
		Renderer::s_RendererAPI = nullptr;
	}
	BaseTextures::BaseTextures() {
		uint32_t whiteTexturedata = 0xffffffff;
		WhiteTexture = Texture2D::Create(&whiteTexturedata,TextureConfiguration("White Texture") );


		uint32_t blackTexturedata = 0xFF000000;
		BlackTexture = Texture2D::Create(&blackTexturedata,TextureConfiguration("Black Texture"));


		TextureConfiguration cubeTextureConfig;
		cubeTextureConfig.GenerateMips = false;
		cubeTextureConfig.Height = 1024;
		cubeTextureConfig.Width = 1024;
		cubeTextureConfig.Storage = true;
		cubeTextureConfig.Format = ImageFormat::RGBA16F;
		cubeTextureConfig.Wrap = TextureWrap::ClampEdge;

		uint8_t* data = new uint8_t[cubeTextureConfig.Height * cubeTextureConfig.Width * Utils::BytesPerPixel(ImageFormat::RGBA16F)];
		// since image is in format is a float we use 0x3F to normalize for floating poitn for white
		std::memset(data, 0x3F, cubeTextureConfig.Height * cubeTextureConfig.Width * Utils::BytesPerPixel(ImageFormat::RGBA16F));
		WhiteTextureCube = TextureCube::Create(data, cubeTextureConfig);
		
		std::memset(data, 0, cubeTextureConfig.Height * cubeTextureConfig.Width * Utils::BytesPerPixel(ImageFormat::RGBA16F));
		BlackTextureCube = TextureCube::Create(data, cubeTextureConfig);
		delete[] data;	
	}
}