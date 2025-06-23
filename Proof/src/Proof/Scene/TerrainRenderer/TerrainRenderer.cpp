#include "Proofprch.h"
#include "TerrainRenderer.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Renderer/Vertex.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Material.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/Texture.h"
#include "EndlessTerrain.h"
#include "Proof/Renderer/Colors.h"
#include "EndlessTerrain.h"
#include "NormalTerrain.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Input/Input.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "../Entity.h"
#include "Proof/Physics/MeshCollider.h"
#include <FastNoise/FastNoiseLite.h>
namespace Proof
{

	enum class NormalizeMode { Local, Global };

	std::vector<float> GenerateNoiseMap(
		int mapWidth, int mapHeight, int seed,
		const TerrainRenderer::NoiseSettings& settings,
		glm::vec2 extraOffset,
		NormalizeMode normalizeMode = NormalizeMode::Global
	) {
		std::vector<float> noiseMap(mapWidth * mapHeight);

		FastNoiseLite noise;
		noise.SetSeed(seed);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFractalType(FastNoiseLite::FractalType_FBm);
		noise.SetFrequency(1.0f / settings.Scale);
		noise.SetFractalOctaves(settings.Octaves);
		noise.SetFractalLacunarity(settings.Lacunarity);
		noise.SetFractalGain((float)settings.Persistence);

		float minVal = std::numeric_limits<float>::max();
		float maxVal = std::numeric_limits<float>::lowest();
		float maxPossibleHeight = 0;
		float amplitude = 1;
		float frequency = 1;

		for (int i = 0; i < settings.Octaves; i++) {

			maxPossibleHeight += amplitude;
			amplitude *= settings.Persistence;
		}

		for (int y = 0; y < mapHeight; ++y) {
			for (int x = 0; x < mapWidth; ++x) {
				float sampleX = float(x) + settings.Offset.x + extraOffset.x;
				float sampleY = float(y) - settings.Offset.y - extraOffset.y;

				float val = noise.GetNoise(sampleX, sampleY);
				int index = y * mapWidth + x;
				noiseMap[index] = val;

				minVal = std::min(minVal, val);
				maxVal = std::max(maxVal, val);
			}
		}
		for (int i = 0; i < mapWidth * mapHeight; ++i) {
			if (normalizeMode == NormalizeMode::Local) {
				// Normalize within this map
				noiseMap[i] = (noiseMap[i] - minVal) / (maxVal - minVal);
			}
			else {
				// Normalize based on maximum possible height
				float normalized = (noiseMap[i] + 1) / (maxPossibleHeight);
				noiseMap[i] = std::clamp(normalized, 0.0f, std::numeric_limits<float>::max());
			}
		}

		return noiseMap;
	}


	// Falloff evaluation curve (adjust a and b to shape it)
	//https://www.youtube.com/watch?v=COmtTyLCd6I&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=11

	// makes the fall off gradient not as stron
	float EvaluateFalloff(float value)
	{
		float a = 3.0f;
		float b = 2.2f;
		return std::pow(value, a) / (std::pow(value, a) + std::pow(b - b * value, a));
	}

