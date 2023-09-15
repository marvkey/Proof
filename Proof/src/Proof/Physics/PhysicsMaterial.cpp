#include "Proofprch.h"
#include "PhysicsMaterial.h"
#include "Proof/Physics/PhysicsEngine.h"

namespace Proof
{
    namespace Utils {
        physx::PxCombineMode::Enum ConvertPhysixCombineMode(CombineMode  mode) {
            switch (mode)
            {
                case Proof::CombineMode::Average:
                    return physx::PxCombineMode::Enum::eAVERAGE;
                case Proof::CombineMode::Min:
                    return physx::PxCombineMode::Enum::eMIN;
                case Proof::CombineMode::Mutltiply:
                    return physx::PxCombineMode::Enum::eMULTIPLY;
                case Proof::CombineMode::Max:
                    physx::PxCombineMode::Enum::eMAX;
                default:
                    break;
            }
            PF_CORE_ASSERT(false, "not valid mode");
        }
        CombineMode ConvertProofCombineMode(physx::PxCombineMode::Enum mode) {
            switch (mode)
            {
                case physx::PxCombineMode::Enum::eAVERAGE:
                    return CombineMode::Average;
                case physx::PxCombineMode::Enum::eMIN:
                    return CombineMode::Min;
                case physx::PxCombineMode::Enum::eMULTIPLY:
                    return CombineMode::Mutltiply;
                case physx::PxCombineMode::Enum::eMAX:
                    return CombineMode::Max;
                default:
                    PF_CORE_ASSERT(false, "Not a valid mode");
            }
        }
    }
    PhysicsMaterial::PhysicsMaterial(float staticFriction, float dynamicFriction, float bounciness)
    {
        m_RuntimeBody = PhysicsEngine::GetPhysics()->createMaterial(staticFriction, dynamicFriction, bounciness);
    }

    float PhysicsMaterial::GetStaticFriction()
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        return material->getStaticFriction();
    }
    float PhysicsMaterial::GetDynamicFriction()
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        return material->getDynamicFriction();
    }

    float PhysicsMaterial::GetBounciness()
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        return material->getRestitution();
    }

    void PhysicsMaterial::SetStaticFriction(float fricionon)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        material->setStaticFriction(fricionon);
    }

    void PhysicsMaterial::SetDynamicFriction(float friction)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        material->setDynamicFriction(friction);
    }

    void PhysicsMaterial::SetBounciness(float bounciness)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        material->setRestitution(bounciness);
    }
    CombineMode PhysicsMaterial::GetFrictionCombineMode()
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        return Utils::ConvertProofCombineMode(material->getFrictionCombineMode());
    }
    CombineMode PhysicsMaterial::GetBouncinessCombineMode()
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        return Utils::ConvertProofCombineMode(material->getRestitutionCombineMode());
    }
 
    void PhysicsMaterial::SetFrictionCombineMode(CombineMode mode)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;

        material->setFrictionCombineMode(Utils::ConvertPhysixCombineMode(mode));
    }

    void PhysicsMaterial::SetBouncinessCombineMode(CombineMode mode)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        material->setRestitutionCombineMode(Utils::ConvertPhysixCombineMode(mode));
    }
}