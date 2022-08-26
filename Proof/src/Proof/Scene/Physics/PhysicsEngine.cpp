#include "Proofprch.h"
#include "PhysicsEngine.h"
#include "../Component.h"
#include "../World.h"
#include "../Entity.h"
#include "NVDIAPhysX.h"
#include "Proof/Resources/Asset/AssetManager.h"
#include "../Material.h"
#include "Proof/Resources/Asset/PhysicsMaterialAsset.h"
#include "../CollisionInfo.h"
#include "../Script.h"
#include "../Mesh.h"
namespace Proof {
	physx::PxMaterial* defauultMaterial;
	
	void PhysicsEngine::Start(){
		PF_PROFILE_FUNC();
		switch (m_PhysicsType) 
		{
			case PhysicsEngineType::NvdiaPhysics:
				StartNvdiaPhysx();
				break;
			case PhysicsEngineType::BulletPhysics:
				StartBulletPhysics();
				break;
		};
	}
	void PhysicsEngine::Simulate(float delta){
		PF_PROFILE_FUNC();
		switch (m_PhysicsType) 
		{
			case PhysicsEngineType::NvdiaPhysics:
				UpdateNvdiaPhysx(delta);
				break;
			case PhysicsEngineType::BulletPhysics:
				UpdateBulletPhysics(delta);
				break;
		};
	}
	void PhysicsEngine::End(){
		PF_PROFILE_FUNC();
		switch (m_PhysicsType) 
		{
			case PhysicsEngineType::NvdiaPhysics:
				EndNvdiaPhysx();
				break;
			case PhysicsEngineType::BulletPhysics:
				EndBulletPhysics();
				break;
		};
	}
	
