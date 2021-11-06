#pragma once
#include "Proof/Renderer/Texture.h"
namespace Proof{
	enum class FrameBufferTextureType:uint32_t
	{
		Texture2D = GL_TEXTURE_2D,
		CubeMapPosX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		CubeMapNegX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		CubeMapPosY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		CubeMapNegY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		CubeMapPosZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		CubeMapNegZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	enum class FrameBufferAttachmentType:uint32_t
	{
		DepthStencilAttachment = GL_DEPTH_STENCIL_ATTACHMENT,
		DepthAttachment = GL_DEPTH_ATTACHMENT
	};

	class FrameBuffer{ 
	public:
		/*Sets as the current framebuffer*/
		virtual void Bind() =0;
		/*Disable as current framebuffer*/
		virtual void UnBind()=0;
		
		/* return the value of the frame buffer*/
		virtual uint32_t GetID()=0;
		/*Read the frame buffer and writes it content to the id of the frame buffer passed
		*@param frameBufferID the id of the frame buffer needed to write to
		*/
		virtual void WriteBuffer(const uint32_t frameBufferID)=0;
		/**
		* attaches a color textre to the fame buffer
		* @param type the type of the texture being applied
		* @param coloratach positon the posstion of the texture to e stored 0-30
		* @param id the id of the texture that would be attached to the frame buffer
		* @param level=0 if not 0 it would create a mipmap for the texture
		*/ 
		virtual void AttachColourTexture(FrameBufferTextureType type,uint32_t colourAttachPosition,uint32_t id,uint32_t level=0) =0;
		/**
		* attaches a texture apart from color texture to the frame buffer
		* @param type the type of the texture being applied
		* @param the texture type that would be attached
		* @param id the id of the texture that would be attached to the frame buffer
		* @param level=0 if not 0 it would create a mipmap for the texture
		*/
		virtual void AttachOtherTexture(FrameBufferAttachmentType type,FrameBufferTextureType textureType,uint32_t id,uint32_t level = 0)=0;
		/**
		* attaches a rendere buffe to the frame buffer
		* @param type the type of the texture being applied
		* @param id the id of the render buffer that would be attached to the frame buffer
		*/
		virtual void AttachRenderBuffer(FrameBufferAttachmentType type,uint32_t id)=0;

		static Count<FrameBuffer>Create();
	};
}
