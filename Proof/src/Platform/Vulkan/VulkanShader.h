#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Shader.h"
#include "Vulkan.h"
#include "VulkanShaderResource.h"
namespace Proof
{
    
    struct ShaderResoureData {
        uint32_t Set = 0;
        uint32_t Binding = 0;
    };

    struct ShaderResourceBufferInfo {
        uint32_t Size;
        uint32_t Offset;
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
        const std::unordered_map < ShaderStage, VkShaderModule>& GetShaderModule()const{ return m_ShaderModule; };
        virtual void Reload();
        const std::map < uint32_t, ShaderDescriptorSet>& GetShaderDescriptorSet()const {return m_ShaderDescriptorSet;};
        const std::map < uint32_t, ShaderDescriptorResource>& GetDescriptorResource() {return m_DescriptorResource;};

        const std::map<uint32_t, std::vector<VkDescriptorPoolSize>>& GetTypeCounts()const { return m_TypeCounts; };
        const std::unordered_map<std::string, VkPushConstantRange>& GetPushConstants()const { return m_PushConstants; };
        const std::unordered_map < ShaderStage, VkPipelineShaderStageCreateInfo>& GetShaderStages()const { return m_ShaderStages; };
        const SahderInputDeclaration* GetInputDeclaration(std::string name)const;
        const ShaderResourceBufferInfo* GetPushConstantInput(const std::string& storageName,const std::string& name)const;
        const ShaderResourceBufferInfo* GetStorageBufferInput(const std::string& storageName, const std::string& name)const;
        // storageBufferName, (totalSizeofBuffer)(name, data)
        const std::unordered_map<std::string, std::pair<uint32_t, std::unordered_map<std::string, ShaderResourceBufferInfo>>>& GetStorageInfo()const { return m_StorageBufferResourceInfo; }
        const std::unordered_map<std::string, std::pair<uint32_t, std::unordered_map<std::string, ShaderResourceBufferInfo>>>& GetPushConstantInfo()const { return m_PushConstantResourceInfo; }

        virtual uint32_t AddShaderReloadCallback(const ShaderReloadCallback& callback);
        virtual void RemoveShaderReloadCallback(uint32_t index);
    private:

        std::map<uint32_t,ShaderReloadCallback> m_ShaderReloads;
        static void CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule);
        void CreateShader();
        void CompileOrGetBinaries(const std::filesystem::path& filePath);

        // if compile was a success
        bool Compile(const std::unordered_map<ShaderStage, std::string>& sourceCode);
        void Release();
        std::string ProcessStage(ShaderStage stage, const std::filesystem::path& path);

        std::string m_Name;
        std::unordered_map<ShaderStage,VkPipelineShaderStageCreateInfo> m_ShaderStages;
        std::unordered_map < ShaderStage, VkShaderModule>m_ShaderModule;
        std::unordered_map<ShaderStage, std::string> m_Paths;
        std::unordered_map<ShaderStage, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<ShaderStage, std::string> m_SourceCode;
        std::map<uint32_t, std::vector<VkDescriptorPoolSize>> m_TypeCounts;
        std::unordered_map<std::string, VkPushConstantRange> m_PushConstants;
        //(set, data)
        std::map < uint32_t, ShaderDescriptorSet> m_ShaderDescriptorSet;

        std::map < uint32_t, ShaderDescriptorResource> m_DescriptorResource;

        std::unordered_map<std::string, SahderInputDeclaration> m_InputDeclaration;

        // storageBufferName, (totalSizeofBuffer)(name, data)
        std::unordered_map<std::string, std::pair<uint32_t, std::unordered_map<std::string, ShaderResourceBufferInfo>>> m_StorageBufferResourceInfo;

        // storageBufferName, (totalSizeofBuffer)(name, data)
        std::unordered_map<std::string, std::pair<uint32_t,std::unordered_map<std::string, ShaderResourceBufferInfo>>> m_PushConstantResourceInfo;
        const VkWriteDescriptorSet* GetDescriptorSet(uint32_t set = 0)const ;
        bool m_ConstructorSamePaths = false;

        // the first compile of the shader
        bool m_InitialCompile = true;
        friend class VulkanGraphicsPipeline;


    };
}