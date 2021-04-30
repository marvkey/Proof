#pragma once
#include<iostream>

namespace Proof {
	class Proof_API Texture {
	public:
		Texture(const std::string& Path,bool IsImageTransperant=false,uint16_t Size = 1); // Transperant if the image is Png IT has an alpha channel
		Texture(const std::string& Path, uint16_t ImageWidth, uint16_t ImageHeight, uint16_t Size=1);
		virtual ~Texture();
		void BindTexture(uint16_t Slot)const;
		void BindTexture() const;
		std::string GetTextureType() { return Type; };
	private:
		unsigned int TextureObject;
		int Width, Height,Channel;
		unsigned char* Data;
		std::string TexturePath;
		uint16_t  TextureSize;
		std::string FolderOfCurrentDirectory = "../Proof/";
		std::string Type;
	};
}

