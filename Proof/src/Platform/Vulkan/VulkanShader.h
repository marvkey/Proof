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
        ~VulkanShader();
        virtual const std::string& GetPath()const { return m_Path; }
        virtual const std::string& GetName()const { return m_Name; }
        void Reflect(Shader::ShaderStage stage);
    private:
        std::unordered_map<Shader::ShaderStage, std::string> PreProcess(const std::filesystem::path& filePath);
        static void CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule);
        void CreateShader();
        void CompileOrGetBinaries(const std::filesystem::path& filePath);
        void Compile(const std::filesystem::path& filePath);
		VkShaderModule m_VertexShaderModule;
		VkShaderModule m_FragmentShaderModule;
		VkPipelineShaderStageCreateInfo m_ShaderStages[2];
		friend class VulkanGraphicsPipeline;
        std::string m_Path = "gg";
        std::string m_Name ="empty ";
        std::unordered_map<Shader::ShaderStage, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<Shader::ShaderStage, std::string> m_SourceCode;
	};
}