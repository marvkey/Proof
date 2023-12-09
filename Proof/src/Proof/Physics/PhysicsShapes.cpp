#include "Proofprch.h"
#include "PhysicsShapes.h"
#include "Proof/Asset/AssetManager.h"
#include "PhysicsMaterial.h"
#include "PhysicsActor.h"
#include "MeshCollider.h"
#include "Proof/Math/MathResource.h"
#include "PhysicsMeshCache.h"
#include "PhysicsMeshCooker.h"
#include "PhysicsEngine.h"
#include "PhysicsWorld.h"
namespace Proof {
	ColliderShape::ColliderShape(ColliderType type, Entity entity, bool isShared)
		: m_Type(type), m_IsShared(isShared) ,m_Entity(entity)

	{
		if (m_Material == nullptr)
			m_Material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();
	}
	void ColliderShape::Release()
	{
	}
	void ColliderShape::SetMaterial(Count<class PhysicsMaterial> material)
	{
		Release();
		m_Material = material;

		auto [shapes, numShapes] = GetShapes();
		if (shapes == nullptr)
			return;
		for (physx::PxU32 i = 0; i < numShapes; ++i)
		{

			physx::PxMaterial* physxMat = &m_Material->GetPhysxMaterial();
			//overrides all materials
			shapes[i].setMaterials(&physxMat, 1); // Apply the material to the shape
		}
	}
	BoxColliderShape::BoxColliderShape(const BoxColliderComponent& component, const PhysicsActor& actor, Entity entity)
		: ColliderShape(ColliderType::Box, entity)
	{
		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);
		Count<PhysicsMaterial> material;
		if (component.HasPhysicsMaterial())
			material = component.GetPhysicsMaterial();

		if (!material)
			material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();

		SetMaterial(material);

		glm::vec3 halfSize = (component.Size / 2.f);
		glm::vec3 colliderSize = glm::abs(worldTransform.Scale * halfSize);

		physx::PxBoxGeometry geometry = physx::PxBoxGeometry(colliderSize.x, colliderSize.y, colliderSize.z);
		

		m_Shape = physx::PxRigidActorExt::createExclusiveShape(actor.GetPhysXActor(), geometry, m_Material->GetPhysxMaterial());

		m_Shape->setSimulationFilterData(actor.GetFilterData());
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

		glm::vec3 halfSize = (size / 2.f);

		glm::vec3 colliderSize = worldTransform.Scale * size; // should be negative

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
		Count<PhysicsMaterial> material;
		if (component.HasPhysicsMaterial())
			material = component.GetPhysicsMaterial();

		if (!material)
			material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();

		SetMaterial(material);

		worldTransform.Scale = glm::abs(worldTransform.Scale);

		float largestComponent = glm::max(worldTransform.Scale.x, glm::max(worldTransform.Scale.y, worldTransform.Scale.z));
		physx::PxSphereGeometry geometry = physx::PxSphereGeometry(largestComponent * component.Radius);

		m_Shape = physx::PxRigidActorExt::createExclusiveShape(actor.GetPhysXActor(), geometry, m_Material->GetPhysxMaterial());
		m_Shape->setSimulationFilterData(actor.GetFilterData());
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
	
