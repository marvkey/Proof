#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Mesh.h"
namespace Proof
{
	//https://github.com/gasgiant/Ocean-URP/blob/main/Assets/OceanSystem/Runtime/ClipmapMeshBuilder.cs#L57
	class FFTClipMap
	{
	public:
		static inline int ClipLevelHalfSize(int vertexDensity) { return (vertexDensity + 1) * 4 - 1; };

		static Count<Mesh> BuildClipMapPlane(int vertexDensity, int clipMapLevels);
	private:
		static Count<Mesh> BuildRing(int clipLevelHalfSize);
		static Count<Mesh> BuildSkirt(int clipLevelHalfSize, float outerBorderScale);
		static Count<Mesh> BuildPlane(int width, int height, glm::vec3 pivot, bool geomorphOffsetInUv, bool morphShiftX = false, bool morphShiftZ = false, int trianglesShift = 0);
	};
}