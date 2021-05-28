#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace Proof {
   class Proof_API Shader {
   public:
      Shader(const std::string& VertexPath,const std::string& FragmentPath, bool IsProofDirectory = false); // CODE NEEDS TO BE NEATEN UP
      Shader(const std::string& ShaderPath, bool IsProofDirectory =false);
      ~Shader() {
         glDeleteProgram(ID);
      }
      void UseShader();
      void SetBool(const std::string& Name,bool Value);
      void SetInt(const std::string& Name,int Value);
      void SetFloat(const std::string& Name,float Value);

      void SetVec2(const std::string& Name,const glm::vec2& Value);
      void SetVec2(const std::string& Name,float X,float Y);

      void SetVec3(const std::string& Name,const glm::vec3& Value);
      void SetVec3(const std::string& Name,float X,float Y,float Z);

      void SetVec4(const std::string& Name,const glm::vec4& Value);
      void SetVec4(const std::string& Name,float X,float Y,float Z,float W);

      void SetMat2(const std::string& Name,const glm::mat2& Value);
      void SetMat3(const std::string& Name,const glm::mat3& Value);
      void SetMat4(const std::string& Name,const glm::mat4& Value);

      unsigned int GetID() {
         return ID;
      }
   private:
      std::string FolderOfCurrentDirectory = "../Proof/src/";
      unsigned int ID;
   };
}