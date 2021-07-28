#pragma once
#include "Proof/Renderer/Texture.h"
namespace Proof
{
	class Proof_API OpenGLTexture2D: public Texture2D {
	public:
		OpenGLTexture2D(const std::string& Path,TextureType _TextureType); // Transperant if the image is Png IT has an alpha channel
		OpenGLTexture2D(uint32_t ImageWidth,uint32_t ImageHeight,TextureType _TextureType);
		virtual ~OpenGLTexture2D();
		virtual void BindTexture(uint32_t Slot = 0)override;
		virtual uint32_t GetID() { return TextureObject; }
		virtual void unBind() override;
		virtual void SetData(void* data,uint32_t size) override;
		virtual TextureType GetTextureType()override {
			return m_TextureType;
		}

		virtual std::string GetPath() {
			return m_Path;
		};
	private:
		uint32_t TextureObject;
		int Width,Height,Channel;
		unsigned char* Data;
		friend class OpenGLFrameBuffer;
		GLenum m_InternalFormat,m_DataFormat;
		std::string m_Path;
		TextureType m_TextureType = TextureType::None;
	};


	class Proof_API OpenGLCubeMap: public CubeMap {
	public:
		OpenGLCubeMap(const std::vector<std::string>& Paths);
		OpenGLCubeMap(const std::string& Path);
		virtual void Bind()override;
		virtual uint32_t GetID() {
			return m_ID;
		}
		virtual void unBind()override;
	private:
		uint32_t m_ID;
		int m_Width,m_Height,m_NrChannels;
		unsigned char* m_Data;
	};
}

