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
#include "Proof/Math/Random.h"
#include "../GrassRenderer/GrassRenderer.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Scene/Prefab.h" 
#include "Proof/Renderer/UniformBuffer.h"

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


	struct alignas(16) UBTerrainShaderInfos
	{
		uint32_t LayerCount;
		float MinHeight;
		float MaxHeight;
	};

	
	struct alignas(16) UBTerrainLayer
	{
		float StartHeight;
		float BlendStrength;
		float ColorTint;
		float TextureScale;

		glm::vec3 Colour;
	};
	TerrainRenderer::TerrainRenderer()
	{
		m_Chunks.clear();
		m_TerrainRenderMaterial = RenderMaterial::Create("Terrain Material",Renderer::GetShader("TerrainShader"));
		m_SBTerainLayers = StorageBufferSet::Create(sizeof(UBTerrainLayer));
		m_UBTerrainInfo = UniformBufferSet::Create(sizeof(UBTerrainShaderInfos));

		m_TerrainRenderMaterial->Set("TerrainInfos", m_UBTerrainInfo);
		m_TerrainRenderMaterial->Set("TerrainLayers", m_SBTerainLayers);
	}

	TerrainRenderer::TerrainRenderer(Count<TerrainRenderer> otherTerrain)
	{
		m_Chunks.clear();
		m_TerrainRenderMaterial = RenderMaterial::Create("Terrain Material", Renderer::GetShader("TerrainShader"));
		m_SBTerainLayers = StorageBufferSet::Create(sizeof(UBTerrainLayer));
		m_UBTerrainInfo = UniformBufferSet::Create(sizeof(UBTerrainShaderInfos));

		m_TerrainRenderMaterial->Set("TerrainInfos", m_UBTerrainInfo);
		m_TerrainRenderMaterial->Set("TerrainLayers", m_SBTerainLayers);

		NoiseParams = otherTerrain->NoiseParams;

		TerrainScale = otherTerrain->TerrainScale;
		Curve = otherTerrain->Curve;
		Seed = otherTerrain->Seed;
		LevelOfDetail = otherTerrain->LevelOfDetail;
		UseFallOff = otherTerrain->UseFallOff;

		LayerStack = otherTerrain->LayerStack;
		ItemSpawner = otherTerrain->ItemSpawner;

		if (!m_NormalTerrain)
		{
			m_NormalTerrain = Count<NormalTerrain>::Create(this);
			//m_GrassBladePanel = Count<GrassBladePlane>::Create(m_GrassBlades);
			EndGenerateTerrain();
		}
		//m_SpawnLater = otherTerrain->m_SpawnLater;
	}

	TerrainRenderer::~TerrainRenderer()
	{
	}

	void TerrainRenderer::RegenerateTerrainMesh()
	{
		if (m_NormalTerrain)
		{
			//m_SpawnLater.clear(); // clear any spawn later items
			m_NormalTerrain->Regenirate();
			//m_GrassBladePanel = Count<GrassBladePlane>::Create(m_GrassBlades);
			EndGenerateTerrain();
		}
	}

	
	struct TerrainType
	{
		std::string Name; // opotional
		float Height;
		glm::vec4 colour;

	};

	std::vector<TerrainType> regions = {
		{ "Deep Water",0.0f, glm::vec4(0.0f, 0.0f, 0.6f, 1.0f) }, // Deep Water
		{ "Shallow Water",0.4f, glm::vec4(0.2f, 0.4f, 0.8f, 1.0f) }, // Shallow Water
		{ "Sand",0.45f, glm::vec4(0.85f, 0.8f, 0.5f, 1.0f) }, // Sand
		{ "Grass",0.5f,  glm::vec4(0.3f, 0.6f, 0.2f, 1.0f) }, // Grass
		{ "Darker Grass",0.6f,  glm::vec4(0.2f, 0.5f, 0.2f, 1.0f) }, // Darker Grass
		{ "Rock",0.75f, glm::vec4(0.3f, 0.2f, 0.2f, 1.0f) }, // Rock
		{ "Dark Rock",0.9f,  glm::vec4(0.2f, 0.15f, 0.15f, 1.0f) }, // Dark Rock
		{ "Snow",1.0f,  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) }  // Snow
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
		{
			m_NormalTerrain = Count<NormalTerrain>::Create(this);
			//m_GrassBladePanel = Count<GrassBladePlane>::Create(m_GrassBlades);
			EndGenerateTerrain();
		}

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
	}

	void TerrainRenderer::Render(Count<class WorldRenderer> renderer)
	{
		PF_PROFILE_FUNC();
		{
			UBTerrainShaderInfos terrainInfo;
			terrainInfo.LayerCount = LayerStack.Layers.size();
			terrainInfo.MinHeight = GetMinHeight();
			terrainInfo.MaxHeight = GetMaxHeight();
			m_UBTerrainInfo->GetBuffer()->SetData(Buffer(&terrainInfo, sizeof(UBTerrainShaderInfos)));
			
		}
		if (!LayerStack.Layers.empty())
		{
			std::vector<UBTerrainLayer> layers(LayerStack.Layers.size());

			for (int i = 0; i < layers.size(); i++)
			{
				layers[i].StartHeight = LayerStack.Layers[i].StartHeight;
				layers[i].Colour = LayerStack.Layers[i].ColorTint;
				layers[i].ColorTint = LayerStack.Layers[i].ColorTintStrength;
				layers[i].TextureScale = LayerStack.Layers[i].TextureScale;
				layers[i].BlendStrength = LayerStack.Layers[i].BlendStrength;
			}
			Buffer buffer{ (void*)layers.data(), layers.size() * sizeof(UBTerrainLayer) };
			m_SBTerainLayers->GetBuffer()->Resize(buffer);
		}

		if (!LayerStack.Layers.empty())
		{
			std::vector<Count<Texture2D>> textures(LayerStack.Layers.size());
			for (int i = 0; i < textures.size(); i++)
			{
				if (LayerStack.Layers[i].Texture.IsValid())
					textures[i] = LayerStack.Layers[i].Texture.GetAsset<Texture2D>();
				else
					textures[i] = Renderer::GetWhiteTexture();
			}

			m_TerrainRenderMaterial->Set("u_Textures", textures);
		}

		for (auto& terrainChunk : m_Chunks)
		{
			if (!terrainChunk.GetIsVisible())
				continue;

			if (terrainChunk.Mesh)
				renderer->SubmitMesh(terrainChunk.Mesh, m_TerrainRenderMaterial, terrainChunk.Transform.GetTransform() * m_Transform.GetTransform());
		}

		//if (m_GrassBladePanel)
		{
			//renderer->SubmitGrassPlane(m_GrassBladePanel, m_Transform.GetTransform());
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
	bool IsInRegionRange(const std::vector<TerrainType>& regions, const std::string& name, float height)
	{
		for (size_t i = 0; i < regions.size(); ++i)
		{
			if (regions[i].Name != name)
				continue;

			float minH = regions[i].Height;
			float maxH = (i + 1 < regions.size()) ? regions[i + 1].Height : 1.0f;

			return height >= minH && height < maxH;
		}

		return false;
	}

	static bool ShouldSpawnItem(const TerrainItemSpawner& spawner, float currentHeight, glm::vec2 offset)
	{
		// Check height range
		if (currentHeight < spawner.MinHeight || currentHeight > spawner.MaxHeight)
			return false;

		// Create and configure FastNoiseLite (based on spawner settings)
		FastNoiseLite noise;
		noise.SetSeed(12345); // You can make this customizable per spawner if needed
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(spawner.Density); // You might want to pass this in via spawner too

		// Sample and normalize
		float noiseVal = noise.GetNoise(offset.x, offset.y); // [-1, 1]
		float normalized = (noiseVal + 1.0f) * 0.5f; // Normalize to [0, 1]

		// Threshold check
		if (normalized > spawner.SpawnThreshold)
		{
			if (Random::Real(0.0f, 1.0f) < spawner.ChanceSpawan) // e.g 30 then 30% chance to actually spawn
				return true;
		}
		return false;
	}


	
	TerrainChunkNoiseData TerrainRenderer::GenerateNoiseData(glm::vec2 extraOffset)
	{
#if 0
		TerrainItemSpawner Spawner =
		{
				.MinHeight = 0.11f,
				.MaxHeight = 0.35f,
				.Density = 0.01f,
				.SpawnThreshold = 0.5f
		};

		Spawner.Items.push_back(AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Cube)->GetID());
#endif
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

		InterpolationCurve curveCopy = Curve;
		float ScaleY = TerrainScale;
		// Terrain center offset for mesh alignment
		float topLeftX = (width - 1) / -2.0f;
		float topLeftZ = (height - 1) / 2.0f;
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

				for (uint32_t spawnerIndex = 0; spawnerIndex < ItemSpawner.size(); spawnerIndex++)
				{
					const TerrainItemSpawner& Spawner = ItemSpawner[spawnerIndex];
					// avoid dividing by 0
					if (Spawner.Spacing != 0)
					{
						// Only allow spawn every 'spacing' units
						if ((x) % Spawner.Spacing != 0 || (y) % Spawner.Spacing != 0)
							continue;
					}

					float offsetX = Random::Real(0.0f, 1.0f) * Spawner.MaxPositionOffset;
					float offsetZ = Random::Real(0.0f, 1.0f) * Spawner.MaxPositionOffset;

					float worldX = extraOffset.x + topLeftX + x + offsetX;
					float worldZ = extraOffset.y + topLeftZ - y - offsetZ;
					float worldY = curveCopy.Evaluate(currentHeight) * ScaleY;

					// Check if the item should spawn based on noise and chance
					if (ShouldSpawnItem(Spawner, curveCopy.Evaluate(currentHeight), { worldX, worldZ }))
					{
						glm::vec3 rootPos(worldX, worldY, worldZ);
						TerrainSpawnlater spawnLater;
						spawnLater.Prefab = Spawner.Items[Random::Int<int>(0, Spawner.Items.size() - 1)];
						spawnLater.Position = rootPos;
						m_SpawnLater.push_back(spawnLater);
					}

				}
				

#if 0
				GrassBladeDefaultSettings grassBladeSettings;
				grassBladeSettings.MinHeight = 0.5;
				grassBladeSettings.MaxHeight = 1.0f;
				grassBladeSettings.MinBend = 20;
				grassBladeSettings.MaxBend = 25;


				for (int i = 0; i < regions.size(); i++) {
					if (currentHeight >= regions[i].Height) 
					{
						colourMap[y * width + x] = ConvertToBytes(regions[i].colour);

						if (IsInRegionRange(regions, "Grass", currentHeight) && Random::Bool() == true)
						{
							int grassDensity = Random::Int(1, 10);
							for (int i = 0; i < grassDensity; ++i)
							{
								// Random offset within the 1x1 cell space (to avoid perfect alignment)
								float offsetX = Random::Real(0.0f, 1.0f);
								float offsetZ = Random::Real(0.0f, 1.0f);

								float worldX = extraOffset.x + topLeftX + x + offsetX;
								float worldZ = extraOffset.y + topLeftZ - y - offsetZ;

								float rawHeight = chunkNoiseData.HeightMap[y * width + x];
								float worldY = curveCopy.Evaluate(rawHeight) * ScaleY;

								glm::vec3 rootPos(worldX, worldY, worldZ);

								UBGrassBlade blade = UBGrassBlade(rootPos, grassBladeSettings);
								m_GrassBlades.push_back(blade);
							}

						}

						if (IsInRegionRange(regions, "Darker Grass", currentHeight))
						{
							int grassDensity = Random::Int(1, 5);
							for (int i = 0; i < grassDensity; ++i)
							{
								// Random offset within the 1x1 cell space (to avoid perfect alignment)
								float offsetX = Random::Real(0.0f, 1.0f);
								float offsetZ = Random::Real(0.0f, 1.0f);

								float worldX = extraOffset.x + topLeftX + x + offsetX;
								float worldZ = extraOffset.y + topLeftZ - y - offsetZ;

								float rawHeight = chunkNoiseData.HeightMap[y * width + x];
								float worldY = curveCopy.Evaluate(rawHeight) * ScaleY;

								glm::vec3 rootPos(worldX, worldY, worldZ);

								UBGrassBlade blade = UBGrassBlade(rootPos, grassBladeSettings);
								m_GrassBlades.push_back(blade);
							}
						}

					}
					else
						break;
				}
#endif
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


		for (auto& laterItem : m_SpawnLater)
		{
			Entity e = m_World.Lock()->CreateEntity("TerrainItem",laterItem.Prefab.GetAsset<Prefab>(),TransformComponent());
			e.GetTransformComponent().Location = laterItem.Position;
			e.GetTransformComponent().Location *= m_Transform.Scale;
			e.GetTransformComponent().Scale *= m_Transform.Scale;
			//e.GetTransformComponent().SetRotation(glm::vec3(0, Random::Real(0.0f, 360.0f), 0));
			//e.GetTransformComponent().SetScale(glm::vec3(1.0f));
		}
		//m_SpawnLater.clear();
	}


	void TerrainRenderer::EndGenerateTerrain()
	{
		m_GrassBlades.clear();
	}

	TerrainChunk::TerrainChunk(Count<TerrainRenderer> terrain)
	{
		m_TerrainRenderer = terrain.Get();
		if (!terrain->GetPhysicsEntity().IsValid())
			return;


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
		
			m_PhysicsEntity.GetTransformComponent() = TransformComponent();
		}

		m_MeshCollider = Count<MeshCollider>::Create(Mesh->GetID());

		m_MeshCollider->CollisionComplexity = ECollisionComplexity::UseComplexAsSimple;
		AssetManager::CreateRuntimeAsset(m_MeshCollider, fmt::format("Mesh Chunk coord Collider {}", Math::ToString(Coord)));

		m_PhysicsEntity.AddorReplaceComponent<MeshColliderComponent>(m_MeshCollider->GetID());
		m_PhysicsEntity.AddorReplaceComponent<RigidBodyComponent>();

		m_PhysicsEntity.GetComponent<TransformComponent>().SetTransform(Transform.GetTransform());
	}
}