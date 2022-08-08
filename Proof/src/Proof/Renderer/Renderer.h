#pragma once
#include "RendererCommand.h"
#include "Shader.h"
#include "Proof/Core/Core.h"
namespace Proof {
	struct  RendererConfig {
		uint32_t FramesFlight = 2;
	};
	class Proof_API Renderer{
	public:
		static void BeginFrame() {

		}

		static void EndFrame() {
			s_CurrentFrame = (s_CurrentFrame + 1) % GetConfig().FramesFlight;
		}

		static uint32_t GetCurrentFrame() {
			return s_CurrentFrame;
		}
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

		static const RendererConfig GetConfig() {
			return RendererConfig();
		}
	//	static void Reset();
		//static void Draw();

		static const std::string GetRenderCompany(){
			return s_RenderCompany;
		}
		static const std::string GetGraphicsCard() {
			return s_GraphicsCard;

		}
		static const std::string GetGraphicsCardVersion() {
			return s_GraphicsCardVersion;
		}
		
	private:
		static Count<class GraphicsContext>m_GraphicsContext;
		static uint32_t s_CurrentFrame;
		static ShaderLibrary* AllShaders;
		static std::string s_RenderCompany;
		static std::string s_GraphicsCard;
		static std::string s_GraphicsCardVersion;
		friend class OpenGLGraphicsContext;
	};
}

