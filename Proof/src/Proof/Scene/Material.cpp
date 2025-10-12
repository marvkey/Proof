#include "Proofprch.h"
#include "Material.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Platform/Vulkan/VulkanRenderMaterial.h"
#include "Proof/Platform/Vulkan/VulkanDescriptorManager.h"
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
    Material::Material(const Material& material)
    {
        Name = material.Name;
        m_DefaultShader = material.m_DefaultShader;
        m_RenderMaterial = Count<VulkanRenderMaterial>::CreateFrom( material.m_RenderMaterial.As<VulkanRenderMaterial>());
    }
    MaterialTypes Material::GetSurfaceType()
    {
		auto shader = m_RenderMaterial->GetConfig().Shader; 

        if (shader->GetAllShaderMacroDefines().contains("MATERIAL_SURFACE"))
            return MaterialTypes::Surface;
        if(shader->GetAllShaderMacroDefines().contains("MATERIAL_POST_PROCESS"))
			return MaterialTypes::PostProcess;

        return MaterialTypes::Other;
    }
    Material::Material(const std::string& name, Count<class Shader> shader)
    {
        m_RenderMaterial = RenderMaterial::Create(name, shader);
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static"))
            m_DefaultShader = true;
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBRTransparent_Static"))
            m_DefaultShader = true;
        SetDefault();
    }
    Material::Material()
        :Name("UnnamedMaterial")

    {
          // means it is using pbr shader
        m_RenderMaterial = RenderMaterial::Create(RenderMaterialConfiguration(Name, Renderer::GetShader("ProofPBR_Static")));
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static") )
            m_DefaultShader = true;
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBRTransparent_Static"))
            m_DefaultShader = true;

        SetDefault();
    }
    void Material::SetMaterialShader(const std::string& materialName,Count<class Shader> shader)
    {
        m_DefaultShader = false;
        m_RenderMaterial = RenderMaterial::Create(materialName, shader);
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBR_Static"))
            m_DefaultShader = true;
        if (m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBRTransparent_Static"))
            m_DefaultShader = true;
        SetDefault();
    }
    std::map<std::string, VariableTypes> Material::GetMaterialVariables()
    {
        auto shader = m_RenderMaterial->GetConfig().Shader;
        std::map<std::string, VariableTypes> vars;
        for (auto& [uniforName,uniformData] : shader.As<VulkanShader>()->GetPushConstantInfo())
        {
            for (auto& [varID, varData] : uniformData.second)
            {
               switch(varData.VarType)
                {
                case ShaderResourceBufferVarType::Bool:
                    vars[varID] = VariableTypes::Bool;
                    break;
                case ShaderResourceBufferVarType::Int:
                    vars[varID] = VariableTypes::Int;
                    break;
                case ShaderResourceBufferVarType::Float:
                    vars[varID] = VariableTypes::Float;
                    break;
                case ShaderResourceBufferVarType::Vec2:
                    vars[varID] = VariableTypes::Vec2;
                    break;
                case ShaderResourceBufferVarType::Vec3:
                    vars[varID] = VariableTypes::Vec3;
                    break;
                case ShaderResourceBufferVarType::Vec4:
                    vars[varID] = VariableTypes::Vec4;
                    break;
                case ShaderResourceBufferVarType::Double:
                case ShaderResourceBufferVarType::DVec2:
                case ShaderResourceBufferVarType::DVec3:
                case ShaderResourceBufferVarType::DVec4:
                case ShaderResourceBufferVarType::Mat2:
                case ShaderResourceBufferVarType::Mat3:
                case ShaderResourceBufferVarType::Mat4:
                case ShaderResourceBufferVarType::Int64:
                case ShaderResourceBufferVarType::Uint64:
                case ShaderResourceBufferVarType::BVec2:
                case ShaderResourceBufferVarType::BVec3:
                case ShaderResourceBufferVarType::BVec4:
                case ShaderResourceBufferVarType::IVec2:
                case ShaderResourceBufferVarType::IVec3:
                case ShaderResourceBufferVarType::IVec4:
                case ShaderResourceBufferVarType::UVec2:
                case ShaderResourceBufferVarType::UVec3:
                case ShaderResourceBufferVarType::UVec4:
                case ShaderResourceBufferVarType::Sampler:
                case ShaderResourceBufferVarType::SampledImage:
                case ShaderResourceBufferVarType::Image:
                case ShaderResourceBufferVarType::AccelerationStructure:
                case ShaderResourceBufferVarType::RayQuery:
                case ShaderResourceBufferVarType::Struct:
                case ShaderResourceBufferVarType::AtomicCounter:
                default:
                    PF_EC_WARN("Material::GetMaterialVariables() {} - Unsupported variable Name:{} type: {}", shader->GetName(), varID, EnumReflection::EnumString(varData.VarType));
                    break;
			   }
            }
        }

        return vars;
    }
    std::vector<std::string> Material::GetEditableTextures()
    {
		std::vector<std::string> result;
        auto shader  = m_RenderMaterial->GetConfig().Shader;
        for (auto& [set, setData] : shader.As<VulkanShader>()->GetShaderDescriptorSet())
        {
            if(set != 0)
				continue;

            for (auto& [id, resourcedata] : setData.ImageSamplers)
            {
                if(resourcedata.DescriptorCount > 1)
                {
                    PF_EC_WARN("Material::GetEditableTextures() {} - Texture2D array detected, only single textures are supported", shader->GetName());
                    continue;
				}
                result.push_back(resourcedata.Name);
            }
        }
        return result;
    }
    void Material::SetDefault()
    {
        if (!m_DefaultShader)
            return;

		PbrSurfaceMaterial surfaceMaterial(this);
        if(m_RenderMaterial->GetConfig().Shader == Renderer::GetShader("ProofPBRTransparent_Static"))
            m_RenderMaterial->Set("u_MaterialUniform.Albedo", glm::vec4(0.8f));
        else
            surfaceMaterial.SetAlbedo(glm::vec3(0.8f));


        surfaceMaterial.SetMetalness(0.0f);
        surfaceMaterial.SetRoughness(0.4f);
        surfaceMaterial.SetEmission(0.0f); 
        surfaceMaterial.SetNormalTextureToggle(false);

        surfaceMaterial.SetTiling({ 1,1 });
        surfaceMaterial.SetOffset({ 0,0 });

        surfaceMaterial.SetAlbedoMap(Renderer::GetWhiteTexture());
        surfaceMaterial.SetNormalMap(Renderer::GetWhiteTexture());
        surfaceMaterial.SetMetalnessMap(Renderer::GetWhiteTexture());
        surfaceMaterial.SetRoughnessMap(Renderer::GetWhiteTexture());

        surfaceMaterial.SetEmissionOverrideColor(glm::vec3{ 0 });

        
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

    PbrSurfaceMaterial::PbrSurfaceMaterial(Count<Material> material)
        :m_Material(material)
    {
		m_RenderMaterial =  material->GetRenderMaterial();
    }


    glm::vec3& PbrSurfaceMaterial::GetAlbedoColor() const
    {
        return m_RenderMaterial->GetVector("u_MaterialUniform.Albedo");
    }
    void PbrSurfaceMaterial::SetAlbedo(const glm::vec3& vec)
    {
        m_RenderMaterial->Set("u_MaterialUniform.Albedo", vec);
    }
    float& PbrSurfaceMaterial::GetMetalness()const
    {
        return m_RenderMaterial->GetFloat("u_MaterialUniform.Metalness");
    }
    void PbrSurfaceMaterial::SetMetalness(float metallness)
    {
        m_RenderMaterial->Set("u_MaterialUniform.Metalness", metallness);
    }
    float& PbrSurfaceMaterial::GetRoughness()const
    {
        return m_RenderMaterial->GetFloat("u_MaterialUniform.Roughness");
    }
    void PbrSurfaceMaterial::SetRoughness(float roghness)
    {
        m_RenderMaterial->Set("u_MaterialUniform.Roughness", roghness);
    }
    bool& PbrSurfaceMaterial::GetNormalTextureToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.NormalTexToggle");
    }
    void PbrSurfaceMaterial::SetNormalTextureToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.NormalTexToggle", value);
    }
    bool& PbrSurfaceMaterial::GetEmissionOverrideColorToggle()
    {
        return m_RenderMaterial->GetBool("u_MaterialUniform.EmissionOverrideColorToggle");
    }
    void PbrSurfaceMaterial::SetEmissionOverrideColorToggle(bool value)
    {
        m_RenderMaterial->Set("u_MaterialUniform.EmissionOverrideColorToggle", value);
    }
    glm::vec3& PbrSurfaceMaterial::GetEmissionOverrideColor()const
    {
        return m_RenderMaterial->GetVector("u_MaterialUniform.EmissionOverrideColor");
    }
    void PbrSurfaceMaterial::SetEmissionOverrideColor(const glm::vec3& vec)
    {
        m_RenderMaterial->Set("u_MaterialUniform.EmissionOverrideColor", vec);
    }
    glm::vec2& PbrSurfaceMaterial::GetTiling()const
    {
        return m_RenderMaterial->GetVector2("u_MaterialUniform.TextureTiling");
    }
    void PbrSurfaceMaterial::SetTiling(const glm::vec2& vec)
    {
        m_RenderMaterial->Set("u_MaterialUniform.TextureTiling", vec);
    }
    glm::vec2& PbrSurfaceMaterial::GetOffset()const
    {
        return m_RenderMaterial->GetVector2("u_MaterialUniform.TextureOffset");
    }
    void PbrSurfaceMaterial::SetOffset(const glm::vec2& value)const
    {
        return m_RenderMaterial->Set("u_MaterialUniform.TextureOffset", value);
    }
    void PbrSurfaceMaterial::SetAlbedoMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_AlbedoMap", texture);
    }
    void PbrSurfaceMaterial::SetNormalMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_NormalMap", texture);
    }
    void PbrSurfaceMaterial::SetMetalnessMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_MetallicMap", texture);
    }
    void PbrSurfaceMaterial::SetRoughnessMap(Count<class Texture2D> texture)
    {
        m_RenderMaterial->Set("u_RoughnessMap", texture);
    }

    float& PbrSurfaceMaterial::GetEmission()const
    {
        return m_RenderMaterial->GetFloat("u_MaterialUniform.Emission");
    }
    void PbrSurfaceMaterial::SetEmission(float emisive)
    {
        return m_RenderMaterial->Set("u_MaterialUniform.Emission", emisive);
    }
    Count<class Texture2D> PbrSurfaceMaterial::GetAlbedoMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_AlbedoMap");
    }
    Count<class Texture2D> PbrSurfaceMaterial::GetNormalMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_NormalMap");
    }
    Count<class Texture2D> PbrSurfaceMaterial::GetMetalnessMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_MetallicMap");
    }
    Count<class Texture2D> PbrSurfaceMaterial::GetRoughnessMap()
    {
        return m_RenderMaterial->TryGetTexture2D("u_RoughnessMap");
    }
}