#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Shader.h"
#include<vulkan/vulkan.h>
namespace Proof
{
    class Proof_API VulkanShader : public Shader {
    public:
        VulkanShader(const std::string& _NameShader, const std::string& VertexPath, const std::string& FragmentPath);
        ~VulkanShader();
        static void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
        virtual void Bind() override {}
        virtual void UnBind() override {}
        virtual void SetBool(const std::string& Name, bool Value)override {}
        virtual void SetInt(const std::string& Name, int Value)override {}
        virtual void SetFloat(const std::string& Name, float Value)override {}
        virtual void SetIntArray(const std::string& Name, int* Value, uint32_t Size)override {}

        virtual void SetVec2(const std::string& Name, const glm::vec2& Value)override{}
        virtual void SetVec2(const std::string& Name, float X, float Y)override{}

        virtual void SetVec3(const std::string& Name, const glm::vec3& Value)override{}
        virtual void SetVec3(const std::string& Name, float X, float Y, float Z)override{}

        virtual void SetVec4(const std::string& Name, const glm::vec4& Value)override{}
        virtual void SetVec4(const std::string& Name, float X, float Y, float Z, float W)override{}

        virtual void SetMat2(const std::string& Name, const glm::mat2& Value)override{}
        virtual void SetMat3(const std::string& Name, const glm::mat3& Value)override{}
        virtual void SetMat4(const std::string& Name, const glm::mat4& Value)override{}
        virtual uint32_t GetID()override { return 0; }
        virtual const std::string& GetPath()const { return m_Path; }
        virtual const std::string& GetName()const { return m_Name; }
        virtual void Reload(){}
    private:
		static std::vector<char> ReadFile(const std::string& filePath);
		VkShaderModule m_VertexShaderModule;
		VkShaderModule m_FragmentShaderModule;
		VkPipelineShaderStageCreateInfo m_ShaderStages[2];
		friend class VulkanGraphicsPipeline;
       

        std::string m_Path = "gg";
        std::string m_Name ="empty ";
	};
}