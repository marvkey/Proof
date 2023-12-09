#pragma once

#include "Proof/Core/Core.h""
namespace Proof 
{
	//mesh workshop does not half anythign so if you wnat to generate a 1 by 1 by 1 pass the half value
	class MeshWorkShop 
	{
	public:
		static Count<class Mesh> GenerateCube(const glm::vec3& size = glm::vec3{ 1 });
		static Count<class Mesh> GenerateSphere(float radius = 0.5f, float sectors = 36.0f, float stacks = 18.0f);
		static Count<class Mesh> GenerateCapsule(float radius = 0.5f, float height = 1.0f, uint32_t segments = 12, uint32_t subdivisionsHeight = 8);

		static Count<class Mesh> GenerateCone(uint32_t sectorCount = 36, uint32_t stackCount = 15, float radius = 0.5f, float height = 1.0f);
		static Count<class Mesh> GenerateCylinder(uint32_t sectorCount = 36, uint32_t stackCount = 1, float topradius = 0.5f, float baseRadius = 0.5f, float height = 1.0f);
		static Count<class Mesh> GenerateTorus(uint32_t numSegments = 20, uint32_t numRings = 10, float majorRadius = 0.5f, float minorRadius = 0.1f);
		static Count<class Mesh> GeneratePlane(uint32_t numSegments = 10, float size = 1.0f);
	};
}