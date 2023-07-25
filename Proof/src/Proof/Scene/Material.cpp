#include "Proofprch.h"
#include "Material.h"
#include "Proof/Scene/Physics/PhysicsEngine.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
namespace Proof {
   
    Material::Material(const std::string& name)
        :Name(name)
    {
        // means it is using pbr shader
        m_RenderMaterial = RenderMaterial::Create(RenderMaterialConfiguration(name, Shader::Get("ProofPBR_Static")));
        if (m_RenderMaterial->GetConfig().Shader == Shader::Get("ProofPBR_Static"))
            m_DefaultShader = true;   
        SetDefault();
    }
    void Material::SetDefault()
    {
        if (!m_DefaultShader)
            return;
        SetAlbedo(Vector(0.8f));
        //emision
        SetMetalness(0.0f);
        SetRoughness(0.4f);

        SetAlbedoTextureToggle(true);
        SetNormalTextureToggle(false);
        SetRoughnessTextureToggle(false);
        SetMetalnessTextureToggle(false);

        SetAlbedoMap(Renderer::GetWhiteTexture());
        SetNormalMap(Renderer::GetWhiteTexture());
        SetMetalnessMap(Renderer::GetWhiteTexture());
        SetRoughnessMap(Renderer::GetWhiteTexture());
    }

    Vector& Material::GetAlbedoColor() const
    {
        return m_RenderMaterial->GetVector("u_MaterialUniform.Albedo");
    }
    void Material::SetAlbedo(const Vector& vec)
    {
        m_RenderMaterial->Set("u_MaterialUniform.Albedo", vec);
    }
    float& Material::GetMetalness()const
    {
        return m_RenderMaterial->GetFloat("u_MaterialUniform.Metalness");
    }
    void Material::SetMetalness(float metallness)
    {
        m_RenderMaterial->Set("u_MaterialUniform.Metalness",metallness);
    }
    float& Material::GetRoughness()const
    {
        return m_RenderMaterial->GetFloat("u_MaterialUniform.Roughness");
    }
    void Material::SetRoughness(float roghness)
    {
        m_RenderMaterial->Set("u_MaterialUniform.Roughness", roghness);
    }
    bool& Material::GetAlbedoTextureToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.AlbedoTexToggle");
    }
    void Material::SetAlbedoTextureToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.AlbedoTexToggle",value);
    }
    bool& Material::GetNormalTextureToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.NormalTexToggle");
    }
    void Material::SetNormalTextureToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.NormalTexToggle",value);
    }
    bool& Material::GetRoughnessTextureToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.RoghnessTexToggle");
    }
    void Material::SetRoughnessTextureToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.RoghnessTexToggle",value);
    }
    bool& Material::GetMetalnessTextureToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.MetallnesTexToggle");
    }
    void Material::SetMetalnessTextureToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.MetallnesTexToggle", value);
    }
    Vector2& Material::GetTiling()const
    {
        return m_RenderMaterial->GetVector2("u_MaterialUniform.Tiling");
    }
    void Material::SetTiling(const Vector2& vec)
    {
        m_RenderMaterial->Set("u_MaterialUniform.Tiling",vec);
    }
    Vector2& Material::GetOffset()const
    {
        return m_RenderMaterial->GetVector2("u_MaterialUniform.Offset");
    }
    void Material::SetOffset(const Vector2& value)const
    {
        return m_RenderMaterial->Set("u_MaterialUniform.Offset", value);
    }
    void Material::SetAlbedoMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_AlbedoMap", texture);
    }
    void Material::SetNormalMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_NormalMap", texture);
    }
    void Material::SetMetalnessMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_MetallicMap", texture);
    }
    void Material::SetRoughnessMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_RoughnessMap", texture);
    }
    Count<class Texture2D> Material::GetAlbedoMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_AlbedoMap");
    }
    Count<class Texture2D> Material::GetNormalMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_NormalMap");
    }
    Count<class Texture2D> Material::GetMetalnessMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_MetallicMap");
    }
    Count<class Texture2D> Material::GetRoughnessMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_RoughnessMap");
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