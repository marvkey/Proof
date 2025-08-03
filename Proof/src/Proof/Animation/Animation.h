#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include "Skeleton.h"
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
namespace Proof
{
	class MeshSource;
	class SkeletonData;

	template<typename T>
	struct AnimationFrameKey
	{
		T Value;
		float TimeStep;// 0.0f = beginning of animation clip, 1.0f = end of animation clip
		uint32_t Track; // Track is the index of the bone (BoneIndex) in the skeleton that this keyframe applies to.
		AnimationFrameKey() = default;
		AnimationFrameKey	(const float timeStep, const uint32_t track, const T& value)
			: TimeStep(timeStep), Track(track), Value(value) {}

	};

	using LocationKey = AnimationFrameKey<glm::vec3>;
	using RotationKey = AnimationFrameKey<glm::quat>;
	using ScaleKey = AnimationFrameKey<glm::vec3>;

	class InternalAnimation
	{
	public:
		InternalAnimation(const std::string_view name, const float duration)
			:m_Name(name), m_Duration(duration)
		{

		}

		const std::string& GetName() const { return m_Name; }
		float GetDuration() const { return m_Duration; }
		void SetKeyFrames(const std::vector<LocationKey>& translations, const std::vector<RotationKey>& rotations, const std::vector<ScaleKey>& scales)
		{
			m_LocationKeys = translations;
			m_RotationKeys = rotations;
			m_ScaleKeys = scales;
		}

		const auto& GetLocationKeys() const { return m_LocationKeys; }
		const auto& GetRotationKeys() const { return m_RotationKeys; }
		const auto& GetScaleKeys() const { return m_ScaleKeys; }
	private:
		
		std::vector<LocationKey> m_LocationKeys;
		std::vector<RotationKey> m_RotationKeys;
		std::vector<ScaleKey> m_ScaleKeys;
		std::string m_Name;
		float m_Duration;
	};

	class Animation : public Asset
	{
	public:
		Animation(const Count<MeshSource> meshSource,uint32_t animationIndex);

		ASSET_CLASS_TYPE(Animation);

		Count<MeshSource> GetMeshSource() const;
		uint32_t GetAnimationCount() const;
		const InternalAnimation& GetAnimation(const SkeletonData& skeleton) const;

		uint32_t GetAnimationIndex()
		{
			return m_AnimationIndex;
		}
	private:
		Count<MeshSource> m_MeshSource;
		uint32_t m_AnimationIndex = 0; 
	};
}