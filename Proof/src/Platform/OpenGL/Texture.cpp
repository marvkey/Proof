#include "Proofprch.h"
#include "Texture.h"
#include "GL/glew.h"
#include"../vendor/stb_image.h"
namespace Proof {
	Texture::Texture(const std::string& Path,bool IsImageTransperant,uint16_t Size):
		TextureSize(Size)
	{
		TexturePath = FolderOfCurrentDirectory + Path;
		glGenTextures(1, &TextureObject);
		glBindTexture(GL_TEXTURE_2D, TextureObject);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		Data = stbi_load(TexturePath.c_str(), &Width, &Height, &Channel, 0);
		if (Data) {
			if(IsImageTransperant == false)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			PF_ENGINE_ERROR("Failed to Load Texture");
			char full[_MAX_PATH];
			if (_fullpath(full, TexturePath.c_str(), _MAX_PATH) != NULL)
				PF_ENGINE_INFO("Texture Path {}", full);
		}
		stbi_image_free(Data);
	}
	Texture::Texture(const std::string& Path, uint16_t ImageWidth, uint16_t ImageHeight, uint16_t Size):
		Width(ImageWidth),
		Height(ImageHeight),
		TextureSize(Size)
	{
		TexturePath = FolderOfCurrentDirectory + Path;
		glGenTextures(1, &TextureObject);
		glBindTexture(GL_TEXTURE_2D, TextureObject);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		Data = stbi_load(TexturePath.c_str(), &Width, &Height, &Channel, 0);
		if (Data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			PF_ENGINE_ERROR("Failed to Load Texture");
			char full[_MAX_PATH];
			if (_fullpath(full, TexturePath.c_str(), _MAX_PATH) != NULL)
				PF_ENGINE_INFO("Texture Path {}", full);
		}
		stbi_image_free(Data);
	}
	Texture::~Texture(){
		glDeleteTextures(TextureSize, &TextureObject);
	}
	void Texture::BindTexture(uint16_t Slot)const {
		glBindTextureUnit(Slot, TextureObject);
	}
	void Texture::BindTexture() const{
		glBindTexture(GL_TEXTURE_2D, TextureObject);
	}
}
