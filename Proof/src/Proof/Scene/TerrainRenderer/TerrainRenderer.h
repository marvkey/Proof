#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{
	class TerrainRenderer : RefCounted
	{
	public:
		TerrainRenderer();
		Count<class Mesh> GetTerrainMesh() { return m_TerrainMesh; };

		Count<class Texture2D> GetNoiseTexture() { return m_NoiseTexture; };

	private:
		void GenerateMesh(const std::vector<float>& heightMap, uint32_t size);
	private:
		Count<class Mesh> m_TerrainMesh;
		Count<class Texture2D> m_NoiseTexture;

		friend class World;
	};
}