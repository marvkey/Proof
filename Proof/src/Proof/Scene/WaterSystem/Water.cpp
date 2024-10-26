#include "Proofprch.h"
#include "Water.h"
#include "Wave.h"
#include "GerstnerWave.h"

namespace Proof
{
	Water::Water(Proof::WaveType waveType)
	{
		SetWave(waveType);
	}
	Water::Water()
	{
		SetWave(WaveType::GerstnerWave);
	}
	Water::Water(Count<Water> other)
	{
		CopyWave(other->GetWave());
	}
	void Water::Update(float deltaTime, const glm::mat4& transform)
	{
		m_Transform = transform;
		m_Wave->Update(deltaTime);
	}
	void Water::Render(Count<class WorldRenderer> renderer)
	{
		m_Wave->Render(renderer);
	}
	void Water::Render2D(Count<class Renderer2D> renderer2D)
	{
		m_Wave->Render2D(renderer2D);
	}
	void Water::SetWave(WaveType waveType)
	{
		switch (waveType)
		{
			case Proof::WaveType::GerstnerWave:
				m_Wave = Count<GerstnerWave>::Create(this,waveType);
				break;
			case Proof::WaveType::FastFourierTransformWave:
				//m_Wave = Count<GerstnerWave>::Create();
				break;
			default:
				break;
		}
	}
	void Water::CopyWave(Count<Wave> wave)
	{
		switch (wave->GetWaveType())
		{
			case Proof::WaveType::GerstnerWave:
				m_Wave = Count<GerstnerWave>::Create(wave.As<GerstnerWave>());
				break;
			case Proof::WaveType::FastFourierTransformWave:
				//m_Wave = Count<GerstnerWave>::Create();
				break;
			default:
				break;
		}
	}
	WaveType Water::GetWaveType()
	{
		return m_Wave->GetWaveType();
	}
	Count<class Wave> Water::GetWave()
	{
		return m_Wave;
	}
	void Water::SetWorld(Count<World> world)
	{
		m_World = world;
	}
	Count<World> Water::GetWorld()
	{
		return m_World;
	}
}
