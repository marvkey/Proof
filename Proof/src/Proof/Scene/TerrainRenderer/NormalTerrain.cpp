#include "Proofprch.h"
#include "NormalTerrain.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/Texture.h"
namespace Proof
{

	NormalTerrain::NormalTerrain(Count<class TerrainRenderer> terrain)
        :m_Terrain(terrain)
	{
        GenerateChunks(Size,Size);
	}
	void NormalTerrain::OnUpdate(float deltaTime)
	{

	}
	void NormalTerrain::GenerateChunks(uint32_t widht, uint32_t height)
	{
        if (!m_Terrain)
            return;

        m_Terrain->m_Chunks.clear();

        const uint32_t chunkSize = m_Terrain->GetChunkSize();
        const uint32_t numChunksX = Math::RoundToInt(static_cast<float>(widht) / chunkSize);
        const uint32_t numChunksY = Math::RoundToInt(static_cast<float>(height) / chunkSize);

        uint32_t chunksCount;
        for (uint32_t y = 0; y < numChunksY; ++y)
        {
            for (uint32_t x = 0; x < numChunksX; ++x)
            {
                glm::vec2 coord = glm::vec2(x, y);
                m_Terrain->AddChunk(coord, chunkSize-1);


                //  apply world offset to noise, so edges match up between chunks
                glm::vec2 worldOffset = coord * static_cast<float>(chunkSize-1);

                auto noiseData = m_Terrain->GenerateNoiseData(worldOffset);
                auto terrainData = m_Terrain->GenerateMesh(noiseData.HeightMap, chunkSize, chunkSize);

               TerrainChunk* chunk = m_Terrain->GetChunk(coord);

               chunk->Mesh = terrainData.GenerateMesh();
               AssetManager::CreateRuntimeAsset(chunk->Mesh, "Terrain Mesh chunk");

               Buffer colorBuffer(noiseData.ColourMap.data(), noiseData.ColourMap.size() * sizeof(uint32_t), true);

               TextureConfiguration config;
               config.DebugName = "Color Texture";
               config.Width = chunkSize;
               config.Height = chunkSize;
               config.Format = ImageFormat::RGBA;
               config.GenerateMips = true;
               auto colorTexture = Texture2D::Create(config, colorBuffer, SamplerFactory::GetPoint());

			   PbrSurfaceMaterial mat(chunk->Mesh->GetMaterialTable()->GetMaterial(0));
               mat.SetAlbedoMap(colorTexture);
               mat.SetAlbedo(glm::vec3(1));
               mat.SetEmission(0.0f);

               colorBuffer.Release();

               chunksCount++;
            }
        }
        for (auto& chunk : m_Terrain->m_Chunks)
            chunk.GeneratePhysicsCollisons();

        PF_EC_TRACE("Terrain Generator generated {} Chunks", chunksCount);
	}
}
