#include "Proofprch.h"
#include "VulkanShader.h"
#include <fstream>
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/RendererBase.h"

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

        static const char* GetVulkanCachedShaderExtension(ShaderStage stage) {
            switch (stage) {
                case ShaderStage::Vertex: return ".cach_vulkan_shader.vertex";
                case ShaderStage::Fragment: return ".cach_vulkan_shader.fragment";
                case ShaderStage::Compute: return ".cach_vulkan_shader.compute";
                case ShaderStage::Geometry: return ".cach_vulkan_shader.geometry";
            }
            PF_CORE_ASSERT(false, "Invalid Shader Stage");
            return "";
        }
        static void CreateCacheDirectoryIfNeeded() {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }
        static shaderc_shader_kind ShaderStageToShaderC(ShaderStage stage) {
            switch (stage) {
                case ShaderStage::Vertex:   return shaderc_glsl_vertex_shader;
                case ShaderStage::Fragment: return shaderc_glsl_fragment_shader;
                case ShaderStage::Compute: return shaderc_glsl_compute_shader;
                case ShaderStage::Geometry: return shaderc_glsl_geometry_shader;
            }
            PF_CORE_ASSERT(false,"Invalid Shader stage");
            return (shaderc_shader_kind)0;
        }

     
    }

    std::string VulkanShader::ProcessStage(ShaderStage stage, const std::filesystem::path& path) {
        std::string symbol;
        switch (stage) {
            case Proof::ShaderStage::Vertex:
                symbol = "#Vertex Shader";
                break;
            case Proof::ShaderStage::Fragment:
                symbol = "#Fragment Shader";
                break;
            case Proof::ShaderStage::Compute:
                symbol = "#Compute Shader";
                break;
            case Proof::ShaderStage::Geometry:
                symbol = "#Geometry Shader";
                break;
            default:
                return "";
        }
        std::ifstream shaderFile;
        shaderFile.open(path);
        if (shaderFile.is_open() == false) {
            PF_ENGINE_ERROR("cannot open shader path to read {}", path.string());
            PF_CORE_ASSERT(false);
            return "";
        }
        std::string line;
        bool enableread = false;
        std::string source;
        while (std::getline(shaderFile, line)) {
            if (line == symbol) {
                enableread = true;
                continue;
            }

            if ((line == "#Vertex Shader" || line == "#Fragment Shader" || line == "#Compute Shader" || line == "#Geometry Shader")&& enableread)
                break;
            if (enableread) {
                source += line;
                source += "\n";
            }
        }
        shaderFile.close();
        return source;
    }
   


    VulkanShader::VulkanShader(const std::string& name, const std::filesystem::path& filePath) {
       
        magic_enum::enum_for_each<ShaderStage>([&](ShaderStage stage) {
            std::string source = ProcessStage(stage, filePath);
            if (source.empty() == false)
                m_SourceCode.insert({ stage,source });
        });
        Utils::CreateCacheDirectoryIfNeeded();
        Compile();
        CreateShader();
    }

    VulkanShader::VulkanShader(const std::string& name, const std::unordered_map<ShaderStage, std::string> shaders) {
        for (auto& [stage, source] : shaders) {
            m_SourceCode.insert({ stage,source });
        }
        Utils::CreateCacheDirectoryIfNeeded();
        Compile();
        CreateShader();
    }
    VulkanShader::~VulkanShader() {
        for (int i = 0; i < 2; i++) {
          //  vkDestroyShaderModule(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_ShaderModule[i], nullptr);
        }
    }
    void VulkanShader::CompileOrGetBinaries(const std::filesystem::path& filePath) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

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

    void VulkanShader::Compile() {
        auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, graphicsContext->GetVulkanVersion());
        const bool optimize = true;
        if (optimize)
            compilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

        auto& shaderData = m_VulkanSPIRV;
        shaderData.clear();
        for (auto& [stage, source] : m_SourceCode) {
            shaderc::SpvCompilationResult shaderModule = compiler.CompileGlslToSpv(source, Utils::ShaderStageToShaderC(stage), m_Paths[stage].c_str(), compilerOptions);
            if (shaderModule.GetCompilationStatus() != shaderc_compilation_status_success) {
                PF_ENGINE_ERROR("Shader Stage:: {}  Error:: {}", EnumReflection::EnumString<ShaderStage>(stage), shaderModule.GetErrorMessage());
                PF_ENGINE_TRACE("   {}", source);
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
        if (vkCreateShaderModule(RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
            PF_CORE_ASSERT(false, "Failed To Create Shader Module");

    }
    void VulkanShader::Reflect(ShaderStage stage) {
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
            case Proof::ShaderStage::Vertex:
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
            case Proof::ShaderStage::Fragment:
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
            case Proof::ShaderStage::Geometry:
                {
                    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
                    shaderStageInfo.module = shaderModule;
                    shaderStageInfo.pName = "main";
                    shaderStageInfo.flags = 0;
                    shaderStageInfo.pNext = nullptr;
                    shaderStageInfo.pSpecializationInfo = nullptr;
                }
                break;
            case Proof::ShaderStage::Compute:
                {
                    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
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