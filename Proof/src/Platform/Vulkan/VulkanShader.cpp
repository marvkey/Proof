#include "Proofprch.h"
#include "VulkanShader.h"
#include <fstream>
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/RendererBase.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include "Proof/Renderer/Renderer.h"
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

    void VulkanShader::Release()
    {
        for (auto& [data, shaderModule] : m_ShaderModule)
        {
            Renderer::SubmitDatafree([shaderModuler =shaderModule] {
                vkDestroyShaderModule(Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice(), shaderModuler, nullptr);
            });
        }
    }

    VulkanShader::VulkanShader(const std::string& name, const std::filesystem::path& filePath) {
        m_Name = name;
        m_ConstructorSamePaths = true;
        magic_enum::enum_for_each<ShaderStage>([&](ShaderStage stage) {
            m_Paths[stage] = filePath.string();
            std::string source = ProcessStage(stage, filePath);
            if (source.empty() == false)
                m_SourceCode.insert({ stage,source });
        });
        Utils::CreateCacheDirectoryIfNeeded();
        Compile();
        CreateShader();
    }

    VulkanShader::VulkanShader(const std::string& name, const std::unordered_map<ShaderStage, std::string> shaders) {
        m_Name = name;
        for (auto& [stage, path] : shaders) {
            std::ifstream shaderFile;
            shaderFile.open(path);
            if (shaderFile.is_open() == false)
            {
                PF_ENGINE_ERROR("cannot open shader path to read {}", path);
                PF_CORE_ASSERT(false);
            }
            std::string line;
            std::string source;
            while (std::getline(shaderFile, line))
            {
                source += line;
                source += "\n";
            }
            shaderFile.close();
            m_SourceCode.insert({ stage,source });
        }
        Utils::CreateCacheDirectoryIfNeeded();
        Compile();
        CreateShader();
    }
    VulkanShader::~VulkanShader() {
        Release();
    }
    void VulkanShader::CompileOrGetBinaries(const std::filesystem::path& filePath) {
		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();

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
    class ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
    {
        //https://github.com/beaumanvienna/vulkan/blob/617f70e1a311c6f498ec69507dcc9d4aadb86612/engine/platform/Vulkan/VKshader.cpp
        shaderc_include_result* GetInclude(const char* requestedSource, shaderc_include_type type, const char* requestingSource, size_t includeDepth) override {
            std::string msg = std::string(requestingSource);
            msg += std::to_string(type);
            msg += static_cast<char>(includeDepth);

            const std::string name = std::string(requestedSource);
            const std::string contents = ReadFile(name);

            auto container = new std::array<std::string, 2>;
            (*container)[0] = name;
            (*container)[1] = contents;

            auto data = new shaderc_include_result;

            data->user_data = container;

            data->source_name = (*container)[0].data();
            data->source_name_length = (*container)[0].size();

            data->content = (*container)[1].data();
            data->content_length = (*container)[1].size();

            return data;
        }
        void ReleaseInclude(shaderc_include_result* data) override {
            delete static_cast<std::array<std::string, 2>*>(data->user_data);
            delete data;
        }
        std::string ReadFile(const std::string& filepath)
        {
            
            std::string truePath = ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/" + filepath;
            std::string sourceCode;
            std::ifstream in(truePath, std::ios::in | std::ios::binary);
            if (in)
            {
                in.seekg(0, std::ios::end);
                size_t size = in.tellg();
                if (size > 0)
                {
                    sourceCode.resize(size);
                    in.seekg(0, std::ios::beg);
                    in.read(&sourceCode[0], size);
                }
                else
                {
                    PF_ENGINE_WARN("ShaderIncluder::ReadFile: Could not read shader file '{0}'", truePath);
                }
            }
            else
            {
                PF_ENGINE_WARN("ShaderIncluder::ReadFile Could not open shader file '{0}'", truePath);
            }
            return sourceCode;
        }
    };
    void VulkanShader::Compile() {
        auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();

        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, graphicsContext->GetVulkanVersion());
        const bool optimize = true;
        if (optimize)
            compilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);


        Special<shaderc::CompileOptions::IncluderInterface> inclue = CreateSpecial< ShaderIncluder>();
        compilerOptions.SetIncluder(std::move(inclue));

        auto& shaderData = m_VulkanSPIRV;
        shaderData.clear();
        for (auto& [stage, source] : m_SourceCode) {
            shaderc::SpvCompilationResult shaderModule = compiler.CompileGlslToSpv(source, Utils::ShaderStageToShaderC(stage), m_Name.c_str(), compilerOptions);
            if (shaderModule.GetCompilationStatus() != shaderc_compilation_status_success) {
                uint32_t num = 1;
                PF_ENGINE_ERROR("Shader Stage:: {}  Error:: {}", EnumReflection::EnumString<ShaderStage>(stage), shaderModule.GetErrorMessage());
                std::istringstream iss(source);
                for (std::string line; std::getline(iss, line); )
                {
                    PF_ENGINE_TRACE("{} {}", num, line);
                    num++;
                }
                PF_CORE_ASSERT(false);
            }
            shaderData[stage]; 
            shaderData[stage] = std::vector<uint32_t>(shaderModule.cbegin(), shaderModule.cend());
        }
        for (auto&& [stage, data] : m_VulkanSPIRV)
            Reflect(stage);
    }

    

    void VulkanShader::Reload()
    {
        if (m_ConstructorSamePaths)
        {
            m_SourceCode.clear();
            magic_enum::enum_for_each<ShaderStage>([&](ShaderStage stage) {
                std::string source = ProcessStage(stage, m_Paths[stage]);
                if (source.empty() == false)
                    m_SourceCode.insert({ stage,source });
            });
            Utils::CreateCacheDirectoryIfNeeded();
            Compile();
            CreateShader();
            return;
        }
        PF_CORE_ASSERT(false, "cannot reload because you did not use path constructuro");
    }

    void VulkanShader::CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size() * sizeof(uint32_t);// because spirv module needs mutliple of 4
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        if (vkCreateShaderModule(RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
            PF_CORE_ASSERT(false, "Failed To Create Shader Module");

    }
    void VulkanShader::Reflect(ShaderStage stage) {
        return;
        if (m_VulkanSPIRV.find(stage) == m_VulkanSPIRV.end()) {
            PF_ENGINE_ERROR("{} {} Shader stage does not exist", m_Name, EnumReflection::EnumString(stage));
            return;
        }
        auto& data = m_VulkanSPIRV.at(stage);
        auto& shaderSrc = m_SourceCode.at(stage);

        spirv_cross::Compiler compiler(data);

        //this line is crahsing for some reason
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
                    break;
                }
            case Proof::ShaderStage::Fragment:
                {
                    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                    shaderStageInfo.module = shaderModule;
                    shaderStageInfo.pName = "main";
                    shaderStageInfo.flags = 0;
                    shaderStageInfo.pNext = nullptr;
                    shaderStageInfo.pSpecializationInfo = nullptr;
                    break;
                }
            case Proof::ShaderStage::Geometry:
                {
                    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
                    shaderStageInfo.module = shaderModule;
                    shaderStageInfo.pName = "main";
                    shaderStageInfo.flags = 0;
                    shaderStageInfo.pNext = nullptr;
                    shaderStageInfo.pSpecializationInfo = nullptr;
                    break;
                }
            case Proof::ShaderStage::Compute:
                {
                    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
                    shaderStageInfo.module = shaderModule;
                    shaderStageInfo.pName = "main";
                    shaderStageInfo.flags = 0;
                    shaderStageInfo.pNext = nullptr;
                    shaderStageInfo.pSpecializationInfo = nullptr;
                    break;
                }
            }
            m_ShaderModule.insert({ shaderStage,{shaderModule} });
            m_ShaderStages.emplace_back(shaderStageInfo);
        }
    }
}