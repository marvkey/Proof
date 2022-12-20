#pragma once
#include <glm/glm.hpp>
#include "Proof/Renderer/Shader.h"
namespace Proof
{
    class Proof_API OpenGLShader: public Shader {
    public:
        OpenGLShader(const std::string& _NameShader,const std::string& VertexPath,const std::string& FragmentPath); // CODE NEEDS TO BE NEATEN UP
        OpenGLShader(const std::string& _NameShader,const std::string& ShaderPath);
        virtual void Bind() override;
        virtual void UnBind() override;
        virtual void SetBool(const std::string& Name,bool Value)override;
        virtual void SetInt(const std::string& Name,int Value)override;
        virtual void SetFloat(const std::string& Name,float Value)override;
        virtual void SetIntArray(const std::string& Name,int* Value,uint32_t Size)override;

        virtual void SetVec2(const std::string& Name,const glm::vec2& Value)override;
        virtual void SetVec2(const std::string& Name,float X,float Y)override;

        virtual void SetVec3(const std::string& Name,const glm::vec3& Value)override;
        virtual void SetVec3(const std::string& Name,float X,float Y,float Z)override;

        virtual void SetVec4(const std::string& Name,const glm::vec4& Value)override;
        virtual void SetVec4(const std::string& Name,float X,float Y,float Z,float W)override;

        virtual void SetMat2(const std::string& Name,const glm::mat2& Value)override;
        virtual void SetMat3(const std::string& Name,const glm::mat3& Value)override;
        virtual void SetMat4(const std::string& Name,const glm::mat4& Value)override;
        virtual uint32_t GetID()override;
        virtual const std::unordered_map<ShaderStage, std::string>& GetPath()const {
            std::unordered_map<ShaderStage, std::string> temp;
            temp[ShaderStage::Vertex] = "m_Path";
            return temp;
        }
        virtual const std::string& GetName()const{return m_Name;}
        virtual void Reload();
    private:
        uint32_t m_ID;
        std::string m_Path;
        std::string m_Name;
    };
}