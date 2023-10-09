#include "Proofprch.h"
#include "PhysicsShapes.h"
#include "Proof/Asset/AssetManager.h"
#include "PhysicsMaterial.h"
#include "PhysicsActor.h"
namespace Proof {
	ColliderShape::ColliderShape(ColliderType type, Entity entity, bool isShared)
		: m_Type(type), m_IsShared(isShared) ,m_Entity(entity)

	{
		if (m_Material == nullptr)
			m_Material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();
	}
	void ColliderShape::Release()
	{
		if (m_Material)
			m_Material->Release();
	}
	void ColliderShape::SetMaterial(Count<class PhysicsMaterial> material)
	{
		Release();
		m_Material = material;
	}
	BoxColliderShape::BoxColliderShape(const BoxColliderComponent& component, const PhysicsActor& actor, Entity entity)
		: ColliderShape(ColliderType::Box, entity)
	{
		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);

		glm::vec3 colliderSize = glm::abs(worldTransform.Scale * component.Size);
		physx::PxBoxGeometry geometry = physx::PxBoxGeometry(colliderSize.x, colliderSize.y, colliderSize.z);
		
		physx::PxMaterial* physxMaterial = (physx::PxMaterial*)m_Material->GetPhysicsBody();

		m_Shape = physx::PxRigidActorExt::createExclusiveShape(actor.GetPhysXActor(), geometry, *physxMaterial);

