#include "Proofprch.h"
#include "PhysicsController.h"
#include "Proof/Scene/Entity.h"
#include "PhysicsEngine.h"
#include "PhysicsWorld.h"
#include "PhysicsMaterial.h"
#include "PhysicsUtils.h"
#include "Proof/Asset/AssetManager.h"
namespace Proof
{

	//https://www.youtube.com/watch?v=UUJMGQTT5ts&list=PLwyUzJb_FNeQrIxCEjj5AMPwawsw5beAy&index=2&ab_channel=iHeartGameDev
	//https://www.youtube.com/watch?v=e94KggaEAr4&t=9s&ab_channel=iHeartGameDev
	PhysicsController::PhysicsController(Count<class PhysicsWorld> world, Entity entity)
		:
		PhysicsActorBase(world, PhysicsControllerType::Controller,entity)
	{
		Init();
	}

	PhysicsController::~PhysicsController()
	{

	}
	void PhysicsController::Init()
	{
		auto& controller = m_Entity.GetComponent<CharacterControllerComponent>();
		if (AssetManager::HasAsset(controller.PhysicsMaterialID))
			m_Material = AssetManager::GetAsset<PhysicsMaterial>(controller.PhysicsMaterialID);
		else
			controller.PhysicsMaterialID = 0;

		if (m_Material == nullptr)
			m_Material = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>();

		if (IsCapsuleController())
			GenerateCapsule();
		else
			GenerateCube();
		if (!PhysicsLayerManager::IsLayerValid(m_Entity.GetComponent<CharacterControllerComponent>().PhysicsLayerID))
		{
			m_Entity.GetComponent<CharacterControllerComponent>().PhysicsLayerID = 0;
		}
		SetSimulationData(m_Entity.GetComponent<CharacterControllerComponent>().PhysicsLayerID);

	}
	void PhysicsController::Release()
	{
		m_Controller->release();
	}

	void PhysicsController::GenerateCapsule()
	{
		auto& controller = m_Entity.GetComponent<CharacterControllerComponent>();
		auto& transform = m_Entity.GetComponent<TransformComponent>();
		float radiusScale = glm::max(transform.Scale.x, transform.Scale.z);
		physx::PxCapsuleControllerDesc desc;
		desc.position = PhysXUtils::ToPhysXExtendedVector(m_Entity.Transform().Location + controller.Center); // not convinced this is correct.  (e.g. it needs to be world space, not local)
		desc.height = controller.Height/2 * transform.Scale.y;
		desc.radius = controller.Radius * radiusScale;
		//desc.radius = controller.Radius;
		desc.nonWalkableMode = PhysXUtils::ToPhysXPxControllerNonWalkableMode(controller.WalkableMode);  // TODO: get from component
		desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
		desc.slopeLimit = std::max(0.0f, cos(controller.SlopeLimitRadians));
		desc.stepOffset = controller.StepOffset;
		desc.contactOffset = controller.SkinOffset;                                                     // TODO: get from component
		desc.material = &m_Material->GetPhysxMaterial();
		desc.upDirection = PhysXUtils::ToPhysXVector(Math::GetUpVector());

		PF_CORE_ASSERT(desc.isValid(), "Capsule Controller is not valid");
		m_Controller = m_PhysicsWorld->GetPhysXControllerManager()->createController(desc);

		m_Controller->getActor()->userData = this;
	}
	void PhysicsController::GenerateCube()
	{
		auto& controller = m_Entity.GetComponent<CharacterControllerComponent>();
		auto& transform = m_Entity.GetComponent<TransformComponent>();

		physx::PxBoxControllerDesc desc;
		desc.position = PhysXUtils::ToPhysXExtendedVector(m_Entity.Transform().Location + controller.Center); // not convinced this is correct.  (e.g. it needs to be world space, not local)
		desc.halfHeight = (controller.Size.y * transform.Scale.y);
		desc.halfSideExtent = (controller.Size.x * transform.Scale.x);
		desc.halfForwardExtent = (controller.Size.z * transform.Scale.z);
		desc.nonWalkableMode = PhysXUtils::ToPhysXPxControllerNonWalkableMode(controller.WalkableMode);  // TODO: get from component
		desc.slopeLimit = std::max(0.0f, cos(controller.SlopeLimitRadians));
		desc.stepOffset = controller.StepOffset;
		desc.contactOffset = controller.SkinOffset;                                                     // TODO: get from component
		desc.material = &m_Material->GetPhysxMaterial();
		desc.upDirection = PhysXUtils::ToPhysXVector(Math::GetUpVector());
		
		PF_CORE_ASSERT(desc.isValid(), "Box Controller is not valid");
		m_Controller = m_PhysicsWorld->GetPhysXControllerManager()->createController(desc);

		m_Controller->getActor()->userData = this;
	}
	void PhysicsController::SetGravityEnabled(const bool enableGravity)
	{
		m_Entity.GetComponent<CharacterControllerComponent>().GravityEnabled = enableGravity;
	}
	bool PhysicsController::IsGravityEnabled() const
	{
		return m_Entity.GetComponent<CharacterControllerComponent>().GravityEnabled;
	}
	void PhysicsController::SetGravityScale(float scale)
	{
		auto& controller = m_Entity.GetComponent<CharacterControllerComponent>();
		controller.GravityScale = scale;
	}
	float PhysicsController::GetGravityScale()
	{
		auto& controller = m_Entity.GetComponent<CharacterControllerComponent>();
		return controller.GravityScale;
	}
	void PhysicsController::SetSlopeLimit(const float slopeLimitRadians)
	{
		m_Controller->setSlopeLimit(std::max(0.0f, cos(slopeLimitRadians)));
		m_Entity.GetComponent<CharacterControllerComponent>().SlopeLimitRadians = slopeLimitRadians;
	}
	void PhysicsController::SetStepOffset(const float stepOffset)
	{
		m_Controller->setStepOffset(stepOffset);
		m_Entity.GetComponent<CharacterControllerComponent>().StepOffset = stepOffset;
	}
	
