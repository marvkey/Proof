#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
namespace Proof      
{
	class RenderCommandBuffer;
	class ComputePass;
	class StorageBuffer;
	
	struct alignas(16) SBBoids
	{
		glm::vec3 Position;

		glm::vec3 Direction;
		
		glm::vec3 FlockHeading;

		glm::vec3 FlockCenter;

		glm::vec3 AvoidanceHeading;
		int NumFlockmates;
	};


	class BoidManager : public RefCounted
	{
	public:
		BoidManager(Count<class PhysicsWorld> world);
		virtual  ~BoidManager();
		void OnUpdate(float deltaTime);
	private:
		Count<class PhysicsWorld> m_PhysicsWorld;
		Count<RenderCommandBuffer> m_CommandBuffer;
		Count<ComputePass> m_BoidPass;
		Count<class StorageBuffer> m_SBBoids;
		std::vector<SBBoids> m_Boids;
	};
}