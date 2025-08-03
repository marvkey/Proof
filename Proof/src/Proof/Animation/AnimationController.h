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
	class AnimationData : public RefCounted
	{
	public:

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
		uint32_t StateIndex = 0;

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

	class AnimationState : public RefCounted
	{
	public:
		AnimationState(AssetKey<AssetType::Animation> animation) : Animation(animation)
		{

		}

		glm::vec3 RootTranslationMask = { 1.0f, 1.0f, 0.0f };         // default is to apply root bone transforms in X and Y only. (Z is extracted as root motion)
		glm::vec3 RootTranslationExtractMask = { 0.0f, 0.0f, 1.0f };  // default is to extract root bone transform in forwards (Z) axis
		float RootRotationMask = 1.0f;                                // default is to apply root bone rotation (Y-axis)
		float RootRotationExtractMask = 0.0f;                         // default is to not extract root bone rotation

		bool IsLooping = true;

		AssetKey<AssetType::Animation> Animation;
	};

	class AnimationController : public Asset
	{
	public:
		ASSET_CLASS_TYPE(AnimationController);

		void SetSkeleton(Count<Skeleton> skeletonAsset);
		void OnUpdateAnimationData(float deltaTime, Count<class AnimationData> animationData);

		Count< class Skeleton> GetSkeleton();

		Count<AnimationState> GetAnimationState(const size_t stateIndex) { return m_AnimationStates[stateIndex]; }
		Count<AnimationState> GetAnimationState(const size_t stateIndex) const { return m_AnimationStates[stateIndex]; }

		const std::vector< Count<AnimationState>>& GetAnimationStates() { return m_AnimationStates; };


		Count<AnimationState> AddAnimationState(AssetKey<AssetType::Animation> animation);
		void RemoveAnimationState(uint32_t stateIndex);
	private:
		std::vector< Count<AnimationState>> m_AnimationStates;
		AssetKey<AssetType::Skeleton> m_Skeleton;
		friend class AnimationControllerSerializer;
		friend class AnimationControllerPanel;
	};
}