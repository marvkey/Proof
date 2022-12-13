#include "Proofprch.h"
#include"../vendor/stb_image.h"
#include "Glad/glad.h"
#include<iostream>
#include "OpenGLTexture.h"
namespace Proof
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& Path,TextureType _TextureType)
		:
		m_Path(Path),
		m_TextureType(_TextureType) 	{
		glGenTextures(1,&TextureObject);
		Data = stbi_load(Path.c_str(),&Width,&Height,&Channel,0);
		if (Data) {
			glGenTextures(1,&TextureObject);
			Data = stbi_load(Path.c_str(),&Width,&Height,&Channel,0);
			stbi_set_flip_vertically_on_load(true);

			GLenum Format = GL_RGB;
			if (Channel == 1)
				Format = GL_RED;
			else if (Channel == 3)
				Format = GL_RGB;
			else if (Channel == 4)
				Format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D,TextureObject);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,Width,Height,0,Format,GL_UNSIGNED_BYTE,Data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			stbi_image_free(Data);
		}
		else {
			PF_ENGINE_ERROR("Failed to Load Texture");
			char full[_MAX_PATH];
			if (_fullpath(full,Path.c_str(),_MAX_PATH) != NULL)
				PF_ENGINE_INFO("Texture Path {}",full);
		}
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t ImageWidth,uint32_t ImageHeight,TextureType _TextureType):
		Width(ImageWidth),
		Height(ImageHeight),
		m_TextureType(_TextureType){
		m_Path = "Null";
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D,1,&TextureObject);
		glTextureStorage2D(TextureObject,1,m_InternalFormat,Width,Height);

		glTextureParameteri(TextureObject,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTextureParameteri(TextureObject,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

		glTextureParameteri(TextureObject,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTextureParameteri(TextureObject,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width,uint32_t height,DataFormat dataFormat,InternalFormat internalFormat,TextureBaseTypes WrapS,TextureBaseTypes WrapT,TextureBaseTypes MinFilter,TextureBaseTypes MagFilter,type baseType,bool usWrap) {
		glGenTextures(1,&TextureObject);
		glBindTexture(GL_TEXTURE_2D,TextureObject);

		glTexImage2D(GL_TEXTURE_2D,0,(uint32_t)internalFormat,width,height,0,(uint32_t)dataFormat,(uint32_t)baseType,nullptr);
		if(usWrap){
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,(uint32_t)WrapS);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,(uint32_t)WrapT);
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(uint32_t)MinFilter);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,(uint32_t)MagFilter);
	}
	void OpenGLTexture2D::SetData(void* data,uint32_t size) {
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		PF_CORE_ASSERT(size == Width * Height * bpp,"Data must be entire texture!");
		glTextureSubImage2D(TextureObject,0,0,0,Height,Height,m_DataFormat,GL_UNSIGNED_BYTE,data);
	}

	void OpenGLTexture2D::GenerateMipMap() {
		glBindTexture(GL_TEXTURE_2D,TextureObject);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1,&TextureObject);
	}
	void OpenGLTexture2D::Bind(uint32_t Slot) {
		glBindTextureUnit(Slot,TextureObject);
	}
	void OpenGLTexture2D::unBind() {
		glDisable(GL_TEXTURE_2D);
	}
	 


	OpenGLCubeMap::OpenGLCubeMap(const std::vector<std::string>& Paths) {
		glGenTextures(1,&m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_ID);
		for (unsigned int i = 0; i < 6; i++) {
			
			m_Data = stbi_load(Paths[i].c_str(),&m_Width,&m_Height,&m_NrChannels,0);
			if (m_Data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,m_Width,m_Height,0,GL_RGB,GL_UNSIGNED_BYTE,m_Data);
				stbi_image_free(m_Data);
			}
			else {
				PF_ENGINE_ERROR("Cube map is empty load path: {}",Paths[i].c_str());
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	}
	OpenGLCubeMap::OpenGLCubeMap(const std::string& Path) {
		glGenTextures(1,&m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_ID);
		for (unsigned int i = 0; i < 6; i++) {

			m_Data = stbi_load(Path.c_str(),&m_Width,&m_Height,&m_NrChannels,0);
			if (m_Data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,m_Width,m_Height,0,GL_RGB,GL_UNSIGNED_BYTE,m_Data);
				stbi_image_free(m_Data);
			}
			else {
				PF_ENGINE_ERROR("Texture passed in cube map is empty {}",Path.c_str());
				return;
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	}
	OpenGLCubeMap::OpenGLCubeMap(uint32_t textureWidht,uint32_t textureHeight,bool generateMipMap) {
		glGenTextures(1,&m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_ID);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB16F,textureWidht,textureHeight,0,GL_RGB,GL_FLOAT,nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		if(generateMipMap==true)
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	void OpenGLCubeMap::Bind(uint32_t Slot) {
		glBindTextureUnit(Slot,m_ID);
	}

	void OpenGLCubeMap::unBind() {
		glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	}

	void OpenGLCubeMap::GenerateMipMap() {
		glBindTexture(GL_TEXTURE_CUBE_MAP,m_ID);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}


	OpenGLHDRTexture::OpenGLHDRTexture(const std::string& path): 
		m_Path(path)
	{
		stbi_set_flip_vertically_on_load(true);
		m_Data = stbi_loadf(m_Path.c_str(),&m_Width,&m_Height,&m_Components,0);
		if (m_Data) {
			glGenTextures(1,(GLuint*)m_ID);
			glBindTexture(GL_TEXTURE_2D, (GLuint)m_ID);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,m_Width,m_Height,0,GL_RGB,GL_FLOAT,m_Data);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			stbi_image_free(m_Data);
		}
		else {
			PF_ENGINE_ERROR("Failed to load HDR Texture {}",m_Path);
		}
	}
	void OpenGLHDRTexture::Bind(uint32_t Slot) {
		glBindTextureUnit(Slot, (GLuint)m_ID);
	}
	void OpenGLHDRTexture::unBind() {
		glDisable(GL_TEXTURE_2D);
	}
	void OpenGLHDRTexture::GenerateMipMap() {
		glBindTexture(GL_TEXTURE_2D, (GLuint)m_ID);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}
