#include "Proofprch.h"
#include "VulkanShader.h"
#include <fstream>
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
namespace Proof
{
    namespace Utils
    {
        static const char* GetCacheDirectory() {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/VULKAN";
        }

        static const char* GetVulkanCachedShaderExtension(Shader::ShaderStage stage) {
            switch (stage) {
            case Shader::ShaderStage::Vertex: return ".cach_vulkan_shader.vertex";
            case Shader::ShaderStage::Fragment: return ".cach_vulkan_shader.fragment";
            }
            PF_CORE_ASSERT(false);
            return "";
        }
        static void CreateCacheDirectoryIfNeeded() {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }
        static shaderc_shader_kind  ShaderStageToShaderC(Shader::ShaderStage stage) {
            switch (stage) {
            case Shader::ShaderStage::Vertex:   return shaderc_glsl_vertex_shader;
            case Shader::ShaderStage::Fragment: return shaderc_glsl_fragment_shader;
            }
            PF_CORE_ASSERT(false);
            return (shaderc_shader_kind)0;
        }
    }

    std::unordered_map<Shader::ShaderStage, std::string> VulkanShader::PreProcess(const std::filesystem::path& filePath) {

        bool VertexChecked = false;
        bool FragmentChecked = false;
        std::ifstream ShaderFile;
        std::string VertexSource;
        std::string FragmentSource;
        ShaderFile.open(filePath);

        if (ShaderFile.is_open() == true) {
            std::string Line;
            while (std::getline(ShaderFile, Line)) {
                if (Line == "#Fragment Shader")
                    VertexChecked = false;
                if (VertexChecked == true) {
                    VertexSource += Line;
                    VertexSource += "\n";
                }
                if (FragmentChecked == true) {
                    FragmentSource += Line;
                    FragmentSource += "\n";
                }
                if (Line == "#Vertex Shader")
                    VertexChecked = true;
                if (Line == "#Fragment Shader")
                    FragmentChecked = true;
            }
            ShaderFile.close();
        }

        std::unordered_map<Shader::ShaderStage, std::string> temp;
        temp[Shader::ShaderStage::Vertex] = VertexSource;
        temp[Shader::ShaderStage::Fragment] = FragmentSource;
        return temp;
    }
    VulkanShader::VulkanShader(const std::string& name, const std::filesystem::path& filePath) {
        m_Path = filePath.string();
        Utils::CreateCacheDirectoryIfNeeded();
        m_SourceCode = PreProcess(filePath);
        Compile(filePath);
        CreateShader();
    }

    VulkanShader::~VulkanShader() {
        for (int i = 0; i < 2; i++) {
          //  vkDestroyShaderModule(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_ShaderModule[i], nullptr);
        }
    }
    void VulkanShader::CompileOrGetBinaries(const std::filesystem::path& filePath) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
        const bool optimize = true;
        if (optimize)
            compilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
        auto& shaderData = m_VulkanSPIRV;
        shaderData.clear();

        for (auto& [stage, source] : m_SourceCode) {
            std::filesystem::path shaderFilePath = filePath;
            std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GetVulkanCachedShaderExtension(stage));

            std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
            if (in.is_open()) {
                in.seekg(0, std::ios::end);
                auto& data = shaderData[stage];

                size_t fileSize = size_t(in.tellg());

                data.resize(fileSize);
                in.seekg(0); // seek go to start of file so we can read the data
                in.read((char*)data.data(), fileSize);
                in.close();
            }
            else {
                shaderc::SpvCompilationResult shaderModule = compiler.CompileGlslToSpv(source, Utils::ShaderStageToShaderC(stage), shaderFilePath.string().c_str(), compilerOptions);
                if (shaderModule.GetCompilationStatus() != shaderc_compilation_status_success) {
                    PF_ENGINE_ERROR("Shader Stage:: {}  Error:: {}", EnumReflection::EnumString<Shader::ShaderStage>(stage), shaderModule.GetErrorMessage());
                    PF_CORE_ASSERT(false);
                }
                shaderData[stage] = std::vector<uint32_t>(shaderModule.cbegin(), shaderModule.cend());
                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open()) {
                    auto& data = shaderData[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }
        for (auto&& [stage, data] : m_SourceCode)
            Reflect(stage);
    }

    void VulkanShader::Compile(const std::filesystem::path& filePath) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
        const bool optimize = true;
        if (optimize)
            compilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

