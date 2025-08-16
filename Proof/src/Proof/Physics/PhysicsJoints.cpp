#include "Proofprch.h"
#include "PhysicsJoints.h"
#include "PhysicsEngine.h"
#include "PhysicsActor.h"
namespace Proof
{
#if 0
    struct RigidPose {
        glm::vec3 p;   // position
        glm::quat q;   // rotation (w,x,y,z)
    };

    // helpers
    inline glm::vec3 TransformPoint(const RigidPose& t, const glm::vec3& localP) {
        return t.p + (t.q * localP);
    }
    inline glm::vec3 WorldToLocalPoint(const RigidPose& t, const glm::vec3& worldP) {
        glm::quat qInv = glm::conjugate(t.q);
        return qInv * (worldP - t.p);
    }
    void AutoConfigureConnectedAnchor(PhysicsJointTargetAnchor& main, PhysicsJointTargetAnchor& connected)
    {
        // Get anchor world position from main side
        glm::vec3 anchorW;
        glm::vec3 anchorW;
        if (main.IsActor())
        {
            RigidPose mainW{ main.Actor->GetLocation(),
                              main.Actor->GetRotation() };
            anchorW = TransformPoint(mainW, main.ActorFrame.Position);
        }
        else
        {
            anchorW = main.WorldFrame.Position; // already world-space
        }


        // Compute connected anchor in correct space
        if (connected.IsActor())
        {
            RigidPose connW{ connected.Actor->GetLocation(),
                    connected.Actor->GetRotation() };
            connected.ActorFrame.Position = WorldToLocalPoint(connW, anchorW);
            // For hinge/spherical joints you might also want to set the local rotation:
            // connected.ActorFrame.Rotation = glm::conjugate(connW.q) * mainWorldFrameRotation;
        }
        else
        {
            connected.WorldFrame.Position = anchorW;
        }
    }

    inline physx::PxVec3 ToPx(const glm::vec3& v) { return { v.x, v.y, v.z }; }
    inline physx::PxQuat ToPx(const glm::quat& q) { return { q.x, q.y, q.z, q.w }; }
    inline physx::PxTransform ToPx(const glm::vec3& p, const glm::quat& q) { return { ToPx(p), ToPx(q) }; }


    // Source (always actor-local in your struct)
    inline void ToPhysX(const PhysicsJointSourceAnchor& src, physx::PxRigidActor*& outActor, physx::PxTransform& outLocal)
    {
        PF_CORE_ASSERT(src.Actor); // source must be on an actor
        outActor = &src.Actor->GetPhysXActor();
        outLocal = ToPx(src.Position, src.Rotation); // LOCAL to source actor
    }

    // Target (could be world or actor-local)
    inline void ToPhysX(const PhysicsJointTargetAnchor& tgt, physx::PxRigidActor*& outActor, physx::PxTransform& outLocalOrWorld)
    {
        if (tgt.IsWorld())
        {
            outActor = nullptr;
            outLocalOrWorld = (tgt.WorldFrame.Position, tgt.WorldFrame.Rotation); // WORLD frame
        }
        else
        {
            PF_CORE_ASSERT(tgt.Actor);
            outActor = &tgt.Actor->GetPhysXActor();
            outLocalOrWorld = ToPx(tgt.ActorFrame.Position, tgt.ActorFrame.Rotation); // LOCAL to target actor
        }
    }

	PhysicsJoint::PhysicsJoint(Count<PhysicsActor> base, JointType type):
		m_BaseActor(base), m_PhysxJoint(nullptr), m_Type(type)
	{
		PF_CORE_ASSERT(m_BaseActor);
        CreateJoint();
	}


