#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
#include "Proof/Utils/Curve.h"
#include "Proof/Math/AABB.h"
#include "../SceneUtils.h"
namespace Proof
{
	struct TerrainChunk
	{
		Count<class Mesh> Mesh;
		Transform Transform;
		AABB Bounds;

		glm::vec2 Coord;
		bool GetIsVisible() const { return m_IsVisible; }
		void SetVisible(bool visible) { m_IsVisible = visible; }
		void UpdateTerrainChunk(glm::vec3 viewerPosition, float maxViewDst)
		{
			float viewerDstFromNearestEdge = glm::sqrt(Bounds.GetSqrDistanceToPoint(viewerPosition));
			bool visible = viewerDstFromNearestEdge <= maxViewDst;
			SetVisible(visible);
		}
	private:
		bool m_IsVisible = false;
	};


	class TerrainRenderer : RefCounted
	{
	public:
		TerrainRenderer();

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

		uint32_t GetChunkSize() { return MapChunkSize - 1; }

		void AddChunk(glm::vec2 coord, int chunkSize);
		bool HasChunk(glm::vec2 coord);
		TerrainChunk* GetChunk(glm::vec2 coord)
		{
			for (auto& chunk : m_Chunks)
			{
				if (chunk.Coord == coord)
					return &chunk;
			}
			return nullptr; // Not found
		}

		void Update(float deltaTime, const glm::mat4& transform);
		void Render(Count<class WorldRenderer> renderer);
	private:
		void GenerateMesh(const std::vector<float>& heightMap, uint32_t width, uint32_t	height);
	private:
		//https://www.youtube.com/watch?v=417kJGPKwDg&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=6
		
		const uint32_t MapChunkSize = 241;
		Count<class Texture2D> m_NoiseTexture;
		Count<class Texture2D> m_ColorTexture;

		std::vector< TerrainChunk> m_Chunks;
		Count<class EndlessTerrain> m_EndlessTerrain;
		Transform m_Transform;
		friend class World;
	};
}