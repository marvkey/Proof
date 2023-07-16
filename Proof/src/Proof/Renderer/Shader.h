#pragma once
#include "Proof/Core/Core.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <iostream>
namespace Proof
{
    enum class ShaderStage {
        Vertex,
        Fragment,
        Compute,
        Geometry
    };
    class Shader : public RefCounted {
    public:
        virtual ~Shader();
        static Count<Shader>Create(const std::string& name, const std::string& ShaderPath);
        static Count<Shader>Create(const std::string& name, const std::unordered_map<ShaderStage, std::string> strings);
        static Count<Shader>GetOrCreate(const std::string& name, const std::string& path);
        static Count<Shader>GetOrCreate(const std::string& name, const std::unordered_map<ShaderStage, std::string> strings);
        //does not check if has shader
        static Count<Shader>Get(const std::string& name);

        virtual const std::string& GetName()const= 0;
        virtual const std::unordered_map<ShaderStage, std::string>& GetPath()const =0;
        virtual void Reload() =0;
    protected:
        friend class Application;
    };

    class ShaderLibrary {
    public:
        void LoadShader(const std::string& name, const std::filesystem::path& path);
        Count<Shader> GetShader(const std::string& name);
        bool HasShader(const std::string& name);
        void AddShader(const Count<Shader>& shader);

        template<typename T>
        void ForEachShader(T f) {
            for (auto& [ID,shader] : ShaderMap)
            {
                f(shader);
            }
        }
        void ReloadeShaders() {
            for (auto& shader : ShaderMap) {
                shader.second->Reload();
            }
        }

    private:
        std::unordered_map<std::string, Count<Shader>> ShaderMap;
        friend class Shader;
    };
};