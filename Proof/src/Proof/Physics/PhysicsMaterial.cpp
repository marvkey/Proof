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
                    return physx::PxCombineMode::Enum::eMAX;
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
        m_PhysxMaterial = PhysicsEngine::GetPhysics()->createMaterial(staticFriction, dynamicFriction, bounciness);
        m_PhysxMaterial->userData = this;
    }

    PhysicsMaterial::~PhysicsMaterial()
    {
        m_PhysxMaterial->release(); // make sure
    }

    float PhysicsMaterial::GetStaticFriction()
    {
        return m_PhysxMaterial->getStaticFriction();
    }
    float PhysicsMaterial::GetDynamicFriction()
    {
        return m_PhysxMaterial->getDynamicFriction();
    }


    float PhysicsMaterial::GetBounciness()
    {
        return m_PhysxMaterial->getRestitution();
    }

    void PhysicsMaterial::SetStaticFriction(float fricionon)
    {
        m_PhysxMaterial->setStaticFriction(fricionon);
    }

    void PhysicsMaterial::SetDynamicFriction(float friction)
    {
        m_PhysxMaterial->setDynamicFriction(friction);
    }

    void PhysicsMaterial::SetBounciness(float bounciness)
    {
        m_PhysxMaterial->setRestitution(bounciness);
    }
    CombineMode PhysicsMaterial::GetFrictionCombineMode()
    {
        return Utils::ConvertProofCombineMode(m_PhysxMaterial->getFrictionCombineMode());
    }
    CombineMode PhysicsMaterial::GetBouncinessCombineMode()
    {
        return Utils::ConvertProofCombineMode(m_PhysxMaterial->getRestitutionCombineMode());
    }
 
    void PhysicsMaterial::SetFrictionCombineMode(CombineMode mode)
    {
        m_PhysxMaterial->setFrictionCombineMode(Utils::ConvertPhysixCombineMode(mode));
    }

    void PhysicsMaterial::SetBouncinessCombineMode(CombineMode mode)
    {
        m_PhysxMaterial->setRestitutionCombineMode(Utils::ConvertPhysixCombineMode(mode));
    }
    void PhysicsMaterial::Release()
    {
        // materails will always have by default always 1 thign pointing
        if (m_PhysxMaterial->getReferenceCount() == 1)return;
        m_PhysxMaterial->release(); // make sure
        /*
        if (GetID() == (uint64_t)DefaultRuntimeAssets::PhysicsMaterial)
        {
            // basically the default physics material will awlays have somethign pointing to it 
            if (material->getReferenceCount() == 1)return;// we dont want to delete it 
            material->release(); // make sure
        }
        else
        {
            material->release();
        }
        */
    }
}