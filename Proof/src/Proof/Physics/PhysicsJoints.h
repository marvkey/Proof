#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsUtils.h"

namespace Proof
{
	class PhysicsActor;
	enum class JointType
	{
		None = 0,
		Fixed,
		Spring,
		Hinge,
		BallSocket,
	};

	struct PhysicsJointSourceAnchor
	{
		Count<PhysicsActor> Actor;
		glm::vec3 Position = glm::vec3(0.0f);
		glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // identity
	};

	enum class PhysicsJointTargetAnchorType : uint8_t
	{
		WorldFrame = 0,   // Connected to the world (frame is in WORLD space)
		ActorFrame = 1    // Connected to a PhysicsActor (frame is in that ACTOR'S LOCAL space)
	};

	struct PhysicsJointTargetAnchor
	{
		PhysicsJointTargetAnchorType Type = PhysicsJointTargetAnchorType::WorldFrame;

		// Only valid when Type == ActorFrame
		Count<PhysicsActor> Actor;

		struct Frame
		{
			glm::vec3 Position = glm::vec3(0.0f);
			glm::quat Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // identity
		};

		Frame ActorFrame; // used if ActorFrame
		Frame WorldFrame; // used if WorldFrame


		// Convenience factories
		static PhysicsJointTargetAnchor World(const glm::vec3& worldPos,
			const glm::quat& worldRot = glm::quat(1, 0, 0, 0))
		{
			PhysicsJointTargetAnchor e;
			e.Type = PhysicsJointTargetAnchorType::WorldFrame;
			e.WorldFrame.Position = worldPos;
			e.WorldFrame.Rotation = worldRot;
			return e;
		}

		static PhysicsJointTargetAnchor ActorLocal(Count<PhysicsActor> actor,
			const glm::vec3& localPos = glm::vec3(0.0f),
			const glm::quat& localRot = glm::quat(1, 0, 0, 0))
		{
			PhysicsJointTargetAnchor e;
			e.Type = PhysicsJointTargetAnchorType::ActorFrame;
			e.Actor = actor;
			e.ActorFrame.Position = localPos;
			e.ActorFrame.Rotation = localRot;
			return e;
		}

		bool IsWorld() const { return Type == PhysicsJointTargetAnchorType::WorldFrame; }
		bool IsActor() const { return Type == PhysicsJointTargetAnchorType::ActorFrame; }
	};

#if 0

	class PhysicsJoint : public RefCounted
	{
	public:
		PhysicsJoint(Count<PhysicsActor> base, JointType type);

		PhysicsJoint(Count<PhysicsActor> base, Count<PhysicsActor> target, JointType type);
		virtual ~PhysicsJoint() {};
		void SetTarget(Count<PhysicsActor> target);
		Count<PhysicsActor> GetTarget() const { return m_Target; }
		void SetBase(Count<PhysicsActor> base);
		Count<PhysicsActor> GetBase() const { return m_BaseActor; }

		physx::PxJoint* GetPhysxJoint() const { return m_PhysxJoint; }
		JointType GetType() const { return m_Type; }


		// Break Force
		void GetBreakForces(float& outForce, float& outTorque) const
		{
			if (m_PhysxJoint)
				m_PhysxJoint->getBreakForce(outForce, outTorque);
		}

		void SetBreakForces(float force, float torque)
		{
			if (m_PhysxJoint)
				m_PhysxJoint->setBreakForce(force, torque);
		}

		// Enable Collision
		bool IsCollisionEnabled() const
		{
			return m_PhysxJoint && (m_PhysxJoint->getConstraintFlags() & physx::PxConstraintFlag::eCOLLISION_ENABLED);
		}

		void SetCollisionEnabled(bool enabled)
		{
			if (m_PhysxJoint)
				m_PhysxJoint->setConstraintFlag(physx::PxConstraintFlag::eCOLLISION_ENABLED, enabled);
		}

