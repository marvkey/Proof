#pragma once
#include "Proof/Core/Core.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <iostream>
#include <functional>
#include <filesystem>
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
        virtual ~Shader() = default;
        static Count<Shader>Create(const std::string& name, const std::string& ShaderPath);
        static Count<Shader>Create(const std::string& name, const std::unordered_map<ShaderStage, std::string> strings);

        virtual const std::string& GetName()const= 0;
        virtual const std::unordered_map<ShaderStage, std::string>& GetPath()const =0;
        virtual void Reload() =0;

        //rturns teh index where the shader reload callbakc ppointer is stored
        // when ur done with the function call remove it
        // so the shader does not call unneded data
        // do not call this function in the funciton pointer being passed
        virtual uint32_t AddShaderReloadCallback(const ShaderReloadCallback& callback) = 0;
        virtual void RemoveShaderReloadCallback(uint32_t index) = 0;
    protected:
        friend class Application;
        friend class Renderer;
    };

    class ShaderLibrary : RefCounted{
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
        void ReloadAllShaders() {
            for (auto& shader : ShaderMap) {
                shader.second->Reload();
            }
        }
        const std::unordered_map<std::string, Count<Shader>>& GetShaderMap()const { return ShaderMap; }

    private:
        std::unordered_map<std::string, Count<Shader>> ShaderMap;
        friend class Shader;
    };
};