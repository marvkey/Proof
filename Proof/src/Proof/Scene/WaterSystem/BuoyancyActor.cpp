#include "Proofprch.h"
#include "BuoyancyActor.h"
#include "Proof/Scene/Component.h"
#include "proof/Physics/PhysicsActor.h"
#include "proof/Physics/PhysicsWorld.h"
#include "proof/Scene/World.h"
#include "proof/Scene/SceneUtils.h"
#include "Proof/Scene/WaterSystem/FFTWave/FFTWave.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include <glm/gtx/quaternion.hpp>
//https://github.com/dbrizov/NaughtyWaterBuoyancy/blob/2e526032574c1cb963d680c3d813ac8012e8cfa0/Assets/NaughtyWaterBuoyancy/Scripts/Core/FloatingObject.cs
namespace Proof
{
	BuoyancyActor::BuoyancyActor(Entity entity)
		:m_Entity(entity)
	{
		if (!m_Entity.HasComponent< BuoyancyComponent>())
		{
			PF_EC_ERROR("Buoyancy Actor {} needs a BuoyancyComponent", m_Entity.GetName());
			PF_CORE_ASSERT(false);
			return;
		}
		if (!m_Entity.HasComponent<RigidBodyComponent>())
		{
			PF_EC_ERROR("Buoyancy Actor {} needs a rigid body component", m_Entity.GetName());
			return;
		}
		m_World = m_Entity.GetCurrentWorld();
		auto phyiscsActor = m_World->GetPhysicsWorld()->GetActor(m_Entity);


		if (m_Entity.HasComponent<BoxColliderComponent>())
			m_ColliderShape = phyiscsActor->GetCollider<BoxColliderShape>();
		if (m_Entity.HasComponent<SphereColliderComponent>())
			m_ColliderShape = phyiscsActor->GetCollider<SphereColliderShape>();

		if (m_ColliderShape == nullptr)
		{
			PF_EC_ERROR("Buoyancy Actor {} only supports box or sphere colliders, a Collider must be provided", m_Entity.GetName());
		}

		

		m_PhysicsActor = phyiscsActor;
		CutIntoVoxels();
		m_InitialDrag = m_PhysicsActor->GetLinearDrag();
		m_InitialAngularDrag = m_PhysicsActor->GetAngularDrag();
	}
	BuoyancyActor::~BuoyancyActor()
	{
		FreeVoxels();
	}
	void BuoyancyActor::OnUpdate(float deltaTime)
	{
		
	}
	void BuoyancyActor::OnPhysicsUpdate(float deltaTime)
	{

		if (m_Voxels.empty())
			return;

		PF_PROFILE_FUNC();

		{
			PF_PROFILE_SCOPE_DYNAMIC("Set Wave QueryID")
			const glm::mat4 transform = m_PhysicsActor->GetTransform();

			m_Entity.GetCurrentWorld()->ForEachEnitityWith<WaterComponent>([&](Entity e)
				{
					auto wave = e.GetComponent<WaterComponent>().Water->GetWave();
					if (wave->GetWaveType() != WaveType::FastFourierTransformWave)
						return;

					Count<FFTWave> fftWave = wave.As<FFTWave>();
					for (auto& [id, pos] : m_Voxels)
					{
						if (!fftWave->HasWaveHeightQueryID(id))
							fftWave->PushWaveHeightQueryID(id);

						fftWave->UpdateWaveHeightQueryID(id, Utils::TransformPoint(transform, pos));
					}

					ApplyBuoyancy(deltaTime, fftWave, e);
				});
		}

	}
	void BuoyancyActor::CutIntoVoxels()
	{
		if (m_ColliderShape == nullptr)
			return;

		BuoyancyComponent& buoyancyComponent = m_Entity.GetComponent< BuoyancyComponent>();
		glm::quat initialRotation = m_PhysicsActor->GetRotation();

		m_PhysicsActor->SetRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

		glm::mat4 coliderTransform = m_ColliderShape->GetInitalShapeWorldTransform() ;

		AABB bounds = m_ColliderShape->GetBoundingBox();
		m_VoxelSize.x = bounds.GetSize().x * buoyancyComponent.VoxelRelativeSize;
		m_VoxelSize.z = bounds.GetSize().y * buoyancyComponent.VoxelRelativeSize;
		m_VoxelSize.y = bounds.GetSize().z * buoyancyComponent.VoxelRelativeSize;

		int voxelCountPerAxis = static_cast<int>(std::round(1.0f / buoyancyComponent.VoxelRelativeSize));

		FreeVoxels();
		m_Voxels.reserve(voxelCountPerAxis * voxelCountPerAxis * voxelCountPerAxis);

		for (int i = 0; i < voxelCountPerAxis; ++i) 
		{
			for (int j = 0; j < voxelCountPerAxis; ++j)
			{
				for (int k = 0; k < voxelCountPerAxis; ++k)
				{
					float pX = bounds.Min.x + m_VoxelSize.x * (0.5f + i);
					float pY = bounds.Min.y + m_VoxelSize.y * (0.5f + j);
					float pZ = bounds.Min.z + m_VoxelSize.z * (0.5f + k);

					glm::vec3 point = { pX,pY,pZ };

					//if (m_ColliderShape->UseRayIsPointInsideCollider(point, glm::length(bounds.GetSize())))
					if (m_ColliderShape->IsPointInsideCollider(point))
					{
						auto data = std::make_pair( UUID(),Utils::WorldToLocal(point,coliderTransform) );
						m_Voxels.emplace_back(data);
					}
				}
			}
		}

		m_PhysicsActor->SetRotation(initialRotation);
	}

