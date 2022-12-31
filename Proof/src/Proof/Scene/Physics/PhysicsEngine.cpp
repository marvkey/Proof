#include "Proofprch.h"
#include "PhysicsEngine.h"
#include "../Component.h"
#include "../World.h"
#include "../Entity.h"
#include "Proof/Asset/AssetManager.h"
#include "../Material.h"
#include "Proof/Asset/PhysicsMaterialAsset.h"
#include "../CollisionInfo.h"
#include "../Script.h"
#include "../Mesh.h"

namespace Proof {
	physx::PxMaterial* defauultMaterial;
	namespace Utils {
		static physx::PxMat44 glmMat4ToPhysxMat4(const glm::mat4& mat4) {
			physx::PxMat44 newMat;

			newMat[0][0] = mat4[0][0];
			newMat[0][1] = mat4[0][1];
			newMat[0][2] = mat4[0][2];
			newMat[0][3] = mat4[0][3];

			newMat[1][0] = mat4[1][0];
			newMat[1][1] = mat4[1][1];
			newMat[1][2] = mat4[1][2];
			newMat[1][3] = mat4[1][3];

			newMat[2][0] = mat4[2][0];
			newMat[2][1] = mat4[2][1];
			newMat[2][2] = mat4[2][2];
			newMat[2][3] = mat4[2][3];

			newMat[3][0] = mat4[3][0];
			newMat[3][1] = mat4[3][1];
			newMat[3][2] = mat4[3][2];
			newMat[3][3] = mat4[3][3];


			return newMat;
		}
	}
	PhysicsEngine::~PhysicsEngine()
	{
		m_Pvd->disconnect();
		m_Cooking->release();
		m_Physics->release();
		m_Foundation->release();
	}
	physx::PxFilterFlags shaderControl(
		physx::PxFilterObjectAttributes attributes0,
		physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1,
		physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock,
		physx::PxU32 constantBlockSize)
	{
		PX_UNUSED(constantBlock);
		PX_UNUSED(constantBlockSize);
		// let triggers through
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlags();
		}
		
		

		pairFlags |= physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eTRIGGER_DEFAULT;

