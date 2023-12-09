#include "Proofprch.h"
#include "Shader.h"
//#include "Proof/Platform/OpenGL/OpenGLShader.h"
#include "Proof/Platform/Vulkan/VulkanShader.h"
#include "Renderer.h"
#include "Renderer.h"
namespace Proof
{

    Count<Shader> Shader::Create(const std::string& name, const std::string& path) {
        switch (Renderer::GetAPI()) {
            case Renderer::API::None:  PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
            case Renderer::API::OpenGL: return nullptr;//Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(_ShaderName, ShaderPath));
            case Renderer::API::Vulkan:
                {
                    Count<Shader> shader = Count<class VulkanShader>::Create(name, path);
                    return shader;
                }
        }
    }
    Count<Shader> Shader::Create(const std::string& name, const std::unordered_map<ShaderStage, std::string> stages) {
        switch (Renderer::GetAPI()) {
            case Renderer::API::None:  PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
            case Renderer::API::Vulkan:
            {
                Count<Shader> shader = Count<class VulkanShader>::Create(name, stages);
                return shader;
            }
        }
        return nullptr;
    }
 
  
    void ShaderLibrary::LoadShader(const std::string& name, const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
        {
            PF_CORE_ASSERT(false,fmt::format("Failed to load Shader: {}, Path {}", name, path.string()).c_str());
            return;
        }
        PF_CORE_ASSERT(HasShader(name) ==false, "Canot load shader with thesame name");

        auto shader = Shader::Create(name, path.string());
        AddShader(shader);
    }
    Count<Shader> ShaderLibrary::GetShader(const std::string& name) {
        PF_CORE_ASSERT(HasShader(name), fmt::format("Does not contain shader {}", name).c_str());
        return ShaderMap[name];
    }
    bool ShaderLibrary::HasShader(const std::string& name) {
        return ShaderMap.contains(name);
    }
    void ShaderLibrary::AddShader(const Count<Shader>& shader) {
        if (HasShader(shader->GetName()) == false) {
            ShaderMap.insert({ shader->GetName(),shader });
            return;
        }
        PF_ENGINE_ERROR("{} shader called Already exist", shader->GetName().c_str());
        PF_CORE_ASSERT(false, "shader library cannot add existing shader");
    }
}