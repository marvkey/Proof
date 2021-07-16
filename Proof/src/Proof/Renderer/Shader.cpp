#include "Proofprch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"
#include "Renderer.h"
namespace Proof {
    Shader* Shader::s_CurrentBindShader;
    Count<Shader> Shader::Create(const std::string& _ShaderName,const std::string& VertexPath,const std::string& FragmentPath) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Shader None it needs an api"); return nullptr;
            case RendererAPI::API::OpenGL: return Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(_ShaderName,VertexPath,FragmentPath));
        }
    }

    Count<Shader> Shader::Create(const std::string& _ShaderName,const std::string& ShaderPath) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Shader None it needs an api"); return nullptr;
            case RendererAPI::API::OpenGL: return Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(_ShaderName,ShaderPath));
        }
    }
    Count<Shader>* ShaderLibrary::GetShader(const std::string& Name) {
        if (HasShader(Name) == false) {
            PF_CORE_ASSERT(false,"shader library does not have a shader named");
            return nullptr;
        }
        return &ShaderMap[Name];
    }
    void ShaderLibrary::AddShader(const std::string& Name,const Count<Shader>& _Shader) {
        if (HasShader(Name) == false) {
            ShaderMap.insert({Name,_Shader});
            return;
        }
        PF_ENGINE_ERROR("shader library already has a shader called %s",Name.c_str());
    }
    bool ShaderLibrary::HasShader(const std::string& Name) {
        return ShaderMap.find(Name) != ShaderMap.end();
    }
}