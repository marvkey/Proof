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
	 Shader::Shader(const std::string VertexPath, const std::string FragmentPath){
		 // THIS NEEDS TO BE NEATEN UP A FEW UNEEDED VARIABLES
		 std::string VertexShaderSource;
		 std::string FragmentShaderSource;
		 std::ifstream VertexShaderFile;
		 std::ifstream FragmentShaderFile;

		 std::string VertexFileLocation = FolderOfCurrentDirectory + VertexPath;
		 VertexShaderFile.open(VertexFileLocation.c_str());
		 std::string FragmentFileLocation = FolderOfCurrentDirectory + FragmentPath;
		 FragmentShaderFile.open(FragmentFileLocation.c_str());

		 if (VertexShaderFile.is_open() == true && FragmentShaderFile.is_open() == true) {
			 std::stringstream vShaderStream, fShaderStream;
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
			 if (_fullpath(full, VertexFileLocation.c_str(), _MAX_PATH) != NULL)
				 PF_ENGINE_INFO("Full path is: {}", full);
			 if (_fullpath(full, FragmentFileLocation.c_str(), _MAX_PATH) != NULL)
				 PF_ENGINE_INFO("Full path is: {}", full);
		 }
		 unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
		 const char* CharVertexShader = VertexShaderSource.c_str();
		 glShaderSource(VertexShader, 1, &CharVertexShader, NULL);
		 glCompileShader(VertexShader);

		 int Success;
		 char InfoLog[512];
		 glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
		 if (!Success) {
			 glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
			 PF_ENGINE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED {}", InfoLog);
		 }

		 unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		 const char* CharFragmentShader = FragmentShaderSource.c_str();

		 glShaderSource(FragmentShader, 1, &CharFragmentShader, NULL);
		 glCompileShader(FragmentShader);
		 glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
		 if (!Success) {
			 glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
			 PF_ENGINE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED {}", InfoLog);
		 }

		 ID = glCreateProgram();
		 glAttachShader(ID, VertexShader);
		 glAttachShader(ID, FragmentShader);
		 glLinkProgram(ID);
		 glGetProgramiv(ID, GL_LINK_STATUS, &Success);
		 if (!Success) {
			 glGetProgramInfoLog(ID, 512, NULL, InfoLog);
			 PF_ENGINE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED {}", InfoLog);
		 }
		 //PF_ENGINE_INFO("{}", CharVertexShader);
		 glDeleteShader(VertexShader);
		 glDeleteShader(FragmentShader);
	 }


	 Shader::Shader(const std::string ShaderPath){
		 std::string ShaderSource;
		 std::ifstream ShaderFile;
		 std::string FragmentSource;
		 std::string VertexSource;
		 ShaderFile.open(FolderOfProofCurrentDirectory + ShaderPath);
		 if (ShaderFile.is_open()) {
			 std::stringstream VertexShaderStream;
		 }
		 else {
			 PF_ENGINE_ERROR("Shader File Is Incorrect");
			 char full[_MAX_PATH];
			 if (_fullpath(full,(FolderOfCurrentDirectory+ShaderPath).c_str(), _MAX_PATH) != NULL)
				 PF_ENGINE_INFO("Full path is: {}", full);
		 }
		 
	 }
	void Shader::UseShader() {
		glUseProgram(ID);
	}
	void Shader::SetBool(const std::string Name, bool Value){
		glUniform1i(glGetUniformLocation(ID, Name.c_str()), (int)Value);
	}
	void Shader::SetInt(const std::string Name, const int Value){
		glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
	}
	void Shader::SetFloat(const std::string Name, float Value){
		glUniform1f(glGetUniformLocation(ID, Name.c_str()), Value);
	}
	void Shader::SetVec2(const std::string Name, const glm::vec2& Value){
		glUniform2fv(glGetUniformLocation(ID, Name.c_str()), 1, glm::value_ptr(Value));
	}
	void Shader::SetVec2(const std::string Name, float X, float Y){
		glUniform2f(glGetUniformLocation(ID, Name.c_str()),X,Y);
	}
	void Shader::SetVec3(const std::string Name, const glm::vec3& Value){
		glUniform3fv(glGetUniformLocation(ID, Name.c_str()), 1,glm::value_ptr(Value));
	}
	void Shader::SetVec3(const std::string Name, float X, float Y, float Z){
		glUniform3f(glGetUniformLocation(ID, Name.c_str()), X, Y,Z);
	}
	void Shader::SetVec4(const std::string Name, const glm::vec4& Value){
		glUniform4fv(glGetUniformLocation(ID, Name.c_str()), 1, glm::value_ptr(Value));
	}
	void Shader::SetVec4(const std::string Name, float X, float Y, float Z, float W){
		glUniform4f(glGetUniformLocation(ID, Name.c_str()), X, Y, Z,W);
	}
	void Shader::SetMat2(const std::string Name, const glm::mat2& Value){
		glUniformMatrix2fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Value));
	}
	void Shader::SetMat3(const std::string Name, const glm::mat3& Value){
		glUniformMatrix4fv(glGetUniformLocation(ID, Name.c_str()), 1,GL_FALSE, glm::value_ptr(Value));
	}
	void Shader::SetMat4(const std::string Name, const glm::mat4& Value){
		glUniformMatrix4fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Value));
	}
}

