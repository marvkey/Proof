#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/fwd.hpp>
namespace Proof
{
	class Skeleton;

	struct RootPose
	{
		glm::vec3 Translation;
		glm::quat Rotation;
	};

	// Represents the motion (i.e. difference between current pose and a previous one) of the root bone of a skeleton.
	// Only translation and rotation for now (ignore scale).
	using RootMotion = RootPose;

	// AnimationData stores a bunch of information regarding the current state of an animated character.
	// It includes such things as the bone transforms and root motion (from most recent animation update)
	// and other cached data used to efficiently sample animation clips.
	struct AnimationData : public RefCounted
	{

		std::vector<glm::vec3> LocalTranslations;
		std::vector<glm::quat> LocalRotations;
		std::vector<glm::vec3> LocalScales;

		RootPose RootPoseStart; // root pose at start of animation (for current state)
		RootPose RootPoseEnd;   // root pose and end of animation (for current state)
		RootPose RootPose;      // last sampled root pose
		RootMotion RootMotion;  // "motion" of root (i.e the difference between current sampled root pose and previous sampled root pose)

		float PreviousAnimationTime = 0.0f;
		float AnimationTime = 0.0f;
		float PlaybackSpeed = 1.0f;
		bool IsAnimationPlaying = true;

		AnimationData() = default;

		AnimationData(Count<AnimationData> other):
			 LocalTranslations(other->LocalTranslations)
			, LocalRotations(other->LocalRotations)
			, LocalScales(other->LocalScales)
			, RootPoseStart(other->RootPoseStart)
			, RootPoseEnd(other->RootPoseEnd)
			, RootPose(other->RootPose)
			, RootMotion(other->RootMotion)
			, PreviousAnimationTime(other->PreviousAnimationTime)
			, AnimationTime(other->AnimationTime)
			, IsAnimationPlaying(other->IsAnimationPlaying)
			, PlaybackSpeed(other->PlaybackSpeed)
		{
		}

		//uint32_t GetSize() const
		//{
		//	return m_TranslationCache.GetSize();
		//}

		void Resize(uint32_t numBones)
		{
			//m_TranslationCache.Resize(numBones);
			//m_RotationCache.Resize(numBones);
			//m_ScaleCache.Resize(numBones);
		}

		void SetLocalTransforms(const std::vector<glm::vec3>& localTranslations, const std::vector<glm::quat>& localRotations, const std::vector<glm::vec3>& localScales)
		{
			LocalTranslations = localTranslations;
			LocalRotations = localRotations;
			LocalScales = localScales;
		}
		
	};


	class AnimationController : public Asset
	{
	public:
		void SetSkeleton(Count<Skeleton> skeletonAsset);
		void OnUpdate(float deltaTime, Count<AnimationData> data) const;

		ASSET_CLASS_TYPE(AnimationController);
		Count< class Skeleton> GetSkeleton();

	private:
		AssetKey<AssetType::Skeleton> m_Skeleton;
		glm::vec3 m_RootTranslationMask = { 1.0f, 1.0f, 0.0f };         // default is to apply root bone transforms in X and Y only. (Z is extracted as root motion)
		glm::vec3 m_RootTranslationExtractMask = { 0.0f, 0.0f, 1.0f };  // default is to extract root bone transform in forwards (Z) axis
		float m_RootRotationMask = 1.0f;                                // default is to apply root bone rotation (Y-axis)
		float m_RootRotationExtractMask = 0.0f;                         // default is to not extract root bone rotation

		bool m_IsLooping = true;

		friend class AnimationControllerSerializer;
		friend class AnimationControllerPanel;
	};
}