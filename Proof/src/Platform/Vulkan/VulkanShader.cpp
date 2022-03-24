#include "Proofprch.h"
#include "VulkanShader.h"
#include <fstream>
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
    VulkanShader::VulkanShader(const std::string& _NameShader, const std::string& VertexPath, const std::string& FragmentPath) {
        auto vertCode = ReadFile(VertexPath);
        auto fragCode = ReadFile(FragmentPath);
        CreateShaderModule(vertCode, &m_VertexShaderModule);
        CreateShaderModule(fragCode, &m_FragmentShaderModule);
        m_ShaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_ShaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        m_ShaderStages[0].module = m_VertexShaderModule;
        m_ShaderStages[0].pName = "main"; // main funciton
        m_ShaderStages[0].flags = 0;
        m_ShaderStages[0].pNext = nullptr;
        m_ShaderStages[0].pSpecializationInfo = nullptr;

        m_ShaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_ShaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        m_ShaderStages[1].module = m_FragmentShaderModule;
        m_ShaderStages[1].pName = "main";
        m_ShaderStages[1].flags = 0;
        m_ShaderStages[1].pNext = nullptr;
        m_ShaderStages[1].pSpecializationInfo = nullptr;
    }
    VulkanShader::~VulkanShader() {
        //vkDestroyShaderModule(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_VertexShaderModule, nullptr);
        //vkDestroyShaderModule(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_FragmentShaderModule, nullptr);
        //m_VertexShaderModule = VK_NULL_HANDLE;
        //m_FragmentShaderModule = VK_NULL_HANDLE;
    }
    std::vector<char> VulkanShader::ReadFile(const std::string& filePath) {
        // ate = when open go to end imediately, helps us get size more convieneinet
        // binary  =
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (file.is_open() == false)
            PF_ASSERT(false, "failed to open file, %s", filePath);
        // cause of at we are at last position so tellg will give us size
        size_t fileSize = size_t(file.tellg()); 
        std::vector<char>  buffer(fileSize);
        file.seekg(0); // seek go to start of file so we can read the data
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
 
    void VulkanShader::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        if (vkCreateShaderModule(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
            PF_CORE_ASSERT(false, "Failed To Create Shader Module");

    }
}