	std::vector<float> GenerateFalloffMap(int width, int height)
	{
		std::vector<float> map(width * height);

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				float nx = x / static_cast<float>(width) * 2.0f - 1.0f;
				float ny = y / static_cast<float>(height) * 2.0f - 1.0f;

				float value = glm::max(std::abs(nx), std::abs(ny));
				map[y * width + x] = EvaluateFalloff(value);
			}
		}

		return map;
	}


	TerrainRenderer::TerrainRenderer()
	{
		m_Chunks.clear();
	}

	TerrainRenderer::TerrainRenderer(Count<TerrainRenderer> otherTerrain)
	{
		m_Chunks.clear();

		NoiseParams = otherTerrain->NoiseParams;

		TerrainScale = otherTerrain->TerrainScale;
		Curve = otherTerrain->Curve;
		Seed = otherTerrain->Seed;
		LevelOfDetail = otherTerrain->LevelOfDetail;
		UseFallOff = otherTerrain->UseFallOff;
	}

	void TerrainRenderer::RegenerateTerrainMesh()
	{
		if (m_NormalTerrain)
			m_NormalTerrain->Regenirate();
	}

	struct TerrainType
	{
		float Height;
		glm::vec4 colour;
	};

	std::vector<TerrainType> regions = {
		{ 0.0f, glm::vec4(0.0f, 0.0f, 0.6f, 1.0f) }, // Deep Water
		{ 0.4f, glm::vec4(0.2f, 0.4f, 0.8f, 1.0f) }, // Shallow Water
		{ 0.45f, glm::vec4(0.85f, 0.8f, 0.5f, 1.0f) }, // Sand
		{ 0.5f,  glm::vec4(0.3f, 0.6f, 0.2f, 1.0f) }, // Grass
		{ 0.6f,  glm::vec4(0.2f, 0.5f, 0.2f, 1.0f) }, // Darker Grass
		{ 0.75f, glm::vec4(0.3f, 0.2f, 0.2f, 1.0f) }, // Rock
		{ 0.9f,  glm::vec4(0.2f, 0.15f, 0.15f, 1.0f) }, // Dark Rock
		{ 1.0f,  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) }  // Snow
	};


	void TerrainRenderer::AddChunk(glm::vec2 coord, int chunkSize)
	{
		if (HasChunk(coord))
			return;

		TerrainChunk& chunk = m_Chunks.emplace_back(this);
		chunk.Coord = coord;

		// Set world position
		glm::vec3 worldPosition = glm::vec3(coord.x * chunkSize, 0.0f, coord.y * chunkSize);

		// Apply transform
		chunk.Transform.Location = worldPosition;
		chunk.Transform.Scale = glm::vec3(1.0f);
		//chunk.Transform.Scale = glm::vec3((float)chunkSize / 10.0f);

		// Optionally inherit rotation/scale from parent if your engine supports hierarchical transforms
		// This is up to how your engine handles parenting (e.g., combine parent+child matrices on render)
		// You could do something like:
		// chunk.Transform = CombineTransform(parentTransform, chunk.Transform);

		// Set bounds (XZ plane like Unity)
		glm::vec3 center = glm::vec3(worldPosition.x, 0.0f, worldPosition.z);
		glm::vec3 extents = glm::vec3((float)chunkSize / 2.0f);
		chunk.Bounds = AABB(center - extents, center + extents);

		// Assign mesh (assumes you have a method to generate a flat plane or terrain mesh)
		//chunk.Mesh = MeshWorkShop::GeneratePlane(chunkSize, chunkSize);
		//AssetManager::CreateRuntimeAsset(chunk.Mesh, fmt::format("Terrain chunk {}", m_Chunks.size() - 1));

		// Initial visibility off
		chunk.SetVisible(false);

	}

	bool TerrainRenderer::HasChunk(glm::vec2 coord)
	{
		for (const auto& chunk : m_Chunks)
		{
			if (chunk.Coord == coord)
				return true;
		}
		return false;
	}

	TerrainChunk* TerrainRenderer::GetChunk(glm::vec2 coord)
	{
		for (auto& chunk : m_Chunks)
		{
			if (chunk.Coord == coord)
				return &chunk;
		}
		return nullptr; // Not found
	}

	void TerrainRenderer::Update(float deltaTime, const glm::mat4& transform)
	{
		//if(!m_EndlessTerrain)
			//m_EndlessTerrain = Count<EndlessTerrain>::Create(this);

		if (!m_NormalTerrain)
			m_NormalTerrain = Count<NormalTerrain>::Create(this);

		m_Transform.SetTransform(transform);
		m_NormalTerrain->OnUpdate(deltaTime);
		if (m_PhysicsEntity.IsValid())
			m_PhysicsEntity.GetTransformComponent().SetTransform(transform);

		//if (m_EndlessTerrain)
		//{
		//	m_EndlessTerrain->OnUpdate(deltaTime);
		//}


		glm::vec3 ViewPosition = glm::vec3(0);
		glm::vec2 viewerPosition = glm::vec2(ViewPosition.x, ViewPosition.z);

		uint32_t visibleCunks = 0;
		for (auto& chunk : m_Chunks)
		{
			if (chunk.GetIsVisible())
				visibleCunks++;

		}

		float maxViewDistance = MapChunkSize * visibleCunks;


		for (auto& chunk : m_Chunks)
			chunk.UpdateTerrainChunk(viewerPosition, maxViewDistance);


#if 0

		{
			std::lock_guard<std::mutex> lock(queueMutex);
			while (!mapDataQueue.empty()) {
				auto& info = mapDataQueue.front();
				info.Callback(info.Data);
				mapDataQueue.pop();
			}

			while (!meshDataQueue.empty()) {
				auto& info = meshDataQueue.front();
				info.Callback(info.Data);
				meshDataQueue.pop();
			}
		}
#endif
	}

	void TerrainRenderer::Render(Count<class WorldRenderer> renderer)
	{
		for (auto& terrainChunk : m_Chunks)
		{
			if (!terrainChunk.GetIsVisible())
				continue;

			if (terrainChunk.Mesh)
				renderer->SubmitMesh(terrainChunk.Mesh, terrainChunk.Mesh->GetMaterialTable(), terrainChunk.Transform.GetTransform() * m_Transform.GetTransform());
		}
	}

	TerrainMeshBuilderData TerrainRenderer::GenerateMesh(const std::vector<float>& heightMap, uint32_t width, uint32_t height)
	{
		TerrainMeshBuilderData meshBuilderData = TerrainMeshBuilderData(width, height);
		float ScaleY = TerrainScale;
		InterpolationCurve curveCopy = Curve;
		// just to make the pivot at the center
		float topLeftX = (width - 1) / -2.0f;
		// just to make the pivot at the center
		float topLeftZ = (height - 1) / 2.0f;

		int meshSimplificationIncrement = (LevelOfDetail == 0) ? 1 : LevelOfDetail * 2;
		int verticesPerLine = (width - 1) / meshSimplificationIncrement + 1; //https://www.youtube.com/watch?v=417kJGPKwDg&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=6


		uint32_t vertexIndex = 0;
		for (uint32_t y = 0; y < height; y += meshSimplificationIncrement)
		{
			for (uint32_t x = 0; x < width; x += meshSimplificationIncrement)
			{
				Vertex v;
				v.Position = glm::vec3(topLeftX + x, curveCopy.Evaluate(heightMap[y * width + x]) * ScaleY, topLeftZ - y);

				meshBuilderData.Vertices[vertexIndex] = v;
				meshBuilderData.Vertices[vertexIndex].TexCoord = glm::vec2(
					x / float(width - 1),
					1.0f - (y / float(height - 1)) // Flip Y
				);
				if (x < width - 1 && y < height - 1)
				{
					meshBuilderData.AddTriangle(vertexIndex, vertexIndex + verticesPerLine + 1, vertexIndex + verticesPerLine);
					meshBuilderData.AddTriangle(vertexIndex + verticesPerLine + 1, vertexIndex, vertexIndex + 1);
				}

				vertexIndex++;
			}
		}
		return meshBuilderData;
	}

	TerrainChunkNoiseData TerrainRenderer::GenerateNoiseData(glm::vec2 extraOffset)
	{
		if (UseFallOff)
		{
			if (m_FallOffData.empty())
				m_FallOffData = GenerateFalloffMap(MapChunkSize, MapChunkSize);
		}
		uint32_t width = MapChunkSize;
		uint32_t height = MapChunkSize;

		TerrainChunkNoiseData chunkNoiseData;
		chunkNoiseData.HeightMap = GenerateNoiseMap(MapChunkSize, MapChunkSize, Seed, NoiseParams, extraOffset);
		chunkNoiseData.ColourMap.resize(width * height);

		std::vector<uint32_t> noiseMapData(width * height);

		auto& colourMap = chunkNoiseData.ColourMap;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				if (UseFallOff)
				{
					chunkNoiseData.HeightMap[y * width + x] = glm::clamp(chunkNoiseData.HeightMap[y * width + x] - m_FallOffData[y * width + x], 0.f, 1.0f);
				}

				float currentHeight = chunkNoiseData.HeightMap[y * width + x]; // assumes row-major layout
				glm::vec3 color = Math::Lerp(Colors::Black, Colors::White, currentHeight); // grayscale
				uint32_t packed = ConvertToBytes(color);
				noiseMapData[x + y * width] = packed;

				for (int i = 0; i < regions.size(); i++) {
					if (currentHeight >= regions[i].Height) {
						colourMap[y * width + x] = ConvertToBytes(regions[i].colour);
					}
					else
						break;

				}
			}
		}
