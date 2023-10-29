#include "Proofprch.h"
#include "VulkanShader.h"

#include "Proof/Renderer/Renderer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "Proof/Math/Random.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer.h"
#include "VulkanDevice.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <fstream>
#include <fmt/format.h>

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

    

    const VkWriteDescriptorSet* VulkanShader::GetDescriptorSet(uint32_t set) const
    {
        return nullptr;
    }

    void VulkanShader::Release()
    {
        for (auto& [data, shaderModule] : m_ShaderModule)
        {
            Renderer::SubmitResourceFree([shaderModuler =shaderModule] {
                if(shaderModuler)
                    vkDestroyShaderModule(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice(), shaderModuler, nullptr);
            });
            shaderModule = nullptr;
        }
        VkDevice device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

        for (auto& [set, shaderResouce] : m_DescriptorResource)
        {
            Renderer::SubmitResourceFree([shaderResouce = shaderResouce, device = device]()
            {
                if(shaderResouce.Layout)
                    vkDestroyDescriptorSetLayout(device, shaderResouce.Layout, nullptr);
                if (shaderResouce.Set)
                    vkFreeDescriptorSets(device, shaderResouce.Pool, 1, &shaderResouce.Set);
                if(shaderResouce.Pool)
                    vkDestroyDescriptorPool(device, shaderResouce.Pool, nullptr);
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
        Compile(m_SourceCode);
        CreateShader();
        m_InitialCompile = false;
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
        Compile(m_SourceCode);
        CreateShader();
        m_InitialCompile = false;
    }
    VulkanShader::~VulkanShader() {
        Release();
    }
    void VulkanShader::CompileOrGetBinaries(const std::filesystem::path& filePath) {

        PF_CORE_ASSERT(false, "Function may not be fineshes yet");		
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, graphicsContext->GetVulkanVersion());
        
        const bool optimize = false;/// so we can load the shader attributes
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

            auto container = pnew std::array<std::string, 2>;
            (*container)[0] = name;
            (*container)[1] = contents;

            auto data = pnew shaderc_include_result;

            data->user_data = container;

            data->source_name = (*container)[0].data();
            data->source_name_length = (*container)[0].size();

            data->content = (*container)[1].data();
            data->content_length = (*container)[1].size();

            return data;
        }
        void ReleaseInclude(shaderc_include_result* data) override {
            pdelete static_cast<std::array<std::string, 2>*>(data->user_data);
            pdelete data;
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
    bool VulkanShader::Compile(const std::unordered_map<ShaderStage, std::string>& sourceCode) {

        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

        shaderc::Compiler compiler;
        shaderc::CompileOptions compilerOptions;
        
        compilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, graphicsContext->GetVulkanVersion());
        const bool optimize = false;/// so we can load the shader attributes
        if (optimize)
            compilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);


        Special<shaderc::CompileOptions::IncluderInterface> include = CreateSpecial< ShaderIncluder>();
        compilerOptions.SetIncluder(std::move(include));

        for(auto [name, value] : Renderer::GetShaderDefines())
            compilerOptions.AddMacroDefinition(name.c_str(), name.size(), value.c_str(),value.size());

        // do not want to change any of the current shader properteis excpet if the shader has been compiled properly
        std::unordered_map<ShaderStage, shaderc::SpvCompilationResult> shaderModules;
        for (auto& [stage, source] : sourceCode) {
            
            shaderModules[stage] = compiler.CompileGlslToSpv(source, Utils::ShaderStageToShaderC(stage), m_Name.c_str(), compilerOptions);
            auto& shaderModule = shaderModules[stage];

            if (shaderModule.GetCompilationStatus() != shaderc_compilation_status_success) {
                uint32_t num = 1;
                
                PF_ENGINE_ERROR("Shader Stage:: {}  Error:: {}", EnumReflection::EnumString<ShaderStage>(stage), shaderModule.GetErrorMessage());
                std::istringstream iss(source);
                for (std::string line; std::getline(iss, line); )
                {
                    PF_ENGINE_TRACE("{} {}", num, line);
                    num++;
                }
                if(m_InitialCompile)
                    PF_CORE_ASSERT(false);
                
                return false;
            }
           
        }

        auto& shaderData = m_VulkanSPIRV;
        shaderData.clear();
        for (auto& [stage, shaderModule] : shaderModules)
        {
            shaderData[stage] = std::vector<uint32_t>(shaderModule.cbegin(), shaderModule.cend());
        }
        m_InputDeclaration.clear();
        for (auto&& [stage, data] : m_VulkanSPIRV)
            Reflect(stage);
        return true;
    }

    

    void VulkanShader::Reload()
    {
        if (m_ConstructorSamePaths)
        {
            std::unordered_map<ShaderStage, std::string> shaderCode;
            magic_enum::enum_for_each<ShaderStage>([&](ShaderStage stage) {
                std::string source = ProcessStage(stage, m_Paths[stage]);
                if (source.empty() == false)
                    shaderCode.insert({ stage,source });
            });
            Utils::CreateCacheDirectoryIfNeeded();
            if (Compile(shaderCode))
            {
                // if shader is succesful then we can copy the source doe
                m_SourceCode = shaderCode;
                CreateShader();
                PF_ENGINE_INFO("Succesfully Compiled {} Shader", m_Name);

                for (auto& [index,callback] : m_ShaderReloads)
                    callback();
            }
            else
            {
                PF_ENGINE_ERROR("Failed Compiled {} Shader", m_Name);
            }

            return;
        }
        PF_CORE_ASSERT(false, "cannot reload because you did not use path constructuro");
    }

    const ShaderResourceBufferInfo* VulkanShader::GetPushConstantInput(const std::string& storageName, const std::string& name) const
    {
        if (m_PushConstantResourceInfo.contains(storageName))
        {
            if (m_PushConstantResourceInfo.at(storageName).second.contains(name))
            {
                return &m_PushConstantResourceInfo.at(storageName).second.at(name);
            }
        }
        return nullptr;
    }

    const SahderInputDeclaration* VulkanShader::GetInputDeclaration(std::string name)const
    {
        if (m_InputDeclaration.contains(name))
            return &m_InputDeclaration.at(name);
        return nullptr;
    }

    const ShaderResourceBufferInfo* VulkanShader::GetStorageBufferInput(const std::string& storageName, const std::string& name) const
    {
        if (m_StorageBufferResourceInfo.contains(storageName))
        {
            if (m_StorageBufferResourceInfo.at(storageName).second.contains(name))
            {
                return &m_StorageBufferResourceInfo.at(storageName).second.at(name);
            }
        }
        return nullptr;
    }

    uint32_t VulkanShader::AddShaderReloadCallback(const ShaderReloadCallback& callback)
    {
        if (m_ShaderReloads.size() == 0)
        {
            m_ShaderReloads[1] = callback;
            return 1;
        }
        // map ordered larges to smallest so basically we are getting the largest by going to the back
        uint32_t index = m_ShaderReloads.rbegin()->first + 1;
        m_ShaderReloads[index] = callback;
        return index;
    }

    void VulkanShader::RemoveShaderReloadCallback(uint32_t index)
    {
        if (m_ShaderReloads.contains(index))
        {
            m_ShaderReloads[index] = nullptr;
            m_ShaderReloads.erase(index);
        }
    }

    void VulkanShader::CreateShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size() * sizeof(uint32_t);// because spirv module needs mutliple of 4
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        if (vkCreateShaderModule(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
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

        //this line is crahsing for some reason
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        PF_ENGINE_TRACE("{} Vulkan::Shader Reflect - {} ", m_Name, EnumReflection::EnumString(stage));
        PF_ENGINE_INFO("{}\n", shaderSrc);
        PF_ENGINE_TRACE("    {} uniform buffers", resources.uniform_buffers.size());
        PF_ENGINE_TRACE("    {} sampled images", resources.sampled_images.size());
        PF_ENGINE_TRACE("    {} sotrage images", resources.storage_images.size());
        PF_ENGINE_TRACE("    {} push constant buffers  ", resources.push_constant_buffers.size());
        PF_ENGINE_TRACE("    {} storage buffers  ", resources.storage_buffers.size());

        PF_ENGINE_TRACE("Uniform buffers : ");
        for (const auto& resource : resources.uniform_buffers) {
            const auto& bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
            
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            int memberCount = bufferType.member_types.size();
            
            PF_ENGINE_TRACE("  {}", resource.name);
            PF_ENGINE_TRACE("    Size = {}", bufferSize);
            PF_ENGINE_TRACE("    Set = {}", descriptorSet);
            PF_ENGINE_TRACE("    Binding = {}", binding);
            PF_ENGINE_TRACE("    Members = {}", memberCount);
            auto& uniformBuffer = m_ShaderDescriptorSet[descriptorSet].UniformBuffers[binding];
            // make sure we only bidn to stages 
            uniformBuffer.Stage |= (int)Utils::ProofShaderToVulkanShader(stage);
            uniformBuffer = { resource.name,DescriptorResourceType::UniformBuffer,(int)uniformBuffer.Stage,1};
           // PF_CORE_ASSERT(!m_InputDeclaration.contains(resource.name),"Dont use the same name for shader data");
            m_InputDeclaration[resource.name] = { descriptorSet,binding };
        }
        PF_ENGINE_TRACE("Storage constant buffers:");
        for (const auto& resource : resources.storage_buffers)
        {
            const auto& bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);

            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            int memberCount = bufferType.member_types.size();

            PF_ENGINE_TRACE("  {}", resource.name);
            PF_ENGINE_TRACE("    Size = {}", bufferSize);
            PF_ENGINE_TRACE("    Set = {}", descriptorSet);
            PF_ENGINE_TRACE("    Binding = {}", binding);
            PF_ENGINE_TRACE("    Members = {}", memberCount);

            auto& storageBuffer = m_ShaderDescriptorSet[descriptorSet].StorageBuffers[binding];
            storageBuffer.Stage |= (int)Utils::ProofShaderToVulkanShader(stage);
            storageBuffer = { resource.name,DescriptorResourceType::StorageBuffer, (int)storageBuffer.Stage,1 };
            PF_CORE_ASSERT(!m_InputDeclaration.contains(resource.name), "Dont use the same name for shader data");
            m_InputDeclaration[resource.name] = { descriptorSet,binding };
        }
        PF_ENGINE_TRACE("push constant buffers:");
        for (const auto& resource : resources.push_constant_buffers)
        {
            const auto& bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);

            uint32_t offset = compiler.get_decoration(resource.id, spv::DecorationOffset);
            int memberCount = bufferType.member_types.size();

            std::unordered_map<std::string, ShaderResourceBufferInfo> resourceInputs;
            for (size_t i = 0; i < memberCount; ++i)
            {
                const auto& memberName = compiler.get_member_name(resource.base_type_id, i);
                const auto& memberSize = compiler.get_declared_struct_member_size(bufferType, i);
                auto memberOffset = compiler.get_member_decoration(resource.base_type_id, i, spv::DecorationOffset);
                auto newName = fmt::format("{}.{}", resource.name, memberName);
                resourceInputs[newName] = { (uint32_t)memberSize,(uint32_t)memberOffset };
            }
            m_PushConstantResourceInfo[resource.name] = { bufferSize, resourceInputs };
            uint32_t pushStage = (int)m_PushConstants[resource.name].stageFlags;
            pushStage |= (uint32_t) Utils::ProofShaderToVulkanShader(stage);
            m_PushConstants[resource.name] = VkPushConstantRange{pushStage, offset,bufferSize };

            PF_ENGINE_TRACE("  {}", resource.name);
            PF_ENGINE_TRACE("    Size = {}", bufferSize);
            PF_ENGINE_TRACE("    Members = {}", memberCount);
        }

        PF_ENGINE_TRACE("Sampled images:");
        for (const auto& resource : resources.sampled_images) {
           
            const auto& type = compiler.get_type(resource.type_id);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            auto& sampledImage = m_ShaderDescriptorSet[descriptorSet].ImageSamplers[binding];
            sampledImage.Stage |= (int)Utils::ProofShaderToVulkanShader(stage);

             //https://github.com/KhronosGroup/SPIRV-Cross/wiki/Reflection-API-user-guide
            //( why  we need  this)
            /**
             uniform sampler2D uSampler[10]; .
             for (const Resource &resource : res.sampled_images)
             {
                const SPIRType &type = comp.get_type(resource.type_id); // Notice how we're using type_id here because we need the array information and not decoration information.
                print(type.array.size()); // 1, because it's one dimension.
                print(type.array[0]); // 10
                print(type.array_size_literal[0]); // true
             }
             */
            uint32_t nImages = 0;
            uint32_t arraySize = type.array.size();
            for (int i = 0; i < arraySize; i++)
            {
                nImages += type.array[i];
            }


            if (nImages == 0)nImages = 1;
            sampledImage = { resource.name,DescriptorResourceType::ImageSampler, (int)sampledImage.Stage,nImages };
            PF_CORE_ASSERT(!m_InputDeclaration.contains(resource.name), "Dont use the same name for shader data");
            
            PF_ENGINE_TRACE("   {}", resource.name);
            PF_ENGINE_TRACE("   ID = {}", resource.id);
            PF_ENGINE_TRACE("   Count = {}", nImages);
            m_InputDeclaration[resource.name] = { descriptorSet,binding };
        }
        PF_ENGINE_TRACE("Storage images:");
        for (const auto& resource : resources.storage_images)
        {

            const auto& type = compiler.get_type(resource.type_id);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            auto& sampledImage = m_ShaderDescriptorSet[descriptorSet].StorageImages[binding];
            sampledImage.Stage |= (int)Utils::ProofShaderToVulkanShader(stage);

             //https://github.com/KhronosGroup/SPIRV-Cross/wiki/Reflection-API-user-guide
            //( why  we need  this)
            /**
             uniform sampler2D uSampler[10]; .
             for (const Resource &resource : res.sampled_images)
             {
                const SPIRType &type = comp.get_type(resource.type_id); // Notice how we're using type_id here because we need the array information and not decoration information.
                print(type.array.size()); // 1, because it's one dimension.
                print(type.array[0]); // 10
                print(type.array_size_literal[0]); // true
             }
             */
            uint32_t nImages = 0;
            uint32_t arraySize = type.array.size();
            for (int i = 0; i < arraySize; i++)
            {
                nImages += type.array[i];
            }
            if (nImages == 0)nImages = 1;

            sampledImage = { resource.name,DescriptorResourceType::ImageSampler, (int)sampledImage.Stage,nImages };
            PF_CORE_ASSERT(!m_InputDeclaration.contains(resource.name), "Dont use the same name for shader data");

            PF_ENGINE_TRACE("   {}", resource.name);
            PF_ENGINE_TRACE("   ID = {}", resource.id);
            PF_ENGINE_TRACE("   Count = {}", nImages);
            m_InputDeclaration[resource.name] = { descriptorSet,binding };
        }
        PF_ENGINE_TRACE("Seperate Samplers:");
        for (const auto& resource : resources.separate_samplers)
        {
            const auto& type = compiler.get_type(resource.type_id);

            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            auto& seperateSampler = m_ShaderDescriptorSet[descriptorSet].SeperateSamplers[binding];
            seperateSampler.Stage |= (int)Utils::ProofShaderToVulkanShader(stage);


             //https://github.com/KhronosGroup/SPIRV-Cross/wiki/Reflection-API-user-guide
            //( why  we need  this)
            /**
             uniform sampler2D uSampler[10]; .
             for (const Resource &resource : res.sampled_images)
             {
                const SPIRType &type = comp.get_type(resource.type_id); // Notice how we're using type_id here because we need the array information and not decoration information.
                print(type.array.size()); // 1, because it's one dimension.
                print(type.array[0]); // 10
                print(type.array_size_literal[0]); // true
             }
             */
            uint32_t nImages = 0;
            uint32_t arraySize = type.array.size();
            for (int i = 0; i < arraySize; i++)
            {
                nImages += type.array[i];
            }

            if (nImages == 0)nImages = 1;

            seperateSampler = { resource.name,DescriptorResourceType::ImageSampler, (int)seperateSampler.Stage,nImages };
            PF_CORE_ASSERT(!m_InputDeclaration.contains(resource.name), "Dont use the same name for shader data");

            PF_ENGINE_TRACE("   {}", resource.name);
            PF_ENGINE_TRACE("   ID = {}", resource.id);
            PF_ENGINE_TRACE("   Count = {}", nImages);
            m_InputDeclaration[resource.name] = { descriptorSet,binding };
        }

        PF_ENGINE_TRACE("Seperate Textures:");
        for (const auto& resource : resources.separate_images)
        {
            const auto& type = compiler.get_type(resource.type_id);

            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            auto& seperateTextures = m_ShaderDescriptorSet[descriptorSet].SeperateTextures[binding];
            seperateTextures.Stage |= (int)Utils::ProofShaderToVulkanShader(stage);

             //https://github.com/KhronosGroup/SPIRV-Cross/wiki/Reflection-API-user-guide
            //( why  we need  this)
            /**
             uniform sampler2D uSampler[10]; .
             for (const Resource &resource : res.sampled_images)
             {
                const SPIRType &type = comp.get_type(resource.type_id); // Notice how we're using type_id here because we need the array information and not decoration information.
                print(type.array.size()); // 1, because it's one dimension.
                print(type.array[0]); // 10
                print(type.array_size_literal[0]); // true
             }
             */
            uint32_t nImages = 0;
            uint32_t arraySize = type.array.size();
            for (int i = 0; i < arraySize; i++)
            {
                nImages += type.array[i];
            }
            if (nImages == 0)nImages = 1;

            seperateTextures = { resource.name,DescriptorResourceType::ImageSampler, (int)seperateTextures.Stage,nImages };
            PF_CORE_ASSERT(!m_InputDeclaration.contains(resource.name), "Dont use the same name for shader data");

            PF_ENGINE_TRACE("   {}", resource.name);
            PF_ENGINE_TRACE("   ID = {}", resource.id);
            PF_ENGINE_TRACE("   Count = {}", nImages);
            m_InputDeclaration[resource.name] = { descriptorSet,binding };
        }
        
        VkDevice device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
        m_TypeCounts.clear();
        for (auto& [set, shaderDescriptorSet] : m_ShaderDescriptorSet)
        {
            auto& shaderDescriptorSet = m_ShaderDescriptorSet[set];
            if (!shaderDescriptorSet) // empty
                continue;
            if (!shaderDescriptorSet.UniformBuffers.empty())
            {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                typeCount.descriptorCount = shaderDescriptorSet.UniformBuffers.size();
            }

            if (!shaderDescriptorSet.StorageBuffers.empty())
            {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                typeCount.descriptorCount = shaderDescriptorSet.StorageBuffers.size();
            }

            if (!shaderDescriptorSet.ImageSamplers.empty())
            {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                typeCount.descriptorCount = shaderDescriptorSet.ImageSamplers.size();
            }

            if (!shaderDescriptorSet.SeperateTextures.empty())
            {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                typeCount.descriptorCount = shaderDescriptorSet.SeperateTextures.size();
            }

            if (!shaderDescriptorSet.SeperateSamplers.empty())
            {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_SAMPLER;
                typeCount.descriptorCount = shaderDescriptorSet.SeperateSamplers.size();
            }

            if (!shaderDescriptorSet.StorageImages.empty())
            {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                typeCount.descriptorCount = shaderDescriptorSet.StorageImages.size();
            }
            VkDescriptorPoolCreateInfo descriptorPoolInfo{};
            descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            descriptorPoolInfo.pNext = nullptr;
            descriptorPoolInfo.poolSizeCount = m_TypeCounts[set].size();
            descriptorPoolInfo.pPoolSizes = m_TypeCounts[set].data();
            descriptorPoolInfo.maxSets = 1;
            //VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &m_DescriptorResource[set].Pool));
            
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            for (auto& [binding, uniformBuffer] : shaderDescriptorSet.UniformBuffers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = uniformBuffer.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[uniformBuffer.Name];
                writeDescritporSet = {};
                writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                writeDescritporSet.dstBinding = layoutBinding.binding;
            }

            for (auto& [binding, storageBuffer] : shaderDescriptorSet.StorageBuffers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = storageBuffer.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[storageBuffer.Name];
                writeDescritporSet = {};
                writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                writeDescritporSet.dstBinding = layoutBinding.binding;
            }

            for (auto& [binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                layoutBinding.descriptorCount = imageSampler.DescriptorCount;
                layoutBinding.stageFlags = imageSampler.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[imageSampler.Name];
                writeDescritporSet = {};
                writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                writeDescritporSet.dstBinding = layoutBinding.binding;
            }

            for (auto& [binding, seperateTexture] : shaderDescriptorSet.SeperateTextures)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                layoutBinding.descriptorCount = seperateTexture.DescriptorCount;
                layoutBinding.stageFlags = seperateTexture.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[seperateTexture.Name];
                writeDescritporSet = {};
                writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                writeDescritporSet.dstBinding = layoutBinding.binding;
            }

            for (auto& [binding, seperateSampler] : shaderDescriptorSet.SeperateSamplers)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                layoutBinding.descriptorCount = seperateSampler.DescriptorCount;
                layoutBinding.stageFlags = seperateSampler.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[seperateSampler.Name];
                writeDescritporSet = {};
                writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                writeDescritporSet.dstBinding = layoutBinding.binding;
            }

            for (auto& [binding, storageImage] : shaderDescriptorSet.StorageImages)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                layoutBinding.descriptorCount = storageImage.DescriptorCount;
                layoutBinding.stageFlags = storageImage.Stage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;
                //PF_CORE_ASSERT()

                VkWriteDescriptorSet& writeDescritporSet = shaderDescriptorSet.WriteDesriptorSet[storageImage.Name];
                writeDescritporSet = {};
                writeDescritporSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescritporSet.descriptorType = layoutBinding.descriptorType;
                writeDescritporSet.descriptorCount = layoutBinding.descriptorCount;
                writeDescritporSet.dstBinding = layoutBinding.binding;
            }

            VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo{};
            descriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorLayoutInfo.pNext = nullptr;
            descriptorLayoutInfo.bindingCount = layoutBindings.size();
            descriptorLayoutInfo.pBindings = layoutBindings.data();
            // have to allocate because of graphics pipline needs set layout to to be used
            VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayoutInfo, nullptr, &m_DescriptorResource[set].Layout));

            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.pNext = nullptr;
            allocInfo.pSetLayouts = &m_DescriptorResource[set].Layout;
            allocInfo.descriptorPool = m_DescriptorResource[set].Pool;
            allocInfo.descriptorSetCount = 1;
           // VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &m_DescriptorResource[set].Set));
        }
        // we need every descriptor to have a layout for shader reasons 
        for (int set = 0; set < 4; set++)
        {
            if (!m_DescriptorResource.contains(set))
            {
                VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo{};
                descriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                descriptorLayoutInfo.pNext = nullptr;
                descriptorLayoutInfo.bindingCount = 0;
                descriptorLayoutInfo.pBindings = nullptr;
                VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayoutInfo, nullptr, &m_DescriptorResource[set].Layout));

            }
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
            m_ShaderStages[shaderStage] = shaderStageInfo;
        }
    }
}