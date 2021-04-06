#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace Proof {
    class Proof_API Shader {
    public:
        unsigned int ID;
        Shader(std::string VertexPath, std::string FragmentPath );
        ~Shader() {
            glDeleteProgram(ID);
        }
       
        void UseShader();

        void SetBool(std::string &Name, bool& Value);
        void SetInt(std::string& Name, int& Value);
        void SetFloat(std::string& Name, float& Value);

        void SetVec2(const std::string& Name, glm::vec2& Value);
        void SetVec2(const std::string& Name, float X, float Y);

        void SetVec3(const std::string& Name, glm::vec3& Value);
        void SetVec3(const std::string& Name, float X, float Y, float Z);

        void SetVec4(const std::string& Name, glm::vec4& Value);
        void SetVec4(const std::string& Name, float X, float Y, float Z, float W);

        void SetMat2(const std::string& Name, glm::mat2& Value);
        void SetMat3(const std::string& Name, glm::mat3& Value);
        void SetMat4(const std::string& Name, glm::mat4& Value);
    private:
        std::string FolderOfCurrentDirectory = "../Proof/src/";
    };
}

