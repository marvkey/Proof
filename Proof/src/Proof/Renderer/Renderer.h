#pragma once
#include "RendererCommand.h"
#include "Shader.h"
#include "Proof/Core/Core.h"
namespace Proof {
	class Proof_API Renderer{
	public:
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

		static ShaderLibrary* AllShaders;
		static std::string s_RenderCompany;
		static std::string s_GraphicsCard;
		static std::string s_GraphicsCardVersion;
		friend class OpenGLGraphicsContext;
	};
}

