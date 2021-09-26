#include "Proofprch.h"
#include "Renderer3DCore.h"
namespace Proof{
	float Renderer3DData::s_GammaCorrection=2.2f;
	bool Renderer3DData::s_UseGama=true;
	//Renderer3DData Renderer3DCore::m_Renderer3DData;
	Count<UniformBuffer> Renderer3DCore::s_CameraBuffer;
}