		return physx::PxFilterFlags();


	}
	void PhysicsEngine::SetUpBase() {
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback,
			m_DefaultErrorCallback);
		if (!m_Foundation)
			PF_CORE_ASSERT(false, "PxCreateFoundation failed!");

		m_Pvd = physx::PxCreatePvd(*m_Foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		m_ToleranceScale.length = 100;        // typical length of an object one meter
		m_ToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, m_Pvd);
		if (!m_Physics)
			PF_CORE_ASSERT(false, "PxCreateFoundation failed!");

		m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(m_Physics->getTolerancesScale()));
		if (!m_Cooking)
			PF_CORE_ASSERT(false, "PxCreateCooking failed!");
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = m_Dispatcher;
		sceneDesc.simulationEventCallback = &m_CollisionCallback;
		sceneDesc.filterShader = shaderControl;
		//persistant collion
		//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
		m_Scene = m_Physics->createScene(sceneDesc);

		physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}


	}
	void PhysicsEngine::Start() {
		PF_PROFILE_FUNC();
		defauultMaterial = m_Physics->createMaterial(0.6f, 0.6f, 0.f);
		m_World->ForEachEnitityWith<RigidBodyComponent>([&](Entity entity) {
			const auto& transformComponent = *entity.GetComponent<TransformComponent>();
			const auto worldLocation = m_World->GetWorldLocation(entity);
			const auto worldRotation = m_World->GetWorldRotation(entity);
			auto& rigidBodyComponent = *entity.GetComponent<RigidBodyComponent>();

			physx::PxActor* rigidBodyBase = nullptr;
			if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Dynamic)
			{
				glm::mat4 transform = m_World->GetWorldTransform(entity);
				
				physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(physx::PxTransform{ physx::PxVec3(worldLocation.X,worldLocation.Y,worldLocation.Z),
					physx::PxQuat(worldRotation.X,worldRotation.Y,worldRotation.Z,1) });
				rigidBodyComponent.m_RuntimeBody = body;
				body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
				body->setMass(rigidBodyComponent.Mass);
				body->setAngularDamping(rigidBodyComponent.AngularDrag);
				body->setLinearDamping(rigidBodyComponent.LinearDrag);
				body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, Math::InverseBool(rigidBodyComponent.Gravity));
				body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidBodyComponent.Kinimatic);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBodyComponent.FreezeLocation.X);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBodyComponent.FreezeLocation.Y);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBodyComponent.FreezeLocation.Z);

				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.X);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Y);
				body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Z);
				rigidBodyBase = body;
			}
			else
			{
				rigidBodyComponent.m_RuntimeBody = m_Physics->createRigidStatic(physx::PxTransform{ physx::PxVec3(worldLocation.X,worldLocation.Y,worldLocation.Z),
				physx::PxQuat(worldRotation.X,worldRotation.Y,worldRotation.Z,1) });
				physx::PxRigidStatic* body = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
				rigidBodyBase = body;
			}
			m_Scene->addActor(*rigidBodyBase);
		});

		m_World->ForEachEnitityWith<SphereColliderComponent, RigidBodyComponent>([&](Entity entity) {
			auto sphereCollider = entity.GetComponent<SphereColliderComponent>();
			auto& rigidBodyComponent = *entity.GetComponent<RigidBodyComponent>();
			float size = sphereCollider->Radius * m_World->GetWorldScale(entity).GetMaxAbsolute();

			physx::PxMaterial* colliderMaterial = sphereCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)sphereCollider->GetPhysicsMaterial()->m_RuntimeBody;
			physx::PxShape* body = m_Physics->createShape(physx::PxSphereGeometry(size), *colliderMaterial, true);
			body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
			{
				//glm::mat4 rotationTransform = glm::toMat4(glm::quat(glm::vec3{ glm::radians(0.f), glm::radians(0.f), glm::radians(90.f) }));
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ sphereCollider->OffsetLocation });
				body->setLocalPose(physx::PxTransform{ Utils::glmMat4ToPhysxMat4(transform) });
			}

			if (sphereCollider->IsTrigger == true)
			{
				body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			}
			else
			{
				body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			}
			if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic)
			{
				auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
				runtimeBody->attachShape(*body);
			}
			else
			{
				auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
				runtimeBody->attachShape(*body);
			}
		});
		m_World->ForEachEnitityWith<CubeColliderComponent, RigidBodyComponent>([&](Entity entity) {
			//the world positon for this will be held in the rigid body
			auto cubeCollider = entity.GetComponent<CubeColliderComponent>();
			auto& rigidBodyComponent = *entity.GetComponent<RigidBodyComponent>();
			physx::PxMaterial* colliderMaterial = cubeCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)cubeCollider->GetPhysicsMaterial()->m_RuntimeBody;
			auto scalePositiveTransform = entity.GetCurrentWorld()->GetWorldScale(entity).GetPositive();
			auto scalePositiveCollider = cubeCollider->OffsetScale.GetPositive();

			auto size = scalePositiveTransform * scalePositiveCollider;
			physx::PxShape* body = m_Physics->createShape(physx::PxBoxGeometry(size.X, size.Y, size.Z), *colliderMaterial, true);
			body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion

			{
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ cubeCollider->OffsetLocation });
				body->setLocalPose(physx::PxTransform{ Utils::glmMat4ToPhysxMat4(transform) });
			}
			if (cubeCollider->IsTrigger == true)
			{
				body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			}
			else
			{
				body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			}

			if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic)
			{
				auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
				runtimeBody->attachShape(*body);
			}
			else
			{
				auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
				runtimeBody->attachShape(*body);
			}

		});

		m_World->ForEachEnitityWith<CapsuleColliderComponent, RigidBodyComponent>([&](Entity entity) {
			auto capsuleCollider = entity.GetComponent<CapsuleColliderComponent>();
			auto& rigidBodyComponent = *entity.GetComponent<RigidBodyComponent>();
			auto worldScale = m_World->GetWorldScale(entity);
			physx::PxMaterial* colliderMaterial = capsuleCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)capsuleCollider->GetPhysicsMaterial()->m_RuntimeBody;
			float radius = capsuleCollider->Radius * entity.GetCurrentWorld()->GetWorldScale(entity).GetMaxAbsolute();
			float height = capsuleCollider->Height;
			physx::PxShape* body = m_Physics->createShape(physx::PxCapsuleGeometry(radius, height), *colliderMaterial, true);
			body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
		// Location
			Vector capsuleRotation = { 0,0,0 };// originial local pos is {0,0,0}
			switch (capsuleCollider->Direction)
			{
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
			body->setLocalPose(physx::PxTransform{ Utils::glmMat4ToPhysxMat4(transform) });

			if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic)
			{
				auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
				runtimeBody->attachShape(*body);
			}
			else
			{
				auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
				runtimeBody->attachShape(*body);
			}
		});
		m_World->ForEachEnitityWith<MeshColliderComponent, RigidBodyComponent>([&](Entity entity) {
			auto meshCollider = entity.GetComponent<MeshColliderComponent>();
			auto& rigidBodyComponent = *entity.GetComponent<RigidBodyComponent>();
			auto worldScale = m_World->GetWorldScale(entity);

			if (meshCollider)
			{
				auto mesh = meshCollider->GetMesh();
				if (mesh != nullptr)
				{
					std::vector<physx::PxVec3> vertices;
					for (const SubMesh& subMesh : mesh->GetSubMeshes())
					{
						for (const auto& vertex : subMesh.m_Vertices)
							vertices.emplace_back(physx::PxVec3{ vertex.Vertices.X,vertex.Vertices.Y,vertex.Vertices.Z });
					}

					std::vector<uint32_t> indices;
					for (const SubMesh& subMesh : mesh->GetSubMeshes())
					{
						for (const auto& val : subMesh.m_Indices)
							indices.emplace_back(val);
					}
					physx::PxConvexMeshDesc convexDesc;
					convexDesc.points.count = vertices.size();
					convexDesc.points.stride = sizeof(physx::PxVec3);
					convexDesc.points.data = vertices.data();

					convexDesc.indices.count = indices.size();
					convexDesc.indices.stride = sizeof(uint32_t);
					convexDesc.indices.data = indices.data();
					convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

					convexDesc.flags |= physx::PxConvexFlag::ePLANE_SHIFTING;

					physx::PxDefaultMemoryOutputStream buf;
					physx::PxConvexMeshCookingResult::Enum error;
					if (!m_Cooking->cookConvexMesh(convexDesc, buf, &error))
					{
						switch (error)
						{
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
					physx::PxConvexMesh* convexMesh = m_Physics->createConvexMesh(input);
					meshCollider->m_ConvexMeshRuntimeBody = convexMesh;

					physx::PxMaterial* colliderMaterial = meshCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)meshCollider->GetPhysicsMaterial()->m_RuntimeBody;
					physx::PxShape* body = m_Physics->createShape(
						physx::PxConvexMeshGeometry(convexMesh, physx::PxVec3{ worldScale.X,worldScale.Y,worldScale.Z }),
						*colliderMaterial, true);
					//ADD CONVEX MESH TO ASSET
					body->setName(fmt::to_string(entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
					//glm::mat4 transform = m_World->GetWorldTransform(entity);
					//body->setLocalPose(physx::PxTransform{ Utils::glmMat4ToPhysxMat4(transform) });
					if (rigidBodyComponent.GetType() == RigidBodyType::Dynamic)
					{
						auto runtimeBody = (physx::PxRigidDynamic*)rigidBodyComponent.m_RuntimeBody;
						runtimeBody->attachShape(*body);
					}
					else
					{
						auto runtimeBody = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
						runtimeBody->attachShape(*body);
					}
				}
			}
		});
	}
	void PhysicsEngine::Simulate(float delta) {
		PF_PROFILE_FUNC();
		m_World->ForEachEnitityWith<RigidBodyComponent>([&](Entity entity) {
			const auto transform = entity.GetComponent<TransformComponent>();
			auto rigidBody = *entity.GetComponent<RigidBodyComponent>();
			if (rigidBody.m_RigidBodyType == RigidBodyType::Dynamic)
			{
				physx::PxRigidDynamic* body = (physx::PxRigidDynamic*)rigidBody.m_RuntimeBody;
				body->setMass(rigidBody.Mass);
				body->setAngularDamping(rigidBody.AngularDrag);
				body->setLinearDamping(rigidBody.LinearDrag);
				if (rigidBody.Gravity == true)
				{
					bool firstSet = body->getActorFlags() & physx::PxActorFlag::eDISABLE_GRAVITY;
					if (firstSet == true) // no gravity enabled
					{
						body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
						body->wakeUp();
					}

				}
				else
				{
					bool firstSet = body->getActorFlags() & physx::PxActorFlag::eDISABLE_GRAVITY;
					if (firstSet == false)
					{
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
				const auto bodyTransform = body->getGlobalPose();
				// needs to be chgned to local location and rotation
				transform->Location = Vector{ bodyTransform.p.x,bodyTransform.p.y,bodyTransform.p.z };
				transform->Rotation = Vector{ glm::degrees(bodyTransform.q.x),glm::degrees(bodyTransform.q.y),glm::degrees(bodyTransform.q.z) };
			}
		});

		m_Scene->simulate(delta);
		m_Scene->fetchResults(true);


	}
	void PhysicsEngine::End() {
		PF_PROFILE_FUNC();
	}
}