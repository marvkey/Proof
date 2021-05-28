#include "Proofprch.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <conio.h>
namespace Proof {
   Shader::Shader(const std::string& VertexPath,const std::string& FragmentPath,bool IsProofDirectory) {
	  // THIS NEEDS TO BE NEATEN UP A FEW UNEEDED VARIABLES
	  std::string VertexShaderSource;
	  std::string FragmentShaderSource;
	  std::ifstream VertexShaderFile;
	  std::ifstream FragmentShaderFile;
	  if(IsProofDirectory ==false)
		 VertexShaderFile.open(VertexPath.c_str());
	  else
		 VertexShaderFile.open((FolderOfCurrentDirectory+VertexPath).c_str());

	  if (IsProofDirectory == false)
		 FragmentShaderFile.open(FragmentPath.c_str());
	  else
		 FragmentShaderFile.open((FolderOfCurrentDirectory+FragmentPath).c_str());

	  if (VertexShaderFile.is_open() == true && FragmentShaderFile.is_open() == true) {
		 std::stringstream vShaderStream,fShaderStream;
		 vShaderStream << VertexShaderFile.rdbuf();//put to stream
		 fShaderStream << FragmentShaderFile.rdbuf();
		 VertexShaderFile.close();
		 FragmentShaderFile.close();

		 VertexShaderSource = vShaderStream.str();
		 FragmentShaderSource = fShaderStream.str();
	  }
	  else {
		 PF_ENGINE_ERROR("Shader File Is Incorrect");
		 char full[_MAX_PATH];

		 if (IsProofDirectory == false) {
			if (_fullpath(full,VertexPath.c_str(),_MAX_PATH) != NULL)
			   PF_ENGINE_INFO("Full path is: {}",full);

			if (_fullpath(full,FragmentPath.c_str(),_MAX_PATH) != NULL)
			   PF_ENGINE_INFO("Full path is: {}",full);
		 }

		 else {
			if (_fullpath(full,(FolderOfCurrentDirectory + VertexPath).c_str(),_MAX_PATH) != NULL)
			   PF_ENGINE_INFO("Full path is: {}",full);

			if (_fullpath(full,(FolderOfCurrentDirectory + FragmentPath).c_str(),_MAX_PATH) != NULL)
			   PF_ENGINE_INFO("Full path is: {}",full);
		 }
		 return;
	  }

	  unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
	  const char* CharVertexShader = VertexShaderSource.c_str();
	  glShaderSource(VertexShader,1,&CharVertexShader,NULL);
	  glCompileShader(VertexShader);

	  int Success;
	  char InfoLog[512];
	  glGetShaderiv(VertexShader,GL_COMPILE_STATUS,&Success);
	  if (!Success) {
		 glGetShaderInfoLog(VertexShader,512,NULL,InfoLog);
		 PF_ENGINE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED {}",InfoLog);
	  }
	  unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	  const char* CharFragmentShader = FragmentShaderSource.c_str();

	  glShaderSource(FragmentShader,1,&CharFragmentShader,NULL);
	  glCompileShader(FragmentShader);
	  glGetShaderiv(FragmentShader,GL_COMPILE_STATUS,&Success);
	  if (!Success) {
		 glGetShaderInfoLog(FragmentShader,512,NULL,InfoLog);
		 PF_ENGINE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED {}",InfoLog);
	  }

	  ID = glCreateProgram();
	  glAttachShader(ID,VertexShader);
	  glAttachShader(ID,FragmentShader);
	  glLinkProgram(ID);
	  glGetProgramiv(ID,GL_LINK_STATUS,&Success);
	  if (!Success) {
		 glGetProgramInfoLog(ID,512,NULL,InfoLog);
		 PF_ENGINE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED {}",InfoLog);
	  }
	  glDeleteShader(VertexShader);
	  glDeleteShader(FragmentShader);
   }

