#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
#include "Proof/Utils/Curve.h"
#include "Proof/Math/AABB.h"
#include "../SceneUtils.h"
#include "Proof/Scene/Mesh.h"
#include "../Entity.h"
#include <queue>
#include <mutex>
#include <functional>
namespace Proof
{


	class TerrainMeshBuilderData
	{

	public:
		TerrainMeshBuilderData(uint32_t width, uint32_t height)
		{
			Vertices.resize(width * height);
			Indices.resize(((width - 1) * (height - 1) * 2)); // 2 triangles per quad
		}

		void AddTriangle(uint32_t a, uint32_t b, uint32_t c) {
			Indices[m_TriangleIndex] = Index{ a, b, c };
			m_TriangleIndex++;
		}

		Count<Mesh> GenerateMesh()
		{
			RecalculateNormals(Vertices, Indices);
			return Count<Mesh>::Create("Terrain Mesh", Vertices, Indices);
		}

		void RecalculateNormals(std::vector<Vertex>& vertices, const std::vector<Index>& triangles) {
			// Clear existing normals
			for (auto& vertex : vertices) {
				vertex.Normal = glm::vec3(0.0f);
			}

			// Accumulate face normals
			for (const auto& tri : triangles) {
				const glm::vec3& v0 = vertices[tri.V1].Position;
				const glm::vec3& v1 = vertices[tri.V2].Position;
				const glm::vec3& v2 = vertices[tri.V3].Position;

				glm::vec3 edge1 = v1 - v0;
				glm::vec3 edge2 = v2 - v0;
				glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

				vertices[tri.V1].Normal += faceNormal;
				vertices[tri.V2].Normal += faceNormal;
				vertices[tri.V3].Normal += faceNormal;
			}

			// Normalize all normals
			for (auto& vertex : vertices) {
				vertex.Normal = glm::normalize(vertex.Normal);
			}
		}
		std::vector<Vertex> Vertices;
		std::vector<Index> Indices;
	private:

		uint32_t m_TriangleIndex = 0;
	};





	struct TerrainChunkNoiseData 
	{
		std::vector<float> HeightMap;
		std::vector<uint32_t> ColourMap;
	};

	template<typename T>
	struct MapThreadInfo {
		std::function<void(T)> Callback;
		T Data;

		MapThreadInfo(std::function<void(T)> callback, const T& data)
			: Callback(callback), Data(data) {
		}
	};
	struct TerrainChunk;
	class TerrainRenderer : public RefCounted
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
		bool UseFallOff = false;
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

		uint32_t GetChunkSize() { return MapChunkSize; }

		void AddChunk(glm::vec2 coord, int chunkSize);
		bool HasChunk(glm::vec2 coord);
		struct TerrainChunk* GetChunk(glm::vec2 coord);

		void Update(float deltaTime, const glm::mat4& transform);
		void Render(Count<class WorldRenderer> renderer);

		// Asynchronous requests
		void RequestMapData(std::function<void(TerrainChunkNoiseData)> callback);
		void RequestMeshData(const TerrainChunkNoiseData& mapData, std::function<void(TerrainMeshBuilderData)> callback);

	private:
		// Worker threads
		void MapDataThread(std::function<void(TerrainChunkNoiseData)> callback);
		void MeshDataThread(const TerrainChunkNoiseData& mapData, std::function<void(TerrainMeshBuilderData)> callback);


		TerrainMeshBuilderData GenerateMesh(const std::vector<float>& heightMap, uint32_t width, uint32_t height);
		TerrainChunkNoiseData GenerateNoiseData(glm::vec2 extraOffset = glm::vec2(0));

		friend class NormalTerrain;
	private:
		//https://www.youtube.com/watch?v=417kJGPKwDg&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=6
		
		const uint32_t MapChunkSize = 241;
		Count<class Texture2D> m_NoiseTexture;
		Count<class Texture2D> m_ColorTexture;

		std::vector< TerrainChunk> m_Chunks;
		Count<class EndlessTerrain> m_EndlessTerrain;
		Count<class NormalTerrain> m_NormalTerrain;
		std::vector<float> m_FallOffData;
		Transform m_Transform;

		std::queue<MapThreadInfo<TerrainChunkNoiseData>> mapDataQueue;
		std::queue<MapThreadInfo<TerrainMeshBuilderData>> meshDataQueue;
		std::mutex queueMutex;

		friend class World;
	};


	struct TerrainChunk
	{

		TerrainChunk(Count<TerrainRenderer> terrain);
		Count<class Mesh> Mesh;
		Transform Transform;
		AABB Bounds;

		glm::vec2 Coord;
		bool GetIsVisible() const { return m_IsVisible; }
		void SetVisible(bool visible) { m_IsVisible = visible; }
		void UpdateTerrainChunk(glm::vec2 viewerPosition, float maxViewDst)
		{
			float viewerDstFromNearestEdge = glm::sqrt(Bounds.GetSqrDistanceToPoint(viewerPosition));
			bool visible = viewerDstFromNearestEdge <= maxViewDst;
			SetVisible(visible);
		}

		void OnMapDataReceived(TerrainChunkNoiseData mapData);
		void OnMeshDataReceived(TerrainMeshBuilderData meshData);

	private:
		uint32_t m_Lod = 0;
		bool m_IsVisible = false;
		WeakCount<TerrainRenderer> m_TerrainRenderer;
	};
}