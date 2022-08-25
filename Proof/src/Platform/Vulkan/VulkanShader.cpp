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
            PF_CORE_ASSERT(false, "Invalid Shader Stage");
            return "";
        }
        static void CreateCacheDirectoryIfNeeded() {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }
        static shaderc_shader_kind ShaderStageToShaderC(Shader::ShaderStage stage) {
            switch (stage) {
            case Shader::ShaderStage::Vertex:   return shaderc_glsl_vertex_shader;
            case Shader::ShaderStage::Fragment: return shaderc_glsl_fragment_shader;
            }
            PF_CORE_ASSERT(false,"Invalid Shader stage");
            return (shaderc_shader_kind)0;
        }

     
    }

    std::unordered_map<Shader::ShaderStage, std::string> VulkanShader::PreProcess(const std::filesystem::path& filePath) {

        std::string vertexShaderSymbol = "#Vertex Shader";
        std::string fragmentShaderSymbol = "#Fragment Shader";

        std::string vertexSource;
        std::string fragmentSource;
        
        bool hasVertex = false;
        bool hasFragment = false;

        std::ifstream shaderFile;
        shaderFile.open(filePath);
        if (shaderFile.is_open() == false) {
            PF_ENGINE_ERROR("cannot open shader path to read {}", filePath.string());
            PF_CORE_ASSERT(false);
            return {};
        }
        std::string Line;
        while (std::getline(shaderFile, Line)) {
            if (Line == vertexShaderSymbol) {
                hasVertex = true;
                hasFragment = false;
                continue;
            }

            if (Line == fragmentShaderSymbol) {
                hasFragment = true;
                hasVertex = false;
                continue;
            }

            if (hasVertex == true) {
                vertexSource += Line;
                vertexSource += "\n";
                continue;
            }

            if (hasFragment == true) {
                fragmentSource += Line;
                fragmentSource += "\n";
                continue;
            }
        }
        shaderFile.close();

        std::unordered_map<Shader::ShaderStage, std::string> temp;

        if(vertexSource.empty() == false)
            temp[Shader::ShaderStage::Vertex] = vertexSource;
        if (fragmentSource.empty() == false)
            temp[Shader::ShaderStage::Fragment] = fragmentSource;

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
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, graphicsContext->GetVulkanVersion());
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
                    PF_ENGINE_ERROR("Shader Stage:: {} \n ErrorType {} \n Error Message:: {}",EnumReflection::EnumString(shaderModule.GetCompilationStatus()), EnumReflection::EnumString(stage), shaderModule.GetErrorMessage());
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
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, graphicsContext->GetVulkanVersion());
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

        PF_ENGINE_TRACE("{} Vulkan::Shader Reflect - {} ", m_Name, EnumReflection::EnumString(stage));
        PF_ENGINE_INFO("{}\n", shaderSrc);
        PF_ENGINE_TRACE("    {} uniform buffers", resources.uniform_buffers.size());
        PF_ENGINE_TRACE("    {} sampled images", resources.sampled_images.size());
        PF_ENGINE_TRACE("    {} push constant buffers  ", resources.push_constant_buffers.size());
        PF_ENGINE_TRACE("    {} storage buffers  ", resources.storage_buffers.size());
        PF_ENGINE_TRACE("Uniform buffers : ");

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
        for (auto& [shaderStage,code] : m_VulkanSPIRV) {
            VkShaderModule shaderModule;
            VkPipelineShaderStageCreateInfo shaderStageInfo;
            CreateShaderModule(code, &shaderModule);
            
            switch (shaderStage) {
            case Proof::Shader::ShaderStage::Vertex:
                {
                    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                    shaderStageInfo.module = shaderModule;
                    shaderStageInfo.pName = "main"; // main funciton
                    shaderStageInfo.flags = 0;
                    shaderStageInfo.pNext = nullptr;
                    shaderStageInfo.pSpecializationInfo = nullptr;
                }
                break;
            case Proof::Shader::ShaderStage::Fragment:
                {
                    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                    shaderStageInfo.module = shaderModule;
                    shaderStageInfo.pName = "main";
                    shaderStageInfo.flags = 0;
                    shaderStageInfo.pNext = nullptr;
                    shaderStageInfo.pSpecializationInfo = nullptr;
                }
                break;
            default:
                break;
            }
            m_ShaderModule.insert({ shaderStage,{shaderModule} });
            m_ShaderStages.emplace_back(shaderStageInfo);
        }
    }
}