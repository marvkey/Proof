#include "Proofprch.h"
#include "Proof/Physics/Boids/BoidManager.h"
#include "Proof/Scene/Component.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Scene/World.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Scene/Entity.h"
#include "BoidFlock.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Renderer/ComputePass.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/UniformBuffer.h"
namespace Proof
{


	const int THREAD_GROUPS_SIZE = 1024;
	BoidManager::BoidManager(Count<class PhysicsWorld> world)
		:m_PhysicsWorld(world)
	{
		m_CommandBuffer = RenderCommandBuffer::Create("Boid Manager");
		m_BoidPass = ComputePass::Create("BoidCompute", Renderer::GetShader("BoidCompute"));
		m_Boids.resize(1024);
		m_SBBoids = StorageBuffer::Create(m_Boids.size() * sizeof(SBBoids));
		world->GetWorld()->ForEachEnitityWith<BoidFlockComponent>([&](Entity entity)
		{
			entity.GetComponent<BoidFlockComponent>().Flock->SetWorld(world->GetWorld());
		});
	}

	BoidManager::~BoidManager()
	{
		m_PhysicsWorld->GetWorld()->ForEachEnitityWith<BoidFlockComponent>([&](Entity entity)
		{
			entity.GetComponent<BoidFlockComponent>().Flock->SetWorld(nullptr);
			entity.GetComponent<BoidFlockComponent>().Flock->m_Boids.clear();
		});
	}

	void BoidManager::OnUpdate(float deltaTime)
	{
		PF_PROFILE_FUNC();

		auto world = m_PhysicsWorld->GetWorld();

		Renderer::BeginCommandBuffer(m_CommandBuffer);

		Renderer::BeginComputePass(m_CommandBuffer,m_BoidPass);
		world->ForEachEnitityWith<BoidFlockComponent>([&](Entity entity)
			{
				entity.GetComponent<BoidFlockComponent>().Flock->SetWorld(world);

				std::vector<UUID> listOrderAdded;
				auto& boidFlockcmpt = entity.GetComponent<BoidFlockComponent>();
				Count<BoidFlock> flock = boidFlockcmpt.Flock;

				{
					uint32_t index = 0;
					for (auto it = flock->m_Boids.begin(); it != flock->m_Boids.end(); )
					{
						const auto boidID = it->first;

						if (!world->HasEntity(boidID))
						{
							it = flock->m_Boids.erase(it);
							continue; // skip to next iteration (important)
						}

						auto& boid = it->second;
						{
							m_Boids[index].Position = boid->GetEntity().GetTransformComponent().Location;
							m_Boids[index].Direction = boid->m_Forward;
						}
						listOrderAdded.push_back(boidID);
						index++;
						++it;
					}
				}

				int numBoids = flock->m_Boids.size();
				if (numBoids == 0)
					return;
#if 0
				struct BoidPC
				{
					int numBoids;
					float viewRadius;
					float avoidRadius;
				}params;

				params.numBoids = numBoids;
				params.viewRadius = flock->GetSettings().PerceptionRadius;
				params.avoidRadius = flock->GetSettings().AvoidanceRadius;

				m_SBBoids->SetData(Buffer(m_Boids.data(), m_Boids.size() * sizeof(SBBoids)));
				m_BoidPass->PushData("u_Params", &params);
				m_BoidPass->SetInput("BoidBuffer", m_SBBoids);

				int threadGroups = glm::ceil(numBoids / (float)THREAD_GROUPS_SIZE);
				m_BoidPass->Dispatch({ threadGroups,1,1 });


				std::vector<SBBoids> boidData = m_SBBoids->GetDataAsVector<SBBoids>();
				if (boidData.empty())
					return;

			for (uint32_t i = 0; i < numBoids; i++)
			{
				flock->m_Boids[listOrderAdded[i]]->m_AvgFlockHeading = boidData[i].FlockHeading;
				flock->m_Boids[listOrderAdded[i]]->m_CentreOfFlockmates = boidData[i].FlockCenter;
				flock->m_Boids[listOrderAdded[i]]->m_AvgFlockHeading = boidData[i].AvoidanceHeading;
				flock->m_Boids[listOrderAdded[i]]->m_NumPerceivedFlockmates = boidData[i].NumFlockmates;

				flock->m_Boids[listOrderAdded[i]]->OnUpdate(deltaTime);
			}
#else
			float viewRadius = flock->GetSettings().PerceptionRadius;
			float avoidRadius = flock->GetSettings().AvoidanceRadius;

			for (int i = 0; i < numBoids; i++)
			{
				for (int indexB = 0; indexB < numBoids; indexB++)
				{
					if (indexB == i)
						continue;

					SBBoids boidB = m_Boids[indexB];
					glm::vec3 offset = boidB.Position - m_Boids[i].Position;
					float sqrDst = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;

					if (sqrDst < viewRadius * viewRadius)
					{
						m_Boids[i].NumFlockmates += 1;
						m_Boids[i].FlockHeading += boidB.Direction;
						m_Boids[i].FlockCenter += boidB.Position;

						if (sqrDst < avoidRadius * avoidRadius)
						{
							m_Boids[i].AvoidanceHeading -= offset / sqrDst;
						}
					}
				}
			}

			for (uint32_t i = 0; i < numBoids; i++)
			{
				flock->m_Boids[listOrderAdded[i]]->m_AvgFlockHeading = m_Boids[i].FlockHeading;
				flock->m_Boids[listOrderAdded[i]]->m_CentreOfFlockmates = m_Boids[i].FlockCenter;
				flock->m_Boids[listOrderAdded[i]]->m_AvgAvoidanceHeading = m_Boids[i].AvoidanceHeading;
				flock->m_Boids[listOrderAdded[i]]->m_NumPerceivedFlockmates = m_Boids[i].NumFlockmates;

				flock->m_Boids[listOrderAdded[i]]->OnUpdate(deltaTime);
			}
#endif
			
			m_Boids.clear();
			m_Boids.resize(1024);
			});

		Renderer::EndComputePass(m_BoidPass);
		
		Renderer::EndCommandBuffer(m_CommandBuffer);
		Renderer::SubmitCommandBuffer(m_CommandBuffer);
	}
}