   Shader::Shader(const std::string& ShaderPath,bool IsProofDirectory) {
	  std::ifstream ShaderFile;
	  std::string VertexSource;
	  std::string FragmentSource;
	  std::stringstream  VertexStream;
	  if(IsProofDirectory == false)
		 ShaderFile.open(ShaderPath);
	  else
	  ShaderFile.open(FolderOfCurrentDirectory + ShaderPath);

	  bool VertexChecked = false;
	  bool FragmentChecked = false;
	  if (ShaderFile.is_open() == true) {
		 std::string Line;
		 while (std::getline(ShaderFile,Line)) {
			if (Line == "#Fragment Shader")
			   VertexChecked = false;

			if (VertexChecked == true) {
			   VertexSource += Line;
			   VertexSource += "\n";
			}

			if (FragmentChecked == true) {
			   FragmentSource += Line;
			   FragmentSource += "\n";
			}
			if (Line == "#Vertex Shader")
			   VertexChecked = true;
			if (Line == "#Fragment Shader")
			   FragmentChecked = true;
		 }
		 ShaderFile.close();
	  }
	  else {
		 PF_ENGINE_ERROR("Shader File Is Incorrect");
		 char full[_MAX_PATH];

		 if (IsProofDirectory == false) {
			if (_fullpath(full,ShaderPath.c_str(),_MAX_PATH) != NULL)
			   PF_ENGINE_INFO("Full path is: {}",full);
		 }
		 else {
			if (_fullpath(full,(FolderOfCurrentDirectory + ShaderPath).c_str(),_MAX_PATH) != NULL)
			   PF_ENGINE_INFO("Full path is: {}",full);
		 }

		 return;
	  }
	  unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
	  const char* CharVertexShader = VertexSource.c_str();
	  glShaderSource(VertexShader,1,&CharVertexShader,NULL);
	  glCompileShader(VertexShader);

	  int Success;
	  char InfoLog[512];
	  glGetShaderiv(VertexShader,GL_COMPILE_STATUS,&Success);
	  if (!Success) {
		 glGetShaderInfoLog(VertexShader,512,NULL,InfoLog);
		 PF_ENGINE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED {}",InfoLog);
	  }

	  unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	  const char* CharFragmentShader = FragmentSource.c_str();

	  glShaderSource(FragmentShader,1,&CharFragmentShader,NULL);
	  glCompileShader(FragmentShader);
	  glGetShaderiv(FragmentShader,GL_COMPILE_STATUS,&Success);
	  if (!Success) {
		 glGetShaderInfoLog(FragmentShader,512,NULL,InfoLog);
		 PF_ENGINE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED {}",InfoLog);
	  }

	  ID = glCreateProgram();
	  glAttachShader(ID,VertexShader);
	  glAttachShader(ID,FragmentShader);
	  glLinkProgram(ID);
	  glGetProgramiv(ID,GL_LINK_STATUS,&Success);
	  if (!Success) {
		 glGetProgramInfoLog(ID,512,NULL,InfoLog);
		 PF_ENGINE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED {}",InfoLog);
	  }

	  glDeleteShader(VertexShader);
	  glDeleteShader(FragmentShader);
   }
   void Shader::UseShader() {
	  glUseProgram(ID);
   }
   void Shader::SetBool(const std::string& Name,bool Value) {
	  glUniform1i(glGetUniformLocation(ID,Name.c_str()),(int)Value);
   }
   void Shader::SetInt(const std::string& Name,const int Value) {
	  glUniform1i(glGetUniformLocation(ID,Name.c_str()),Value);
   }
   void Shader::SetFloat(const std::string& Name,float Value) {
	  glUniform1f(glGetUniformLocation(ID,Name.c_str()),Value);
   }
   void Shader::SetVec2(const std::string& Name,const glm::vec2& Value) {
	  glUniform2fv(glGetUniformLocation(ID,Name.c_str()),1,glm::value_ptr(Value));
   }
   void Shader::SetVec2(const std::string& Name,float X,float Y) {
	  glUniform2f(glGetUniformLocation(ID,Name.c_str()),X,Y);
   }
   void Shader::SetVec3(const std::string& Name,const glm::vec3& Value) {
	  glUniform3fv(glGetUniformLocation(ID,Name.c_str()),1,glm::value_ptr(Value));
   }
   void Shader::SetVec3(const std::string& Name,float X,float Y,float Z) {
	  glUniform3f(glGetUniformLocation(ID,Name.c_str()),X,Y,Z);
   }
   void Shader::SetVec4(const std::string& Name,const glm::vec4& Value) {
	  glUniform4fv(glGetUniformLocation(ID,Name.c_str()),1,glm::value_ptr(Value));
   }
   void Shader::SetVec4(const std::string& Name,float X,float Y,float Z,float W) {
	  glUniform4f(glGetUniformLocation(ID,Name.c_str()),X,Y,Z,W);
   }
   void Shader::SetMat2(const std::string& Name,const glm::mat2& Value) {
	  glUniformMatrix2fv(glGetUniformLocation(ID,Name.c_str()),1,GL_FALSE,glm::value_ptr(Value));
   }
   void Shader::SetMat3(const std::string& Name,const glm::mat3& Value) {
	  glUniformMatrix4fv(glGetUniformLocation(ID,Name.c_str()),1,GL_FALSE,glm::value_ptr(Value));
   }
   void Shader::SetMat4(const std::string& Name,const glm::mat4& Value) {
	  glUniformMatrix4fv(glGetUniformLocation(ID,Name.c_str()),1,GL_FALSE,glm::value_ptr(Value));
   }
}