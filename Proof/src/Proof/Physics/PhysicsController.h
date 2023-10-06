#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsActorBase.h"
#include "PhysicsUtils.h"
namespace Proof
{
	class Entity;
	class PhysicsController : public PhysicsActorBase
	{
	public:
		PhysicsController(Count<class PhysicsWorld> world, Entity entity);
		virtual ~PhysicsController();

		virtual void SetGravityEnabled(const bool enableGravity) override;
		virtual bool IsGravityEnabled() const override;
		//virtual bool SetSimulationData(uint32_t layerId) override;
		void SetSlopeLimit(const float slopeLimitRadians);
		void SetStepOffset(const float stepOffset);

		virtual glm::vec3 GetLocation() const override;
		virtual void SetLocation(const glm::vec3& translation, const bool autowake = true) override;

		float GetSpeedDown() const;
		bool IsGrounded() const;
		//CollisionFlags GetCollisionFlags() const;

		void Move(glm::vec3 displacement);
		void Jump(float jumpPower);

		bool IsCapsuleController()const;

		float GetMinMoveDistance()const;
		//If the character tries to move below the indicated value, 
		//it will not move at all. This can be used to reduce jitter. In most situations this value should be left at 0.
		void SetMinMoveDistance(float distance);
	private:
		physx::PxController* m_Controller = nullptr;
		physx::PxControllerCollisionFlags m_CollisionFlags = {};
		float m_SpeedDown = 0.0f;        // speed of controller in down direction at last update
		glm::vec3 m_Displacement = {};   // displacement (if any) for next update (comes from Move() calls)
		Count<class PhysicsMaterial> m_Material;

	private:
		void OnUpdate(float dt);
		virtual void SyncTransform() override;
		void Init();
		void GenerateCapsule();
		void Release();
		friend class PhysicsWorld;
	};
}
