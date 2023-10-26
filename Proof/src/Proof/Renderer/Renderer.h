#pragma once
#include <glm/glm.hpp>
#include <functional>
#include "Proof/Math/Vector.h" 
#include "Proof/Events/WindowEvent.h"
#include <filesystem>
#include "Proof/Core/Buffer.h"
namespace Proof {
	struct  RendererConfig {
		uint32_t FramesFlight = 2;
		uint32_t MaxImageCount = 2;
	};

	struct CurrentFrame {
		uint32_t FrameinFlight;
		uint32_t ImageIndex;
	};
	
	class Renderer {
	public:
		enum class API {
			None =0, OpenGL =1, Vulkan=2
		};

		
		static void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount=1, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0);
		static void DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0);

		static void BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		static void EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		static void SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		static void SubmitCommand(std::function<void(class CommandBuffer*)> func);
		//static void SubmitDatafree(std::function<void()> func);

		// explicit clear means the framebuffer will overide its base command and clear all attachemtns when set true
		static void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear = false);
		static void BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass,bool explicitClear = false);
		static void EndRenderPass(Count<class RenderPass> renderPass);
		static void RenderPassPushRenderMaterial(Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial);

		static void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer,Count<class ComputePass> computPass);
		static void BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> commandBuffer, Count<class ComputePass> computPass);
		static void EndComputePass(Count<class ComputePass> computPass);
		static void ComputePassPushRenderMaterial(Count<class ComputePass> computePass, Count<class RenderMaterial> renderMaterial);

		static Count<class Texture2D> GenerateBRDFLut();
		static class CommandQueue& GetRenderCommandQueue();
		static CurrentFrame GetCurrentFrame();
		static CurrentFrame RT_GetCurrentFrame();
		static const RendererConfig GetConfig();
		static Count<class GraphicsContext> GetGraphicsContext();
		static Renderer::API GetAPI();

		static Count<class TextureCube> CreatePreethamSky(float turbidity, float azimuth, float inclination, uint32_t imageDimension = 1048);
		static Count<class Texture2D> GetWhiteTexture();
		static Count<class Texture2D> GetBlackTexture();
		static Count<class TextureCube> GetWhiteTextureCube();
		static Count<class TextureCube> GetBlackTextureCube();
		
		static Count<class Shader> GetShader(const std::string& name);
		static const std::unordered_map<std::string, std::string>& GetShaderDefines();
		static Count<class ShaderLibrary> GetShaderLibrary();

		static void SubmitFullScreenQuad(Count<RenderCommandBuffer> renderCOmmandBuffer, Count<RenderPass> pass, std::unordered_map<std::string, Buffer> pushBuffer = {});
		static void SubmitFullScreenQuad(Count<RenderCommandBuffer> renderCOmmandBuffer, Count<RenderPass> pass, Count<RenderMaterial> material);

		//environment and prefilter
		static std::pair<Count<class TextureCube>, Count<class TextureCube>>CreateEnvironmentMap(const std::filesystem::path& path);

		template<typename FuncT>
		static void Submit(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		template<typename FuncT>
		static void SubmitResourceFree(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				pFunc->~FuncT();
			};
			Submit([renderCmd, func]()
			{
				const uint32_t index = Renderer::RT_GetCurrentFrameIndex();
				auto storageBuffer = GetRenderResourceReleaseQueue(index).Allocate(renderCmd, sizeof(func));
				new (storageBuffer) FuncT(std::forward<FuncT>((FuncT&&)func));
			});
		}
	private:
		static void OnWindowResize(WindowResizeEvent& e);
		static void BeginFrame();
		static void EndFrame();
		static void Init(class Window* window);
		static void Shutdown();
		friend class RendererBase;
		friend class Application;
	};

	struct BaseTextures 
	{
		Count<class Texture2D> WhiteTexture;
		Count<class Texture2D> BlackTexture;
		Count<class Texture2D> NullTexture;
		Count<class TextureCube> WhiteTextureCube;
		Count<class TextureCube> BlackTextureCube;
		BaseTextures();
	};
}
