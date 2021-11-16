#pragma once
#include "glm/glm.hpp"
#include <unordered_map>
namespace Proof {
    class Proof_API Shader {
    public:   
        virtual void Bind() = 0;
        virtual void UnBind() =0;
        virtual void SetBool(const std::string& Name,bool Value) = 0;
        virtual void SetInt(const std::string& Name,int Value) = 0;

        virtual void SetIntArray(const std::string& Name,int* Value,uint32_t Size) = 0;
        virtual void SetFloat(const std::string& Name,float Value) = 0;

        virtual void SetVec2(const std::string& Name,const glm::vec2& Value) = 0;
        virtual void SetVec2(const std::string& Name,float X,float Y) = 0;

        virtual void SetVec3(const std::string& Name,const glm::vec3& Value) = 0;
        virtual void SetVec3(const std::string& Name,float X,float Y,float Z) = 0;

        virtual void SetVec4(const std::string& Name,const glm::vec4& Value) = 0;
        virtual void SetVec4(const std::string& Name,float X,float Y,float Z,float W) = 0;

        virtual void SetMat2(const std::string& Name,const glm::mat2& Value) = 0;
        virtual void SetMat3(const std::string& Name,const glm::mat3& Value) = 0;
        virtual void SetMat4(const std::string& Name,const glm::mat4& Value) = 0;

        virtual uint32_t GetID() = 0;
        static Count<Shader>Create(const std::string& _ShaderName,const std::string& ShaderPath);
        static Count<Shader>GetOrCreate(const std::string& name,const std::string& path);
        virtual const std::string& GetName()const=0;
        virtual const std::string& GetPath()const =0;
        virtual ~Shader();
        virtual void Reload() =0;
    protected:
        friend class Application;
    };

    class ShaderLibrary {
    public:
        Count<Shader> GetShader(const std::string& Name);
        void AddShader(const std::string& Name,const Count<Shader>&_Shader);
        bool HasShader(const std::string& Name);
        std::unordered_map<std::string, Count<Shader>> ShaderMap;
        void ReloadeShaders(){
            for(auto& shader: ShaderMap){
                shader.second->Reload();
            }
        }
    private:
        Count<Shader>AddShader(const Count<Shader>&_Shader) {
           if (HasShader(_Shader->GetName()) == false) {
                ShaderMap.insert({_Shader->GetName(),_Shader});
                return _Shader;
            }
           PF_ENGINE_ERROR(" shader called %s Already exist",_Shader->GetName().c_str());
            PF_CORE_ASSERT(false,"shader library cannot add existing shader",_Shader->GetName().c_str());
        }
        friend class Shader;
    };
};