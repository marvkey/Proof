#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include "Water.h"
namespace Proof
{
	

	enum class WaveType
	{
		GerstnerWave = 1,// better for small bodies of water
		FastFourierTransformWave, // use for large bodies of water
		RealisticFastFourierTransformWave 
	};


	class Wave : public RefCounted
	{
	public:
		Wave(Water* water, WaveType type);
		virtual void Update(float deltaTime) {};
		virtual void Render(Count<class WorldRenderer> renderer) {};
		virtual void Render2D(Count<class Renderer2D> renderer2D) {};

		WaveType GetWaveType() { return m_WaveType; };
		Count<class World> GetWorld();
		Count<class Water> GetWater()const;
		glm::mat4 GetTransform()const;
		glm::mat4 GetTransform();
	protected:
		Water* m_Water; // so it does not keep teh water alive
		WaveType m_WaveType;
	};
}