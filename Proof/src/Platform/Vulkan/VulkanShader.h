#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Shader.h"
#include<vulkan/vulkan.h>
#include <filesystem>

namespace Proof
{
    struct ShaderArrayData {
        uint32_t Size;
        uint32_t* Data;
    };
    class Proof_API VulkanShader : public Shader {
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
    private:
        static void CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule);
        void CreateShader();
        void CompileOrGetBinaries(const std::filesystem::path& filePath);
        void Compile();
        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
        std::unordered_map < ShaderStage, VkShaderModule>m_ShaderModule;

        friend class VulkanGraphicsPipeline;
        std::unordered_map<ShaderStage, std::string> m_Paths;
        std::string m_Name = "empty ";
        std::unordered_map<ShaderStage, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<ShaderStage, std::string> m_SourceCode;

        std::string ProcessStage(ShaderStage stage, const std::filesystem::path& path);
    };
}