#pragma once
#include "RendererCommand.h"
namespace Proof {
	class Proof_API Renderer{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static void BeginScene(); 
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray> _VertexArray);
	};
}

