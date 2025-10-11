#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include "Proof/Scene/Entity.h"

namespace Proof
{
	class BuoyancyActor : public RefCounted
	{
	public:
		BuoyancyActor(Entity entity);
		virtual ~BuoyancyActor();

		void OnUpdate(float deltaTime);
		void OnPhysicsUpdate(float deltaTime);
		void SetEntity(Entity entity);
		void CutIntoVoxels();
		glm::vec3 CalculateMaxBuoyancyForce(Count<class Water> water);

		const std::vector<std::pair<UUID, glm::vec3>>& GetVoxels() {return m_Voxels;}; // ID and pos
		glm::vec3 GetVoxelSize() { return m_VoxelSize; };

	private:
		std::vector<std::pair<UUID, glm::vec3>> m_Voxels; // ID and pos
		glm::vec3 m_VoxelSize;
		Entity m_Entity;
		Count<World> m_World = nullptr;
		Count< class ColliderShape> m_ColliderShape;
	private:

		Count<class PhysicsActor> m_PhysicsActor;
		float m_InitialDrag;
		float m_InitialAngularDrag;
		void FreeVoxels();
		void ApplyBuoyancy(float deltaTime, Count<class FFTWave> wave,Entity waterEntity);
	};
}