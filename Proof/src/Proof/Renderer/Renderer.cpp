#include "Proofprch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Proof {
	ShaderLibrary* Renderer::AllShaders = new ShaderLibrary;
	void Renderer::Init() {
		Renderer2D::Init();
		Renderer3D::Init();
		Renderer2D::Reset();
		Renderer3D::Reset();
	}
	void Renderer::Reset() {
		Renderer2D::Reset();
		Renderer3D::Reset();
	}
	void Renderer::Draw() {
		Renderer2D::EndContext();
		Renderer3D::EndContext();
	}
}