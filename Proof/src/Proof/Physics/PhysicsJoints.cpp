#include "Proofprch.h"
#include "PhysicsJoints.h"
#include "PhysicsEngine.h"
#include "PhysicsActor.h"
namespace Proof
{
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
        switch (m_Type)
        {
        case Proof::JointType::None:
            break;

        case Proof::JointType::Fixed:
            m_PhysxJoint = physx::PxFixedJointCreate(
                *PhysicsEngine::GetPhysics(),
                &m_BaseActor->GetPhysXActor(),
                physx::PxTransform(PhysXUtils::ToPhysXVector(m_BaseActor->GetLocation()), PhysXUtils::ToPhysXQuat(m_BaseActor->GetRotation())),
                m_Target ? &m_Target->GetPhysXActor() : nullptr,
                m_Target ? physx::PxTransform(PhysXUtils::ToPhysXVector(m_Target->GetLocation()), PhysXUtils::ToPhysXQuat(m_Target->GetRotation()))
                : physx::PxTransform(physx::PxIdentity)

            );

            if (m_PhysxJoint)
            {
                auto* fixed_joint = static_cast<physx::PxFixedJoint*>(m_PhysxJoint);
                fixed_joint->setProjectionLinearTolerance(0.1f);
                fixed_joint->setProjectionAngularTolerance(0.01f);
            }
          
            break;

        case Proof::JointType::Hinge:
            m_PhysxJoint = physx::PxRevoluteJointCreate(
                *PhysicsEngine::GetPhysics(),
                &m_BaseActor->GetPhysXActor(),
                physx::PxTransform(PhysXUtils::ToPhysXVector(m_BaseActor->GetLocation()), PhysXUtils::ToPhysXQuat(m_BaseActor->GetRotation())),
                m_Target ? &m_Target->GetPhysXActor() : nullptr,
                m_Target ? physx::PxTransform(PhysXUtils::ToPhysXVector(m_Target->GetLocation()), PhysXUtils::ToPhysXQuat(m_Target->GetRotation()))
                : physx::PxTransform(physx::PxIdentity)
            );

            if (m_PhysxJoint)
            {
                auto* fixed_joint = static_cast<physx::PxRevoluteJoint*>(m_PhysxJoint);
                fixed_joint->setProjectionLinearTolerance(0.1f);
                fixed_joint->setProjectionAngularTolerance(0.01f);
            }
            break;

        case Proof::JointType::Spring:
            m_PhysxJoint = physx::PxDistanceJointCreate(
                *PhysicsEngine::GetPhysics(),
                &m_BaseActor->GetPhysXActor(),
                physx::PxTransform(PhysXUtils::ToPhysXVector(m_BaseActor->GetLocation()), PhysXUtils::ToPhysXQuat(m_BaseActor->GetRotation())),
                m_Target ? &m_Target->GetPhysXActor() : nullptr,
                m_Target ? physx::PxTransform(PhysXUtils::ToPhysXVector(m_Target->GetLocation()), PhysXUtils::ToPhysXQuat(m_Target->GetRotation()))
                : physx::PxTransform(physx::PxIdentity)
            );

            break;

        case Proof::JointType::BallSocket:
            m_PhysxJoint = physx::PxPrismaticJointCreate(
                *PhysicsEngine::GetPhysics(),
                &m_BaseActor->GetPhysXActor(),
                physx::PxTransform(PhysXUtils::ToPhysXVector(m_BaseActor->GetLocation()), PhysXUtils::ToPhysXQuat(m_BaseActor->GetRotation())),
                m_Target ? &m_Target->GetPhysXActor() : nullptr,
                m_Target ? physx::PxTransform(PhysXUtils::ToPhysXVector(m_Target->GetLocation()), PhysXUtils::ToPhysXQuat(m_Target->GetRotation()))
                : physx::PxTransform(physx::PxIdentity)
            );

            if (m_PhysxJoint)
            {
                auto* fixed_joint = static_cast<physx::PxSphericalJoint*>(m_PhysxJoint);
                fixed_joint->setProjectionLinearTolerance(0.1f);
            }
            break;

        default:
            break;
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
