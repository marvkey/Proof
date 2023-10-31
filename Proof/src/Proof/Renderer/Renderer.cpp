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
#include "Platform/Vulkan/VulkanRenderer.h"
#include "GraphicsContext.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanImage.h"
#include "Platform/Vulkan/VulkanTexutre.h"
#include "vulkan/vulkan.h"
#include "Proof/Core/Application.h"
#include "Platform/Window/WindowsWindow.h"
#include "Proof/Core/RenderThread.h"
namespace Proof {

	/*
	*Adding `mutable` to the lambda capture list is necessary when you want to modify variables captured by value within a lambda function. 
	In your code, you have a lambda function that captures the `instance` variable, among others. The `mutable` keyword allows you to modify these captured variables within the lambda, even if the lambda itself is declared as a `const` or if the lambda is called on a `const` instance.

	In your case, by adding `mutable`, you've indicated that the lambda should be allowed to modify the `instance` variable, 
	and this change has resolved the crash. Without `mutable`, the lambda captures variables by value, 
	which means they are treated as const within the lambda unless explicitly marked as `mutable`. 
	If you attempt to modify a captured variable without `mutable`, it would result in a compilation error or unexpected behavior, potentially leading to a crash.

	Here's the modified lambda capture list:

	```cpp
	[instance, groupCountX, groupCountY, groupCountZ]() mutable {
		// Lambda code...
	}
	```

	The `mutable` keyword in this context ensures that the lambda can modify the captured variables as needed,
	which appears to have resolved the issue in your case.
	*/
	constexpr static uint32_t s_RenderCommandQueueCount = 2;
	static CommandQueue* s_CommandQueue[s_RenderCommandQueueCount];
	static std::atomic<uint32_t> s_RenderCommandQueueSubmissionIndex = 0;
	static CommandQueue* s_ResourceFreeQueue;

	static class RendererAPI* s_RendererAPI;

	static Count<class ShaderLibrary> ShaderLibrary;
	static Count<ComputePass> PrethamSkyPass;
	static BaseTextures* s_BaseTextures;

	static std::unordered_map<std::string, std::string> s_ShaderDefines;

