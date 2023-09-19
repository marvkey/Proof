#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include "Proof/Math/Math.h"
#include "AudioTools.h"
namespace Proof 
{
	
	class AudioListenerActor : public RefCounted
	{
	public:
		void UpdateTransform(const AudioTransform& transform);
		void UpdateConeListener(float innerAngleRadians = 6.283185f, float outerAngleRadians = 6.283185f, float outerGrain =0.0);
		void UpdateVelocity(const glm::vec3& velocity = {});
	private:
		friend class AudioEngine;
		uint32_t m_IndexPosition = 0;
	};
}