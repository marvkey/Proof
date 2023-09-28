#include "Proofprch.h"
#include "ParticleSystem.h"
#include "Proof/Math/MathInclude.h"

namespace Proof {
	
	void ParticleHandler::Play()
	{
		m_State = ParticleHandlerState::Play;
		m_PoolIndex = m_ParticleSystem->MaxParticles - 1;
		m_CurrentNumParticles = 0;
	}

	void ParticleHandler::Pause()
	{
		if(m_State == ParticleHandlerState::Play)
			m_State = ParticleHandlerState::Pause;
	}

	void ParticleHandler::End()
	{
		m_State = ParticleHandlerState::End;
		Reset();
	}
	ParticleHandler::ParticleHandler(Count<ParticleSystem> particleSystem)
	{
		m_ParticleSystem = particleSystem;
		Reset();
		if (m_ParticleSystem->PlayOnAwake)
			Play();

	}
	void ParticleHandler::Update(float ts, Vector position)
	{
		if (m_State != ParticleHandlerState::Play)
			return;
		m_CurrentPosition = position;
		if(m_PoolIndex != 0)
			UpdateEmission(ts);
		
		//(update)
		m_CurrentNumParticles = 0;
		for (auto& particle : m_ParticlePool)
		{
			if (particle.Active == false)continue;
			if (particle.Life <= 0.0f)
			{
				particle.Active = false;
				continue;
			}
			UpdateSize(particle, ts);
			UpdateColor(particle, ts);
			particle.Life -= ts;
			particle.Position += ProofToglmVec(particle.Velocity) * ts;
			if (!m_ParticleSystem->Use3D)
				particle.Rotation += 0.01 * ts;
			else
				particle.Rotation += 0.01 * ts;

			//else
			//	particle.Rotation3D += Vect0.01 * ts;
			m_CurrentNumParticles++;
		}
		if (m_PoolIndex == 0 && m_ParticleSystem->Loop)
			m_PoolIndex = m_ParticlePool.size() - 1;

		if (m_PoolIndex == 0 && m_ParticleSystem->Emision.SpawnRateDistance == 0 && m_CurrentNumParticles ==0)
		{
			// no need to clear particle is done
			m_State = ParticleHandlerState::End;
		}
		m_LastPosition = position;
	}

	void ParticleHandler::Reset()
	{
		m_ParticlePool.clear();
		m_ParticlePool.resize(m_ParticleSystem->MaxParticles);
		m_PoolIndex = m_ParticleSystem->MaxParticles-1;
		m_CurrentNumParticles = 0;
	}


	void ParticleHandler::UpdateEmission(float ts)
	{
		auto& emmission = m_ParticleSystem->Emision;
		if (emmission.Enabled == false)return;
		// has to be larger
		if (m_CurrentPosition.GetDistance(m_LastPosition) < emmission.SpawnRateDistance)
			return;

		uint32_t spawn = emmission.ParticleOverTime;
		//if (m_CurrentNumParticles + spawn >= m_ParticleSystem->MaxParticles)
		//{
		//	spawn = m_ParticleSystem->MaxParticles - m_CurrentNumParticles;
		//}
		
		for (int spawnAmount = 0; spawnAmount < spawn; spawnAmount++)
		{
			Particle& particle = m_ParticlePool[m_PoolIndex];
			particle.Active = true;
			particle.Position = ProofToglmVec( m_CurrentPosition);
			particle.Rotation = Random::Real<float>(0,2) * Math::PIE();
			particle.Rotation3D = m_ParticleSystem->Rotation3D;
			// Velocity
			particle.Velocity =m_ParticleSystem->Velocity;
			particle.Velocity.X += m_ParticleSystem->VelocityVariation.X * Random::Real<float>(-0.5,0.5);
			particle.Velocity.Y += m_ParticleSystem->VelocityVariation.Y * Random::Real<float>(-0.5, 0.5);
			particle.Velocity.Z += m_ParticleSystem->VelocityVariation.Z * Random::Real<float>(-0.5, 0.5);

			// Color
			particle.Color = m_ParticleSystem->ColorBegin;

			// Size
			particle.Size = m_ParticleSystem->SizeBegin + m_ParticleSystem->SizeVariation * Random::Real<float>(-0.5, 0.5);
			particle.Size3D = m_ParticleSystem->SizeBegin3D + m_ParticleSystem->SizeVariation3D * Random::Real<float>(-0.5, 0.5);

			// Life
			particle.Life = m_ParticleSystem->LifeTime;

			m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
			if (m_PoolIndex == 0)
			{
				break;
			}
		}
	}

	void ParticleHandler::UpdateSize(Particle& particle, float ts)
	{
		float life = particle.Life / m_ParticleSystem->LifeTime;

		if (m_ParticleSystem->Use3D)
		{
			particle.Size3D = Math::Lerp(m_ParticleSystem->SizeEnd3D, m_ParticleSystem->SizeBegin3D, life);
			return;
		}
		particle.Size = Math::Lerp(m_ParticleSystem->SizeEnd, m_ParticleSystem->SizeBegin , life);
	}
	void ParticleHandler::UpdateColor(Particle& particle,float ts)
	{
		float life = particle.Life/ m_ParticleSystem->LifeTime ;
		particle.Color = Math::Lerp(m_ParticleSystem->ColorEnd, m_ParticleSystem->ColorBegin, life);
		//particle.Color.a = particle.Color.a*life;
	}
}