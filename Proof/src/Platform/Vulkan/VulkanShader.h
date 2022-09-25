#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Shader.h"
#include<vulkan/vulkan.h>
#include <filesystem>

namespace Proof
{
    class Proof_API VulkanShader : public Shader {
    public:
        VulkanShader(const std::string& name, const std::filesystem::path& filePath);
        VulkanShader(const std::string& name, const std::unordered_map<Shader::ShaderStage, std::string> m_Paths);
        ~VulkanShader();
        virtual const std::unordered_map<Shader::ShaderStage, std::string>& GetPath()const { return m_Paths; }
        virtual const std::string& GetName()const { return m_Name; }
        void Reflect(Shader::ShaderStage stage);

        uint32_t GetStageCount() {
            return m_VulkanSPIRV.size();
        }
    private:
        static void CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule);
        void CreateShader();
        void CompileOrGetBinaries(const std::filesystem::path& filePath);
        void Compile();
        std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
        std::unordered_map < Shader::ShaderStage, VkShaderModule>m_ShaderModule;

        friend class VulkanGraphicsPipeline;
        std::unordered_map<Shader::ShaderStage, std::string> m_Paths;
        std::string m_Name = "empty ";
        std::unordered_map<Shader::ShaderStage, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<Shader::ShaderStage, std::string> m_SourceCode;

        std::string ProcessStage(Shader::ShaderStage stage, const std::filesystem::path& path);
    };
}