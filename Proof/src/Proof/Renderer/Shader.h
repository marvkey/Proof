#pragma once
#include "Proof/Core/Core.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <iostream>
#include <functional>
namespace Proof
{
    enum class ShaderStage {
        Vertex,
        Fragment,
        Compute,
        Geometry
    };

    using ShaderReloadCallback = std::function<void()>;
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

        //erturns teh index where the shader reload callbakc ppointer is stored
        // when ur done with the function call remove it
        // so the shader does not call unneded data
        // do not call this function in the funciton pointer being passed
        virtual uint32_t AddShaderReloadCallback(const ShaderReloadCallback& callback) = 0;
        virtual void RemoveShaderReloadCallback(uint32_t index) = 0;
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