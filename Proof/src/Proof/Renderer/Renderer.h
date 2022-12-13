#pragma once
#include "RendererCommand.h"
#include "Shader.h"
#include "Proof/Core/Core.h"
namespace Proof {
	struct  RendererConfig {
		uint32_t FramesFlight = 2;
		uint32_t ImageSize;
	};

	struct CurrentFrame {
		uint32_t FrameinFlight;
		uint32_t ImageIndex;
	};
	class Proof_API Renderer{
	public:
		static void BeginFrame();

		static void EndFrame();

		static CurrentFrame GetCurrentFrame();
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static ShaderLibrary& GetShaderLibrary() {
			return *AllShaders;
		};

		static Count<class GraphicsContext> GetGraphicsContext() {
			return m_GraphicsContext;
		}
		static GraphicsContext* GetGraphicsContextPointer() {
			return m_GraphicsContext.get();
		}
		static void Init(Window* window);
		static void Destroy();

		static const RendererConfig GetConfig();

		static const std::string GetRenderCompany(){
			return s_RenderCompany;
		}
		static const std::string GetGraphicsCard() {
			return s_GraphicsCard;

		}
		static const std::string GetGraphicsCardVersion() {
			return s_GraphicsCardVersion;
		}
		
		static void OnWindowResize(WindowResizeEvent& e);
	private:
		static Count<class GraphicsContext>m_GraphicsContext;
		static ShaderLibrary* AllShaders;
		static std::string s_RenderCompany;
		static std::string s_GraphicsCard;
		static std::string s_GraphicsCardVersion;
		friend class OpenGLGraphicsContext;
		friend class Application;
	};
}

