#pragma once
#include "Proof/Core/Core.h"
#include "Vulkan.h"
#include "Proof/Renderer/Shader.h"
#include "VulkanShaderResource.h"
namespace Proof
{
    
    struct ShaderResoureData {
        std::string Name;
       // RendererResourceType ResourceType;
        uint32_t Set = 0;
        uint32_t Binding = 0;
    };

    struct ShaderDescriptorResource {
        VkDescriptorSet Set;
        VkDescriptorPool Pool;
        VkDescriptorSetLayout Layout;
    };
    struct SahderInputDeclaration {
        uint32_t Set;
        uint32_t Binding;
    };
    class VulkanShader : public Shader {
    public:
        VulkanShader(const std::string& name, const std::filesystem::path& filePath);
        VulkanShader(const std::string& name, const std::unordered_map<ShaderStage, std::string> shaders);
        ~VulkanShader();
        virtual const std::unordered_map<ShaderStage, std::string>& GetPath()const { return m_Paths; }
        virtual const std::string& GetName()const { return m_Name; }
        void Reflect(ShaderStage stage);

        uint32_t GetStageCount() {
            return m_ShaderStages.size();
        }
     
        virtual void Reload();
        const std::unordered_map < uint32_t, ShaderDescriptorSet>& GetShaderDescriptorSet()const {return m_ShaderDescriptorSet;};
        const std::unordered_map < uint32_t, ShaderDescriptorResource>& GetDescriptorResource() {return m_DescriptorResource;};

        const std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>>& GetTypeCounts()const { return m_TypeCounts; };
        const std::unordered_map<std::string, VkPushConstantRange>& GetPushConstants()const { return m_PushConstants; };

        const SahderInputDeclaration* GetInputDelcaration(std::string name)const ;
    private:
        static void CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule);
        void CreateShader();
        void CompileOrGetBinaries(const std::filesystem::path& filePath);
        void Compile();
        void Release();
        std::string ProcessStage(ShaderStage stage, const std::filesystem::path& path);

        std::string m_Name;
        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
        std::unordered_map < ShaderStage, VkShaderModule>m_ShaderModule;
        std::unordered_map<ShaderStage, std::string> m_Paths;
        std::unordered_map<ShaderStage, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<ShaderStage, std::string> m_SourceCode;
        std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>> m_TypeCounts;
        std::unordered_map<std::string, VkPushConstantRange> m_PushConstants;
        //(set, data)
        std::unordered_map < uint32_t, ShaderDescriptorSet> m_ShaderDescriptorSet;

        std::unordered_map < uint32_t, ShaderDescriptorResource> m_DescriptorResource;

        std::unordered_map<std::string, SahderInputDeclaration> m_InputDeclration;
        const VkWriteDescriptorSet* GetDescriptorSet(uint32_t set = 0)const ;
        bool m_ConstructorSamePaths = false;
        friend class VulkanGraphicsPipeline;


    };
}