	CapsuleColliderShape::CapsuleColliderShape(const CapsuleColliderComponent& component, const PhysicsActor& actor, Entity entity)
		: ColliderShape(ColliderType::Capsule, entity)
	{
		World* world = entity.GetCurrentWorld();
		TransformComponent worldTransform = world->GetWorldSpaceTransformComponent(entity);
		Count<PhysicsMaterial> material;
		if (component.HasPhysicsMaterial())
			material = component.GetPhysicsMaterial();

		if (!material)
			material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();

		SetMaterial(material);

		auto capsuleData = GetCapsuleData(component.Direction, worldTransform);

		//if (capsuleData.scaleDirection == 1.0f)
		//	capsuleData.scaleDirection /= 2;
		//if (capsuleData.radiusScale != 1)
		//	capsuleData.radiusScale /= 2;
		//else if(capsuleData.radiusScale == 1)
		//	capsuleData.scaleDirection -= 0.50;
		//else if (capsuleData.radiusScale / 2 > capsuleData.scaleDirection)
		//{
		//	float greaterValue = capsuleData.radiusScale / capsuleData.scaleDirection*2;
		//	capsuleData.scaleDirection -= greaterValue *1.5;
		//}
		//
		//if (capsuleData.radiusScale > 1)
		//{
		//
		//	capsuleData.radiusScale /= 4;
		//	capsuleData.radiusScale -= 1;
		//}
		

		physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(component.Radius * capsuleData.radiusScale, (component.Height/4) * worldTransform.Scale.y);
		m_Shape = physx::PxRigidActorExt::createExclusiveShape(actor.GetPhysXActor(), geometry, m_Material->GetPhysxMaterial());
		m_Shape->setSimulationFilterData(actor.GetFilterData());
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

	ConvexMeshShape::ConvexMeshShape(MeshColliderComponent& component, const PhysicsActor& actor, Entity entity)
		: ColliderShape(ColliderType::ConvexMesh, entity, component.UseSharedShape)
	{
		PF_PROFILE_FUNC();
		Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);
		PF_CORE_ASSERT(colliderAsset);

		SharedShapeManager::SharedShapeData* sharedData = SharedShapeManager::FindSuitableSharedShape(ColliderType::ConvexMesh, colliderAsset);
		if (component.UseSharedShape && sharedData != nullptr && sharedData->Shapes.find(component.SubMeshIndex) != sharedData->Shapes.end())
		{
			for (auto [submeshIndex, shape] : sharedData->Shapes)
			{
				actor.GetPhysXActor().attachShape(*shape);
				m_Shapes.push_back(shape);
			}
			m_BlockSetMaterial = true;
		}
		else
		{
			Count<PhysicsMaterial> material;
			if (AssetManager::HasAsset(colliderAsset->PhysicsMaterial))
			{
				material = AssetManager::GetAsset<PhysicsMaterial>(colliderAsset->PhysicsMaterial);
			}
			if(component.HasPhysicsMaterial())
			material = component.GetPhysicsMaterial();

			if (!material)
				material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();

			SetMaterial(material);

			const CachedColliderData& colliderData = PhysicsMeshCache::GetMeshData(colliderAsset);
			const auto& meshData = colliderData.SimpleColliderData;
			PF_CORE_ASSERT(meshData.SubMeshes.size() > component.SubMeshIndex);

			const SubMeshColliderData& submesh = meshData.SubMeshes[component.SubMeshIndex];
			glm::vec3 submeshTranslation;
			glm::quat submeshRotation;
			glm::vec3 submeshScale;
			MathResource::DecomposeTransform(submesh.Transform, submeshTranslation, submeshRotation, submeshScale);

			physx::PxDefaultMemoryInputData input(submesh.ColliderData.As<physx::PxU8>(), submesh.ColliderData.Size);
			physx::PxConvexMesh* convexMesh = PhysicsEngine::GetPhysics()->createConvexMesh(input);

			if (convexMesh)
			{
				auto scene = entity.GetCurrentWorld();
				TransformComponent worldTransform = scene->GetWorldSpaceTransformComponent(entity);

				physx::PxConvexMeshGeometry convexGeometry = physx::PxConvexMeshGeometry(convexMesh, physx::PxMeshScale(PhysXUtils::ToPhysXVector(submeshScale * worldTransform.Scale)));
				convexGeometry.meshFlags = physx::PxConvexMeshGeometryFlag::eTIGHT_BOUNDS;

				physx::PxShape* shape = PhysicsEngine::GetPhysics()->createShape(convexGeometry, m_Material->GetPhysxMaterial(), !component.UseSharedShape);
				shape->setSimulationFilterData(actor.GetFilterData());
				shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, colliderAsset->CollisionComplexity != ECollisionComplexity::UseComplexAsSimple && !component.IsTrigger);
				if(component.UseSharedShape == false)
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, component.IsTrigger);
				else
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);

				shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, colliderAsset->CollisionComplexity == ECollisionComplexity::UseSimpleAsComplex);
				shape->userData = this;

				if (component.UseSharedShape)
				{
					if (sharedData != nullptr)
					{
						// Found suitable shared data, but our submesh doesn't exist in it
						sharedData->Shapes[component.SubMeshIndex] = shape;
					}
					else
					{
						// Failed to find suitable shape data, create a new one
						sharedData = SharedShapeManager::CreateSharedShapeData(ColliderType::ConvexMesh, colliderAsset->ColliderMesh);
						sharedData->Usage = colliderAsset->CollisionComplexity;
						sharedData->Shapes[component.SubMeshIndex] = shape;
					}
				}

				actor.GetPhysXActor().attachShape(*shape);

				m_Shapes.push_back(shape);

				shape->release();
				convexMesh->release();
			}
			else
			{
				PF_ENGINE_ERROR("Physics Failed to create Convex shape!");
				//m_Material->();
			}
		}
	}

	ConvexMeshShape::~ConvexMeshShape()
	{
	}

	void ConvexMeshShape::SetTrigger(bool isTrigger)
	{
		auto& component = m_Entity.GetComponent<MeshColliderComponent>();
		if (component.UseSharedShape)
			return;

		Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);

		for (auto shape : m_Shapes)
		{
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, colliderAsset->CollisionComplexity != ECollisionComplexity::UseComplexAsSimple && !isTrigger);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
		}

		component.IsTrigger = isTrigger;
	}

	void ConvexMeshShape::SetFilterData(const physx::PxFilterData& filterData)
	{
		auto& component = m_Entity.GetComponent<MeshColliderComponent>();
		if (component.UseSharedShape)
			return;

		for (auto shape : m_Shapes)
			shape->setSimulationFilterData(filterData);
	}

	void ConvexMeshShape::DetachFromActor(physx::PxRigidActor* actor)
	{
		PF_CORE_ASSERT(actor);

		uint32_t referenceCount = m_Shapes[0]->getReferenceCount();

		for (auto shape : m_Shapes)
		{
			PF_CORE_ASSERT(shape);
			actor->detachShape(*shape);
		}

		const auto& component = m_Entity.GetComponent<MeshColliderComponent>();

		// Reference count was 1, meaning the shape has now been destroyed, so clear it from the map
		if (referenceCount == 1)
		{
			Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);
			SharedShapeManager::RemoveSharedShapeData(ColliderType::ConvexMesh, colliderAsset, true, component.SubMeshIndex);
		}

		m_Shapes.clear();
	}

	void ConvexMeshShape::SetMaterial(Count<class PhysicsMaterial> material)
	{
		if (!m_BlockSetMaterial)
			ColliderShape::SetMaterial(material);
	}

	TriangleMeshShape::TriangleMeshShape(MeshColliderComponent& component, const PhysicsActor& actor, Entity entity)
		: ColliderShape(ColliderType::TriangleMesh, entity, component.UseSharedShape)
	{
		PF_PROFILE_FUNC();

		Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);
		PF_CORE_ASSERT(colliderAsset);

		SharedShapeManager::SharedShapeData* sharedData = SharedShapeManager::FindSuitableSharedShape(ColliderType::TriangleMesh, colliderAsset);
		if (component.UseSharedShape && sharedData != nullptr)
		{
			for (auto [submeshIndex, shape] : sharedData->Shapes)
			{
				actor.GetPhysXActor().attachShape(*shape);
				m_Shapes.push_back(shape);
			}
			m_BlockSetMaterial = true;
		}
		else
		{
			Count<PhysicsMaterial> material;
			if (AssetManager::HasAsset(colliderAsset->PhysicsMaterial))
			{
				material = AssetManager::GetAsset<PhysicsMaterial>(colliderAsset->PhysicsMaterial);
			}
			if (component.HasPhysicsMaterial())
				material = component.GetPhysicsMaterial();

			if (!material)
				material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();

			SetMaterial(material);

			const CachedColliderData& colliderData = PhysicsMeshCache::GetMeshData(colliderAsset);
			const auto& meshData = colliderData.ComplexColliderData;

			auto scene = entity.GetCurrentWorld();
			TransformComponent worldTransform = scene->GetWorldSpaceTransformComponent(entity);

			for (size_t i = 0; i < meshData.SubMeshes.size(); i++)
			{
				const SubMeshColliderData& submeshData = meshData.SubMeshes[i];

				glm::vec3 submeshTranslation;
				glm::quat submeshRotation;
				glm::vec3 submeshScale;
				MathResource::DecomposeTransform(submeshData.Transform, submeshTranslation, submeshRotation, submeshScale);

				physx::PxDefaultMemoryInputData input(submeshData.ColliderData.As<physx::PxU8>(), submeshData.ColliderData.Size);
				physx::PxTriangleMesh* trimesh = PhysicsEngine::GetPhysics()->createTriangleMesh(input);

				if (trimesh)
				{
					physx::PxTriangleMeshGeometry triangleGeometry = physx::PxTriangleMeshGeometry(trimesh, physx::PxMeshScale(PhysXUtils::ToPhysXVector(submeshScale * worldTransform.Scale)));

					physx::PxShape* shape = PhysicsEngine::GetPhysics()->createShape(triangleGeometry, m_Material->GetPhysxMaterial(), !component.UseSharedShape);
					shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, colliderAsset->CollisionComplexity == ECollisionComplexity::UseComplexAsSimple);
					shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
					shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, colliderAsset->CollisionComplexity != ECollisionComplexity::UseSimpleAsComplex);
					shape->setSimulationFilterData(actor.GetFilterData());
					shape->setLocalPose(PhysXUtils::ToPhysXTransform(submeshTranslation, submeshRotation));
					shape->userData = this;

					if (component.UseSharedShape)
					{
						if (sharedData != nullptr)
						{
							sharedData->Shapes[(uint32_t)i] = shape;
						}
						else
						{
							sharedData = SharedShapeManager::CreateSharedShapeData(ColliderType::TriangleMesh, colliderAsset->ColliderMesh);
							sharedData->Usage = colliderAsset->CollisionComplexity;
							sharedData->Shapes[(uint32_t)i] = shape;
						}
					}

					actor.GetPhysXActor().attachShape(*shape);

					m_Shapes.push_back(shape);

					shape->release();
					trimesh->release();
				}
				else
				{
					PF_ENGINE_ERROR("Physics", "Failed to create Triangle shape!");
				}
			}
		}
	}
	TriangleMeshShape::~TriangleMeshShape()
	{
	}
	void TriangleMeshShape::SetTrigger(bool isTrigger)
	{
		// cannot be set as triggers
	}
	void TriangleMeshShape::SetFilterData(const physx::PxFilterData& filterData)
	{
		const auto& component = m_Entity.GetComponent<MeshColliderComponent>();
		if (component.UseSharedShape)
			return;

		for (auto shape : m_Shapes)
			shape->setSimulationFilterData(filterData);
	}
	void TriangleMeshShape::DetachFromActor(physx::PxRigidActor* actor)
	{
		PF_CORE_ASSERT(actor);

		uint32_t referenceCount = m_Shapes[0]->getReferenceCount();

		for (auto shape : m_Shapes)
		{
			PF_CORE_ASSERT(shape);
			actor->detachShape(*shape);
		}

		const auto& component = m_Entity.GetComponent<MeshColliderComponent>();

		// Reference count was 1, meaning the shape has now been destroyed, so clear it from the map
		if (referenceCount == 1)
		{
			Count<MeshCollider> colliderAsset = AssetManager::GetAsset<MeshCollider>(component.ColliderID);
			SharedShapeManager::RemoveSharedShapeData(ColliderType::TriangleMesh, colliderAsset);
		}

		m_Shapes.clear();
	}

	void TriangleMeshShape::SetMaterial(Count<class PhysicsMaterial> material)
	{
		if (!m_BlockSetMaterial)
			ColliderShape::SetMaterial(material);
	}

	SharedShapeManager::SharedShapeData* SharedShapeManager::CreateSharedShapeData(ColliderType colliderType, AssetID colliderHandle)
	{
		SharedShapeData* sharedData = pnew SharedShapeData();
		s_SharedShapes[colliderType][colliderHandle].push_back(sharedData);
		return sharedData;
	}

	SharedShapeManager::SharedShapeData* SharedShapeManager::FindSuitableSharedShape(ColliderType colliderType, const Count<class MeshCollider>& collider, const bool checkSubmeshIndex, const uint32_t submeshIndex)
	{
		if (s_SharedShapes.find(colliderType) == s_SharedShapes.end())
			return nullptr;

		auto& colliderShapeMap = s_SharedShapes.at(colliderType);

		if (colliderShapeMap.find(collider->ColliderMesh) == colliderShapeMap.end())
			return nullptr;

		for (SharedShapeData* shapeData : colliderShapeMap.at(collider->ColliderMesh))
		{
			if (shapeData->Usage != collider->CollisionComplexity)
				continue;

			if (checkSubmeshIndex && shapeData->Shapes.find(submeshIndex) == shapeData->Shapes.end())
				continue;

			return shapeData;
		}

		return nullptr;
	}

	void SharedShapeManager::RemoveSharedShapeData(ColliderType colliderType, const Count<class MeshCollider>& collider, const bool checkSubmeshIndex, const uint32_t submeshIndex)
	{
		auto* shapeData = FindSuitableSharedShape(colliderType, collider, checkSubmeshIndex, submeshIndex);

		if (shapeData == nullptr)
			return;

		auto& sharedShapes = s_SharedShapes[colliderType][collider->ColliderMesh];
		sharedShapes.erase(std::find(sharedShapes.begin(), sharedShapes.end(), shapeData));
		pdelete shapeData;
	}

	void SharedShapeManager::ClearSharedShapes()
	{
		for (auto& [colliderType, colliderShapeMap] : s_SharedShapes)
		{
			for (auto& [colliderHandle, sharedShapes] : colliderShapeMap)
			{
				for (auto sharedShapeData : sharedShapes)
					pdelete sharedShapeData;

				sharedShapes.clear();
			}

			colliderShapeMap.clear();
		}

		s_SharedShapes.clear();
	}
	SharedShapeManager::SharedShapeMap SharedShapeManager::s_SharedShapes;
	
}
