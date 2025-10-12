#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Shader.h"
#include "Vulkan.h"
#include "VulkanShaderResource.h"

#include <map>
namespace Proof
{
    enum class ShaderResourceBufferVarType
    {
        None,

        // Scalars
        Bool, Int, Uint, Int64, Uint64, Float, Double,

        // Vectors
        BVec2, BVec3, BVec4,
        IVec2, IVec3, IVec4,
        UVec2, UVec3, UVec4,
        Vec2, Vec3, Vec4,
        DVec2, DVec3, DVec4,

        // Matrices (float)
        Mat2, Mat3, Mat4,
        // Matrices (double)
        DMat2, DMat3, DMat4,

        // Special types
        Sampler,
        SampledImage,
        Image,
        AccelerationStructure,
        RayQuery,
        Struct,
        AtomicCounter,
        // Half, Short, UShort, SByte, UByte — optional for now
    };

    struct ShaderResoureData {
        uint32_t Set = 0;
        uint32_t Binding = 0;
    };

    struct ShaderResourceBufferInfo {
        uint32_t Size;
        uint32_t Offset;
        ShaderResourceBufferVarType VarType;
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
    class VulkanShader : public Shader 
    {
    public:
      
        VulkanShader(const std::string& name, const std::filesystem::path& filePath, const std::unordered_map<std::string, std::string>& macroDefintions = {});
        VulkanShader(const std::string& name, const std::unordered_map<ShaderStage, std::string> shaders, const std::unordered_map<std::string, std::string>& macroDefintions = {});
        VulkanShader(const std::string& name,const std::unordered_map<ShaderStage, std::vector<uint32_t>>& vulkanSPIRV);
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
        const std::unordered_map<std::string, SahderInputDeclaration>& GetInputDeclaration()const {
            return m_InputDeclaration;
        }

        const SahderInputDeclaration* GetInputDeclaration(std::string name)const;
        const std::string GetInputDeclarationName(uint32_t set, uint32_t binding)const;
        const ShaderResourceBufferInfo* GetPushConstantInput(const std::string& storageName,const std::string& name)const;
        const ShaderResourceBufferInfo* GetStorageBufferInput(const std::string& storageName, const std::string& name)const;
        // storageBufferName, (totalSizeofBuffer)(name, data)
        const std::unordered_map<std::string, std::pair<uint32_t, std::unordered_map<std::string, ShaderResourceBufferInfo>>>& GetStorageInfo()const { return m_StorageBufferResourceInfo; }
        const std::unordered_map<std::string, std::pair<uint32_t, std::unordered_map<std::string, ShaderResourceBufferInfo>>>& GetPushConstantInfo()const { return m_PushConstantResourceInfo; }

        virtual uint32_t AddShaderReloadCallback(const ShaderReloadCallback& callback);
        virtual void RemoveShaderReloadCallback(uint32_t index);
        virtual const std::unordered_map<std::string, std::string>& GetAllShaderMacroDefines() const { return m_AllShaderMacroDefines; };
        virtual const std::unordered_map<std::string, std::string>& GetAllShaderMacroDefines() { return m_AllShaderMacroDefines; };

        uint32_t GetHash() { return m_Hash; }
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
        std::map<uint32_t, std::vector<VkDescriptorPoolSize>> m_TypeCounts;
        std::unordered_map<std::string, VkPushConstantRange> m_PushConstants;

        std::unordered_map<ShaderStage, std::string> m_Paths;
        std::unordered_map<ShaderStage, std::string> m_SourceCode;
        std::unordered_map<ShaderStage, std::vector<uint32_t>> m_VulkanSPIRV;
        //(set, data)
        std::map < uint32_t, ShaderDescriptorSet> m_ShaderDescriptorSet;

        std::map < uint32_t, ShaderDescriptorResource> m_DescriptorResource;

        std::unordered_map<std::string, SahderInputDeclaration> m_InputDeclaration;

        // storageBufferName, (totalSizeofBuffer)(name, data)
        std::unordered_map<std::string, std::pair<uint32_t, std::unordered_map<std::string, ShaderResourceBufferInfo>>> m_StorageBufferResourceInfo;

        // storageBufferName, (totalSizeofBuffer)(name, data)
        std::unordered_map<std::string, std::pair<uint32_t,std::unordered_map<std::string, ShaderResourceBufferInfo>>> m_PushConstantResourceInfo;
        bool m_ConstructorSamePaths = false;

        // the first compile of the shader
        bool m_InitialCompile = true;
        friend class VulkanGraphicsPipeline;
		std::unordered_map<std::string, std::string> m_MacroDefinitions;// defines from the construcot

		std::unordered_map<std::string, std::string> m_AllShaderMacroDefines;// all defiens in the shader, from the paremeter

        friend class ShaderPack;
        //bool TryReadReflectionData(StreamReader* serializer);

        ///void SerializeReflectionData(StreamWriter* serializer);

       // void SetReflectionData(const ReflectionData& reflectionData);
        uint32_t m_Hash;
    };
}