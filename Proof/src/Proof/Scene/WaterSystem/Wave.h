#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>

namespace Proof
{
	

	enum class WaveType
	{
		GerstnerWave = 1,// better for small bodies of water
		FastFourierTransformWave // use for large bodies of water
	};
	class Wave : RefCounted
	{
	public:
		Wave(Count<class Water> water, WaveType type)
			:
			m_Water(water), m_WaveType(type)
		{

		}
		virtual void Update(float deltaTime) {};
		virtual void Render(Count<class WorldRenderer> renderer) {};
		virtual void Render2D(Count<class Renderer2D> renderer2D) {};

		WaveType GetWaveType() { return m_WaveType; };
		Count<class World> GetWorld();
		Count<class Water> GetWater() { return m_Water; }
	protected:
		Count<class Water> m_Water;
		WaveType m_WaveType;
	};
}