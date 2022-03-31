#pragma once
#include "Proof/Renderer/UniformBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof/Resources/Math/Vector.h"
#include "../UniformBuffer.h"
#include "Proof/Core/Core.h"
namespace Proof{
	struct CameraData{
		CameraData(){};
		CameraData(const glm::mat4& projection,const glm::mat4& view,const Vector<float>& pos):
			m_Projection(projection),m_View(view),m_Positon(pos) {};
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		Vector<float> m_Positon;
	};
	struct Proof_API Renderer3DData{
		static float GetGammaCorrection(){
			return s_GammaCorrection;
		}
	private:
		static float s_GammaCorrection;
		static bool s_UseGama;
	};
	//static Count<UniformBuffer> m_Renderer3DBaseUniform;
	class Proof_API Renderer3DCore {
	private:
		//static Renderer3DData m_Renderer3DData;
	public:
		static Count<UniformBuffer> s_CameraBuffer;

		static void Init() {
			//m_Renderer3DBaseUniform = UniformBuffer::Create(sizeof(Renderer3DData),0);
			//m_Renderer3DBaseUniform->SetData(&m_Renderer3DData,sizeof(Renderer3DData));
			s_CameraBuffer = UniformBuffer::Create(sizeof(CameraData),1);
		}
		friend class Renderer3DPBR;
		friend class OpenGLRenderer3DPBR;
	};
}