	void PhysicsEngine::StartNvdiaPhysx() {

		m_NVDIAPhysicsEngine = new NVDIAPhysXEngine();
		//
		{
			for (auto& [ID,asset] : AssetManager::s_AssetManager->m_AllPhysicsMaterialAsset) {
				auto& physicsMaterial = asset->m_Material;
				physx::PxMaterial* body = m_NVDIAPhysicsEngine->m_Physics->createMaterial(physicsMaterial.StaticFriction,physicsMaterial.DynamicFriction,
				physicsMaterial.Bounciness);

				body->setFrictionCombineMode((physx::PxCombineMode::Enum)physicsMaterial.FrictionCombineMode);
				body->setRestitutionCombineMode((physx::PxCombineMode::Enum)physicsMaterial.BouncinessCombineMode);
				asset->m_Material.m_RuntimeBody = body;
			}
		}

		//param[in] restitution		The coefficient of restitution
		// that is a measure of bouncinesss
		defauultMaterial = m_NVDIAPhysicsEngine->m_Physics->createMaterial(0.6f, 0.6f, 0.f);
		{
			m_World->ForEachEntitiesWithSingle<RigidBodyComponent>([&](Entity entity) {
				const auto& transformComponent = entity.GetComponent<TransformComponent>();
				const auto worldLocation = transformComponent->GetWorldLocation();
				const auto worldRotation = transformComponent->GetWorldRotation();
				const auto worldScale = transformComponent->GetWorldScale();
				auto& rigidBodyComponent = *entity.GetComponent<RigidBodyComponent>();
				physx::PxActor* rigidBodyBase= nullptr;
				if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Dynamic) {

					physx::PxRigidDynamic* body = m_NVDIAPhysicsEngine->m_Physics->createRigidDynamic(physx::PxTransform{ physx::PxVec3(worldLocation.X,worldLocation.Y,worldLocation.Z),
						physx::PxQuat(worldRotation.X,worldRotation.Y,worldRotation.Z,1) });
					rigidBodyComponent.m_RuntimeBody = body;
					body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
					body->setMass(rigidBodyComponent.Mass);
					body->setAngularDamping(rigidBodyComponent.AngularDrag);
					body->setLinearDamping(rigidBodyComponent.LinearDrag);
					if (rigidBodyComponent.Gravity == true)
						body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
					else
						body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

					body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidBodyComponent.Kinimatic);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBodyComponent.FreezeLocation.X);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBodyComponent.FreezeLocation.Y);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBodyComponent.FreezeLocation.Z);

					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.X);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Y);
					body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Z);
					rigidBodyBase = body;
					//m_NVDIAPhysicsEngine->m_Scene->addActor(*body);
				}
				else {
					rigidBodyComponent.m_RuntimeBody = m_NVDIAPhysicsEngine->m_Physics->createRigidStatic(physx::PxTransform{ physx::PxVec3(worldLocation.X,worldLocation.Y,worldLocation.Z),
					physx::PxQuat(worldRotation.X,worldRotation.Y,worldRotation.Z,1) });
					physx::PxRigidStatic* body = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
					rigidBodyBase = body;
					//m_NVDIAPhysicsEngine->m_Scene->addActor(*body);
				}

				{
					auto sphereCollider = entity.GetComponent<SphereColliderComponent>();
					if (sphereCollider != nullptr) {
						float size = sphereCollider->Radius * transformComponent->GetWorldScale().GetMaxTransformPositive();

						physx::PxMaterial* colliderMaterial = sphereCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)sphereCollider->GetPhysicsMaterial()->m_RuntimeBody;
						physx::PxShape* body = m_NVDIAPhysicsEngine->m_Physics->createShape(physx::PxSphereGeometry(size), *colliderMaterial, true);
						body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
						{
							// LOCAL Pos so it would be added to the pos of the rigid body so we only chanign location thats we we set location
							glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(0.f), glm::radians(0.f), glm::radians(90.f) }));
							glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ sphereCollider->OffsetLocation })* rotationTransform;
							body->setLocalPose(physx::PxTransform{ NVDIAPhysXEngine::glmMat4ToPhysxMat4(transform) });
						}

						//if (sphereCollider->IsTrigger == true) {
						//	body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
						//	body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
						//}
						//else {
						//	body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
						//	body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
						//}
						if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic) {
							auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
							runtimeBody->attachShape(*body);
						}
						else {
							auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
							runtimeBody->attachShape(*body);
						}

					}
					{
						auto cubeCollider = entity.GetComponent<CubeColliderComponent>();
						if (cubeCollider != nullptr) {
							physx::PxMaterial* colliderMaterial = cubeCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)cubeCollider->GetPhysicsMaterial()->m_RuntimeBody;
							auto scalePositiveTransform = transformComponent->GetWorldScale().GetPositive();
							auto scalePositiveCollider = cubeCollider->OffsetScale.GetPositive();

							auto size = scalePositiveTransform * scalePositiveCollider;
							physx::PxShape* body = m_NVDIAPhysicsEngine->m_Physics->createShape(physx::PxBoxGeometry(size.X, size.Y, size.Z), *colliderMaterial, true);
							body->setName(fmt::to_string((uint64_t)entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion

							{
								//only changng the locaiton so we set that and scale cannot be changed on a geometry and is hanled by shape size
								glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ cubeCollider->OffsetLocation });
								body->setLocalPose(physx::PxTransform{ NVDIAPhysXEngine::glmMat4ToPhysxMat4(transform) });
							}
							//if (cubeCollider->IsTrigger == true) {
							//	body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
							//	body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
							//}
							//else {
							//	body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
							//	body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
							//}

							if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic) {
								auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
								runtimeBody->attachShape(*body);
							}
							else {
								auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
								runtimeBody->attachShape(*body);
							}
						}
					}
					{
						auto capsuleCollider = entity.GetComponent<CapsuleColliderComponent>();
						if (capsuleCollider != nullptr) {
							physx::PxMaterial* colliderMaterial = capsuleCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)capsuleCollider->GetPhysicsMaterial()->m_RuntimeBody;
							float radius = capsuleCollider->Radius * transformComponent->GetWorldScale().GetMaxTransformPositive();
							float height = capsuleCollider->Height;
							physx::PxShape* body = m_NVDIAPhysicsEngine->m_Physics->createShape(physx::PxCapsuleGeometry(radius, height), *colliderMaterial, true);
							body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
							// Location
							{
								Vector capsuleRotation = { 0,0,0 };// originial local pos is {0,0,0}
								switch (capsuleCollider->Direction) {
									case CapsuleDirection::X:
									{
										height *= worldScale.GetPositive().X;
										capsuleRotation.X += 0;// default is facing X direction
									}
									break;
									case CapsuleDirection::Y:
									{
										height *= worldScale.GetPositive().Y;
										capsuleRotation.Z += 90; // have to swap ssicne starting is facing X direction
										break;
									}
									case CapsuleDirection::Z:
									{
										height *= worldScale.GetPositive().Z;
										capsuleRotation.Y += 90; // have to swap ssicne starting is facing X direction
										break;
									}

								}
								// setting both local rotation adn local position
								glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(capsuleRotation.X), glm::radians(capsuleRotation.Y), glm::radians(capsuleRotation.Z) }));
								glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ capsuleCollider->OffsetLocation })
									* rotationTransform;
								body->setLocalPose(physx::PxTransform{ NVDIAPhysXEngine::glmMat4ToPhysxMat4(transform) });
							}

							if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic) {
								auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
								runtimeBody->attachShape(*body);
							}
							else {
								auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
								runtimeBody->attachShape(*body);
							}
						}
					}
				}
				{
					auto meshCollider = entity.GetComponent<MeshColliderComponent>();
					if (meshCollider) {
						auto mesh = meshCollider->GetMesh();
						if (mesh != nullptr) {
							std::vector<physx::PxVec3> vertices;
							for (const SubMesh& subMesh : mesh->GetSubMeshes()) {
								for (const auto& vertex : subMesh.m_Vertices)
									vertices.emplace_back(physx::PxVec3{ vertex.Vertices.X,vertex.Vertices.Y,vertex.Vertices.Z });
							}
							
							std::vector<uint32_t> indices;
							for (const SubMesh& subMesh : mesh->GetSubMeshes()) {
								for (const auto& val : subMesh.m_Indices)
									indices.emplace_back(val);
							}
							physx::PxConvexMeshDesc convexDesc;
							convexDesc.points.count = vertices.size() ;
							convexDesc.points.stride = sizeof(physx::PxVec3);
							convexDesc.points.data = vertices.data();
							
							convexDesc.indices.count = indices.size();
							convexDesc.indices.stride = sizeof(uint32_t);
							convexDesc.indices.data = indices.data();
							convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

							convexDesc.flags |= physx::PxConvexFlag::ePLANE_SHIFTING;

							physx::PxDefaultMemoryOutputStream buf;
							physx::PxConvexMeshCookingResult::Enum error;
							if (!m_NVDIAPhysicsEngine->m_Cooking->cookConvexMesh(convexDesc, buf, &error)) {
								switch (error) {
								case physx::PxConvexMeshCookingResult::eSUCCESS:
									break;
								case physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED:
									PF_ERROR("Physx mesh cooking failed,couldn't find 4 initial vertices without a small triangle");
									break;
								case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED:
									PF_ERROR("Physx mesh cooking failed,has reached the 255 polygons limit,Try to simplify the input vertices or try to use the eINFLATE_CONVEX or the eQUANTIZE_INPUT flags");
									break;
								case physx::PxConvexMeshCookingResult::eFAILURE:
									PF_ERROR("Physx mesh cooking failed");
									break;
								}
							}

							physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
							physx::PxConvexMesh* convexMesh = m_NVDIAPhysicsEngine->m_Physics->createConvexMesh(input);
							meshCollider->m_ConvexMeshRuntimeBody = convexMesh;

							physx::PxMaterial* colliderMaterial = meshCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)meshCollider->GetPhysicsMaterial()->m_RuntimeBody;
							physx::PxShape* body = m_NVDIAPhysicsEngine->m_Physics->createShape(
								physx::PxConvexMeshGeometry(convexMesh, physx::PxVec3{ worldScale.X,worldScale.Y,worldScale.Z }),
								*colliderMaterial, true);
							//ADD CONVEX MESH TO ASSET
							body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
							if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic) {
								auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
								runtimeBody->attachShape(*body);
							}
							else {
								auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
								runtimeBody->attachShape(*body);
							}
						}
					}
				}
				m_NVDIAPhysicsEngine->m_Scene->addActor(*rigidBodyBase);

			});
			
		}
		// Colliders
		//{
		//	// Cube Collider
		//	{
		//		m_World->ForEachEntitiesWithSingle<CubeColliderComponent>([&](Entity currentEntity) {
		//			const auto& transform = *currentEntity.GetComponent<TransformComponent>();
		//			auto& cubeCollider = *currentEntity.GetComponent<CubeColliderComponent>();
		//			auto& size = glm::vec3{ transform.GetWorldScale().X * cubeCollider.OffsetScale.X,transform.GetWorldScale().Y * cubeCollider.OffsetScale.Y,transform.GetWorldScale().Z * cubeCollider.OffsetScale.Z };
		//			auto& location = transform.GetWorldLocation() + cubeCollider.OffsetLocation;
		//
		//			cubeCollider.RuntimeBody = m_NVDIAPhysicsEngine->m_Physics->createShape(physx::PxBoxGeometry(1000, 1, 1000), *defauultMaterial,true);
		//			physx::PxShape* body = static_cast<physx::PxShape*>(cubeCollider.RuntimeBody);
		//			body->setLocalPose({ location.X,location.Y,location.Z });
		//			if (cubeCollider.IsTrigger == true) {
		//				body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		//				body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		//			}
		//			else {
		//				body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		//				body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		//			}
		//			auto rigidBodyComponent = currentEntity.GetComponent<RigidBodyComponent>();
		//			if (rigidBodyComponent != nullptr) {
		//				if (rigidBodyComponent->m_RigidBodyType == RigidBodyType::Dynamic) {
		//					physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)rigidBodyComponent->RuntimeBody;
		//					rigidBody->attachShape(*body);
		//				}
		//				else {
		//					auto rigidBody = (physx::PxRigidStatic*)rigidBodyComponent->RuntimeBody;
		//				rigidBody->attachShape(*body);
		//
		//				}
		//			}
		//			body->release();
		//		});
		//		
		//	}
		//}
		
	}
	void PhysicsEngine::UpdateNvdiaPhysx(float delta) {

		m_World->ForEachEntitiesWithSingle<RigidBodyComponent>([&](Entity entity) {
			const auto transform = entity.GetComponent<TransformComponent>();
			auto rigidBody = *entity.GetComponent<RigidBodyComponent>();
			if (rigidBody.m_RigidBodyType == RigidBodyType::Dynamic) {
				physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)rigidBody.m_RuntimeBody;
				body->setMass(rigidBody.Mass);
				body->setAngularDamping(rigidBody.AngularDrag);
				body->setLinearDamping(rigidBody.LinearDrag);
				if (rigidBody.Gravity == true) {
					//body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
					bool firstSet = body->getActorFlags() & physx::PxActorFlag::eDISABLE_GRAVITY;
					if (firstSet == true) // no gravity enabled
					{
						body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
						body->wakeUp();
					}

				}
				else {
					//body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
					bool firstSet = body->getActorFlags() & physx::PxActorFlag::eDISABLE_GRAVITY;
					if (firstSet == false) {
						body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
						body->wakeUp();
					}
				}

				body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidBody.Kinimatic);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBody.FreezeLocation.X);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBody.FreezeLocation.Y);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBody.FreezeLocation.Z);

				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBody.FreezeRotation.X);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBody.FreezeRotation.Y);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBody.FreezeRotation.Z);
				const auto& bodyTransform = body->getGlobalPose();
				transform->Location = Vector{ bodyTransform.p.x,bodyTransform.p.y,bodyTransform.p.z };
				transform->Rotation = Vector{glm::degrees(bodyTransform.q.x),glm::degrees(bodyTransform.q.y),glm::degrees(bodyTransform.q.z) };
			}
		});
		m_NVDIAPhysicsEngine->Simulate(delta);

		//{
		//	auto& cccV = m_World->m_Registry.view<CubeColliderComponent>();
		//	for (auto entity : cccV) {
		//		Entity currentEntity{ entity, m_World };
		//		const auto& transform = *currentEntity.GetComponent<TransformComponent>();
		//		auto& cubeCollider = cccV.get<CubeColliderComponent>(entity);
		//		auto& location = transform.GetWorldLocation() + cubeCollider.OffsetLocation;
		//		auto& size = glm::vec3{ transform.GetWorldScale().X * cubeCollider.OffsetScale.X,transform.GetWorldScale().Y * cubeCollider.OffsetScale.Y,transform.GetWorldScale().Z * cubeCollider.OffsetScale.Z };
		//
		//		physx::PxShape* body = (physx::PxShape*)cubeCollider.RuntimeBody;
		//		//body->setGeometry(physx::PxBoxGeometry(size.x, size.y, size.z));
		//		//body->setLocalPose({ location.X,location.Y,location.Z });
		//		//if (cubeCollider.IsTrigger == true) {
		//		//	body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		//		//	body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		//		//}
		//		//else {
		//		//	body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		//		//	body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		//		//}
		//	}
		//}

	}
	void PhysicsEngine::EndNvdiaPhysx() {
		delete m_NVDIAPhysicsEngine;
	}

	void PhysicsEngine::StartBulletPhysics() {
		
	}

	void PhysicsEngine::UpdateBulletPhysics(float delta) {

	}
	void PhysicsEngine::EndBulletPhysics() {
	}

	void PhysicsEngine::StartProofPhysics() {

	
	}
	void PhysicsEngine::UpdateProofPhysics(float delta) {
		
	}
	void PhysicsEngine::EndProofPhysics() {
	}

}