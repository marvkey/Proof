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
#include "Proof/Platform/Vulkan/VulkanRenderer.h"
#include "GraphicsContext.h"
#include "Proof/Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
#include "Proof/Platform/Vulkan/VulkanTexutre.h"
#include "vulkan/vulkan.h"
#include "Proof/Core/Application.h"
#include "Proof/Platform/Window/WindowsWindow.h"
#include "Proof/Core/RenderThread.h"
#include "HosekDataRGB.h"
#include "Proof/Asset/AssetManager.h"
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

		Count<ComputePass> HosekWilkiePass;
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
	float RendererConfig::GetMaxMipCount()
	{
		return (float)Utils::GetMipLevelCount(EnvironmentMapResolution, EnvironmentMapResolution);
	}
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
		ShaderLibrary->LoadShader("HosekWilkieSky", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/IBL/HosekWilkieSky.glsl");

		// postprocess

		ShaderLibrary->LoadShader("WorldComposite", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/WorldComposite.glsl");
		ShaderLibrary->LoadShader("Wireframe", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/Wireframe.glsl");
		ShaderLibrary->LoadShader("Bloom", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/Bloom.glsl");
		ShaderLibrary->LoadShader("DOF", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/DOF.glsl");
		// ao
		ShaderLibrary->LoadShader("AO-Composite", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/AO-Composite.glsl");
		ShaderLibrary->LoadShader("SSAO", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/SSAO.glsl");
		ShaderLibrary->LoadShader("SSAOBlur", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/SSAOBlur.glsl");

		ShaderLibrary->LoadShader("Deinterleaving", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/HBAO_Deinterleave.glsl");
		ShaderLibrary->LoadShader("Reinterleaving", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/HBAO_Reinterleave.glsl");
		ShaderLibrary->LoadShader("HBAOBlur", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/HBAO_Blur.glsl");
		ShaderLibrary->LoadShader("HBAO", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PostProcess/AmbientOcclusion/HBAO.glsl");

		//2D
		ShaderLibrary->LoadShader("Base2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Base2D.glsl");
		ShaderLibrary->LoadShader("Text2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Text2D.glsl");
		ShaderLibrary->LoadShader("Line2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Line2D.glsl");
		ShaderLibrary->LoadShader("Circle2D", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/2D/Circle2D.glsl");

		s_Data->RenderCommandBuffer = RenderCommandBuffer::Create("RendererCommandBuffer");
		Renderer::BeginCommandBuffer(s_Data->RenderCommandBuffer);
		s_BaseTextures = pnew BaseTextures();
		Renderer::EndCommandBuffer(s_Data->RenderCommandBuffer);
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
			{
				ComputePipelineConfig config;
				config.DebugName = "Hosek Wilkie";
				config.Shader = Renderer::GetShader("HosekWilkieSky");
				auto hosekPipeline = ComputePipeline::Create(config);

				ComputePassConfiguration computeePassConfig;
				computeePassConfig.DebugName = "Hosek Wilkie";
				computeePassConfig.Pipeline = hosekPipeline;
				s_Data->HosekWilkiePass = ComputePass::Create(computeePassConfig);

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
		{
			PF_PROFILE_FUNC("Execute RenderCommandQueue");
			Timer workTimer;
			s_CommandQueue[GetRenderQueueIndex()]->Execute();
			// ExecuteRenderCommandQueue();

			// Rendering has completed, set state to idle
			renderThread->Set(RenderThread::State::Idle);
		}

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


	Count<TextureCube> Renderer::CreatePreethamSky(float turbidity, glm::vec3 sunDirection)
	{
		PF_PROFILE_FUNC();
		glm::vec3 zero{ 0 };
		if (sunDirection == zero || glm::all(glm::isnan(sunDirection)))
		{
			sunDirection = { 0.001,0.001,0.001 };
		}
		sunDirection = glm::normalize(sunDirection);

		const uint32_t cubemapSize = GetConfig().EnvironmentMapResolution;
		const uint32_t irradianceMap = 32;

		ImageFormat format = ImageFormat::RGBA16F;
		TextureConfiguration baseCubeMapConfig;
		baseCubeMapConfig.DebugName = "Pretham Cube";
		baseCubeMapConfig.Wrap = TextureWrap::Repeat;
		baseCubeMapConfig.Filter = TextureFilter::Nearest;
		baseCubeMapConfig.Height = cubemapSize;
		baseCubeMapConfig.Width = cubemapSize;
		baseCubeMapConfig.Storage = true;
		baseCubeMapConfig.GenerateMips = false;
		baseCubeMapConfig.Format = format;

		uint32_t mipLevels = Utils::GetMipLevelCount(cubemapSize, cubemapSize);
		Count<TextureCube> environmentMap = TextureCube::Create(baseCubeMapConfig);
		PrethamSkyPass->SetInput("o_CubeMap", environmentMap);
	
		glm::vec4 params = { sunDirection,turbidity};

		Count<RenderCommandBuffer> commandBuffer = s_Data->RenderCommandBuffer;
		Renderer::BeginComputePass(commandBuffer, PrethamSkyPass);
		PrethamSkyPass->PushData("u_Uniforms", &params);
		PrethamSkyPass->Dispatch(cubemapSize/ irradianceMap, cubemapSize/ irradianceMap, 6);
		Renderer::EndComputePass(PrethamSkyPass);
		return environmentMap;
	}


	float EvaluateSpline(const double* spline, size_t stride, float value)
	{
		//https://github.com/TKscoot/Ivy/blob/3b0a09d719e28c260c8eb5d7fbeb52be876e2af8/projects/Ivy/source/scene/renderpasses/skymodels/HosekWilkieSkyModel.cpp#L46

		return
			1 * pow(1 - value, 5) * spline[0 * stride] +
			5 * pow(1 - value, 4) * pow(value, 1) * spline[1 * stride] +
			10 * pow(1 - value, 3) * pow(value, 2) * spline[2 * stride] +
			10 * pow(1 - value, 2) * pow(value, 3) * spline[3 * stride] +
			5 * pow(1 - value, 1) * pow(value, 4) * spline[4 * stride] +
			1 * pow(value, 5) * spline[5 * stride];
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	float Evaluate(const double* dataset, size_t stride, float turbidity, float albedo, float sunTheta)
	{
		//https://github.com/TKscoot/Ivy/blob/3b0a09d719e28c260c8eb5d7fbeb52be876e2af8/projects/Ivy/source/scene/renderpasses/skymodels/HosekWilkieSkyModel.cpp#L46

		// splines are functions of elevation^1/3
		double elevationK = pow(std::max<float>(0.f, 1.f - sunTheta / (glm::pi<float>() / 2.f)), 1.f / 3.0f);

		// table has values for turbidity 1..10
		int turbidity0 = glm::clamp(static_cast<int>(turbidity), 1, 10);
		int turbidity1 = std::min(turbidity0 + 1, 10);
		double turbidityK = glm::clamp(turbidity - turbidity0, 0.f, 1.f);

		const double* datasetA0 = dataset;
		const double* datasetA1 = dataset + stride * 6 * 10;

		double a0t0 = EvaluateSpline(datasetA0 + stride * 6 * (turbidity0 - 1), stride, elevationK);
		double a1t0 = EvaluateSpline(datasetA1 + stride * 6 * (turbidity0 - 1), stride, elevationK);
		double a0t1 = EvaluateSpline(datasetA0 + stride * 6 * (turbidity1 - 1), stride, elevationK);
		double a1t1 = EvaluateSpline(datasetA1 + stride * 6 * (turbidity1 - 1), stride, elevationK);

		return a0t0 * (1 - albedo) * (1 - turbidityK) + a1t0 * albedo * (1 - turbidityK) + a0t1 * (1 - albedo) * turbidityK + a1t1 * albedo * turbidityK;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	glm::vec3 HosekWilkie(float cos_theta, float gamma, float cos_gamma, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 D, glm::vec3 E, glm::vec3 F, glm::vec3 G, glm::vec3 H, glm::vec3 I)
	{
		//https://github.com/TKscoot/Ivy/blob/3b0a09d719e28c260c8eb5d7fbeb52be876e2af8/projects/Ivy/source/scene/renderpasses/skymodels/HosekWilkieSkyModel.cpp#L46
		glm::vec3 chi = (1.f + cos_gamma * cos_gamma) / pow(1.f + H * H - 2.f * cos_gamma * H, glm::vec3(1.5f));
		return (1.f + A * exp(B / (cos_theta + 0.01f))) * (C + D * exp(E * gamma) + F * (cos_gamma * cos_gamma) + G * chi + I * (float)sqrt(std::max(0.f, cos_theta)));
	}
	Count<TextureCube> Renderer::CreateHosekWilkieSky(float turbidity, float groundReflectance, glm::vec3 sunDirection)
	{
		//https://github.com/TKscoot/Ivy/blob/3b0a09d719e28c260c8eb5d7fbeb52be876e2af8/projects/Ivy/source/scene/renderpasses/skymodels/HosekWilkieSkyModel.cpp#L46
		https://github.com/diharaw/sky-models/blob/master/src/hosek_wilkie_sky_model.cpp

		glm::vec3 zero{ 0 };
		if (sunDirection == zero || glm::all(glm::isnan(sunDirection)))
			sunDirection = { 0.001,0.001,0.001 };
		struct alignas(16) PushData
		{
			glm::vec4 SunDirection;
			glm::vec4 A, B, C, D, E, F, G, H, I,Z;

		}PushInfo;
		PushInfo.SunDirection = glm::vec4(glm::normalize(sunDirection),1);
		
		const float sunTheta = std::acos(glm::clamp(sunDirection.y, 0.f, 1.f));
		for (int i = 0; i < 3; ++i)
		{
			PushInfo.A[i] = Evaluate(datasetsRGB[i] + 0, 9, turbidity, groundReflectance, sunTheta);
			PushInfo.B[i] = Evaluate(datasetsRGB[i] + 1, 9, turbidity, groundReflectance, sunTheta);
			PushInfo.C[i] = Evaluate(datasetsRGB[i] + 2, 9, turbidity, groundReflectance, sunTheta);
			PushInfo.D[i] = Evaluate(datasetsRGB[i] + 3, 9, turbidity, groundReflectance, sunTheta);
			PushInfo.E[i] = Evaluate(datasetsRGB[i] + 4, 9, turbidity, groundReflectance, sunTheta);
			PushInfo.F[i] = Evaluate(datasetsRGB[i] + 5, 9, turbidity, groundReflectance, sunTheta);
			PushInfo.G[i] = Evaluate(datasetsRGB[i] + 6, 9, turbidity, groundReflectance, sunTheta);

			// Swapped in the dataset
			PushInfo.H[i] = Evaluate(datasetsRGB[i] + 8, 9, turbidity, groundReflectance, sunTheta);
			PushInfo.I[i] = Evaluate(datasetsRGB[i]+ 7, 9, turbidity, groundReflectance, sunTheta);

			PushInfo.Z[i] = Evaluate(datasetsRGBRad[i], 1, turbidity, groundReflectance, sunTheta);
		}

		const float normalizeSunY = 1.15f;

		if (normalizeSunY)
		{
			glm::vec3 S = HosekWilkie(std::cos(sunTheta), 0, 1.f, glm::vec3(PushInfo.A), glm::vec3(PushInfo.B), glm::vec3(PushInfo.C), 
				glm::vec3(PushInfo.D), glm::vec3(PushInfo.E),
				glm::vec3(PushInfo.F), glm::vec3(PushInfo.G), glm::vec3(PushInfo.H), glm::vec3(PushInfo.I)) * glm::vec3(PushInfo.Z);

			glm::vec3 z = PushInfo.Z;
			z /= glm::dot(S, glm::vec3(0.2126, 0.7152, 0.0722));
			z *= normalizeSunY;
			PushInfo.Z = glm::vec4(z,1);
		}
		PF_PROFILE_FUNC();
		const uint32_t cubemapSize = GetConfig().EnvironmentMapResolution;
		const uint32_t irradianceMap = 32;

		ImageFormat format = ImageFormat::RGBA16F;
		TextureConfiguration baseCubeMapConfig;
		baseCubeMapConfig.DebugName = "HosekWilkie";
		baseCubeMapConfig.Wrap = TextureWrap::Repeat;
		baseCubeMapConfig.Filter = TextureFilter::Nearest;
		baseCubeMapConfig.Height = cubemapSize;
		baseCubeMapConfig.Width = cubemapSize;
		baseCubeMapConfig.Storage = true;
		baseCubeMapConfig.GenerateMips = false;
		baseCubeMapConfig.Format = format;

		uint32_t mipLevels = Utils::GetMipLevelCount(cubemapSize, cubemapSize);
		Count<TextureCube> environmentMap = TextureCube::Create(baseCubeMapConfig);
		s_Data->HosekWilkiePass->SetInput("o_CubeMap", environmentMap);

		Count<RenderCommandBuffer> commandBuffer = s_Data->RenderCommandBuffer;
		Renderer::BeginComputePass(commandBuffer, s_Data->HosekWilkiePass);
		s_Data->HosekWilkiePass->PushData("u_Uniforms", &PushInfo);
		s_Data->HosekWilkiePass->Dispatch(cubemapSize / irradianceMap, cubemapSize / irradianceMap, 6);
		Renderer::EndComputePass(s_Data->HosekWilkiePass);

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
	std::pair<Count<class TextureCube>, Count<class TextureCube>> Renderer::CreateEnvironmentMap(const std::filesystem::path& path)
	{
		//Refrecnce
		//https://github.com/Nadrin/PBR/blob/master/src/vulkan.cpp

		Count<TextureCube> environmentMapImageCube;

		ImageFormat format = ImageFormat::RGBA16F;
		{
			const uint32_t imageSize = GetConfig().EnvironmentMapResolution;
			TextureConfiguration baseCubeMapConfig;
			baseCubeMapConfig.DebugName = FileSystem::GetFileName(path) + " Base CubeMap";
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
			irradianceTextureConfig.DebugName = "Irradiance map " + FileSystem::GetFileName(path);
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

				Count<RenderCommandBuffer>renderCommandBuffer = Renderer::GetRendererCommandBuffer() ;
				Renderer::BeginComputePass(renderCommandBuffer, computePass);
				computePass->Dispatch(irradianceFilterRate / 32, irradianceFilterRate / 32, 6);
				Renderer::EndComputePass(computePass);

			irradianceMap->GenerateMips();

		}
		const uint32_t prefilterFilterRate = GetConfig().EnvironmentMapResolution;

		Count<TextureCube> prefilterMap;
		{
			TextureConfiguration prefilterTextureConfig;
			prefilterTextureConfig.DebugName = "Prefilter map" + FileSystem::GetFileName(path);
			prefilterTextureConfig.Width = prefilterFilterRate;
			prefilterTextureConfig.Height = prefilterFilterRate;
			prefilterTextureConfig.Storage = true;
			prefilterTextureConfig.Format = format;
			prefilterTextureConfig.Wrap = TextureWrap::ClampEdge;
			prefilterTextureConfig.GenerateMips = true;

			prefilterMap = TextureCube::Create(prefilterTextureConfig, path);

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

			Count<RenderCommandBuffer>renderCommandBuffer = Renderer::GetRendererCommandBuffer();

			Renderer::BeginRenderMaterialComputePass(renderCommandBuffer, computePass);
			const float deltaRoughness = 1.f / glm::max((float)(maxMip - 1), 1.0f);

			for (uint32_t mip = 1, mipsize = prefilterFilterRate / 2; mip < maxMip; mip++, mipsize /= 2)
			{
				uint32_t numGroups = glm::max(mipsize / 32, 1u); // Ensure numGroups is at least 1
				float roughness = deltaRoughness * mip;
				roughness = glm::max(roughness, 0.05f);
				Renderer::Submit([prefilterMaterial,mip,roughness]
					{
						prefilterMaterial->Set("Input.Level", mip - 1);
						prefilterMaterial->Set("Input.Roughness", roughness);
					});

				Renderer::ComputePassPushRenderMaterial(computePass, prefilterMaterial);
				computePass->Dispatch(numGroups, numGroups, 6);
			}
			Renderer::EndComputePass(computePass);
			prefilterImageViews.clear();
		}
		return std::make_pair(irradianceMap, prefilterMap);
	}
	void Renderer::UpdateAllEnvironment()
	{
		for (auto e : Environment::s_Instances)
		{
			if (!e.IsValid())
				continue;
			auto environment = e.Lock();

			if (!environment->m_IsUpdated)
				continue;

			switch (environment->m_EnvironmentState)
			{
			case Proof::EnvironmentState::HosekWilkie:
			{
				auto hosekData = environment->m_HosekWilkieSky;
				auto texture = CreateHosekWilkieSky(hosekData.Turbidity, hosekData.GroundReflectance, hosekData.SunDirection);
				environment->m_PrefilterMap = texture;
				environment->m_IrradianceMap = texture;
			}
			break;
			case Proof::EnvironmentState::PreethamSky:
			{

				auto prethamSky = environment->m_PreethamSky;
				auto texture = CreatePreethamSky(prethamSky.Turbidity, prethamSky.SunDirection);
				environment->m_PrefilterMap = texture; environment->m_IrradianceMap = texture;
			}
			break;
			case Proof::EnvironmentState::EnvironmentTexture:
			{
				if (!AssetManager::HasAsset(environment->m_EnvironmentTexture.Image))
				{
					environment->m_EnvironmentTexture.Image = 0;
					continue;
				}
				auto path = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(AssetManager::GetAsset<Texture2D>(environment->m_EnvironmentTexture.Image)->GetPath()).Path);
				auto [irradiance, prefilter] = Renderer::CreateEnvironmentMap(path);
				environment->m_PrefilterMap = prefilter;
				environment->m_IrradianceMap = irradiance;
			}
			break;
			default:
				break;
			}
			environment->m_IsUpdated = false;
		}
	}
	Count<Texture2D> Renderer::GetBRDFLut()
	{
		return s_BaseTextures->BRDFLutTexture;
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
		//if (s_Data->CommandBufferRecording) 
		//{
		//	Renderer::EndCommandBuffer(s_Data->RenderCommandBuffer);
		//	Renderer::SubmitCommandBuffer(s_Data->RenderCommandBuffer);
		//	s_Data->CommandBufferRecording = false;
		//}
		Renderer::BeginCommandBuffer(s_Data->RenderCommandBuffer);
	}

	void Renderer::EndFrame()
	{
		UpdateAllEnvironment();
		Renderer::EndCommandBuffer(s_Data->RenderCommandBuffer);
		Renderer::SubmitCommandBuffer(s_Data->RenderCommandBuffer);
		s_RendererAPI->EndFrame();
	}

	BaseTextures::BaseTextures() {
		uint32_t whiteTexturedata = 0xffffffff;
		TextureConfiguration textureConfig("White Texture");
		textureConfig.Storage = true;
		WhiteTexture = Texture2D::Create(textureConfig, Buffer(&whiteTexturedata, sizeof(uint32_t)));


		uint32_t blackTexturedata = 0xFF000000;
		BlackTexture = Texture2D::Create(TextureConfiguration("Black Texture"), Buffer(&blackTexturedata, sizeof(uint32_t)));
		//brdflut
		{
			const uint32_t imageSize = 512;
			TextureConfiguration textureConfig;
			textureConfig.DebugName = "BrdfLut";
			textureConfig.Height = imageSize;
			textureConfig.Width = imageSize;
			textureConfig.Storage = true;
			textureConfig.Format = ImageFormat::RG16F;

			BRDFLutTexture = Texture2D::Create(textureConfig);

			ComputePipelineConfig computePipelineConfig;
			computePipelineConfig.DebugName = "BRDFLUT Pipeline";
			computePipelineConfig.Shader = Renderer::GetShader("BRDFLUT");

			Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);


			ComputePassConfiguration computePassConfig;
			computePassConfig.DebugName = "BRDFLUT Pass";
			computePassConfig.Pipeline = computePipeline;

			static auto computePass = ComputePass::Create(computePassConfig);

			computePass->SetInput("brfdLUT", BRDFLutTexture);


			Count<RenderCommandBuffer>renderCommandBuffer = s_Data->RenderCommandBuffer;
			Renderer::BeginComputePass(renderCommandBuffer, computePass);
			computePass->Dispatch(imageSize / 16, imageSize / 16, 1);
			Renderer::EndComputePass(computePass);
		}

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
		BlackTextureCube = TextureCube::Create(Buffer(&blackTexturedata, sizeof(uint32_t)).Data, cubeTextureConfig);
	}
	const RendererAPI* Renderer::GetRenderAPI()
	{
		return s_RendererAPI;
	}

}