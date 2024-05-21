#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include <array>
namespace Proof
{
	class Texture2D;
	class RenderMaterial;
	class Material;
	class MaterialTable;

	class WaterSystem : RefCounted
	{
	public:
		WaterSystem();
		WaterSystem(Count<WaterSystem> other);
		void Update(float deltaTime);
		void Render(Count<class WorldRenderer> renderer,const glm::mat4& transform);
		Count<Texture2D> Texture; //default white texture

		struct alignas(16) WaterDataInfo
		{
			glm::vec4 Color{ 0.0f,0.0f,0.5f,1.0f };

			float Speed = 9.8f;
			uint32_t WaveCount = 5; 
			float WaveSpread = 0.5f; // 0-1
			float WaveDistribution = 0.5f; //0-1

			glm::vec2 WaveDirection = { 1,0 };
			glm::vec2 MinMaxWavelength = { 5,10 };

			glm::vec2 MinMaxSteepness = { 0.4,0.5 }; //0-1
		} WaterData;


	private:
		void BaseInit();
		Count<RenderMaterial> m_RenderMaterial;
		Count<class UniformBufferSet> m_UBWaterDataSet;

		Count<class Mesh> m_Plane;
	};
}