#pragma once
#include "Proof/Core/Core.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <iostream>
namespace Proof
{
    class Proof_API Shader {
    public:
        enum class ShaderStage {
            Vertex,
            Fragment,
            Compute,
            Geometry
        };
        virtual void Bind() {};
        virtual void UnBind() {};
        virtual void SetBool(const std::string& Name, bool Value) {};
        virtual void SetInt(const std::string& Name, int Value) {};

        virtual void SetIntArray(const std::string& Name, int* Value, uint32_t Size) {};
        virtual void SetFloat(const std::string& Name, float Value) {};

        virtual void SetVec2(const std::string& Name, const glm::vec2& Value) {};
        virtual void SetVec2(const std::string& Name, float X, float Y) {};

        virtual void SetVec3(const std::string& Name, const glm::vec3& Value) {};
        virtual void SetVec3(const std::string& Name, float X, float Y, float Z) {};

        virtual void SetVec4(const std::string& Name, const glm::vec4& Value) {};
        virtual void SetVec4(const std::string& Name, float X, float Y, float Z, float W) {};

        virtual void SetMat2(const std::string& Name, const glm::mat2& Value) {};
        virtual void SetMat3(const std::string& Name, const glm::mat3& Value) {};
        virtual void SetMat4(const std::string& Name, const glm::mat4& Value) {};

        virtual uint32_t GetID() { return 0; };
        static Count<Shader>Create(const std::string& _ShaderName, const std::string& ShaderPath);
        static Count<Shader>Create(const std::string& _ShaderName, const std::string& vertPath, const std::string& fragPath);
        static Count<Shader>GetOrCreate(const std::string& name, const std::string& path);
        static Count<Shader>GetOrCreate(const std::string& name, const std::unordered_map<Shader::ShaderStage, std::string> strings);
        virtual const std::string& GetName()const { return ""; };
        virtual const std::unordered_map<Shader::ShaderStage, std::string>& GetPath()const { return {}; };
        virtual ~Shader();
        virtual void Reload() {};
    protected:
        friend class Application;
    };

    class ShaderLibrary {
    public:
        Count<Shader> GetShader(const std::string& Name);
        void AddShader(const std::string& Name, const Count<Shader>& _Shader);
        bool HasShader(const std::string& Name);
        std::unordered_map<std::string, Count<Shader>> ShaderMap;
        void ReloadeShaders() {
            for (auto& shader : ShaderMap) {
                shader.second->Reload();
            }
        }
    private:
        Count<Shader>AddShader(const Count<Shader>& _Shader);
        friend class Shader;
    };
};