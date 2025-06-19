#include "Proofprch.h"
#include "NormalTerrain.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Material.h"
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
                m_Terrain->AddChunk(coord, chunkSize);

                auto noiseData = m_Terrain->GenerateNoiseData();
                auto terrainData = m_Terrain->GenerateMesh(noiseData.HeightMap, chunkSize, chunkSize);

               TerrainChunk* chunk = m_Terrain->GetChunk(coord);

               chunk->Mesh = terrainData.GenerateMesh();
               AssetManager::CreateRuntimeAsset(chunk->Mesh, "Terrain Mesh chunk");


               //chunk->Mesh->GetMaterialTable()->GetMaterial(0)->SetAlbedoMap(m_ColorTexture);
               chunk->Mesh->GetMaterialTable()->GetMaterial(0)->SetAlbedo(glm::vec3(1));
               chunk->Mesh->GetMaterialTable()->GetMaterial(0)->SetEmission(0.0f);
               chunksCount++;
            }
        }

        PF_EC_TRACE("Terrain Generator generated {} Chunks", chunksCount);
	}
}