	bool PhysicsController::SetSimulationData(uint32_t layerId)
	{

		const PhysicsLayer& layerInfo = PhysicsLayerManager::GetLayer(layerId);

		if (layerInfo.CollidesWith == 0)
			return false;

		//TODO ony support continuous detection
		physx::PxFilterData filterData = PhysXUtils::BuildFilterData(layerInfo, CollisionDetectionType::Continuous);

		const auto actor = m_Controller->getActor();
		PF_CORE_ASSERT(actor && actor->getNbShapes() == 1);
		physx::PxShape* shape;
		actor->getShapes(&shape, 1);
		shape->setSimulationFilterData(filterData);
		return true;
	}

	glm::vec3 PhysicsController::GetLocation() const
	{
		const auto& pxPos = m_Controller->getPosition();
		glm::vec3 pos = { pxPos.x, pxPos.y, pxPos.z };

		auto& controller = m_Entity.GetComponent<CharacterControllerComponent>();
		pos -= controller.Center;
		return pos;
	}
	void PhysicsController::SetLocation(const glm::vec3& translation, const bool autowake)
	{
		auto& controller = m_Entity.GetComponent<CharacterControllerComponent>();

		glm::vec3 newPosition = translation + controller.Center;

		// Set the new position for the character controller
		m_Controller->setPosition(PhysXUtils::ToPhysXExtendedVector(newPosition));
	}
	
	bool PhysicsController::IsGrounded() const
	{
		return m_CollisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN;
	}
	CollisionFlags PhysicsController::GetCollisionFlags() const
	{
		return static_cast<CollisionFlags>((physx::PxU8)m_CollisionFlags);;
	}
	void PhysicsController::Move(glm::vec3 displacement)
	{
		m_Displacement += displacement;
	}
	float PhysicsController::GetSpeedDown() const
	{
		return m_Speed.y;
	}
	void PhysicsController::Jump(float jumpPower)
	{
		m_Speed.y = -1.0f * jumpPower;
	}
	bool PhysicsController::IsCapsuleController() const
	{
		return m_Entity.GetComponent<CharacterControllerComponent>().ColliderType == CharacterControllerType::Capsule;
	}
	float PhysicsController::GetMinMoveDistance() const
	{
		return m_Entity.GetComponent<CharacterControllerComponent>().MinMoveDistance;
	}
	void PhysicsController::SetMinMoveDistance(float distance)
	{
		float actualDistance = glm::max<float>(0, distance);
		m_Entity.GetComponent<CharacterControllerComponent>().MinMoveDistance = distance;
	}
	void PhysicsController::OnUpdate(float dt)
	{
		//float gravity = glm::length(PhysicsEngine::GetSettings().Gravity);            // acceleration due to gravity (in direction opposite to controllers "up" vector)

		physx::PxControllerFilters filters;

		if(IsGravityEnabled())
			m_Speed -= PhysicsEngine::GetSettings().Gravity * dt * GetGravityScale() ;

		glm::vec3 displacement = m_Displacement - PhysXUtils::FromPhysXVector(m_Controller->getUpDirection()) * m_Speed * dt;

		m_CollisionFlags = m_Controller->move(PhysXUtils::ToPhysXVector(displacement), 0.0, static_cast<physx::PxF32>(dt), filters);

		if (IsGrounded())
		{
			m_Speed = PhysicsEngine::GetSettings().Gravity * 0.01f; // setting speed back to zero here would be more technically correct,
		}
		// but a small non-zero gives better results (e.g. lessens jerkyness when walking down a slope)
		m_Displacement = {};
	}
	void PhysicsController::SyncTransform()
	{
		TransformComponent& transform = m_Entity.Transform();
		transform.Location = GetLocation();
	}
}

