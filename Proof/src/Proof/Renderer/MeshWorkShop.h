#pragma once

#include "Proof/Core/Core.h""
namespace Proof {
	class MeshWorkShop {
	public:
		static Count<class Mesh> GenerateCube();
		static Count<class Mesh> GenerateUVSphere(float radius = 1.0f, float sectors = 36.f, float stacks = 16);
		//static Count<Mesh> GenerateIcoSphere(float radius=1.0f,float sectors=36.f,float stacks =18);
		static Count<class Mesh>GenerateCapsule(float radius = 0.5f, float height = 2.0f, uint32_t numSides = 32);
	private:
		friend class RendererBase;
	};
}