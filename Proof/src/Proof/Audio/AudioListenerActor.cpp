#include "Proofprch.h"
#include "AudioListenerActor.h"
#include <MiniAudio.h>
#include "AudioEngine.h"
#include "Proof/Scene/Component.h"
namespace Proof
{
	//Audio::Transform Audio::GetAudioTransform(const Proof::TransformComponent* transformComponent)
	//{
	//	auto rotation = transformComponent->GetRotation();
	//	return {
	//		transformComponent->Location,
	//		rotation * Math::GetFowardVector()/* orientation */,
	//		rotation * Math::GetUpVector()  /* up */
	//	};
	//}

	
	void AudioListenerActor::UpdateTransform(const AudioTransform& transform)
	{
		ma_engine_listener_set_position(&AudioEngine::GetEngine(), 0, transform.Position.x, transform.Position.y, transform.Position.z);
		ma_engine_listener_set_direction(&AudioEngine::GetEngine(), 0, transform.Orientation.x, transform.Orientation.y, transform.Orientation.z);
		ma_engine_listener_set_world_up(&AudioEngine::GetEngine(), 0, transform.Up.x, transform.Up.y, transform.Up.z);
	}
	void AudioListenerActor::UpdateConeListener(float innerAngleRadians, float outerAngleRadians, float outerGrain)
	{
		ma_engine_listener_set_cone(&AudioEngine::GetEngine(), m_IndexPosition, innerAngleRadians, outerAngleRadians, outerGrain);
	}
	void AudioListenerActor::UpdateVelocity(const glm::vec3& newVelocity)
	{
		auto isVelocityValid = [](const glm::vec3& velocity) {
			auto inRange = [](auto x, auto low, auto high) {
				return ((x - high) * (x - low) <= 0);
			};

			constexpr float speedOfSound = 343.0f;
			return !(!inRange(velocity.x, -speedOfSound, speedOfSound)
				|| !inRange(velocity.y, -speedOfSound, speedOfSound)
				|| !inRange(velocity.z, -speedOfSound, speedOfSound)); };


		PF_CORE_ASSERT(isVelocityValid(newVelocity));
		ma_engine_listener_set_velocity(&AudioEngine::GetEngine(), m_IndexPosition, newVelocity.x, newVelocity.y, newVelocity.z);
	}
}


