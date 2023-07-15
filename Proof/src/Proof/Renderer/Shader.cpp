#include "Proofprch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "Renderer.h"
#include "Renderer.h"
#include "RendererBase.h"
namespace Proof
{

    Count<Shader> Shader::Create(const std::string& name, const std::string& path) {
        switch (Renderer::GetAPI()) {
            case Renderer::API::None:  PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
            case Renderer::API::OpenGL: return nullptr;//Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(_ShaderName, ShaderPath));
            case Renderer::API::Vulkan:
                {
                    Count<Shader> shader = Count<class VulkanShader>::Create(name, path);
                    RendererBase::GetShaderLibrary()->AddShader(shader);
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
                RendererBase::GetShaderLibrary()->AddShader(shader);
                return shader;
            }
        }
        return nullptr;
    }
    Count<Shader> Shader::GetOrCreate(const std::string& name, const std::string& path) {
        switch (Renderer::GetAPI()) {
            case Renderer::API::None:
                {
                    PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
                    break;
                }
            case Renderer::API::OpenGL:
                {
                    //Count<Shader> shader = Renderer::GetShaderLibrary().GetShader(name);
                    //if (shader != nullptr)
                    //    return shader;
                    //return Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(name, path));
                    PF_CORE_ASSERT(false, "Open gl not support yet"); return nullptr;
                    return nullptr;
                    break;
                }

            case Renderer::API::Vulkan:
                {
                    Count<Shader> shader = RendererBase::GetShaderLibrary()->GetShader(name);
                    if (shader != nullptr)
                        return shader;
                    shader = Count<class VulkanShader>::Create(name, path);
                    RendererBase::GetShaderLibrary()->AddShader(shader);
                    return shader;
                }
        }
    }
    Count<Shader> Shader::GetOrCreate(const std::string& name, const std::unordered_map<ShaderStage, std::string> strings) {
        switch (Renderer::GetAPI()) {
            case Renderer::API::None:
                {
                    PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
                    break;
                }
            case Renderer::API::OpenGL:
                {
                    PF_CORE_ASSERT(false, "Open gl not support yet"); return nullptr;
                    break;
                }

            case Renderer::API::Vulkan:
                {
                    Count<Shader> shader = RendererBase::GetShaderLibrary()->GetShader(name);
                    if (shader != nullptr)
                        return shader;
                    shader = Count<class VulkanShader>::Create(name, strings);
                    RendererBase::GetShaderLibrary()->AddShader(shader);
                    return shader;
                }
        }
    }
    Count<Shader> Shader::Get(const std::string& name)
    {
        Count<Shader> shader = RendererBase::GetShaderLibrary()->GetShader(name);
        return shader;
    }
    Shader::~Shader() {
        //RendererBase::GetShaderLibrary().ShaderMap.erase(this->GetName());
    }
    void ShaderLibrary::LoadShader(const std::string& name, const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
        {
            PF_ENGINE_ERROR("Failed to load Shader: {}, Path {}", name, path.string());
            return;
        }
        if (HasShader(name))return;

        auto shader = Shader::Create(name, path.string());
        ShaderMap[name] = shader;
    }
    Count<Shader> ShaderLibrary::GetShader(const std::string& name) {
        if (HasShader(name)) {
            return ShaderMap[name];
        }
        return nullptr;
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