	PhysicsJoint::PhysicsJoint(Count<PhysicsActor> base, Count<PhysicsActor> target, JointType type)
		:m_BaseActor(base), m_PhysxJoint(nullptr), m_Type(type), m_Target(target)
	{
        PF_CORE_ASSERT(m_BaseActor);
        CreateJoint();
	}
	void PhysicsJoint::SetTarget(Count<PhysicsActor> target)
	{
		PF_CORE_ASSERT(target);
		m_Target = target;

		m_PhysxJoint->setActors(
			&m_BaseActor->GetPhysXActor(),
			m_Target ? &m_Target->GetPhysXActor() : nullptr);
	}
	void PhysicsJoint::SetBase(Count<PhysicsActor> base)
	{
		PF_CORE_ASSERT(base);
		m_BaseActor = base;

        m_PhysxJoint->setActors(
            &m_BaseActor->GetPhysXActor(),
            m_Target ? &m_Target->GetPhysXActor() : nullptr);
	}
	void PhysicsJoint::CreateJoint()
	{
        physx::PxRigidActor* a0; physx::PxTransform f0;
        physx::PxRigidActor* a1; physx::PxTransform f1;
        ToPhysX(m_Base, a0, f0);
        ToPhysX(m_Target, a1, f1);

        auto* px = PhysicsEngine::GetPhysics();
        switch (m_Type)
        {
        case JointType::Fixed:
            m_PhysxJoint = physx::PxFixedJointCreate(*px, a0, f0, a1, f1);
            if (m_PhysxJoint)
            {
                auto* j = static_cast<physx::PxFixedJoint*>(m_PhysxJoint);
                j->setProjectionLinearTolerance(0.1f);
                j->setProjectionAngularTolerance(0.01f);
            }
            break;

        case JointType::Hinge:
            m_PhysxJoint = physx::PxRevoluteJointCreate(*px, a0, f0, a1, f1);
            if (m_PhysxJoint)
            {
                auto* j = static_cast<physx::PxRevoluteJoint*>(m_PhysxJoint);
                j->setProjectionLinearTolerance(0.1f);
                j->setProjectionAngularTolerance(0.01f);
            }
            break;

        case JointType::Spring:
            // Distance joint ignores orientation; f0/f1 positions matter
            m_PhysxJoint = physx::PxDistanceJointCreate(*px, a0, f0, a1, f1);
            break;

        case JointType::BallSocket:
            // FIX: spherical joint, not prismatic
            m_PhysxJoint = physx::PxSphericalJointCreate(*px, a0, f0, a1, f1);
            if (m_PhysxJoint)
            {
                auto* j = static_cast<physx::PxSphericalJoint*>(m_PhysxJoint);
                j->setProjectionLinearTolerance(0.1f);
            }
            break;

        default: break;
        }

        if (m_PhysxJoint)
        {
            m_PhysxJoint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
            m_PhysxJoint->setConstraintFlag(physx::PxConstraintFlag::eCOLLISION_ENABLED, false);
            m_PhysxJoint->setConstraintFlag(physx::PxConstraintFlag::ePROJECTION, true);
        }
	}
    FixedJoint::FixedJoint(Count<PhysicsActor> base)
		:PhysicsJoint(base, JointType::Fixed)
    {
    }
    FixedJoint::FixedJoint(Count<PhysicsActor> base, Count<PhysicsActor> target)
		:PhysicsJoint(base, target, JointType::Fixed)
    {
    }
    SpringJoint::SpringJoint(Count<PhysicsActor> base)
		:PhysicsJoint(base, JointType::Spring)
	{
	}
    SpringJoint::SpringJoint(Count<PhysicsActor> base, Count<PhysicsActor> target)
		:PhysicsJoint(base, target, JointType::Spring)
    {

    }
#endif
#if 0
    HingeJoint::HingeJoint(Count<PhysicsActor> base)
		:PhysicsJoint(base, JointType::Hinge)
	{
	}

	HingeJoint::HingeJoint(Count<PhysicsActor> base, Count<PhysicsActor> target)
		:PhysicsJoint(base, target, JointType::Hinge)
	{
	}
#endif
}
