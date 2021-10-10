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
		
		virtual uint32_t GetID()=0;
		virtual void AttachColourTexture(FrameBufferTextureType type,uint32_t colourAttachPosition,uint32_t id,uint32_t level=0) =0;
		virtual void AttachOtherTexture(FrameBufferAttachmentType type,FrameBufferTextureType textureType,uint32_t id,uint32_t level = 0)=0;

		virtual void AttachRenderBuffer(FrameBufferAttachmentType type,uint32_t id)=0;

		static Count<FrameBuffer>Create();
	};
}