#if 0
		Buffer buffer(noiseMapData.data(), noiseMapData.size() * sizeof(uint32_t), true);
		TextureConfiguration config;
		config.DebugName = "Noise Texture";
		config.Width = width;
		config.Height = height;
		config.Format = ImageFormat::RGBA;
		config.GenerateMips = false;
		m_NoiseTexture = Texture2D::Create(config, buffer);

		buffer.Release();


		Buffer colorBuffer(colourMap.data(), colourMap.size() * sizeof(uint32_t), true);
		config.DebugName = "Color Texture";
		config.Width = width;
		config.Height = height;
		config.Format = ImageFormat::RGBA;
		config.GenerateMips = true;
		m_ColorTexture = Texture2D::Create(config, colorBuffer, SamplerFactory::GetPoint());
#endif
		return chunkNoiseData;
	}

	void TerrainRenderer::RequestMapData(std::function<void(TerrainChunkNoiseData)> callback) {
		std::thread([=]() { MapDataThread(callback); }).detach();
	}

	void TerrainRenderer::SetWorld(Count<World> world)
	{
		PF_CORE_ASSERT(world->GetState() != WorldState::Edit, "Has to be in edit state");

		if (m_PhysicsEntity.IsValid() && m_PhysicsEntity.GetCurrentWorld() != world.Get())
			m_PhysicsEntity.GetCurrentWorld()->DeleteEntity(m_PhysicsEntity);

		if (m_PhysicsEntity.IsValid() && m_PhysicsEntity.GetCurrentWorld() == world.Get())
			return;

		m_PhysicsEntity = world->CreateEntity("Terrian Mesh physics collidres");

		m_World = world.Get();

		m_PhysicsEntity.GetTransformComponent().SetTransform(m_Transform.GetTransform());

		for (auto& chunk : m_Chunks)
			chunk.GeneratePhysicsCollisons();
	}

	void TerrainRenderer::MapDataThread(std::function<void(TerrainChunkNoiseData)> callback) {
		TerrainChunkNoiseData mapData = GenerateNoiseData();
		std::lock_guard<std::mutex> lock(queueMutex);
		mapDataQueue.emplace(callback, mapData);
	}

	void TerrainRenderer::RequestMeshData(const TerrainChunkNoiseData& mapData, std::function<void(TerrainMeshBuilderData)> callback) {
		std::thread([=]() { MeshDataThread(mapData, callback); }).detach();
	}

	void TerrainRenderer::MeshDataThread(const TerrainChunkNoiseData& mapData, std::function<void(TerrainMeshBuilderData)> callback) {
		TerrainMeshBuilderData meshData = GenerateMesh(mapData.HeightMap, MapChunkSize, MapChunkSize); // This should match what Unity was doing
		std::lock_guard<std::mutex> lock(queueMutex);
		meshDataQueue.emplace(callback, meshData);
	}

	TerrainChunk::TerrainChunk(Count<TerrainRenderer> terrain)
	{
		m_TerrainRenderer = terrain.Get();
		if (!terrain->GetPhysicsEntity().IsValid())
			return;


		//terrain->RequestMapData([this](TerrainChunkNoiseData mapData)
		//	{
		//		OnMapDataReceived(mapData);
		//	});
	}

	void TerrainChunk::OnMapDataReceived(TerrainChunkNoiseData mapData)
	{
		if (!m_TerrainRenderer.IsValid())
			return;

		//auto terrain = m_TerrainRenderer.Lock();
		//
		//terrain->RequestMeshData(mapData, [this](TerrainMeshBuilderData meshData)
		//	{
		//		OnMeshDataReceived(meshData);
		//	});
	}

	void TerrainChunk::OnMeshDataReceived(TerrainMeshBuilderData meshData)
	{
		if (Mesh == nullptr)
		{
			Mesh = meshData.GenerateMesh();
			AssetManager::CreateRuntimeAsset(Mesh, "Terrain Mesh chunk");
		}
		else
		{
			meshData.RecalculateNormals(meshData.Vertices, meshData.Indices);
			Mesh->Reset("Terrain Mesh", meshData.Vertices, meshData.Indices);
		}

		//Mesh->GetMaterialTable()->GetMaterial(0)->SetAlbedoMap(m_ColorTexture);
		Mesh->GetMaterialTable()->GetMaterial(0)->SetAlbedo(glm::vec3(1));
		Mesh->GetMaterialTable()->GetMaterial(0)->SetEmission(0.0f);
	}
	void TerrainChunk::GeneratePhysicsCollisons()
	{
		if (!m_TerrainRenderer.IsValid())
			return;

		Count<TerrainRenderer> terrain = m_TerrainRenderer.Lock();
		Count<World> world = terrain->GetWorld();

		if (world == nullptr)
			return;

		if (!m_PhysicsEntity.IsValid())
		{
			m_PhysicsEntity = world->CreateChildEntity(terrain->GetPhysicsEntity(), fmt::format("Mesh Chunk coord {}", Math::ToString(Coord)));
		}

		m_MeshCollider = Count<MeshCollider>::Create(Mesh->GetID());

		m_MeshCollider->CollisionComplexity = ECollisionComplexity::UseComplexAsSimple;
		AssetManager::CreateRuntimeAsset(m_MeshCollider, fmt::format("Mesh Chunk coord Collider {}", Math::ToString(Coord)));

		m_PhysicsEntity.AddorReplaceComponent<MeshColliderComponent>(m_MeshCollider->GetID());
		m_PhysicsEntity.AddorReplaceComponent<RigidBodyComponent>();

		m_PhysicsEntity.GetComponent<TransformComponent>().SetTransform(Transform.GetTransform());
	}
}