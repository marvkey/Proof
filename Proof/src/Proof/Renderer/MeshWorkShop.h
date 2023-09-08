#pragma once

#include "Proof/Core/Core.h""
namespace Proof {
	class MeshWorkShop {
	public:
		static Count<class Mesh> GenerateCube();
		static Count<class Mesh> GenerateSphere(float radius = 1.0f, float sectors = 36.f, float stacks = 18);
		static Count<class Mesh> GenerateCapsule(float radius =0.5, float height=2.0f,uint32_t segments = 12, uint32_t subdivisionsHeight =8);

		static Count<class Mesh> GenerateCone(uint32_t sectorCount = 36, uint32_t stackCount = 15,float radius = 1.0f, float height = 2.0f);
		// stack count affect basically hwo smooth it will be if it chooses to be a smooth mesh
		static Count<class Mesh> GenerateCylinder(uint32_t sectorCount = 36, uint32_t stackCount = 1,float topradius = 1.0f, float baseRadius =1.0f,float height = 2.0f);
		static Count<class Mesh> GenerateTorus(uint32_t numSegments = 20, uint32_t numRings = 10, float majorRadius = 1.0f, float minorRadius = 0.2f);
		static Count<class Mesh> GeneratePlane(uint32_t numSegments = 10, float size =2.0f);
	};
}