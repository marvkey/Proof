#include "Proofprch.h"
#include"../vendor/stb_image.h"
#include "Glad/glad.h"
#include<iostream>
#include "OpenGLTexture.h"
namespace Proof {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& Path,TextureType _TextureType)
		:
		m_Path(Path),
		m_TextureType(_TextureType)
	{
		glGenTextures(1,&TextureObject);
		Data = stbi_load(Path.c_str(),&Width,&Height,&Channel,0);
		if (Data) {
			GLenum Format = GL_RGB;
			if (Channel == 1)
				Format = GL_RED;
			else if (Channel == 3)
				Format = GL_RGB;
			else if (Channel == 4)
				Format = GL_RGBA;
			
			glBindTexture(GL_TEXTURE_2D,TextureObject);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Width,Height,0,Format,GL_UNSIGNED_BYTE,Data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			stbi_set_flip_vertically_on_load(true);
			stbi_image_free(Data);
		}
		else {
			PF_ENGINE_ERROR("Failed to Load Texture");
			char full[_MAX_PATH];
			if (_fullpath(full,Path.c_str(),_MAX_PATH) != NULL)
				PF_ENGINE_INFO("Texture Path %s",full);
		}
	}
	OpenGLTexture2D::OpenGLTexture2D(uint32_t ImageWidth,uint32_t ImageHeight,TextureType _TextureType):
		Width(ImageWidth),
		Height(ImageHeight),
		m_TextureType(_TextureType)
	{
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
	void OpenGLTexture2D::SetData(void* data,uint32_t size) 	{

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		PF_CORE_ASSERT(size == Width * Height* bpp,"Data must be entire texture!");
		glTextureSubImage2D(TextureObject,0,0,0,Height,Height,m_DataFormat,GL_UNSIGNED_BYTE,data);
	}
	
	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1,&TextureObject);
	}
	void OpenGLTexture2D::BindTexture(uint32_t Slot) {
		glBindTextureUnit(Slot,TextureObject);
	}
	void OpenGLTexture2D::unBind() {
		glDisable(GL_TEXTURE_2D);
	}
}
