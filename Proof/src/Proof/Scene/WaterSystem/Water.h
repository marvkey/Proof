#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>

namespace Proof
{
	enum class WaveType;
	class Water : RefCounted
	{
	public:
		Water(WaveType waveType);
		Water();
		Water(Count<Water> other);
		void Update(float deltaTime, const glm::mat4& transform);;
		void Render(Count<class WorldRenderer> renderer);
		void Render2D(Count<class Renderer2D> renderer2D);

		void SetWave(WaveType waveType);
		void CopyWave(Count<class Wave> wave);
		WaveType GetWaveType();
		Count<class Wave> GetWave();

		glm::mat4 GetTransform()const { return m_Transform; };
		glm::mat4 GetTransform(){ return m_Transform; };

		void SetWorld(Count<class World> world);
		Count<World> GetWorld();
	private:
		Count<class World> m_World;
		Count<class Wave> m_Wave;
		glm::mat4 m_Transform = glm::mat4(1.0f);
		friend class World;
	};
}