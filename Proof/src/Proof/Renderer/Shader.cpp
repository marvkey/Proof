#include "Proofprch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "RendererAPI.h"
#include "Renderer.h"
#include "RendererBase.h"
namespace Proof
{

    Count<Shader> Shader::Create(const std::string& name, const std::string& path) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
            case RendererAPI::API::OpenGL: return nullptr;//Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(_ShaderName, ShaderPath));
            case RendererAPI::API::Vulkan:
                {
                    Count<Shader> shader = CreateCount<class VulkanShader>(name, path);
                    RendererBase::GetShaderLibrary()->AddShader(shader);
                    return shader;
                }
        }
    }
    Count<Shader> Shader::Create(const std::string& name, const std::unordered_map<ShaderStage, std::string> stages) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
            case RendererAPI::API::Vulkan:
            {
                Count<Shader> shader = CreateCount<class VulkanShader>(name, stages);
                RendererBase::GetShaderLibrary()->AddShader(shader);
                return shader;
            }
        }
        return nullptr;
    }
    Count<Shader> Shader::GetOrCreate(const std::string& name, const std::string& path) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:
                {
                    PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
                    break;
                }
            case RendererAPI::API::OpenGL:
                {
                    //Count<Shader> shader = Renderer::GetShaderLibrary().GetShader(name);
                    //if (shader != nullptr)
                    //    return shader;
                    //return Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(name, path));
                    PF_CORE_ASSERT(false, "Open gl not support yet"); return nullptr;
                    return nullptr;
                    break;
                }

            case RendererAPI::API::Vulkan:
                {
                    Count<Shader> shader = RendererBase::GetShaderLibrary()->GetShader(name);
                    if (shader != nullptr)
                        return shader;
                    shader = CreateCount<class VulkanShader>(name, path);
                    RendererBase::GetShaderLibrary()->AddShader(shader);
                    return shader;
                }
        }
    }
    Count<Shader> Shader::GetOrCreate(const std::string& name, const std::unordered_map<ShaderStage, std::string> strings) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:
                {
                    PF_CORE_ASSERT(false, "Shader None it needs an api"); return nullptr;
                    break;
                }
            case RendererAPI::API::OpenGL:
                {
                    PF_CORE_ASSERT(false, "Open gl not support yet"); return nullptr;
                    break;
                }

            case RendererAPI::API::Vulkan:
                {
                    Count<Shader> shader = RendererBase::GetShaderLibrary()->GetShader(name);
                    if (shader != nullptr)
                        return shader;
                    shader = CreateCount<class VulkanShader>(name, strings);
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