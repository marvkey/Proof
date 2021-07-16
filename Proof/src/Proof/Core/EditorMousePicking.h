#pragma once
#include "Proof/Core/Core.h"
#include "glad/glad.h"
namespace Proof{
	class Proof_API EditorMousePicking {
	public:
		EditorMousePicking()=default;
		EditorMousePicking(uint32_t Widht, uint32_t Height);
		void EnableWriting() {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_FBO);
		}

		void DisbleWriting() {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
		}

		int ReadPixel(int x,int y) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER,m_FBO);
			glReadBuffer(GL_COLOR_ATTACHMENT0); // reading from zero colour attachment
			float Pixels[3];
			glReadPixels(x,y,1,1,GL_RGB,GL_FLOAT,Pixels);
			return (int)Pixels[0];
		}
	private:
		uint32_t m_PickingTextureId;
		uint32_t m_FBO;
		uint32_t m_RenderID;
		bool Init(uint32_t Widht,uint32_t Height);
		uint32_t m_DepthTextureID;
	
	};
}

