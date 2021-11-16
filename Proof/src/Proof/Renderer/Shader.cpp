#include "Proofprch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"
#include "Renderer.h"
namespace Proof {
 
    Count<Shader> Shader::Create(const std::string& _ShaderName,const std::string& ShaderPath) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Shader None it needs an api"); return nullptr;
            case RendererAPI::API::OpenGL: return Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(_ShaderName,ShaderPath));
           //case RendererAPI::API::OpenGL: return CreateCount<class OpenGLShader>(_ShaderName,ShaderPath);
        }
    }
    Count<Shader> Shader::GetOrCreate(const std::string& name,const std::string& path) {
        switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Shader None it needs an api"); return nullptr;
        case RendererAPI::API::OpenGL: 
            Count<Shader> shader =Renderer::GetShaderLibrary().GetShader(name);
            if(shader != nullptr)
                return shader;
            return Renderer::GetShaderLibrary().AddShader(CreateCount<class OpenGLShader>(name,path));
        }
    }
    Shader::~Shader() {
       // Renderer::GetShaderLibrary().ShaderMap.erase(this->GetName());
    }
    Count<Shader> ShaderLibrary::GetShader(const std::string& Name) {
        if (HasShader(Name) == false) {
            return nullptr;
        }
        return ShaderMap[Name];
    }
    void ShaderLibrary::AddShader(const std::string& Name,const Count<Shader>& _Shader) {
        if (HasShader(Name) == false) {
            ShaderMap.insert({Name,_Shader});
            return; 
        }
        PF_CORE_ASSERT(false,"shader library already has a shader called %s",Name.c_str());
    }
    bool ShaderLibrary::HasShader(const std::string& Name) {
        return ShaderMap.find(Name) != ShaderMap.end();
    }
}