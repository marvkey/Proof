#include "Proofprch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "3DRenderer/Renderer3D.h"
#include "3DRenderer/Renderer3DPBR.h"
#include "Proof/Scene/World.h"
#include "Proof/Renderer/MeshWorkShop.h"
namespace Proof {

	ShaderLibrary* Renderer::AllShaders = new ShaderLibrary;
	std::string Renderer::s_RenderCompany;
	std::string Renderer::s_GraphicsCard;
	std::string Renderer::s_GraphicsCardVersion;
	void Renderer::Init() {
		Renderer3DCore::Init();
		Renderer3DPBR::Init();
		//Renderer3DPBR::Reset();
		Renderer2D::Init();
		Renderer2D::Reset();
		MeshWorkShop::Init();
	}
	void Renderer::Reset() {
		Renderer2D::Reset();
		//Renderer3DPBR::Reset();

	}
	void Renderer::Draw() {
		Renderer2D::EndContext();
		//Renderer3DPBR::EndContext();
	}
}