#pragma once
#include "Proof/Core/Core.h"
#include"Proof/Asset/Asset.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>
namespace Proof {

	struct ParticleEmission {
		bool Enabled = true;
		// how many particle is emmiteed
		uint32_t ParticleOverTime = 5;
		// how much the particle must move for a new spawn
		float SpawnRateDistance = 0;
	};
	class ParticleSystem : public Asset
	{
	public:
		ParticleSystem() {};
		ASSET_CLASS_TYPE(ParticleSystem);
		Vector Velocity{ 1 }, VelocityVariation{ 1 };
		glm::vec4 ColorBegin{ 1 }, ColorEnd{ 1 };
		bool Use3D = false;
		Vector SizeBegin3D, SizeEnd3D, SizeVariation3D;
		Vector Rotation3D;
		float SizeBegin=1, SizeEnd=1, SizeVariation=1;
		float LifeTime = 1.0f;
		uint32_t MaxParticles = 10;
		bool PlayOnAwake = false;
		bool Loop = false;
		Count<class Texture2D> Texture = nullptr;
		ParticleEmission Emision;

	};
    struct Particle {
		glm::vec3 Position;
		Vector Velocity;
		glm::vec4 Color = glm::vec4{ 1 };
		float Rotation;
		float Size;
		Vector Size3D{ 1 };
		Vector Rotation3D;
        float Life = 1.0f;
		bool Active = false;
    };
	enum class ParticleHandlerState {
		None,
		Play,
		Pause,
		End
	};
    class ParticleHandler : public RefCounted {
    public:
		ParticleHandler(Count<ParticleSystem> particleSystem);
        void Update(float ts, Vector position);
        const std::vector<Particle>& GetParticles()const {
            return m_ParticlePool;
        }
		void Play();
		void Pause();
		//ends partilce and resets the whole system
		void End();

		void Restart() 
		{
			End();
			Play();
		}
		//END() == Restart()
		//Play() == Restart()
		ParticleHandlerState GetState() {
			return m_State;
		}
		bool Visible = true;
		Count <ParticleSystem> GetParticleSystem() {
			return m_ParticleSystem;
		}
    private:
		ParticleHandlerState  m_State = ParticleHandlerState::None;
		Vector m_LastPosition;
		Vector m_CurrentPosition;
		uint32_t m_PoolIndex;
		std::vector<Particle> m_ParticlePool;
		Count<ParticleSystem> m_ParticleSystem;
		uint32_t m_CurrentNumParticles;
        void Reset();
		void UpdateEmission(float ts);
		void UpdateColor(Particle& particle,float ts);
		void UpdateSize(Particle& particle,float ts);
    };

	class ParticleHandlerTable : public RefCounted {
	public:
		ParticleHandlerTable() {
		}

		ParticleHandlerTable(const std::map<uint32_t, Count<ParticleHandler>>& particle) {
			m_Handlers = particle;
		}
		// panel can be nulltr
		// index cna be existing or non exisitng
		void SetHandler(uint32_t uiIndex, Count<ParticleHandler> particle) {
			m_Handlers[uiIndex] = particle;
		}

		void RemoveHandler(uint32_t panelIndex) {
			PF_CORE_ASSERT(HasHandler(panelIndex), "Does not conatin panel index");
			m_Handlers.erase(panelIndex);
		}

		Count<ParticleHandler> GetHandler(uint32_t panelIndex) {
			PF_CORE_ASSERT(HasHandler(panelIndex), "Does not conatin Panel index");
			return m_Handlers[panelIndex];
		}

		uint32_t GetHandlerSize() {
			return m_Handlers.size();
		}

		bool HasHandler(uint32_t panelIndex)const {
			return m_Handlers.contains(panelIndex);
		}

		const std::map<uint32_t, Count<ParticleHandler>>& GetHandlers()const {
			return m_Handlers;
		};

		std::map<uint32_t, Count<ParticleHandler>> Generate() {
			std::map<uint32_t, Count<ParticleHandler>> newPanel;
			for (auto& [id, panel] : m_Handlers)
			{
				newPanel[id] = Count<ParticleHandler>::CreateFrom(panel);
			}
			return newPanel;
		}
	private:
		// index, panels
		std::map<uint32_t, Count<ParticleHandler>> m_Handlers;
	};
}