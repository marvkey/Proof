#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include "Proof/Utils/MultiUse.h"

namespace Proof
{

	enum class WaveType;
	class Water : public RefCounted
	{
	public:
		Water(WaveType waveType);
		Water();
		Water(Count<Water> other);
		virtual ~Water();
		void Update(float deltaTime, const glm::mat4& transform);;
		void Render(Count<class WorldRenderer> renderer);
		void Render2D(Count<class Renderer2D> renderer2D);
		void OnPhysicsUpdate(float deltaTime);

		void SetWave(WaveType waveType);
		void CopyWave(Count<class Wave> wave);
		WaveType GetWaveType();
		Count<class Wave> GetWave();

		glm::mat4 GetTransform()const { return m_Transform; };
		glm::mat4 GetTransform(){ return m_Transform; };

		void SetWorld(Count<class World> world);
		Count<World> GetWorld();

	public:
		ClampedValue<float, 0.001f, 1000000.f> Density = 1.0f;
	private:
		Count<class World> m_World;
		Count<class Wave> m_Wave;
		glm::mat4 m_Transform = glm::mat4(1.0f);
		friend class World;
	};
}