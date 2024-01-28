#include "Proofprch.h"
#include "Material.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Platform/Vulkan/VulkanRenderMaterial.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Shader.h"
namespace Proof {
   
    Material::Material(const std::string& name)
        :Name(name)
    {
        // means it is using pbr shader
        m_RenderMaterial = RenderMaterial::Create(RenderMaterialConfiguration(name, Renderer::GetShader("ProofPBR_Static")));
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static"))
            m_DefaultShader = true;   
        SetDefault();
    }
    Material::Material(const std::string& name, Count<RenderMaterial> material)
        :
        Name(name)
    {
        ///m_RenderMaterial = Count<VulkanRenderMaterial>::CreateFrom(material.As<VulkanRenderMaterial>()).As<RenderMaterial>();
        m_RenderMaterial = material;
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static"))
            m_DefaultShader = true;
        SetDefault();
    }
    Material::Material(const std::string& name, Count<class Shader> shader)
    {
        m_RenderMaterial = RenderMaterial::Create(name, shader);
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static"))
            m_DefaultShader = true;
        SetDefault();
    }
    Material::Material()
        :Name("UnnamedMaterial")

    {
          // means it is using pbr shader
        m_RenderMaterial = RenderMaterial::Create(RenderMaterialConfiguration(Name, Renderer::GetShader("ProofPBR_Static")));
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static"))
            m_DefaultShader = true;
        SetDefault();
    }
    void Material::SetDefault()
    {
        if (!m_DefaultShader)
            return;
        SetAlbedo(glm::vec3(0.8f));

        SetMetalness(0.0f);
        SetRoughness(0.4f);
        SetEmission(0.f);
        SetNormalTextureToggle(false);

        SetTiling({ 1,1 });
        SetOffset({ 0,0 });

        SetAlbedoMap(Renderer::GetWhiteTexture());
        SetNormalMap(Renderer::GetWhiteTexture());
        SetMetalnessMap(Renderer::GetWhiteTexture());
        SetRoughnessMap(Renderer::GetWhiteTexture());

        SetEmissionOverrideColor(glm::vec3{ 0 });
    }

    glm::vec3& Material::GetAlbedoColor() const
    {
        return m_RenderMaterial->GetVector("u_MaterialUniform.Albedo");
    }
    void Material::SetAlbedo(const glm::vec3& vec)
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
    bool& Material::GetNormalTextureToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.NormalTexToggle");
    }
    void Material::SetNormalTextureToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.NormalTexToggle",value);
    }
    bool& Material::GetEmissionOverrideColorToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.EmissionOverrideColorToggle");
    }
    void Material::SetEmissionOverrideColorToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.EmissionOverrideColorToggle", value);
    }
    glm::vec3& Material::GetEmissionOverrideColor()const
    {
        return m_RenderMaterial->GetVector("u_MaterialUniform.EmissionOverrideColor");
    }
    void Material::SetEmissionOverrideColor(const glm::vec3& vec)
    {
        m_RenderMaterial->Set("u_MaterialUniform.EmissionOverrideColor", vec);
    }
    glm::vec2& Material::GetTiling()const
    {
        return m_RenderMaterial->GetVector2("u_MaterialUniform.TextureTiling");
    }
    void Material::SetTiling(const glm::vec2& vec)
    {
        m_RenderMaterial->Set("u_MaterialUniform.TextureTiling",vec);
    }
    glm::vec2& Material::GetOffset()const
    {
        return m_RenderMaterial->GetVector2("u_MaterialUniform.TextureOffset");
    }
    void Material::SetOffset(const glm::vec2& value)const
    {
        return m_RenderMaterial->Set("u_MaterialUniform.TextureOffset", value);
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

    float& Material::GetEmission()const
    {
        return m_RenderMaterial->GetFloat("u_MaterialUniform.Emission");
    }
    void Material::SetEmission(float emisive)
    {
        return m_RenderMaterial->Set("u_MaterialUniform.Emission", emisive);
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
   
    

    bool operator==(const MaterialTable& other, const MaterialTable& other1)
    {
        return other.GetMaterials().size() == other1.GetMaterials().size()
            && std::equal(other.GetMaterials().begin(), other.GetMaterials().end(),
                other1.GetMaterials().begin());
    }

    bool operator<(const MaterialTable& other, const MaterialTable& other2)
    {
        /*
        * CHATGPT lexicographical_compare
        Comparing Word by Word: Lexicographical_compare looks at the first words in both lists. It checks which word comes first in the dictionary. That gives it the order for those words.

        If First Words Are Equal: If the first words are the same in both lists, it moves on to the second words and compares them. It continues doing this for each word in the lists.

        Finding the Difference: The moment it finds a pair of words that are different, it decides which word would come first in a dictionary. That tells it the order of the whole lists.

        List Length: If one list is shorter but identical to a certain point, it's considered smaller in comparison.

        No Clear Winner: If it reaches the end of one list and both lists are identical up to that point, the shorter list is considered smaller.

        So, lexicographical_compare is like how you'd arrange a list of words if you were looking them up in a dictionary, comparing them word by word until you find the first difference.
        */
        return std::lexicographical_compare(
            other.GetMaterials().begin(), other.GetMaterials().end(),
            other2.GetMaterials().begin(), other2.GetMaterials().end(),
            [](const std::pair<const uint32_t, Count<Material>>& a, const std::pair<const uint32_t, Count<Material>>& b) {
                return a.second->GetID() < b.second->GetID();
            }
        );
    }

    // rewriteing because if we do the oposite manner if it is eual poperator< will return false therefore this will return true even though its false
    bool operator>(const MaterialTable& other, const MaterialTable& other2)
    {
        return std::lexicographical_compare(
            other.GetMaterials().begin(), other.GetMaterials().end(),
            other2.GetMaterials().begin(), other2.GetMaterials().end(),
            [](const std::pair<const uint32_t, Count<Material>>& a, const std::pair<const uint32_t, Count<Material>>& b) {
            return a.second->GetID() > b.second->GetID();
        }
        );
    }

}