	struct RendererData
	{
		Count<VertexBuffer> QuadVertexBuffer;
		Count<IndexBuffer> QuadIndexBuffer;
		Count<RenderCommandBuffer> RenderCommandBuffer;
		bool CommandBufferRecording = false;
	};
	static RendererAPI* InitRendererAPI()
	{
		switch (RendererAPI::GetAPI())
		{
			//case RendererAPIType::OpenGL: return anew OpenGLRenderer();
			case Renderer::API::Vulkan: return pnew VulkanRenderer();
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	RendererData* s_Data = nullptr;
	void Renderer::Init()
	{
		Timer time;

		s_RendererAPI = InitRendererAPI();

		s_CommandQueue[0] = pnew CommandQueue();
		s_CommandQueue[1] = pnew CommandQueue();

		s_Data = pnew RendererData();
		s_ResourceFreeQueue = pnew CommandQueue[GetConfig().FramesFlight];
		s_RendererAPI->Init();

		ShaderLibrary = Count<class ShaderLibrary>::Create();

		//PBR
		ShaderLibrary->LoadShader("ProofPBR_Static", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/ProofPBR_Static.glsl");

		// predepth
		ShaderLibrary->LoadShader("PreDepth_Static", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PreDepth/PreDepth_Static.glsl");

		// lightculling
		ShaderLibrary->LoadShader("FrustrumGrid", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/LightCulling/FrustrumGrid.glsl");
		ShaderLibrary->LoadShader("LightCulling", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/LightCulling/LightCulling.glsl");

		//Shadows
		ShaderLibrary->LoadShader("DebugShadowMap", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Shadow/DebugShadowMap.glsl");
		ShaderLibrary->LoadShader("ShadowDepthPass", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/Shadow/ShadowDepthPass.glsl");
		
		//IBL
		ShaderLibrary->LoadShader("BRDFLUT", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/BRDFLut.glsl");
		ShaderLibrary->LoadShader("EquirectangularToCubemap", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EquirectangularToCubemap.glsl");
		ShaderLibrary->LoadShader("SkyBox", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/SkyBox.glsl");
		ShaderLibrary->LoadShader("EnvironmentIrradiance", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentIrradiance.glsl");
		ShaderLibrary->LoadShader("EnvironmentIrradianceNonCompute", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentIrradianceNonCompute.glsl");
		ShaderLibrary->LoadShader("EnvironmentPrefilter", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/EnvironmentPrefilter.glsl");
		ShaderLibrary->LoadShader("PreethamSky", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/PreethamSky.glsl");

		// postprocess

		ShaderLibrary->LoadShader("WorldComposite", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/WorldComposite.glsl");
		ShaderLibrary->LoadShader("Wireframe", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/Wireframe.glsl");
		// ao
		ShaderLibrary->LoadShader("AO-Composite", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/AO-Composite.glsl");
		ShaderLibrary->LoadShader("SSAO", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/SSAO.glsl");
		ShaderLibrary->LoadShader("SSAOBlur", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/SSAOBlur.glsl");


		//2D
		ShaderLibrary->LoadShader("Base2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Base2D.glsl");
		ShaderLibrary->LoadShader("Text2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Text2D.glsl");
		ShaderLibrary->LoadShader("Line2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Line2D.glsl");

		s_Data->RenderCommandBuffer = RenderCommandBuffer::Create("RendererCommandBuffer");
		//Renderer::BeginCommandBuffer(s_Data->RenderCommandBuffer);
		//Renderer::EndCommandBuffer(s_Data->RenderCommandBuffer);
		// Compile shaders
		Application::Get()->m_RenderThread.Pump();
		{

			/**
			* 
				In the code snippet you provided, the width and height are both set to 2 for the following reason:

				A full-screen quad in normalized device coordinates (NDC) ranges from -1 to 1 along both the x and y axes. By setting x to -1 and y to -1, and width and height both to 2, you are effectively defining a quad that spans the entire screen.

				Here's a breakdown of the logic:

				x and y are set to -1, which corresponds to the bottom left corner of the screen in NDC.
				width and height are set to 2. Since NDC ranges from -1 to 1 along both axes, a width and height of 2 will result in a quad that extends from -1 to 1 in both directions. This covers the entire screen.
				The four vertices of the quad are defined based on the x, y, width, and height values, along with the texture coordinates. The texture coordinates are set to (0, 0) for the bottom left, (1, 0) for the bottom right, (1, 1) for the top right, and (0, 1) for the top left. 
				This maps the quad to the full extent of a texture, making it suitable for rendering fullscreen effects.
				So, the choice of width and height as 2 is to ensure that the quad spans the entire screen in NDC, allowing you to render fullscreen effects or post-processing operations.

				NDC stands for "Normalized Device Coordinates." It is a coordinate system used in computer graphics to represent positions and vertices in a normalized and device-independent manner. 
				NDC coordinates are a crucial concept in graphics programming, as they provide a standardized way to describe positions on the screen or in a rendering pipeline.
				In NDC, the visible portion of the screen or rendering area is defined by a square or rectangle with corners at (-1, -1) and (1, 1). This square represents the "viewport" and serves as a reference for mapping positions from the 3D world space to the 2D screen space. 
				The range of NDC coordinates along both the x and y axes is from -1 to 1.
				The process of transforming vertices from 3D world coordinates to NDC involves several stages, including model transformation (local to world space), view transformation (world to camera space), and projection transformation (camera to NDC space). 
				The resulting NDC coordinates are then further processed in the rendering pipeline to eventually generate the 2D image that is displayed on the screen.
				Using NDC coordinates allows graphics programmers to write code that is independent of the specific screen dimensions or resolution of the display device, making it easier to create graphics that work consistently across various platforms and screen sizes.
			*/
			// Create fullscreen quad
			float x = -1;
			float y = -1;
			float width = 2, height = 2;

			// set up basic piplines		
			{
				ComputePipelineConfig config;
				config.DebugName = "Pretham Pipeline";
				config.Shader = Renderer::GetShader("PreethamSky");
				auto prethamPipeline = ComputePipeline::Create(config);

				ComputePassConfiguration computeePassConfig;
				computeePassConfig.DebugName = "Pretham SKy pass";
				computeePassConfig.Pipeline = prethamPipeline;
				PrethamSkyPass = ComputePass::Create(computeePassConfig);
			}

			QuadVertex data[4];

			data[0].Position = glm::vec3(x, y, 0.0f);
			data[0].TexCoord = glm::vec2(0, 0);

			data[1].Position = glm::vec3(x + width, y, 0.0f);
			data[1].TexCoord = glm::vec2(1, 0);

			data[2].Position = glm::vec3(x + width, y + height, 0.0f);
			data[2].TexCoord = glm::vec2(1, 1);

			data[3].Position = glm::vec3(x, y + height, 0.0f);
			data[3].TexCoord = glm::vec2(0, 1);

			s_Data->QuadVertexBuffer = VertexBuffer::Create(data, 4 * sizeof(QuadVertex));
			uint32_t indices[6] = { 0, 1, 2, 2, 3, 0, };
			s_Data->QuadIndexBuffer = IndexBuffer::Create(indices, 6 * sizeof(uint32_t));
		}
		Renderer::BeginCommandBuffer(s_Data->RenderCommandBuffer);
		s_Data->CommandBufferRecording = true;
		s_BaseTextures = pnew BaseTextures();

		PF_ENGINE_INFO("Renderer Initialized {}m/s",time.ElapsedMillis());
	}

	void Renderer::Shutdown()
	{
		Timer time;

		// shut down piplines
		{
			PrethamSkyPass = nullptr;
		}

		pdelete s_CommandQueue[0];
		pdelete s_CommandQueue[1];

		pdelete s_BaseTextures;
		ShaderLibrary = nullptr;
		pdelete s_Data;
		s_RendererAPI->ShutDown();
		pdelete s_RendererAPI;

		for (uint32_t i = 0; i < GetConfig().FramesFlight; i++)
		{
			auto& queue = Renderer::GetRenderResourceReleaseQueue(i);
			queue.Execute();
		}

		PF_ENGINE_INFO("Renderer Shutdown {}m/s", time.ElapsedMillis());
	}
	void Renderer::RenderThreadFunc(RenderThread* renderThread)
	{
		PF_PROFILE_THREAD("Render Thread");

		while (renderThread->IsRunning())
		{
			WaitAndRender(renderThread);
		}
	}
	void Renderer::WaitAndRender(RenderThread* renderThread)
	{
		PF_PROFILE_FUNC();
		//auto& performanceTimers = Application::Get().m_PerformanceTimers;

		// Wait for kick, then set render thread to busy
		{
			PF_PROFILE_FUNC("Wait");
			Timer waitTimer;
			renderThread->WaitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);
			//performanceTimers.RenderThreadWaitTime = waitTimer.ElapsedMillis();
		}

		Timer workTimer;
		s_CommandQueue[GetRenderQueueIndex()]->Execute();
		// ExecuteRenderCommandQueue();

		// Rendering has completed, set state to idle
		renderThread->Set(RenderThread::State::Idle);

		//performanceTimers.RenderThreadWorkTime = workTimer.ElapsedMillis();
	}
	void Renderer::SwapQueues()
	{
		s_RenderCommandQueueSubmissionIndex = (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}
	uint32_t Renderer::GetRenderQueueIndex()
	{
		return (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}

	uint32_t Renderer::GetRenderQueueSubmissionIndex()
	{
		return s_RenderCommandQueueSubmissionIndex;
	}
	const std::unordered_map<std::string, std::string>& Renderer::GetShaderDefines()
	{
		return s_ShaderDefines;
	}

	Count<class ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return ShaderLibrary;
	}

	void Renderer::SubmitFullScreenQuad(Count<RenderCommandBuffer> renderCOmmandBuffer, Count<RenderPass> pass, std::unordered_map<std::string, Buffer> pushBuffer)
	{
		/**
		* 
			Think of a full-screen quad like a big piece of paper that covers your entire computer screen. When you're done drawing the main picture (your game scene) on this paper, you might want to add some extra touches to make it look cooler, like coloring some parts or making things blurry.
			But instead of coloring or blurring each tiny part separately, you can do it all at once by shading the whole paper. This is faster because your computer can work on many things at the same time. This paper is your full-screen quad.
			This technique also helps make sure that everything looks the same all over the screen. It's like applying makeup evenly to your face. Plus, you can use this big paper to put other things on top, like stickers (UI elements) or notes.
			So, a full-screen quad is like a big canvas where you apply finishing touches to your game scene quickly, evenly, and with the ability to add extra stuff on top.
		*/

		/**
		*
		* 
			A full-screen quad is a simple and effective technique used in computer graphics to apply post-processing effects and perform certain rendering tasks that involve the entire screen. 
			It might seem counterintuitive to render a quad that covers the entire screen, but it serves some important purposes in modern graphics pipelines:
			Post-Processing Effects:
			After the main scene is rendered, you might want to apply various post-processing effects like bloom, depth of field, motion blur, color correction, and more. These effects typically involve applying shaders that sample from the scene's rendered textures (color and depth buffers) and generate the final output. 
			By rendering a full-screen quad, you provide a canvas for these shaders to process the entire scene.

			Efficiency:
			Modern GPUs are optimized for parallel processing. By rendering a full-screen quad, you can leverage the GPU's parallelism to process pixels concurrently, which is more efficient than applying effects individually to each pixel. 
			This is especially important for effects that require multiple samples or complex calculations.

			Consistency:
			Applying post-processing effects uniformly across the entire screen helps maintain a consistent look throughout the scene. If you applied effects to individual objects or regions, the visual coherence might be compromised.

			Custom Rendering Operations:
			Beyond post-processing, full-screen quads can be used for other operations. For instance, you might want to render a texture onto the screen, display debug information, or execute custom shader-based operations across the entire screen.

			Integration with UI:
			As you mentioned earlier, full-screen quads are often used to integrate UI elements like menus, health bars, crosshairs, and more. By rendering these UI elements on top of the full-screen quad, they can easily overlay the rendered scene.

			In summary, a full-screen quad is a versatile and efficient tool in modern graphics programming. It provides a canvas for applying post-processing effects, rendering UI elements, and performing other operations that involve the entire screen. 
			By leveraging the parallel processing power of GPUs, this technique contributes to the overall visual quality and performance of real-time graphics applications.
		*/
		PF_PROFILE_FUNC();
		auto pipeline = pass->GetPipeline();

		s_Data->QuadVertexBuffer->Bind(renderCOmmandBuffer);
		s_Data->QuadIndexBuffer->Bind(renderCOmmandBuffer);

		for (auto& [name, buffer] : pushBuffer)
		{
			if(buffer)
				pass->PushData(name, buffer.Get());
		}
		Renderer::DrawElementIndexed(renderCOmmandBuffer, s_Data->QuadIndexBuffer->GetSize() /sizeof(uint32_t));
	}

	void Renderer::SubmitFullScreenQuad(Count<RenderCommandBuffer> renderCOmmandBuffer, Count<RenderPass> pass, Count<RenderMaterial> material)
	{
		PF_PROFILE_FUNC();
		auto pipeline = pass->GetPipeline();

		s_Data->QuadVertexBuffer->Bind(renderCOmmandBuffer);
		s_Data->QuadIndexBuffer->Bind(renderCOmmandBuffer);

		Renderer::RenderPassPushRenderMaterial(pass, material);
		Renderer::DrawElementIndexed(renderCOmmandBuffer, s_Data->QuadIndexBuffer->GetSize() / sizeof(uint32_t));
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

	uint32_t Renderer::GetCurrentFrameInFlight()
	{
		return Application::Get()->m_CurrentFrameIndex;
	}

	uint32_t Renderer::RT_GetCurrentFrameInFlight()
	{
		return Application::Get()->GetWindow()->GetSwapChain()->GetFrameIndex();
	}

	const RendererConfig Renderer::GetConfig()
	{
		return Application::Get()->GetWindow()->GetSwapChain()->GetRenderConfig();
	}

	Count<class GraphicsContext> Renderer::GetGraphicsContext()
	{
		return Application::Get()->GetGraphicsContext();
	}

	Renderer::API Renderer::GetAPI()
	{ return RendererAPI::GetAPI(); }


	Count<TextureCube> Renderer::CreatePreethamSky(float turbidity, float azimuth, float inclination, uint32_t imageDimensionadfa)
	{
		PF_PROFILE_FUNC();
		const uint32_t cubemapSize = 512;
		const uint32_t irradianceMap = 32;

		ImageFormat format = ImageFormat::RGBA32F;
		TextureConfiguration baseCubeMapConfig;
		baseCubeMapConfig.DebugName = "Pretham Cube";
		baseCubeMapConfig.Wrap = TextureWrap::Repeat;
		baseCubeMapConfig.Filter = TextureFilter::Nearest;
		baseCubeMapConfig.Height = cubemapSize;
		baseCubeMapConfig.Width = cubemapSize;
		baseCubeMapConfig.Storage = true;
		baseCubeMapConfig.GenerateMips = true;
		baseCubeMapConfig.Format = format;

		uint32_t mipLevels = Utils::GetMipLevelCount(cubemapSize, cubemapSize);
		Count<TextureCube> environmentMap = TextureCube::Create(baseCubeMapConfig);
		PrethamSkyPass->SetInput("o_CubeMap", environmentMap);
	
		glm::vec3 params = { turbidity, azimuth, inclination };
		Count<RenderCommandBuffer> commandBuffer = s_Data->RenderCommandBuffer;
		Renderer::BeginComputePass(commandBuffer, PrethamSkyPass);
		PrethamSkyPass->PushData("u_Uniforms", &params);
		PrethamSkyPass->Dispatch(cubemapSize/32, cubemapSize/32, 6);
		Renderer::EndComputePass(PrethamSkyPass);
		environmentMap->GenerateMips();

		//return;
		// boit 
		#if 0
		auto blitCmd = cmdBufer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
		bool readonly = false;
		auto image=environmentMap.As<VulkanTextureCube>()->GetImage().As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image;
		{
			for (uint32_t face = 0; face < 6; face++)
			{
				VkImageSubresourceRange mipSubRange = {};
				mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				mipSubRange.baseMipLevel = 0;
				mipSubRange.baseArrayLayer = face;
				mipSubRange.levelCount = 1;
				mipSubRange.layerCount = 1;

				// Prepare current mip level as image blit destination
				Utils::InsertImageMemoryBarrier(blitCmd, image,
					0, VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
					mipSubRange);
			}

			for (uint32_t i = 1; i < mipLevels; i++)
			{
				for (uint32_t face = 0; face < 6; face++)
				{
					VkImageBlit imageBlit{};

					// Source
					imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					imageBlit.srcSubresource.layerCount = 1;
					imageBlit.srcSubresource.mipLevel = i - 1;
					imageBlit.srcSubresource.baseArrayLayer = face;
					imageBlit.srcOffsets[1].x = int32_t(cubemapSize >> (i - 1));
					imageBlit.srcOffsets[1].y = int32_t(cubemapSize >> (i - 1));
					imageBlit.srcOffsets[1].z = 1;

					// Destination
					imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					imageBlit.dstSubresource.layerCount = 1;
					imageBlit.dstSubresource.mipLevel = i;
					imageBlit.dstSubresource.baseArrayLayer = face;
					imageBlit.dstOffsets[1].x = int32_t(cubemapSize >> i);
					imageBlit.dstOffsets[1].y = int32_t(cubemapSize >> i);
					imageBlit.dstOffsets[1].z = 1;

					VkImageSubresourceRange mipSubRange = {};
					mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					mipSubRange.baseMipLevel = i;
					mipSubRange.baseArrayLayer = face;
					mipSubRange.levelCount = 1;
					mipSubRange.layerCount = 1;

					// Prepare current mip level as image blit destination
					Utils::InsertImageMemoryBarrier(blitCmd, image,
						0, VK_ACCESS_TRANSFER_WRITE_BIT,
						VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						mipSubRange);

					// Blit from previous level
					vkCmdBlitImage(
						blitCmd,
						image,
						VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						image,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1,
						&imageBlit,
						VK_FILTER_LINEAR);

					// Prepare current mip level as image blit source for next level
					Utils::InsertImageMemoryBarrier(blitCmd, image,
						VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						mipSubRange);
				}
			}

			// After the loop, all mip layers are in TRANSFER_SRC layout, so transition all to SHADER_READ
			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.layerCount = 6;
			subresourceRange.levelCount = mipLevels;

			Utils::InsertImageMemoryBarrier(blitCmd, image ,
				VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, readonly ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_GENERAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				subresourceRange);
		}
		#endif

		

		return environmentMap;
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
	Count<class RenderCommandBuffer> Renderer::GetRendererCommandBuffer()
	{
		return s_Data->RenderCommandBuffer;
	}
	std::pair<Count<class TextureCube>, Count<class TextureCube>> Renderer::CreateEnvironmentMap(Count<Texture2D> texture)
	{
		//Refrecnce
		//https://github.com/Nadrin/PBR/blob/master/src/vulkan.cpp

		Count<TextureCube> environmentMapImageCube;

		ImageFormat format = ImageFormat::RGBA16F;
		{
			const uint32_t imageSize = 1024;
			TextureConfiguration baseCubeMapConfig;
			baseCubeMapConfig.DebugName = FileSystem::GetFileName(texture->GetPath()) + " Base CubeMap";
			baseCubeMapConfig.Height = imageSize;
			baseCubeMapConfig.Width = imageSize;
			baseCubeMapConfig.Storage = true;
			baseCubeMapConfig.GenerateMips = true;
			baseCubeMapConfig.Format = format;
			baseCubeMapConfig.Wrap = TextureWrap::ClampEdge;

			environmentMapImageCube = TextureCube::Create(baseCubeMapConfig, texture);
		}
		const uint32_t irradianceFilterRate = 32;
		Count<TextureCube> irradianceMap; 
		{
			TextureConfiguration irradianceTextureConfig;
			irradianceTextureConfig.DebugName = "Irradiance map " + FileSystem::GetFileName(texture->GetPath());
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
			
			Count<RenderCommandBuffer> commandBuffer = s_Data->RenderCommandBuffer;
			Renderer::BeginComputePass(commandBuffer, computePass);
			computePass->Dispatch(irradianceFilterRate / 32, irradianceFilterRate / 32, 6);
			Renderer::EndComputePass(computePass);
			
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
			prefilterTextureConfig.DebugName = "Prefilter map" + FileSystem::GetFileName(texture->GetPath());
			prefilterTextureConfig.Width = prefilterFilterRate;
			prefilterTextureConfig.Height = prefilterFilterRate;
			prefilterTextureConfig.Storage = true;
			prefilterTextureConfig.Format = format;
			prefilterTextureConfig.Wrap = TextureWrap::ClampEdge;
			prefilterTextureConfig.GenerateMips = true;

			prefilterMap = TextureCube::Create(prefilterTextureConfig, texture->GetPath());

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

			Count<RenderCommandBuffer>renderCommandBuffer = s_Data->RenderCommandBuffer;

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
			prefilterImageViews.clear();
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
		
		static Count<Texture2D> brdfLut = Texture2D::Create(textureConfig);
		
		ComputePipelineConfig computePipelineConfig;
		computePipelineConfig.DebugName = "BRDFLUT Pipeline";
		computePipelineConfig.Shader = GetShader("BRDFLUT");
		
		static Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);

		
		ComputePassConfiguration computePassConfig;
		computePassConfig.DebugName = "BRDFLUT Pass";
		computePassConfig.Pipeline = computePipeline;

		static auto computePass = ComputePass::Create(computePassConfig);
		
		computePass->SetInput("brfdLUT", brdfLut);


		Count<RenderCommandBuffer>renderCommandBuffer = s_Data->RenderCommandBuffer;
		Renderer::BeginComputePass(renderCommandBuffer, computePass);
		computePass->Dispatch(imageSize/16 , imageSize/16 , 1);
		Renderer::EndComputePass(computePass);
		return brdfLut;
	}

	CommandQueue& Renderer::GetRenderCommandQueue()
	{
		return *s_CommandQueue[s_RenderCommandQueueSubmissionIndex];
	}

	CommandQueue& Renderer::GetRenderResourceReleaseQueue(uint32_t index)
	{
		return s_ResourceFreeQueue[index];
	}

	void Renderer::BeginFrame()
	{

		s_RendererAPI->BeginFrame();
		if (s_Data->CommandBufferRecording) 
		{
			Renderer::EndCommandBuffer(s_Data->RenderCommandBuffer);
			Renderer::SubmitCommandBuffer(s_Data->RenderCommandBuffer);
			s_Data->CommandBufferRecording = false;
		}
		Renderer::BeginCommandBuffer(s_Data->RenderCommandBuffer);
	}

	void Renderer::EndFrame()
	{
		Renderer::EndCommandBuffer(s_Data->RenderCommandBuffer);
		Renderer::SubmitCommandBuffer(s_Data->RenderCommandBuffer);
		s_RendererAPI->EndFrame();
	}

	BaseTextures::BaseTextures() {
		uint32_t whiteTexturedata = 0xffffffff;
		WhiteTexture = Texture2D::Create(TextureConfiguration("White Texture"), Buffer(&whiteTexturedata, sizeof(uint32_t)));


		uint32_t blackTexturedata = 0xFF000000;
		BlackTexture = Texture2D::Create(TextureConfiguration("Black Texture"), Buffer(&blackTexturedata, sizeof(uint32_t)));


		TextureConfiguration cubeTextureConfig;
		cubeTextureConfig.DebugName = "White Texture";
		cubeTextureConfig.GenerateMips = true;
		cubeTextureConfig.Height = 1;
		cubeTextureConfig.Width = 1;
		cubeTextureConfig.Storage = true;
		cubeTextureConfig.Format = ImageFormat::RGBA;
		cubeTextureConfig.Wrap = TextureWrap::ClampEdge;

		WhiteTextureCube = TextureCube::Create(Buffer(&whiteTexturedata, sizeof(uint32_t)).Data, cubeTextureConfig);

		cubeTextureConfig.DebugName = "Black Texture";
		BlackTextureCube = TextureCube::Create(cubeTextureConfig, WhiteTexture);
	}
	const RendererAPI* Renderer::GetRenderAPI()
	{
		return s_RendererAPI;
	}

}