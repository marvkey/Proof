#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
#include "Proof/Utils/Curve.h"
#include "Proof/Math/AABB.h"
#include "../SceneUtils.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Asset/Asset.h"
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

	struct TerrainLayer
	{
		std::string Name = "";
		ClampedValue<float, 0.0f, 1.0f> StartHeight = 0.0f;
		AssetKey<AssetType::Texture> Texture = AssetID(0);
		glm::vec3 ColorTint = glm::vec3(1.0f);
		ClampedValue<float, 0.0f, 1.0f> ColorTintStrength = 1.0f;
		ClampedValue<float, 0.0f, 1.0f> BlendStrength = 1.0f;
		float TextureScale = 1.0f;
	};

	struct TerrainLayerStack
	{
		std::vector<TerrainLayer> Layers;

		void AddLayer(const TerrainLayer& layerToAdd) {
			TerrainLayer newLayer = layerToAdd;
			if(newLayer.StartHeight == 0.0f)
				newLayer.StartHeight = Layers.empty()  ? 0.0f : 1.0f; // Default to 1.0 if not redistributing

			Layers.push_back(newLayer);
			RedistributeLayerHeightsIfNeeded();
		}

		void RemoveLayer(size_t index) {
			if (index < Layers.size()) {
				Layers.erase(Layers.begin() + index);
			}
		}

		void RedistributeLayerHeightsIfNeeded() {
			if (Layers.empty())
				return;

			// Only redistribute if the last layer's StartHeight is 1.0
			if (Layers.back().StartHeight != 1.0f)
				return;

			//size_t count = Layers.size();
			//float spacing = 1.0f / static_cast<float>(count);
			//
			//for (size_t i = 0; i < count; ++i) {
			//	Layers[i].StartHeight = std::clamp(i * spacing, 0.0f, 1.0f);
			//}
		}

		void ResortLayerStack() {
			std::sort(Layers.begin(), Layers.end(), [](const TerrainLayer& a, const TerrainLayer& b)
				{
					return a.StartHeight < b.StartHeight;
				});
		}

		void InsertLayerAt(TerrainLayer newLayer, int index) {
			/*
			if (index < 0) index = 0;
			if (index > static_cast<int>(Layers.size())) index = static_cast<int>(Layers.size());

			float minHeight = 0.0f;
			float maxHeight = 1.0f;

			if (!Layers.empty()) {
				if (index == 0) {
					maxHeight = Layers[0].StartHeight;
				}
				else if (index == Layers.size()) {
					minHeight = Layers.back().StartHeight;
				}
				else {
					minHeight = Layers[index - 1].StartHeight;
					maxHeight = Layers[index].StartHeight;
				}
			}

			newLayer.StartHeight = (minHeight + maxHeight) * 0.5f;
			Layers.insert(Layers.begin() + index, newLayer);

			// Redistribute just the local region to make room
			int start = std::max(index - 1, 0);
			int end = std::min(index + 2, static_cast<int>(Layers.size()));

			int count = end - start;
			if (count > 1) {
				for (int i = 0; i < count; ++i) {
					float t = static_cast<float>(i) / static_cast<float>(count - 1);
					Layers[start + i].StartHeight = std::lerp(Layers[start].StartHeight, Layers[end - 1].StartHeight, t);
				}
			}
			*/
			ResortLayerStack(); // maintain order
		}
	};

	struct TerrainItemSpawner
	{
		ClampedValue<float, 0.0f, 1.0f> MinHeight = 0.0f; // minimum height to spawn items
		ClampedValue<float, 0.0f, 1.0f> MaxHeight = 1.0f; // minimum height to spawn items

		std::vector<AssetKey<AssetType::Mesh>> Items; // Randomizes wich one to spawn ( like trees dont wanna spawn teh same trees)

		float Density = 0.01f;        // Frequency of the noise map
		float SpawnThreshold = 0.6f;  // Controls how clustered / sparse items are

		ClampedValue<float, 0.0f, 1.0f> ChanceSpawan = 0.10; // minimum height to spawn items

	};
	struct TerrainChunk;
	class TerrainRenderer : public RefCounted
	{
	public:
		TerrainRenderer();
		TerrainRenderer(Count<TerrainRenderer> otherTerrain);
		virtual ~TerrainRenderer();
	public:
		ClampedValue<float, 0.1f, 1000.0f> TerrainScale = 30.0f; // scale of the terrain
		InterpolationCurve Curve;
		int Seed = 0;
		ClampedValue<int, 0, 6> LevelOfDetail = 0;
		bool UseFallOff = false;
		TerrainLayerStack LayerStack;
	
		struct NoiseSettings
		{
			glm::vec2 Offset = glm::vec2(0.0f);
			// FBD settings
			float Scale = 25.0f;
			ClampedValue<int,1,20> Octaves = 1;
			ClampedValue<float,0.0f,1.0f> Persistence = 0.5f;
			float Lacunarity = 2.0f;

			auto operator<=>(const NoiseSettings&) const = default;  // generates all comparison operators
		} NoiseParams;

	public:

		Count<class Texture2D> GetNoiseTexture() { return m_NoiseTexture; };
		Count<class Texture2D> GetColorTexture() { return m_ColorTexture; };

		void RegenerateTerrainMesh();

		uint32_t GetChunkSize() { return MapChunkSize; }

		void AddChunk(glm::vec2 coord, int chunkSize);
		bool HasChunk(glm::vec2 coord);
		struct TerrainChunk* GetChunk(glm::vec2 coord);

		void Update(float deltaTime, const glm::mat4& transform);
		void Render(Count<class WorldRenderer> renderer);

		Entity GetPhysicsEntity()// only active in runtime
		{
			return m_PhysicsEntity;
		}

		Count<World> GetWorld()
		{
			if (m_World.IsValid())
				return m_World.Lock();

			return nullptr;
		}

		void SetWorld(Count<World> world);

	private:
		void EndGenerateTerrain();// every tiem we generate terrain just clean up adn set some deafult valeus

		TerrainMeshBuilderData GenerateMesh(const std::vector<float>& heightMap, uint32_t width, uint32_t height);
		TerrainChunkNoiseData GenerateNoiseData(glm::vec2 extraOffset = glm::vec2(0));

		friend class NormalTerrain;

		float GetMinHeight() // for shader
		{
			return TerrainScale * Curve.Evaluate(0.0f);
		}
		float GetMaxHeight() // for shader
		{
			return TerrainScale * Curve.Evaluate(1.0f);
		}
	private:
		//https://www.youtube.com/watch?v=417kJGPKwDg&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=6
		
		Transform m_Transform;
		const uint32_t MapChunkSize = 241;
		Count<class Texture2D> m_NoiseTexture;
		Count<class Texture2D> m_ColorTexture;

		std::vector< TerrainChunk> m_Chunks;
		Count<class EndlessTerrain> m_EndlessTerrain;
		Count<class NormalTerrain> m_NormalTerrain;
		std::vector<float> m_FallOffData;

		Entity m_PhysicsEntity;; // only used in runtime for phsycics collisons

		WeakCount<World> m_World;;
		friend class World;

		Count<class GrassBladePlane> m_GrassBladePanel;
		std::vector<struct UBGrassBlade> m_GrassBlades; // gets cleared once its full and sento to gpu

		Count<class RenderMaterial> m_TerrainRenderMaterial;
		Count<class UniformBufferSet> m_UBTerrainInfo; // used for shader infos
		Count<class StorageBufferSet> m_SBTerainLayers; // used for shader infos
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
		void GeneratePhysicsCollisons();
	private:

		Entity m_PhysicsEntity;
		uint32_t m_Lod = 0;
		bool m_IsVisible = false;
		WeakCount<TerrainRenderer> m_TerrainRenderer;

		Count<class MeshCollider> m_MeshCollider; // only in rutnime
		friend class TerrainRenderer;
	};
}