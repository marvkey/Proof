#pragma once
//https://github.com/gasgiant/Ocean-URP/blob/main/Assets/OceanSystem/Runtime/ClipmapMeshBuilder.cs

#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
namespace Proof
{
	class Mesh;
	class ClipMeshBuilder
	{
	public:
		// ClipLevelHalfSize + 1 must be divisible by 4 for correct geomorphing
		static int ClipLevelHalfSize(int vertexDensity) { return (vertexDensity + 1) * 4 - 1; }

		static Count<Mesh> BuildClipMap(int vertexDensity, int clipMapLevels);
	private:
		static Count<Mesh> BuildRing(int clipLevelHalfSize);
		static Count<Mesh> BuildSkirt(int clipLevelHalfSize, float outerBorderScale);
		static Count<class Mesh> BuildPlane(int width, int height, glm::vec3 pivot, bool geomorphOffsetInUv,
			bool morphShiftX = false, bool morphShiftZ = false, int trianglesShift = 0);
	};
}