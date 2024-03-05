#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsSimulationCallback.h"
#include <Physx/PxPhysicsAPI.h>
#include "PhysicsTypes.h"
// NOTE: Allocates roughly 800 bytes once, and then reuses
#define OVERLAP_MAX_COLLIDERS 50
namespace Proof 
{
	struct RaycastHit
	{
		uint64_t HitEntity = 0;
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f);
		float Distance = 0.0f;
		Count<class ColliderShape> HitCollider = nullptr;
	};

	struct OverlapHit
	{
		Count<class PhysicsActorBase> Actor;
		Count<class ColliderShape> Shape;
	};

	class PhysicsWorld : public RefCounted
	{
	public:
		// begin world and end world is already called
		PhysicsWorld(Count<class World> world);
		virtual ~PhysicsWorld();
		void Simulate(float deltaTime);

		void StartWorld();
		void EndWorld();
		Count<World> GetWorld() 
		{
			return m_World;
		}

		physx::PxScene* GetPhysicsScene() 
		{
			return m_PhysXScene;
		}
		bool HasActor(class Entity entity);
		Count<class PhysicsActor> CreateActor(class Entity entity);
		Count<class PhysicsActor> GetActor(class Entity entity);
		void RemoveActor(class Entity entity);

		bool HasController(Entity entity);
		Count<class PhysicsController> CreateController(Entity entity);
		Count<class PhysicsController> GetController(Entity entity);
		void RemoveController(Entity entity);
		const std::unordered_map<UUID, Count<PhysicsActor>>& GetActors() const { return m_Actors; }

		physx::PxControllerManager* GetPhysXControllerManager() const { return m_PhysXControllerManager; }
		const physx::PxSimulationStatistics& GetSimulationStats() const { return m_SimulationStats; }

		bool OverlapSphere(const glm::vec3& origin, float radius, std::array<OverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count);
		bool OverlapBox(const glm::vec3& origin, const glm::vec3& halfSize, std::array<OverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count);

		void AddRadialImpulse(const glm::vec3& origin, float radius, float strength, enum class EFalloffMode falloff = EFalloffMode::Constant, bool velocityChange = false);

		bool Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit* outHit);
		bool SphereCast(const glm::vec3& origin, const glm::vec3& direction, float radius, float maxDistance, RaycastHit* outHit);
		//bool RaycastExcludeEntities(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit* outHit, const std::unordered_set<UUID>& excludedEntities);
	private:

		bool Advance(float deltaTime);
		void SubStepStrategy(float deltaTime);

		void CreateRegions();

		bool OverlapGeometry(const glm::vec3& origin, const physx::PxGeometry& geometry, std::array<OverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count);
	
	private:
		physx::PxSimulationStatistics m_SimulationStats;

		float m_Accumulator = 0.0f;
		// needs to be changed have a way to figure this out
		float m_SubStepSize = 1.0f / 60.0f;
		uint32_t m_NumSubSteps = 0;
		const uint32_t c_MaxSubSteps = 8;

		class physx::PxScene* m_PhysXScene = nullptr;
		class PhysicsSimulationCallback m_CollisionCallback;
		// inserting an element with this technique makes 
		// creates a copy nd uses desctrocture
		// so we would use Count so it is created once instead of twice
		// performance saved
		std::unordered_map<UUID, Count<class PhysicsActor>> m_Actors;
		std::unordered_map<UUID,Count<class PhysicsController>> m_Controllers;
		Count<class World> m_World;
		physx::PxBounds3* m_RegionBounds = nullptr;
		physx::PxControllerManager* m_PhysXControllerManager;

		std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS> m_OverlapBuffer;

	};
}
