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
	private:
		static ShaderLibrary* AllShaders;
	};
}

