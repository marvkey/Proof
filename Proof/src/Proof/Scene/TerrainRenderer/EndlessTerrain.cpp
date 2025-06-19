#include "Proofprch.h"
#include "EndlessTerrain.h"
#include "TerrainRenderer.h"
namespace Proof
{

	

	EndlessTerrain::EndlessTerrain(Count<class TerrainRenderer> terrain)
		:m_Terrain(terrain)
	{

	}
	void EndlessTerrain::OnUpdate(float deltaTime)
	{
		m_ChunkSize = m_Terrain->GetChunkSize()-1;
		m_ChunksVisibleInViewDst = Math::RoundToInt(MaxViewdist /(float) m_ChunkSize);
		UpdateVisibleChunks();
	}
	void EndlessTerrain::UpdateVisibleChunks()
	{

		for (int i = 0; i < m_VisibleChunksLastFrame.size(); i++) {
			m_VisibleChunksLastFrame[i].SetVisible(false);
		}
		m_VisibleChunksLastFrame.clear();

		glm::vec3 ViewPosition = glm::vec3(0);
		m_ViewerPostion = glm::vec2(ViewPosition.x, ViewPosition.z);
		int currentChunkCoordX = glm::round(m_ViewerPostion.x / m_ChunkSize);
		int currentChunkCoordY = glm::round(m_ViewerPostion.y / m_ChunkSize);

		for (int yOffset = -m_ChunksVisibleInViewDst; yOffset <= m_ChunksVisibleInViewDst; yOffset++) {
			for (int xOffset = -m_ChunksVisibleInViewDst; xOffset <= m_ChunksVisibleInViewDst; xOffset++) {
				glm::vec2 viewedChunkCoord = glm::vec2(currentChunkCoordX + xOffset, currentChunkCoordY + yOffset);

				if (m_Terrain->HasChunk(viewedChunkCoord))
				{
					m_Terrain->GetChunk(viewedChunkCoord)->UpdateTerrainChunk(ViewPosition, MaxViewdist);
					if(m_Terrain->GetChunk(viewedChunkCoord)->GetIsVisible())
					{
						m_VisibleChunksLastFrame.push_back(*m_Terrain->GetChunk(viewedChunkCoord));
					}
				}
				else
				{
					m_Terrain->AddChunk(viewedChunkCoord, m_ChunkSize);
				}
			}
		}
	}
}
