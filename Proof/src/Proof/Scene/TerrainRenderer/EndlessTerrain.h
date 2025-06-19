#pragma once
#include "Proof/Core/Core.h"
#include "../SceneUtils.h"
namespace Proof
{
	class EndlessTerrain : RefCounted
	{
	public:
		EndlessTerrain(Count<class TerrainRenderer> terrain);

		void OnUpdate(float deltaTime);
		const float MaxViewdist = 450;
		Transform Viewer;
	private:
		void UpdateVisibleChunks();
	private:
		glm::vec2 m_ViewerPostion;
		Count<class TerrainRenderer> m_Terrain;
		uint32_t m_ChunksVisibleInViewDst;
		uint32_t m_ChunkSize;

		std::vector<struct TerrainChunk> m_VisibleChunksLastFrame;
	};
}