	void BuoyancyActor::FreeVoxels()
	{
		if (m_Voxels.empty())
			return;


		m_Entity.GetCurrentWorld()->ForEachEnitityWith<WaterComponent>([&](Entity e)
			{
				auto wave = e.GetComponent<WaterComponent>().Water->GetWave();
				if (wave->GetWaveType() != WaveType::FastFourierTransformWave)
					return;

				Count<FFTWave> fftWave = wave.As<FFTWave>();
				for (auto& [id, box] : m_Voxels)
				{
					if (fftWave->HasWaveHeightQueryID(id))
						fftWave->RemoveWaveHeightQueryID(id);
				}

			});

		m_Voxels.clear();
	}

	glm::vec3 BuoyancyActor::CalculateMaxBuoyancyForce(Count<class Water> water)
	{
		float mass = m_Entity.GetCurrentWorld()->GetPhysicsWorld()->GetActor(m_Entity).As<PhysicsActor>()->GetMass();
		BuoyancyComponent& buoyancyComponent = m_Entity.GetComponent< BuoyancyComponent>();

		float volume = mass / buoyancyComponent.Density;

		return water->Density * volume * -m_Entity.GetCurrentWorld()->GetPhysicsWorld()->GetGravity();
	}

	void BuoyancyActor::ApplyBuoyancy(float deltaTime, Count<class FFTWave> wave, Entity waterEntity)
	{
		PF_PROFILE_FUNC();

		glm::vec3 forceSingleVoxel = CalculateMaxBuoyancyForce(wave->GetWater()) / (float)m_Voxels.size();

		AABB bounds = m_ColliderShape->GetBoundingBox();
		const glm::mat4 transform = m_PhysicsActor->GetTransform();

		BuoyancyComponent& buoyancyComponent = m_Entity.GetComponent< BuoyancyComponent>();
		float voxelHeight = bounds.GetSize().y * buoyancyComponent.VoxelRelativeSize;

		//PF_EC_INFO("Voxel height {}", voxelHeight);
	//	PF_EC_INFO("	forceSingleVoxel {}:{}:{}", forceSingleVoxel.x, forceSingleVoxel.y, forceSingleVoxel.z);

		float submergedVolume = 0.0f;

		Count<PhysicsActor> physicsActor = m_Entity.GetCurrentWorld()->GetPhysicsWorld()->GetActor(m_Entity);
		for (int i = 0; i < m_Voxels.size(); i++)
		{
			if (!wave->IsWaveheightQueryReady(m_Voxels[i].first))
			{
				PF_ENGINE_WARN("Voxel query is not ready Entity: {} Voxel: {}",m_Entity.GetName(),m_Voxels[i].first);
				continue;
			}

			glm::vec3 worldPoint = Utils::LocalToWorld(m_Voxels[i].second,transform);

			float waveHeight = wave->GetWaveheight(m_Voxels[i].first);
			float deepLevel = waveHeight - worldPoint.y + (voxelHeight / 2.0f); // how deep is the voxel


			float submergedFactor = glm::clamp(deepLevel / voxelHeight, 0.0f, 1.0f); // 0 voxel fully out water, 1 voxel fully submerged

			submergedVolume += submergedFactor;


			glm::vec3 surfaceNormal = glm::vec3(0, 1, 0); // TODO 

			glm::quat surfaceRotation = Utils::FromRotation(wave->GetWater()->GetWorld()->GetWorldSpaceTransformComponent(waterEntity).GetUpVector(), surfaceNormal);

			surfaceRotation = glm::slerp(surfaceRotation, glm::quat(1, 0, 0, 0), submergedFactor);


			glm::vec3 finalVoxelForce = surfaceRotation * (forceSingleVoxel * submergedFactor);
			//glm::vec3 finalVoxelForce = glm::vec3(0.0f, 1.0f, 0.0f) * glm::length(forceSingleVoxel) * submergedFactor;

			//PF_EC_INFO("	SubmergedFactor {}", submergedFactor);

		//	PF_EC_INFO("	finalVoxelVorce {}:{}:{}", finalVoxelForce.x, finalVoxelForce.y, finalVoxelForce.z);
		//	PF_EC_INFO("	worldPoint {}:{}:{}", worldPoint.x, worldPoint.y, worldPoint.z);

			physicsActor->AddForceAtPosition(finalVoxelForce, worldPoint);
			//physicsActor->AddForce(finalVoxelForce);
		}

		submergedVolume /= m_Voxels.size();

#if 1
		// Apply drag force instead of setting drag
		{
			// drag force
			glm::vec3 linearVel = physicsActor->GetLinearVelocity();
			glm::vec3 dragForce = -linearVel * buoyancyComponent.DragInWater * submergedVolume;
			physicsActor->AddForce(dragForce);

			// angular drag force
			glm::vec3 angularVel = physicsActor->GetAngularVelocity();
			glm::vec3 angularDragTorque = -angularVel * buoyancyComponent.AngularDragInWater * submergedVolume;
			physicsActor->AddTorque(angularDragTorque);
		}

#else
		physicsActor->SetLinearDrag(Math::Lerp(m_InitialDrag, buoyancyComponent.DragInWater, submergedVolume));
		m_PhysicsActor->SetAngularDrag(Math::Lerp(m_InitialAngularDrag, buoyancyComponent.AngularDragInWater, submergedVolume));

#endif

	}
}