		// Enable Preprocessing
		bool IsPreprocessingEnabled() const
		{
			return m_PhysxJoint && !(m_PhysxJoint->getConstraintFlags() & physx::PxConstraintFlag::eDISABLE_PREPROCESSING);
		}

		void SetPreprocessingEnabled(bool enabled)
		{
			if (m_PhysxJoint)
				m_PhysxJoint->setConstraintFlag(physx::PxConstraintFlag::eDISABLE_PREPROCESSING, !enabled);
		}

		float GetMassScale() const
		{
			float invScale = m_PhysxJoint->getInvMassScale0();
			return invScale != 0.0f ? 1.0f / invScale : 0.0f;
		}

		void SetMassScale(float scale)
		{
			m_PhysxJoint->setInvMassScale0(scale != 0.0f ? 1.0f / scale : 0.0f);
		}

		float GetInertiaScale() const
		{
			float invScale = m_PhysxJoint->getInvInertiaScale0();
			return invScale != 0.0f ? 1.0f / invScale : 0.0f;
		}

		void SetInertiaScale(float scale)
		{
			m_PhysxJoint->setInvInertiaScale0(scale != 0.0f ? 1.0f / scale : 0.0f);
		}

		float GetConnectedMassScale() const
		{
			float invScale = m_PhysxJoint->getInvMassScale1();
			return invScale != 0.0f ? 1.0f / invScale : 0.0f;
		}

		void SetConnectedMassScale(float scale)
		{
			m_PhysxJoint->setInvMassScale1(scale != 0.0f ? 1.0f / scale : 0.0f);
		}

		float GetConnectedInertiaScale() const
		{
			float invScale = m_PhysxJoint->getInvInertiaScale1();
			return invScale != 0.0f ? 1.0f / invScale : 0.0f;
		}

		void SetConnectedInertiaScale(float scale)
		{
			m_PhysxJoint->setInvInertiaScale1(scale != 0.0f ? 1.0f / scale : 0.0f);
		}
	private:
		void CreateJoint();
	private:
		PhysicsJointSourceAnchor m_Base; // Anchor in base actor's local space
		PhysicsJointTargetAnchor m_Target;
		physx::PxJoint* m_PhysxJoint;

		JointType m_Type = JointType::None;	
	};

	class FixedJoint : public PhysicsJoint
	{
	public:
		FixedJoint(Count<PhysicsActor> base);
		FixedJoint(Count<PhysicsActor> base, Count<PhysicsActor> target);

		physx::PxFixedJoint* GetFixedJoint() const
		{
			return static_cast<physx::PxFixedJoint*>(GetPhysxJoint());
		}

	};

	class SpringJoint : public PhysicsJoint
	{
	public:
		SpringJoint(Count<PhysicsActor> base);
		SpringJoint(Count<PhysicsActor> base, Count<PhysicsActor> target);

		physx::PxDistanceJoint* GetSpringJoint() const
		{
			return static_cast<physx::PxDistanceJoint*>(GetPhysxJoint());
		}
		// Spring Joint Settings
		float GetSpring() const { return GetSpringJoint()->getStiffness(); }
		void SetSpring(float value) { GetSpringJoint()->setStiffness(value); }

		float GetDamper() const { return GetSpringJoint()->getDamping(); }
		void SetDamper(float value) { GetSpringJoint()->setDamping(value); }

		float GetMinDistance() const { return GetSpringJoint()->getMinDistance(); }
		void SetMinDistance(float value) { GetSpringJoint()->setMinDistance(value); }

		float GetMaxDistance() const { return GetSpringJoint()->getMaxDistance(); }
		void SetMaxDistance(float value) { GetSpringJoint()->setMaxDistance(value); }

		float GetTolerance() const { return GetSpringJoint()->getTolerance(); }
		void SetTolerance(float value) { GetSpringJoint()->setTolerance(value); }
	};

