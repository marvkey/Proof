#pragma once
#include "TerrainRenderer.h"
namespace Proof
{
	class NormalTerrain : public RefCounted
	{
	public:
		NormalTerrain(Count<class TerrainRenderer> terrain);

		void OnUpdate(float deltaTime);
		uint32_t Size = 241;

		void Regenirate() { GenerateChunks(Size, Size); };
	private:
		void GenerateChunks(uint32_t widht, uint32_t height);
	private:
		Count<class TerrainRenderer> m_Terrain;
	};
}