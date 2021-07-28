#pragma once
#include "RendererCommand.h"
#include "Shader.h"
namespace Proof {
	class Proof_API Renderer{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static ShaderLibrary& GetShaderLibrary() {
			return *AllShaders;
		};
		static void Init();
		static void Reset();
		static void Draw();

		static const std::string GetRenderCompany(){
			return s_RenderCompany;
		}
		static const std::string GetGraphicsCard() {
			return s_GraphicsCard;

		}static const std::string GetGraphicsCardVersion() {
			return s_GraphicsCardVersion;
		}
	private:
		static ShaderLibrary* AllShaders;
		static std::string s_RenderCompany;
		static std::string s_GraphicsCard;
		static std::string s_GraphicsCardVersion;
		friend class OpenGLGraphicsContext;
	};
}