	// Represents spring properties for a hinge joint (optional in PhysX)
	struct PhysicsHingeJointSpring
	{
		bool UseSpring = false; // If true, spring behavior is enabled
		float Spring = 0.0f;          // How strongly the joint tries to return to the target position (stiffness)
		float Damping = 0.0f;         // How much resistance is applied against oscillations
		float TargetPosition = 0.0f;  // The target angular position (in radians) the spring wants to reach
	};

	// Describes motor behavior for a hinge joint
	struct PhysicsHingeJointMotor
	{
		bool UseMotor = false;
		float Velocity = 0.0f;    // Target angular velocity (radians/sec) that the motor tries to achieve
		float Force = 0.0f;       // Maximum force the motor can use to reach the target velocity
		bool FreeSpin = false;    // If true, motor stops applying force when velocity is achieved (coasting allowed)
	};

	// Defines rotational limits and optional spring for a hinge joint
	struct PhysicsHingeJointLimits
	{
		bool UseLimits = false;
		float Min = 0.0f;             // Lower angular limit (radians)
		float Max = 0.0f;             // Upper angular limit (radians)
		float ContactDistance = 0.0f; // Distance before reaching the limit where constraint begins to activate (buffer)
		float Bounce = 0.0f;          // Bounciness when hitting the limit (0 = no bounce, 1 = full bounce)
		float Spring = 0.0f;          // Stiffness when pushing against the limits
		float Damper = 0.0f;          // Damping when pushing against the limits
	};
#endif
#if 0
	///TODO NOT Done cause missing some things like unity
	class HingeJoint : public PhysicsJoint
	{
	public:
		HingeJoint(Count<PhysicsActor> base);
		HingeJoint(Count<PhysicsActor> base, Count<PhysicsActor> target);
		physx::PxRevoluteJoint* GetHingeJoint() const
		{
			return static_cast<physx::PxRevoluteJoint*>(GetPhysxJoint());
		}

		void SetMotor(const PhysicsHingeJointMotor& motor)
		{
			auto joint = GetHingeJoint();
			joint->setDriveVelocity(motor.Velocity);
			joint->setDriveForceLimit(motor.Force);
			joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_FREESPIN, motor.FreeSpin);
			GetHingeJoint()->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_ENABLED, motor.UseMotor);
		}


		PhysicsHingeJointMotor GetMotor() const
		{
			PhysicsHingeJointMotor motor;
			motor.Velocity = GetHingeJoint()->getDriveVelocity();
			motor.Force = GetHingeJoint()->getDriveForceLimit();
			motor.FreeSpin = GetHingeJoint()->getRevoluteJointFlags() & physx::PxRevoluteJointFlag::eDRIVE_FREESPIN;
			motor.UseMotor = GetHingeJoint()->getRevoluteJointFlags() & physx::PxRevoluteJointFlag::eDRIVE_ENABLED;

			return motor;
		}


		void SetLimits(const PhysicsHingeJointLimits& limits)
		{
			auto joint = GetHingeJoint();
			physx::PxJointAngularLimitPair pxLimits(limits.Min, limits.Max, limits.ContactDistance);
			pxLimits.restitution = limits.Bounce;
			pxLimits.stiffness = limits.Spring;
			pxLimits.damping = limits.Damper;

			joint->setLimit(pxLimits);
			joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, limits.UseLimits);
		}

		PhysicsHingeJointLimits GetLimits() const
		{
			PhysicsHingeJointLimits limits;
			auto joint = GetHingeJoint();
			auto pxLimits = joint->getLimit();

			limits.Min = pxLimits.lower;
			limits.Max = pxLimits.upper;
			limits.ContactDistance = pxLimits.contactDistance;
			limits.Bounce = pxLimits.restitution;
			limits.Spring = pxLimits.stiffness;
			limits.Damper = pxLimits.damping;
			limits.UseLimits = joint->getRevoluteJointFlags() & physx::PxRevoluteJointFlag::eLIMIT_ENABLED;

			return limits;
		}

	};
#endif
}