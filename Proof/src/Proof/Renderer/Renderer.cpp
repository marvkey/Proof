#include "Proofprch.h"
#include "Renderer.h"
#include "RendererBase.h"
#include "CommandQueue.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "ComputePass.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "ComputePipeline.h"
#include "Shader.h"
#include "RenderMaterial.h"
#include "Platform/Vulkan/VulkanTexutre.h"
namespace Proof {
	RendererAPI* Renderer::s_RendererAPI;
	static std::vector<CommandQueue*> s_RenderCommandQueue;
	static uint32_t s_CommandQueueIndex = 1; // so we start with 0 command qeue index
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

	Count<GraphicsContext> Renderer::GetGraphicsContext()
	{
		return RendererBase::GetGraphicsContext();
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
		return RendererBase::s_BaseTextures->WhiteTexture;
	}

	Count<class Texture2D> Renderer::GetBlackTexture()
	{
		return RendererBase::s_BaseTextures->BlackTexture;
	}

	Count<class TextureCube> Renderer::GetWhiteTextureCube()
	{
		return RendererBase::s_BaseTextures->WhiteTextureCube;
	}

	Count<class TextureCube> Renderer::GetBlackTextureCube()
	{
		return RendererBase::s_BaseTextures->BlackTextureCube;
	}

	std::pair<Count<class TextureCube>, Count<class TextureCube>> Renderer::CreateEnvironmentMap(const std::filesystem::path& path)
	{
		Count<TextureCube> environmentMapImageCube;

		const uint32_t imageSize = 256;
		ImageFormat format = ImageFormat::RGBA16F;
		{
			TextureConfiguration baseCubeMapConfig;
			baseCubeMapConfig.DebugName = Utils::FileDialogs::GetFileName(path)+ " Base CubeMap";
			baseCubeMapConfig.Height = imageSize;
			baseCubeMapConfig.Width = imageSize;
			baseCubeMapConfig.Storage = true;
			baseCubeMapConfig.GenerateMips = false;
			baseCubeMapConfig.Format = format;
			baseCubeMapConfig.Wrap = TextureWrap::ClampEdge;

			environmentMapImageCube = TextureCube::Create(baseCubeMapConfig, path);
		}
		TextureConfiguration textureConfig;
		textureConfig.DebugName = "Irradiance map "+ Utils::FileDialogs::GetFileName(path);
		textureConfig.Height = imageSize;
		textureConfig.Width = imageSize;
		textureConfig.Storage = true;
		textureConfig.Format = format;
		textureConfig.Wrap = TextureWrap::ClampEdge;
		textureConfig.GenerateMips = true;
		
		Count<TextureCube> irradianceMap = Count<VulkanTextureCube>::Create(textureConfig);
		
		{
			//ComputePipelineConfig computePipelineConfig;
			//computePipelineConfig.DebugName = "Irradiance Pipline";
			//computePipelineConfig.Shader = Shader::Get("EnvironmentIrradiance");
			//
			//Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);
			//ComputePassConfiguration computePassConfig;
			//computePassConfig.DebugName = "EnvironmentIrradiance Pass";
			//computePassConfig.Pipeline = computePipeline;
			//
			//auto computePass = ComputePass::Create(computePassConfig);
			//computePass->SetInput("inputTexture", environmentMapImageCube);
			//computePass->SetInput("outputTexture", irradianceMap);
			//Renderer::SubmitCommand([&](CommandBuffer* buffer) {
			//
			//	Count<RenderCommandBuffer>renderCommandBuffer = RenderCommandBuffer::Create(buffer);
			//	Renderer::BeginComputePass(renderCommandBuffer, computePass);
			//	computePass->Dispatch(imageSize / 32, imageSize / 32, 6);
			//	Renderer::EndComputePass(computePass);
			//
			//});
			//irradianceMap.As<VulkanTextureCube>()->GenerateMips();

		}
		textureConfig.DebugName = "Prefilter map " + Utils::FileDialogs::GetFileName(path);
		Count<TextureCube> prefilterMap = Count<VulkanTextureCube>::Create(textureConfig);
	
		{
		
			ComputePipelineConfig computePipelineConfig;
			computePipelineConfig.DebugName = "Prefilter Pipline";
			computePipelineConfig.Shader = Shader::Get("EnvironmentPrefilter");
		
			Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);
			ComputePassConfiguration computePassConfig;
			computePassConfig.DebugName = "EnvironmentPrefilter Pass";
			computePassConfig.Pipeline = computePipeline;
		
			auto computePass = ComputePass::Create(computePassConfig);
			computePass->SetInput("u_EnvironmentMap", environmentMapImageCube);
			computePass->SetInput("u_PrefilterMap", prefilterMap);
			uint32_t maxMip = prefilterMap->GetMipLevelCount();
			Renderer::SubmitCommand([&](CommandBuffer* buffer) {
				Count<RenderCommandBuffer>renderCommandBuffer = RenderCommandBuffer::Create(buffer);
				Renderer::BeginRenderMaterialComputePass(renderCommandBuffer, computePass);
				const float deltaRoughness = 1.f / glm::max((float)(maxMip - 1), 1.0f);
		
				for (uint32_t mip = 0, mipsize = imageSize; mip < maxMip; mip++, mipsize /=2)
				{
					uint32_t numGroups = glm::max(mipsize / 32, 1u); // Ensure numGroups is at least 1
					float roughness = deltaRoughness * mip;
					roughness = glm::max(roughness, 0.05f);
					computePass->PushData("Input", &roughness);
					computePass->Dispatch(numGroups, numGroups, 6);
				}
				Renderer::EndComputePass(computePass);
			});
			prefilterMap.As<VulkanTextureCube>()->GenerateMips();
		}
		return std::make_pair(irradianceMap, prefilterMap);
	}
	Count<Texture2D> Renderer::GenerateBRDFLut()
	{
		const uint32_t imageSize = 1024;
		TextureConfiguration textureConfig;
		textureConfig.DebugName = "BrdfLut";
		textureConfig.Height = imageSize;
		textureConfig.Width = imageSize;
		textureConfig.Storage = true;
		textureConfig.Format = ImageFormat::RGBA16F;
		
		// widht* height * bytes per pixel
		uint8_t* data = new uint8_t[imageSize * imageSize * 8];
		//memset(data, 0xFF000000, imageSize * imageSize * 8);
		Count<Texture2D> brdfLut = Texture2D::Create(data,textureConfig);
		
		ComputePipelineConfig computePipelineConfig;
		computePipelineConfig.DebugName = "BRDFLUT Pipeline";
		computePipelineConfig.Shader = Shader::Get("BRDFLUT");
		
		Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);

		
		ComputePassConfiguration computePassConfig;
		computePassConfig.DebugName = "BRDFLUT Pass";
		computePassConfig.Pipeline = computePipeline;

		auto computePass = ComputePass::Create(computePassConfig);
		
		computePass->SetInput("brfdLUT", brdfLut);

		Renderer::SubmitCommand([&](CommandBuffer* buffer) {

			Count<RenderCommandBuffer>renderCommandBuffer = RenderCommandBuffer::Create(buffer);
			Renderer::BeginRenderMaterialComputePass(renderCommandBuffer, computePass);
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

	void Renderer::Init()
	{
		s_RenderCommandQueue.resize(2);
		s_RenderCommandQueue[0] = new CommandQueue();
		s_RenderCommandQueue[1] = new CommandQueue();
	}

	void Renderer::Unit()
	{
		delete s_RenderCommandQueue[0];
		delete s_RenderCommandQueue[1];
		s_RenderCommandQueue.clear();
	}

}