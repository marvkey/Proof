#pragma once
#include "../../ProofPhysics/src/PhysicsWorld.h"
#include "../World.h"
#include "NVDIAPhysX.h"
#include "../Component.h"
namespace Proof {
	enum class PhysicsEngineType {
		ProofPhysics,
		NvdiaPhysics,
		BulletPhysics
	};
	class PhysicsEngine {
	public:
		PhysicsEngine(World* world, PhysicsEngineType type = PhysicsEngineType::NvdiaPhysics) :
			m_World(world), m_PhysicsType(type) {

		}
		~PhysicsEngine() {
			End();
		}
		void Start();
		void Simulate(float delta);
		void End();
		PhysicsEngineType GetPhysicsType() { return m_PhysicsType; }
	private:
		PhysicsEngineType m_PhysicsType;
		ProofPhysicsEngine::PhysicsWorld* m_ProofPhysicsEngine = nullptr;
		NVDIAPhysXEngine* m_NVDIAPhysicsEngine = nullptr;
		World* m_World = nullptr;
		friend class World;

		void StartNvdiaPhysx();
		void UpdateNvdiaPhysx(float delta);
		void EndNvdiaPhysx();

		void StartBulletPhysics();
		void UpdateBulletPhysics(float delta);
		void EndBulletPhysics();

		void StartProofPhysics();
		void UpdateProofPhysics(float delta);
		void EndProofPhysics();
	};
}