		//m_Shape->setSimulationFilterData(actor.GetFilterData());
		m_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !component.IsTrigger);
		m_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, component.IsTrigger);
		m_Shape->setLocalPose(PhysXUtils::ToPhysXTransform(component.Center, glm::vec3(0.0f)));
		m_Shape->userData = this;
	}
	BoxColliderShape::~BoxColliderShape()
	{
	}
	const glm::vec3& BoxColliderShape::GetSize()
	{
		return m_Entity.GetComponent<BoxColliderComponent>().Size;
	}
	void BoxColliderShape::SetSize(const glm::vec3& size)
	{
		Count<PhysicsActor> actor = (PhysicsActor*)m_Shape->getActor()->userData;
		Entity entity = actor->GetEntity();

		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);

		glm::vec3 colliderSize = worldTransform.Scale * size; // should be negative
		//colliderSize /= 2;

		physx::PxBoxGeometry geometry = physx::PxBoxGeometry(colliderSize.x, colliderSize.y, colliderSize.z);
		m_Shape->setGeometry(geometry);

		auto& component = m_Entity.GetComponent<BoxColliderComponent>();
		component.Size = size;
	}
	const glm::vec3& BoxColliderShape::GetCenter() const
	{
		return m_Entity.GetComponent<BoxColliderComponent>().Center;
	}
	void BoxColliderShape::SetCenter(const glm::vec3& center)
	{
		auto& component = m_Entity.GetComponent<BoxColliderComponent>();
		m_Shape->setLocalPose(PhysXUtils::ToPhysXTransform(center, glm::vec3(0.0f)));
		component.Center = center;
	}
	bool BoxColliderShape::IsTrigger() const
	{
		return m_Entity.GetComponent<BoxColliderComponent>().IsTrigger;
	}
	void BoxColliderShape::SetTrigger(bool isTrigger)
	{
		m_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
		m_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
		auto& component = m_Entity.GetComponent<BoxColliderComponent>();
		component.IsTrigger = isTrigger;
	}
	void BoxColliderShape::SetFilterData(const physx::PxFilterData& filterData)
	{
		m_Shape->setSimulationFilterData(filterData);
	}
	void BoxColliderShape::DetachFromActor(physx::PxRigidActor* actor)
	{
		PF_CORE_ASSERT(actor);
		PF_CORE_ASSERT(m_Shape);
		actor->detachShape(*m_Shape);
	}
	SphereColliderShape::SphereColliderShape(const SphereColliderComponent& component, const PhysicsActor& actor, Entity entity)
		: ColliderShape(ColliderType::Sphere, entity)
	{
		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);

		worldTransform.Scale = glm::abs(worldTransform.Scale);

		physx::PxMaterial* physxMaterial = (physx::PxMaterial*)m_Material->GetPhysicsBody();

		float largestComponent = glm::max(worldTransform.Scale.x, glm::max(worldTransform.Scale.y, worldTransform.Scale.z));
		physx::PxSphereGeometry geometry = physx::PxSphereGeometry(largestComponent * component.Radius);

		m_Shape = physx::PxRigidActorExt::createExclusiveShape(actor.GetPhysXActor(), geometry, *physxMaterial);
		//m_Shape->setSimulationFilterData(actor.GetFilterData());
		m_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !component.IsTrigger);
		m_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, component.IsTrigger);
		m_Shape->setLocalPose(PhysXUtils::ToPhysXTransform(component.Center, glm::vec3(0.0f)));
		m_Shape->userData = this;
	}
	SphereColliderShape::~SphereColliderShape()
	{
	}
	void SphereColliderShape::SetRadius(float radius)
	{
		Count<PhysicsActor> actor = (PhysicsActor*)m_Shape->getActor()->userData;
		Entity entity = actor->GetEntity();

		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);
		worldTransform.Scale = glm::abs(worldTransform.Scale);

		float largestComponent = glm::max(worldTransform.Scale.x, glm::max(worldTransform.Scale.y, worldTransform.Scale.z));
		physx::PxSphereGeometry geometry = physx::PxSphereGeometry(largestComponent * radius);
		m_Shape->setGeometry(geometry);

		auto& component = m_Entity.GetComponent<SphereColliderComponent>();
		component.Radius = radius;
	}
	void SphereColliderShape::SetCenter(const glm::vec3& center)
	{
		auto& component = m_Entity.GetComponent<SphereColliderComponent>();
		m_Shape->setLocalPose(PhysXUtils::ToPhysXTransform(center, glm::vec3(0.0f)));
		component.Center = center;
	}
	void SphereColliderShape::SetTrigger(bool isTrigger)
	{
		m_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
		m_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
		auto& component = m_Entity.GetComponent<SphereColliderComponent>();
		component.IsTrigger = isTrigger;
	}
	void SphereColliderShape::SetFilterData(const physx::PxFilterData& filterData)
	{
		m_Shape->setSimulationFilterData(filterData);
	}
	void SphereColliderShape::DetachFromActor(physx::PxRigidActor* actor)
	{
		PF_CORE_ASSERT(actor);
		PF_CORE_ASSERT(m_Shape);
		actor->detachShape(*m_Shape);
	}
	struct CapusleData
	{
		glm::vec3 offsetRotation;
		float radiusScale;
		float scaleDirection;
	};
	static CapusleData GetCapsuleData(CapsuleDirection direction, TransformComponent& worldTransform)
	{
		worldTransform.Scale = glm::abs(worldTransform.Scale);

		float radiusScale = 0;

		float scaleDirection;
		glm::vec3 offsetRotation;
		switch (direction)
		{
			case CapsuleDirection::X:
				{
					scaleDirection = worldTransform.Scale.x;
					offsetRotation = glm::vec3{ 0,0,0 };
					radiusScale = glm::max(worldTransform.Scale.y, worldTransform.Scale.z);
				}
				break;
			case CapsuleDirection::Y:
				{
					offsetRotation = glm::vec3{ 0,0,physx::PxHalfPi };
					scaleDirection = worldTransform.Scale.y;
					radiusScale = glm::max(worldTransform.Scale.x, worldTransform.Scale.z);
				}
				break;
			case CapsuleDirection::Z:
				{
					offsetRotation = glm::vec3{ 0,physx::PxHalfPi,0 };
					scaleDirection = worldTransform.Scale.z;
					radiusScale = glm::max(worldTransform.Scale.y, worldTransform.Scale.x);
				}
				break;
			default:
				break;
		}

		return { offsetRotation,radiusScale,scaleDirection };
	}
	CapsuleColliderShape::CapsuleColliderShape(const CapsuleColliderComponent& component, const PhysicsActor& actor, Entity entity)
		: ColliderShape(ColliderType::Capsule, entity)
	{
		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);

		physx::PxMaterial* physxMaterial = (physx::PxMaterial*)m_Material->GetPhysicsBody();

		auto capsuleData = GetCapsuleData(component.Direction, worldTransform);

		physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(component.Radius * capsuleData.radiusScale, (component.Height / 2.0f) * capsuleData.scaleDirection);
	//	m_Shape->setSimulationFilterData(actor.GetFilterData());
		m_Shape = physx::PxRigidActorExt::createExclusiveShape(actor.GetPhysXActor(), geometry, *physxMaterial);
		m_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !component.IsTrigger);
		m_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, component.IsTrigger);
		m_Shape->setLocalPose(PhysXUtils::ToPhysXTransform(component.Center, capsuleData.offsetRotation));
		m_Shape->userData = this;
	}

	CapsuleColliderShape::~CapsuleColliderShape()
	{
	}

	
	void CapsuleColliderShape::SetRadius(float radius)
	{

		Count<PhysicsActor> actor = (PhysicsActor*)m_Shape->getActor()->userData;
		Entity entity = actor->GetEntity();

		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);
		worldTransform.Scale = glm::abs(worldTransform.Scale);

		physx::PxCapsuleGeometry oldGeometry;
		m_Shape->getCapsuleGeometry(oldGeometry);

		auto capsuleData = GetCapsuleData(GetDirection(), worldTransform);

		physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(capsuleData.radiusScale * radius, oldGeometry.halfHeight);
		m_Shape->setGeometry(geometry);

		auto& component = m_Entity.GetComponent<CapsuleColliderComponent>();
		component.Radius = radius;
	}

	void CapsuleColliderShape::SetDirection(CapsuleDirection direction)
	{

		Count<PhysicsActor> actor = (PhysicsActor*)m_Shape->getActor()->userData;
		Entity entity = actor->GetEntity();

		World* world = entity.GetCurrentWorld();
		entity.GetComponent<CapsuleColliderComponent>().Direction = direction;

		SetHeight(GetHeight());
		SetRadius(GetRadius());
		SetCenter(GetCenter());
	}

	void CapsuleColliderShape::SetHeight(float height)
	{

		Count<PhysicsActor> actor = (PhysicsActor*)m_Shape->getActor()->userData;
		Entity entity = actor->GetEntity();

		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);

		physx::PxCapsuleGeometry oldGeometry;
		m_Shape->getCapsuleGeometry(oldGeometry);

		auto capsuleData = GetCapsuleData(GetDirection(), worldTransform);

		physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(oldGeometry.radius, (height / 2.0f) * capsuleData.scaleDirection);
		m_Shape->setGeometry(geometry);
		m_Shape->setGeometry(geometry);

		auto& component = m_Entity.GetComponent<CapsuleColliderComponent>();
		component.Height = height;
	}

	void CapsuleColliderShape::SetCenter(const glm::vec3& offset)
	{	

		Count<PhysicsActor> actor = (PhysicsActor*)m_Shape->getActor()->userData;
		Entity entity = actor->GetEntity();

		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);
		
		auto capsuleData = GetCapsuleData(GetDirection(), worldTransform);
		auto& component = m_Entity.GetComponent<CapsuleColliderComponent>();
		component.Center = offset;
		m_Shape->setLocalPose(PhysXUtils::ToPhysXTransform(offset, capsuleData.offsetRotation));
	}

	void CapsuleColliderShape::SetTrigger(bool isTrigger)
	{
		auto& component = m_Entity.GetComponent<CapsuleColliderComponent>();
		m_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
		m_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
		component.IsTrigger = isTrigger;
	}

	void CapsuleColliderShape::SetFilterData(const physx::PxFilterData& filterData)
	{
		m_Shape->setSimulationFilterData(filterData);
	}

	void CapsuleColliderShape::DetachFromActor(physx::PxRigidActor* actor)
	{
		PF_CORE_ASSERT(actor);
		PF_CORE_ASSERT(m_Shape);
		actor->detachShape(*m_Shape);
	}

}