        auto& shaderData = m_VulkanSPIRV;
        shaderData.clear();
        for (auto& [stage, source] : m_SourceCode) {
            shaderc::SpvCompilationResult shaderModule = compiler.CompileGlslToSpv(source, Utils::ShaderStageToShaderC(stage), filePath.string().c_str(), compilerOptions);
            if (shaderModule.GetCompilationStatus() != shaderc_compilation_status_success) {
                PF_ENGINE_ERROR("Shader Stage:: {}  Error:: {}", EnumReflection::EnumString<Shader::ShaderStage>(stage), shaderModule.GetErrorMessage());
                PF_CORE_ASSERT(false);
            }
            shaderData[stage] = std::vector<uint32_t>(shaderModule.cbegin(), shaderModule.cend());
        }
        for (auto&& [stage, data] : m_VulkanSPIRV)
            Reflect(stage);
    }

    void VulkanShader::CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        //createInfo.codeSize = code.size();
        createInfo.codeSize = code.size() * sizeof(uint32_t);// because spirv module needs mutliple of 4
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        if (vkCreateShaderModule(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
            PF_CORE_ASSERT(false, "Failed To Create Shader Module");

    }
    void VulkanShader::Reflect(Shader::ShaderStage stage) {
        if (m_VulkanSPIRV.find(stage) == m_VulkanSPIRV.end()) {
            PF_ENGINE_ERROR("{} {} Shader stage does not exist", m_Name, EnumReflection::EnumString(stage));
            return;
        }
        auto& data = m_VulkanSPIRV.at(stage);
        auto& shaderSrc = m_SourceCode.at(stage);

        spirv_cross::Compiler compiler(data);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        PF_ENGINE_TRACE("{} Vulkan::Shader Reflect - {} ", m_Name, fmt::format(EnumReflection::EnumString(stage)));
        PF_ENGINE_INFO("{}\n", shaderSrc);
        PF_ENGINE_TRACE("    {} uniform buffers", resources.uniform_buffers.size());
        PF_ENGINE_TRACE("    {} sampled images", resources.sampled_images.size());
        PF_ENGINE_TRACE("    {} push constant buffers  ", resources.push_constant_buffers.size());
        PF_ENGINE_TRACE("    {} storage buffers  ", resources.storage_buffers.size());
        PF_ENGINE_TRACE("Uniform buffers:");

        for (const auto& resource : resources.uniform_buffers) {
            const auto& bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            int memberCount = bufferType.member_types.size();

            PF_ENGINE_TRACE("  {}", resource.name);
            PF_ENGINE_TRACE("    Size = {}", bufferSize);
            PF_ENGINE_TRACE("    Binding = {}", binding);
            PF_ENGINE_TRACE("    Members = {}", memberCount);
        }

        PF_ENGINE_TRACE("Sampled images:");
        for (const auto& resource : resources.sampled_images) {
            PF_ENGINE_TRACE("   {}", resource.name);
            PF_ENGINE_TRACE("   ID = {}", resource.id);
        }

        PF_ENGINE_TRACE("push constant buffers:");
        for (const auto& resource : resources.push_constant_buffers) {
            PF_ENGINE_TRACE("   {}", resource.name);
            PF_ENGINE_TRACE("   ID = {}", resource.id);
        }

        PF_ENGINE_TRACE("storage constant buffers:");
        for (const auto& resource : resources.storage_buffers) {
            PF_ENGINE_TRACE("   {}", resource.name);
            PF_ENGINE_TRACE("   ID = {}", resource.id);
        }
    }

    void VulkanShader::CreateShader() {
        auto& vertCode = m_VulkanSPIRV[Shader::ShaderStage::Vertex];
        auto& fragCode = m_VulkanSPIRV[Shader::ShaderStage::Fragment];
        CreateShaderModule(vertCode, &m_ShaderModule[(int)Shader::ShaderStage::Vertex]);
        CreateShaderModule(fragCode, &m_ShaderModule[(int)Shader::ShaderStage::Fragment]);
        m_ShaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_ShaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        m_ShaderStages[0].module = m_ShaderModule[(int)Shader::ShaderStage::Vertex];
        m_ShaderStages[0].pName = "main"; // main funciton
        m_ShaderStages[0].flags = 0;
        m_ShaderStages[0].pNext = nullptr;
        m_ShaderStages[0].pSpecializationInfo = nullptr;

        m_ShaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_ShaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        m_ShaderStages[1].module = m_ShaderModule[(int)Shader::ShaderStage::Fragment];
        m_ShaderStages[1].pName = "main";
        m_ShaderStages[1].flags = 0;
        m_ShaderStages[1].pNext = nullptr;
        m_ShaderStages[1].pSpecializationInfo = nullptr;
    }
}