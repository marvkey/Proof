#include "Proofprch.h"
#include "Renderer.h"

namespace Proof {
	void Renderer::BeginScene(){
	}
	void Renderer::EndScene(){
	}
	void Renderer::Submit(const std::shared_ptr<VertexArray> _VertexArray){
		RendererCommand::DrawIndexed(_VertexArray);
	}
}