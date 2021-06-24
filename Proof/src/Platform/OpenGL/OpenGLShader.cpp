#include "Proofprch.h"
#include "OpenGLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <conio.h>
namespace Proof {
	OpenGLShader::OpenGLShader(const std::string& _NameShader,const std::string& VertexPath,const std::string& FragmentPath) {
		ShaderName = _NameShader;
		std::ifstream VertexShaderFile,FragmentShaderFile;
		std::stringstream VertexShaderSource,FragmentShaderSource;
		VertexShaderFile.open(VertexPath.c_str());
		FragmentShaderFile.open(FragmentPath.c_str());

		if (VertexShaderFile.is_open() == true && FragmentShaderFile.is_open() == true) {
			VertexShaderSource << VertexShaderFile.rdbuf();//put to stream
			FragmentShaderSource << FragmentShaderFile.rdbuf();
			VertexShaderFile.close();
			FragmentShaderFile.close();
		}
		else {
			PF_ENGINE_ERROR("%s Shader File Is Incorrect",ShaderName);
			char full[_MAX_PATH];
			if (_fullpath(full,VertexPath.c_str(),_MAX_PATH) != NULL)
				PF_ENGINE_INFO("Full path is: %s",full);

			if (_fullpath(full,FragmentPath.c_str(),_MAX_PATH) != NULL)
				PF_ENGINE_INFO("Full path is: %s",full);
			return;
		}

		unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* CharVertexShader = VertexShaderSource.str().c_str();
		glShaderSource(VertexShader,1,&CharVertexShader,NULL);
		glCompileShader(VertexShader);

		int Success;
		char InfoLog[512];
		glGetShaderiv(VertexShader,GL_COMPILE_STATUS,&Success);
		if (!Success) {
			glGetShaderInfoLog(VertexShader,512,NULL,InfoLog);
			PF_ENGINE_ERROR("%s SHADER::EROOR::VERTEX::COMPILATION_FAILED %s",ShaderName.c_str(),InfoLog);
		}
		unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* CharFragmentShader = FragmentShaderSource.str().c_str();

		glShaderSource(FragmentShader,1,&CharFragmentShader,NULL);
		glCompileShader(FragmentShader);
		glGetShaderiv(FragmentShader,GL_COMPILE_STATUS,&Success);
		if (!Success) {
			glGetShaderInfoLog(FragmentShader,512,NULL,InfoLog);
			PF_ENGINE_ERROR("%s SHADER::ERROR::FRAGMENT::COMPILATION_FAILED %s",ShaderName.c_str(),InfoLog);
		}

		ID = glCreateProgram();
		glAttachShader(ID,VertexShader);
		glAttachShader(ID,FragmentShader);
		glLinkProgram(ID);
		glGetProgramiv(ID,GL_LINK_STATUS,&Success);
		if (!Success) {
			glGetProgramInfoLog(ID,512,NULL,InfoLog);
			PF_ENGINE_ERROR("%s SHADER::ERROR::PROGRAM::LINKING_FAILED %s",ShaderName.c_str(),InfoLog);
		}
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
	}

	OpenGLShader::OpenGLShader(const std::string& _NameShader,const std::string& ShaderPath) {
		ShaderName = _NameShader;
		std::ifstream ShaderFile;
		std::string VertexSource;
		std::string FragmentSource;
		std::stringstream  VertexStream;
		ShaderFile.open(ShaderPath);

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
			PF_ENGINE_ERROR("%s Shader File Is Incorrect",ShaderName.c_str());
			char full[_MAX_PATH];
			if (_fullpath(full,ShaderPath.c_str(),_MAX_PATH) != NULL)
				PF_ENGINE_INFO("Full path is: %s",full);
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
			PF_ENGINE_ERROR("%s ERROR::SHADER::VERTEX::COMPILATION_FAILED %s",ShaderName.c_str(),InfoLog);
		}

		unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* CharFragmentShader = FragmentSource.c_str();
		glShaderSource(FragmentShader,1,&CharFragmentShader,NULL);
		glCompileShader(FragmentShader);
		glGetShaderiv(FragmentShader,GL_COMPILE_STATUS,&Success);
		if (!Success) {
			glGetShaderInfoLog(FragmentShader,512,NULL,InfoLog);
			PF_ENGINE_ERROR("%s ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s",ShaderName.c_str(),InfoLog);
		}

		ID = glCreateProgram();
		glAttachShader(ID,VertexShader);
		glAttachShader(ID,FragmentShader);
		glLinkProgram(ID);
		glGetProgramiv(ID,GL_LINK_STATUS,&Success);
		if (!Success) {
			glGetProgramInfoLog(ID,512,NULL,InfoLog);
			PF_ENGINE_ERROR("%s ERROR::SHADER::PROGRAM::LINKING_FAILED %s",ShaderName.c_str(),InfoLog);
		}

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
	}
	void OpenGLShader::UseShader() {
		glUseProgram(ID);
	}
	void OpenGLShader::SetBool(const std::string& Name,bool Value) {
		glUniform1i(glGetUniformLocation(ID,Name.c_str()),(int)Value);
	}
	void OpenGLShader::SetInt(const std::string& Name,const int Value) {
		glUniform1i(glGetUniformLocation(ID,Name.c_str()),Value);
	}
	void OpenGLShader::SetFloat(const std::string& Name,float Value) {
		glUniform1f(glGetUniformLocation(ID,Name.c_str()),Value);
	}
	void OpenGLShader::SetIntArray(const std::string& Name,int* Value,uint32_t Size) {
		GLuint Location = glGetUniformLocation(ID,Name.c_str());
		glUniform1iv(Location,Size,Value);
	}
	void OpenGLShader::SetVec2(const std::string& Name,const glm::vec2& Value) {
		glUniform2fv(glGetUniformLocation(ID,Name.c_str()),1,glm::value_ptr(Value));
	}
	void OpenGLShader::SetVec2(const std::string& Name,float X,float Y) {
		glUniform2f(glGetUniformLocation(ID,Name.c_str()),X,Y);
	}
	void OpenGLShader::SetVec3(const std::string& Name,const glm::vec3& Value) {
		glUniform3fv(glGetUniformLocation(ID,Name.c_str()),1,glm::value_ptr(Value));
	}
	void OpenGLShader::SetVec3(const std::string& Name,float X,float Y,float Z) {
		glUniform3f(glGetUniformLocation(ID,Name.c_str()),X,Y,Z);
	}
	void OpenGLShader::SetVec4(const std::string& Name,const glm::vec4& Value) {
		glUniform4fv(glGetUniformLocation(ID,Name.c_str()),1,glm::value_ptr(Value));
	}
	void OpenGLShader::SetVec4(const std::string& Name,float X,float Y,float Z,float W) {
		glUniform4f(glGetUniformLocation(ID,Name.c_str()),X,Y,Z,W);
	}
	void OpenGLShader::SetMat2(const std::string& Name,const glm::mat2& Value) {
		glUniformMatrix2fv(glGetUniformLocation(ID,Name.c_str()),1,GL_FALSE,glm::value_ptr(Value));
	}
	void OpenGLShader::SetMat3(const std::string& Name,const glm::mat3& Value) {
		glUniformMatrix4fv(glGetUniformLocation(ID,Name.c_str()),1,GL_FALSE,glm::value_ptr(Value));
	}
	void OpenGLShader::SetMat4(const std::string& Name,const glm::mat4& Value) {
		glUniformMatrix4fv(glGetUniformLocation(ID,Name.c_str()),1,GL_FALSE,glm::value_ptr(Value));
	}
	unsigned int OpenGLShader::GetID() {
		return ID;
	}
}