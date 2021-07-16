#include "Proofprch.h"
#include "EditorMousePicking.h"
#include "Proof/ProofCore.h"
namespace Proof{
	EditorMousePicking::EditorMousePicking(uint32_t Widht,uint32_t Height){
		if(!Init(Widht,Height)){
			PF_ASSERT(false,"Error not abile to initilize editor mouse picker");
		}
	}
	bool EditorMousePicking::Init(uint32_t Widht,uint32_t Height) {
		glGenFramebuffers(1,&m_FBO);
		//glBindFramebuffer(GL_FRAMEBUFFER,m_FBO);

		glGenTextures(1,&m_PickingTextureId);
		glBindTexture(GL_TEXTURE_2D,m_PickingTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT); // s is x direction
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT); // T is y direction
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,Widht,Height,0,GL_RGB,GL_FLOAT,0);

		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_PickingTextureId,0);

		// CREATE THE TEXTURE OBJECT FOR DEPTH BUFFER
		/* MAY NOT MATTER */
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1,&m_DepthTextureID);
		glBindTexture(GL_TEXTURE_2D,m_DepthTextureID);
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,Widht,Height,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_DepthTextureID,0);

		/*   -------------------------------- */

		// DISABLE READING FRAME BUFFER
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			PF_ERROR("frame Buffer is not complete ");
			return false;
		}

		// UnBind the texture and framebuffer
		glBindTexture(GL_TEXTURE_2D,0);
		glBindFramebuffer(GL_FRAMEBUFFER,0);	
		return true;
	}
}