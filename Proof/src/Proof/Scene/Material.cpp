#include "Proofprch.h"
#include "Material.h"
#include "Proof/Scene/Physics/PhysicsEngine.h"
namespace Proof {
   
    Material::Material(const std::string& name)
        :Name(name)
    {
    }

    PhysicsMaterial::PhysicsMaterial(float staticFriction , float dynamicFriction , float bounciness )
    {
        m_RuntimeBody = PhysicsEngine::GetPhysics()->createMaterial(staticFriction, dynamicFriction, bounciness);
    }

    void PhysicsMaterial::SetStaticFriction(float fricionon)
    {
        physx::PxMaterial* material = (physx::PxMaterial *) m_RuntimeBody;
        m_StaticFriction = fricionon;
        material->setStaticFriction(fricionon);
    }

    void PhysicsMaterial::SetDynamicFriction(float friction)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        m_DynamicFriction = friction;
        material->setDynamicFriction(friction);
    }

    void PhysicsMaterial::SetBounciness(float bounciness)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        m_Bounciness = bounciness;
        material->setRestitution(bounciness);
    }
    namespace Utils{
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
    }
    void PhysicsMaterial::SetFrictionCombineMode(CombineMode mode)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        m_FrictionCombineMode = mode;
        
        material->setFrictionCombineMode(Utils::ConvertPhysixCombineMode(m_FrictionCombineMode));
    }

    void PhysicsMaterial::SetBouncinessCombineMode(CombineMode mode)
    {
        physx::PxMaterial* material = (physx::PxMaterial*)m_RuntimeBody;
        m_BouncinessCombineMode = mode;

        material->setRestitutionCombineMode(Utils::ConvertPhysixCombineMode(m_BouncinessCombineMode));
    }

    bool operator==(const MaterialTable& other, const MaterialTable& other1)
    {
        return other.GetMaterials().size() == other1.GetMaterials().size()
            && std::equal(other.GetMaterials().begin(), other.GetMaterials().end(),
                other1.GetMaterials().begin());
    }

}