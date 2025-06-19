#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
#include "Proof/Utils/Curve.h"
namespace Proof
{

	class TerrainRenderer : RefCounted
	{
	public:
		TerrainRenderer();
		Count<class Mesh> GetTerrainMesh() { return m_TerrainMesh; };

		Count<class Texture2D> GetNoiseTexture() { return m_NoiseTexture; };
		Count<class Texture2D> GetColorTexture() { return m_ColorTexture; };

		//uint32_t MapSize = 100;
		ClampedValue<float, 0.1f, 1000.0f> TerrainScale = 30.0f; // scale of the terrain
		InterpolationCurve Curve;
		int Seed = 0;
		ClampedValue<int, 0, 6> LevelOfDetail = 0;
		struct NoiseSettings
		{
			glm::vec2 Offset = glm::vec2(0.0f);
			// FBD settings
			float Scale = 25.0f;
			ClampedValue<int,1,20> Octaves = 1;
			ClampedValue<float,0.0f,1.0f> Persistence = 0.5f;
			float Lacunarity = 2.0f;
		} NoiseParams;

		void RegenerateTerrainMesh();
	private:
		void GenerateMesh(const std::vector<float>& heightMap, uint32_t width, uint32_t	height);
	private:
		//https://www.youtube.com/watch?v=417kJGPKwDg&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=6
		
		const uint32_t MapChunkSize = 241;
		Count<class Mesh> m_TerrainMesh;
		Count<class Texture2D> m_NoiseTexture;
		Count<class Texture2D> m_ColorTexture;

		friend class World;
	};
}