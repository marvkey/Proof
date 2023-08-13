#include "Proofprch.h"
#include "Renderer.h"
#include "CommandQueue.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "ComputePass.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "ComputePipeline.h"
#include "Shader.h"
#include "RenderMaterial.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
#include "GraphicsPipeLine.h"
#include "VertexArray.h"
#include "UniformBuffer.h"
#include "MeshWorkShop.h"
#include "Vertex.h"
#include "Proof/Scene/Mesh.h"
#include "Buffer.h"
#include "SwapChain.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "GraphicsContext.h"
namespace Proof {
	static std::vector<CommandQueue*> s_RenderCommandQueue;
	static uint32_t s_CommandQueueIndex = 1; // so we start with 0 command qeue index
	static class RendererAPI* s_RendererAPI;
	static Count<class GraphicsContext> GraphicsContext;

	static Count<class ShaderLibrary> ShaderLibrary;
	static BaseTextures* s_BaseTextures;

	static std::unordered_map<std::string, std::string> s_ShaderDefines;
	void Renderer::Init(Window* window)
	{
		if (RendererAPI::ActiveAPI == Renderer::API::Vulkan)
			s_RendererAPI = new VulkanRendererAPI();

		GraphicsContext = GraphicsContext::Create(window);
		s_RendererAPI->SetGraphicsContext(GraphicsContext);
		window->m_SwapChain = SwapChain::Create(ScreenSize{ Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight() });
		s_RendererAPI->Init();

		ShaderLibrary = Count<class ShaderLibrary>::Create();

		//PBR
		ShaderLibrary->LoadShader("ProofPBR_Static", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/ProofPBR_Static.glsl");
		//2D
		ShaderLibrary->LoadShader("Base2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Base2D.shader");

		//IBL
		ShaderLibrary->LoadShader("BRDFLUT", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/BRDFLut.glsl");
		ShaderLibrary->LoadShader("EquirectangularToCubemap", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EquirectangularToCubemap.glsl");
		ShaderLibrary->LoadShader("SkyBox", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/SkyBox.glsl");
		ShaderLibrary->LoadShader("EnvironmentIrradiance", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentIrradiance.glsl");
		ShaderLibrary->LoadShader("EnvironmentIrradianceNonCompute", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentIrradianceNonCompute.glsl");
		ShaderLibrary->LoadShader("EnvironmentPrefilter", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentPrefilter.glsl");

		//Shadows
		ShaderLibrary->LoadShader("DebugShadowMap", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Shadow/DebugShadowMap.glsl");
		ShaderLibrary->LoadShader("ShadowDepthPass", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Shadow/ShadowDepthPass.glsl");

		s_BaseTextures = new  BaseTextures();
		s_RenderCommandQueue.resize(2);
		s_RenderCommandQueue[0] = new CommandQueue();
		s_RenderCommandQueue[1] = new CommandQueue();

		PF_ENGINE_TRACE("Renderer Initialized");

	}

	void Renderer::Shutdown()
	{
		delete s_RenderCommandQueue[0];
		delete s_RenderCommandQueue[1];
		s_RenderCommandQueue.clear();

		delete s_BaseTextures;
		ShaderLibrary = nullptr;
		GraphicsContext = nullptr;
		delete s_RendererAPI;
	}
	const std::unordered_map<std::string, std::string>& Renderer::GetShaderDefines()
	{
		return s_ShaderDefines;
	}

	Count<class ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return ShaderLibrary;
	}

	void Renderer::DrawElementIndexed(Count<RenderCommandBuffer> commandBuffer, uint32_t indexCount,uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		s_RendererAPI->DrawElementIndexed(commandBuffer, indexCount, instanceCount, firstIndex,vertexOffset,firstInstance);
	}

	void Renderer::DrawArrays(Count<RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount,
		uint32_t firstVertex, uint32_t firstInstance)
	{
		s_RendererAPI->DrawArrays(commandBuffer,vertexCount,instanceCount,firstVertex,firstInstance);
	}

	void Renderer::BeginCommandBuffer(Count<RenderCommandBuffer> commandBuffer)
	{
		s_RendererAPI->BeginCommandBuffer(commandBuffer);
	}

	void Renderer::EndCommandBuffer(Count<RenderCommandBuffer> commandBuffer)
	{
		s_RendererAPI->EndCommandBuffer(commandBuffer);
	}

	void Renderer::BeginRenderPass(Count<RenderCommandBuffer> commandBuffer, Count<RenderPass> renderPass, bool explicitClear)
	{
		s_RendererAPI->BeginRenderPass(commandBuffer, renderPass,explicitClear);
	}

	void Renderer::BeginRenderMaterialRenderPass(Count<RenderCommandBuffer> commandBuffer, Count<RenderPass> renderPass, bool explicitClear)
	{
		s_RendererAPI->BeginRenderMaterialRenderPass(commandBuffer, renderPass, explicitClear);
	}

	void Renderer::EndRenderPass(Count<RenderPass> renderPass)
	{
		s_RendererAPI->EndRenderPass(renderPass);
	}

	void Renderer::RenderPassPushRenderMaterial(Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial)
	{
		s_RendererAPI->RenderPassPushRenderMaterial(renderPass, renderMaterial);
	}

	void Renderer::BeginComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computPass)
	{
		s_RendererAPI->BeginComputePass(commandBuffer,computPass);
	}

	void Renderer::BeginRenderMaterialComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computPass)
	{
		s_RendererAPI->BeginRenderMaterialComputePass(commandBuffer,computPass);
	}

	void Renderer::EndComputePass(Count<ComputePass> computPass)
	{
		s_RendererAPI->EndComputePass(computPass);
	}

	void Renderer::ComputePassPushRenderMaterial(Count<class ComputePass> computePass, Count<class RenderMaterial> renderMaterial)
	{
		s_RendererAPI->ComputePassPushRenderMaterial(computePass, renderMaterial);
	}

	void Renderer::SubmitCommandBuffer(Count<RenderCommandBuffer> commandBuffer)
	{
		s_RendererAPI->SubmitCommandBuffer(commandBuffer);
	}

	CurrentFrame Renderer::GetCurrentFrame()
	{
		return s_RendererAPI->GetCurrentFrame();
	}

	const RendererConfig Renderer::GetConfig()
	{
		return s_RendererAPI->GetConfig();
	}

	Count<class GraphicsContext> Renderer::GetGraphicsContext()
	{
		return GraphicsContext;
	}

	void Renderer::SubmitCommand(std::function<void(CommandBuffer*)> func)
	{
		s_RendererAPI->Submit(func);
	}

	void Renderer::SubmitDatafree(std::function<void()> func)
	{
		s_RendererAPI->SubmitDatafree(func);
	}

	Renderer::API Renderer::GetAPI()
	{ return RendererAPI::GetAPI(); }

	void Renderer::OnWindowResize(WindowResizeEvent& e)
	{
		s_RendererAPI->OnWindowResize(e);
	}

	Count<Texture2D>Renderer::GetWhiteTexture(){
		return s_BaseTextures->WhiteTexture;
	}

	Count<class Texture2D> Renderer::GetBlackTexture()
	{
		return s_BaseTextures->BlackTexture;
	}

	Count<class TextureCube> Renderer::GetWhiteTextureCube()
	{
		return s_BaseTextures->WhiteTextureCube;
	}

	Count<class TextureCube> Renderer::GetBlackTextureCube()
	{
		return s_BaseTextures->BlackTextureCube;
	}

	Count<class Shader> Renderer::GetShader(const std::string& name)
	{
		return ShaderLibrary->GetShader(name);
	}

	std::pair<Count<class TextureCube>, Count<class TextureCube>> Renderer::CreateEnvironmentMap(const std::filesystem::path& path)
	{
		//Refrecnce
		//https://github.com/Nadrin/PBR/blob/master/src/vulkan.cpp

		Count<TextureCube> environmentMapImageCube;

		ImageFormat format = ImageFormat::RGBA16F;
		{
			const uint32_t imageSize = 1024;
			TextureConfiguration baseCubeMapConfig;
			baseCubeMapConfig.DebugName = Utils::FileDialogs::GetFileName(path)+ " Base CubeMap";
			baseCubeMapConfig.Height = imageSize;
			baseCubeMapConfig.Width = imageSize;
			baseCubeMapConfig.Storage = true;
			baseCubeMapConfig.GenerateMips = true;
			baseCubeMapConfig.Format = format;
			baseCubeMapConfig.Wrap = TextureWrap::ClampEdge;

			environmentMapImageCube = TextureCube::Create(baseCubeMapConfig, path);
		}
		const uint32_t irradianceFilterRate = 32;
		Count<TextureCube> irradianceMap; 
		{
			TextureConfiguration irradianceTextureConfig;
			irradianceTextureConfig.DebugName = "Irradiance map " + Utils::FileDialogs::GetFileName(path);
			irradianceTextureConfig.Width = irradianceFilterRate;
			irradianceTextureConfig.Height = irradianceFilterRate;
			irradianceTextureConfig.Storage = true;
			irradianceTextureConfig.Format = format;
			irradianceTextureConfig.Wrap = TextureWrap::ClampEdge;
			irradianceTextureConfig.GenerateMips = true;
			irradianceMap = TextureCube::Create(irradianceTextureConfig);
		}
		
		{
			/*
			* The size of the irradiance map, usually a cubemap, is typically much smaller than the original environment map from which it is derived.
			* Common sizes for irradiance maps are 32x32, 64x64, 128x128, or 256x256 pixels per face.
			* Smaller sizes offer better performance but may result in lower quality, while larger sizes provide better quality but consume more memory and processing power.
			*/
			
			ComputePipelineConfig computePipelineConfig;
			computePipelineConfig.DebugName = "Irradiance Pipline";
			computePipelineConfig.Shader = GetShader("EnvironmentIrradiance");
			
			Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);
			ComputePassConfiguration computePassConfig;
			computePassConfig.DebugName = "EnvironmentIrradiance Pass";
			computePassConfig.Pipeline = computePipeline;
			
			auto computePass = ComputePass::Create(computePassConfig);
			computePass->SetInput("inputTexture", environmentMapImageCube);
			computePass->SetInput("outputTexture", irradianceMap);
			Renderer::SubmitCommand([&](CommandBuffer* buffer) {
			
				Count<RenderCommandBuffer>renderCommandBuffer = RenderCommandBuffer::Create(buffer);
				Renderer::BeginComputePass(renderCommandBuffer, computePass);
				computePass->Dispatch(irradianceFilterRate / 32, irradianceFilterRate / 32, 6);
				Renderer::EndComputePass(computePass);
			
			});
			irradianceMap->GenerateMips();

		}

		{
			//FrameBufferConfig frameConfig;
			//frameConfig.DebugName = "Texture-CubeIrradiance";
			//frameConfig.Size.X = irradianceFilterRate;
			//frameConfig.Size.Y = irradianceFilterRate;
			//frameConfig.Attachments = { format};
			//frameConfig.Attachments.Attachments[0].ExistingImage = irradianceMap->GetImage();
			//
			//Count<FrameBuffer> frameBuffer = FrameBuffer::Create(frameConfig);
			//
			//GraphicsPipelineConfig pipelineConfig;
			//pipelineConfig.DebugName = "generate cubemap create";
			//pipelineConfig.Shader = Shader::Get("EnvironmentIrradianceNonCompute");
			//
			//pipelineConfig.VertexArray = VertexArray::Create({ sizeof(Vertex) });
			//pipelineConfig.VertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
			//pipelineConfig.TargetBuffer = frameBuffer;
			//Count<GraphicsPipeline> renderPipeline = GraphicsPipeline::Create(pipelineConfig);
			//
			//RenderPassConfig renderPassConfig;
			//renderPassConfig.DebugName = "Irradiance Map Non compute";
			//renderPassConfig.MultiView = true;
			//renderPassConfig.Pipeline = renderPipeline;
			//renderPassConfig.Attachments = { format };
			//Count<RenderPass> renderPass = RenderPass::Create(renderPassConfig);
			//
			//struct UboData {
			//	/// Projection matrix common to each face of the cubemap.
			//	alignas(16) glm::mat4 projection;
			//
			//	/// View matrix to look at the direction of each cubemap face.
			//	alignas(16) glm::mat4 view[6];
			//};
			//UboData uboData = {
			//glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f),
			//	{
			//		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
			//		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			//		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			//		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			//		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			//		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			//	}
			//};
			//
			//Count<UniformBuffer> ubuffer = UniformBuffer::Create(&uboData, sizeof(UboData));
			//auto cube = MeshWorkShop::GenerateCube();
			//
			//renderPass->SetInput("ProjView", ubuffer);
			//renderPass->SetInput("u_EnvironmentMap", environmentMapImageCube);
			//Renderer::SubmitCommand([&](CommandBuffer* buffer) {
			//	Count<RenderCommandBuffer> renderCmd = RenderCommandBuffer::Create(buffer);
			//
			//	Renderer::BeginRenderPass(renderCmd, renderPass);
			//	cube->GetMeshSource()->GetVertexBuffer()->Bind(renderCmd);
			//	cube->GetMeshSource()->GetIndexBuffer()->Bind(renderCmd);
			//
			//	for (const auto& subMesh : cube->GetMeshSource()->GetSubMeshes())
			//	{
			//		Renderer::DrawElementIndexed(renderCmd, subMesh.IndexCount, 1, subMesh.BaseIndex, subMesh.BaseVertex);
			//	}
			//
			//	Renderer::EndRenderPass(renderPass);
			//});
			//irradianceMap.As<VulkanTextureCube>()->GenerateMips();
		}
		const uint32_t prefilterFilterRate = 1024;

		Count<TextureCube> prefilterMap;
		{
			TextureConfiguration prefilterTextureConfig;
			prefilterTextureConfig.DebugName = "Prefilter map" + Utils::FileDialogs::GetFileName(path);
			prefilterTextureConfig.Width = prefilterFilterRate;
			prefilterTextureConfig.Height = prefilterFilterRate;
			prefilterTextureConfig.Storage = true;
			prefilterTextureConfig.Format = format;
			prefilterTextureConfig.Wrap = TextureWrap::ClampEdge;
			prefilterTextureConfig.GenerateMips = true;

			prefilterMap = TextureCube::Create(prefilterTextureConfig,path);

		}
	
		{
			//https://github.com/Nadrin/PBR/blob/master/src/vulkan.cpp
			/*
			
			Prefiltered maps have higher resolution than irradiance maps to capture finer details of specular reflections.
			Common sizes for prefiltered environment maps range from 128x128 to 1024x1024 pixels per face for cubemaps.
			Larger sizes can be used for better quality, but it comes at the cost of increased memory usage and longer precomputing times

			*/
			uint32_t maxMip = prefilterMap->GetMipLevelCount();
			std::vector<Count<ImageView>> prefilterImageViews;
			for (uint32_t mip = 1; mip < maxMip; mip++)
			{
				ImageViewConfiguration imageViewConfig;
				imageViewConfig.DebugName = fmt::format("Prefilter View mip: {}", mip);
				imageViewConfig.Layer = 0;
				imageViewConfig.LayerCount = 6;
				imageViewConfig.Mip = mip;
				imageViewConfig.MipCount = 1;
				imageViewConfig.View = ImageViewType::ViewCube;
				imageViewConfig.Image = prefilterMap->GetImage();

				Count<ImageView> view = ImageView::Create(imageViewConfig);
				prefilterImageViews.push_back(view);
			}

			ComputePipelineConfig computePipelineConfig;
			computePipelineConfig.DebugName = "Prefilter Pipline";
			computePipelineConfig.Shader = GetShader("EnvironmentPrefilter");
			
			Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);
			ComputePassConfiguration computePassConfig;
			computePassConfig.DebugName = "EnvironmentPrefilter Pass";
			computePassConfig.Pipeline = computePipeline;

			Count<RenderMaterial> prefilterMaterial = RenderMaterial::Create(RenderMaterialConfiguration("PrefilterMap Material", GetShader("EnvironmentPrefilter")));
			prefilterMaterial->Set("u_EnvironmentMap", environmentMapImageCube);
			prefilterMaterial->Set("u_PrefilterMap", prefilterImageViews);
			auto computePass = ComputePass::Create(computePassConfig);

			Renderer::SubmitCommand([&](CommandBuffer* buffer) {
				Count<RenderCommandBuffer>renderCommandBuffer = RenderCommandBuffer::Create(buffer);

				Renderer::BeginRenderMaterialComputePass(renderCommandBuffer, computePass);
				const float deltaRoughness = 1.f / glm::max((float)(maxMip-1), 1.0f);

				for (uint32_t mip = 1, mipsize = prefilterFilterRate/2; mip < maxMip; mip++, mipsize /=2)
				{
					uint32_t numGroups = glm::max(mipsize / 32, 1u); // Ensure numGroups is at least 1
					float roughness = deltaRoughness * mip;
					roughness = glm::max(roughness, 0.05f);
					prefilterMaterial->Set("Input.Level", mip-1);
					prefilterMaterial->Set("Input.Roughness", roughness);

					Renderer::ComputePassPushRenderMaterial(computePass, prefilterMaterial);
					computePass->Dispatch(numGroups, numGroups, 6);
				}
				Renderer::EndComputePass(computePass);
			});
		}
		return std::make_pair(irradianceMap, prefilterMap);
	}
	Count<Texture2D> Renderer::GenerateBRDFLut()
	{
		const uint32_t imageSize = 512;
		TextureConfiguration textureConfig;
		textureConfig.DebugName = "BrdfLut";
		textureConfig.Height = imageSize;
		textureConfig.Width = imageSize;
		textureConfig.Storage = true;
		textureConfig.Format = ImageFormat::RG16F;
		
		// widht* height * bytes per pixel
		uint8_t* data = new uint8_t[imageSize * imageSize * Utils::BytesPerPixel(ImageFormat::RG16F)];
		//memset(data, 0xFF000000, imageSize * imageSize * 8);
		Count<Texture2D> brdfLut = Texture2D::Create(data,textureConfig);
		
		ComputePipelineConfig computePipelineConfig;
		computePipelineConfig.DebugName = "BRDFLUT Pipeline";
		computePipelineConfig.Shader = GetShader("BRDFLUT");
		
		Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);

		
		ComputePassConfiguration computePassConfig;
		computePassConfig.DebugName = "BRDFLUT Pass";
		computePassConfig.Pipeline = computePipeline;

		auto computePass = ComputePass::Create(computePassConfig);
		
		computePass->SetInput("brfdLUT", brdfLut);

		Renderer::SubmitCommand([&](CommandBuffer* buffer) {

			Count<RenderCommandBuffer>renderCommandBuffer = RenderCommandBuffer::Create(buffer);
			Renderer::BeginComputePass(renderCommandBuffer, computePass);
			computePass->Dispatch(imageSize/16 , imageSize/16 , 1);
			Renderer::EndComputePass(computePass);

		});
		delete[] data;
		return brdfLut;
	}

	CommandQueue& Renderer::GetRenderCommandQueue()
	{
		return *s_RenderCommandQueue[s_CommandQueueIndex];
	}

	void Renderer::BeginFrame()
	{
		if (s_CommandQueueIndex == 1)
			s_CommandQueueIndex = 0;
		else
			s_CommandQueueIndex = 1;

		s_RendererAPI->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		//GetRenderCommandQueue().Execute();
		s_RendererAPI->EndFrame();
	}

	BaseTextures::BaseTextures() {
		uint32_t whiteTexturedata = 0xffffffff;
		WhiteTexture = Texture2D::Create(&whiteTexturedata, TextureConfiguration("White Texture"));


		uint32_t blackTexturedata = 0xFF000000;
		BlackTexture = Texture2D::Create(&blackTexturedata, TextureConfiguration("Black Texture"));


		TextureConfiguration cubeTextureConfig;
		cubeTextureConfig.GenerateMips = true;
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