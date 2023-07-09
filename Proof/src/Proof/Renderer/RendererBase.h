#pragma once
#include "Proof/Core/Core.h"
#include "GraphicsContext.h"
namespace Proof {
	
	struct BaseTextures;
	class Proof_API RendererBase{
	public:
		static class ShaderLibrary* GetShaderLibrary() {
			return AllShaders;
		};
		static Count<class GraphicsContext> GetGraphicsContext() {
			return m_GraphicsContext;
		}
		static GraphicsContext* GetGraphicsContextPointer() {
			return m_GraphicsContext.Get();
		}

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
		static void Init(Window* window);
		static void Destroy();
		static Count<class GraphicsContext>m_GraphicsContext;
		static class ShaderLibrary* AllShaders;
		static std::string s_RenderCompany;
		static std::string s_GraphicsCard;
		static std::string s_GraphicsCardVersion;
		static BaseTextures* s_BaseTextures;
		friend class Application;
		friend class Renderer;
	};
	struct BaseTextures {
		Count<class Texture2D> WhiteTexture;
		Count<class Texture2D> BlackTexture;
		Count<class Texture2D> NullTexture;
		BaseTextures();
	};
}

