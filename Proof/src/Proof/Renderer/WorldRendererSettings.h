#pragma once
#include <glm/glm.hpp>
namespace Proof
{
	struct WorldRendererOptions
	{
		enum class PhysicsColliderView
		{
			None = 0, Normal = 1, OnTop = 2
		};
		PhysicsColliderView ShowPhysicsColliders = PhysicsColliderView::None;
		glm::vec4 PhysicsColliderColor = glm::vec4{ 0.2f, 1.0f, 0.2f, 1.0f };
		bool ShowLightGrid = false;
	};
	enum class ShadowResolution
	{
		None = 0,
		Low,
		Mediuim,
		High,
		Extreme
	};
	struct ShadowSetting
	{
		//casde fading
		bool CascadeFading = true;
		float CascadeTransitionFade = 1.0f; // min 0, max (Float Max)

		float CascadeSplitLambda = 0.92f; // (min 0.0 max 1.0)
		float CascadeNearPlaneOffset = -50.0f; // min - Floatmax, max 0
		float CascadeFarPlaneOffset = 50.0f; // min 0, max Float Max
		float ScaleShadowCascadesToOrigin = 0.f; //min 0.0f max 1.0f
		float MaxShadowDistance = 200.f; // how far away the shadow can be seen
		float ShadowFade = 50.0f; // the distnace the shader goes from opaque to transperant

		//use Manual cascadeSPlits
		bool UseManualCascadeSplits = false;
		float CascadeSplits[4] = { 0.1,0.2,0.3,1.0 }; // min 0, max Flt max

		bool ShowCascades = false;
		bool SoftShadows = false; // temporarily set to false for performance reasons
		ShadowResolution  ShadowResolution = ShadowResolution::Mediuim;

		bool RenderDebugPass = false;
		int DebugCascade = 0; // min 0 max 3
	};

	struct PreProcessSettings
	{
		WorldRendererOptions GeneralOptions;
		ShadowSetting ShadowSettings;
	};
	struct AmbientOcclusion
	{
		bool Enabled = true;
		enum class AmbientOcclusionType
		{
			None = 0,
			SSAO, // Scren space Ambient Occlusion
			HBAO, // High 
			GTAO //Ground Truth Ambient Occlusion
		};

		AmbientOcclusionType Type = AmbientOcclusionType::SSAO;

		struct SSAOSettings
		{
			float Radius = 0.5f;
			float Bias = 0.25f;
			int KernelSize = 64;
			int NoiseSize = 4;
		};

		SSAOSettings SSAO;
	};

	struct BloomSettings
	{
		bool Enabled = true;
		float Threshold = 1.0f;
		float Knee = 0.1f;
		float UpsampleScale = 1.0f;
		float Intensity = 1.0f;
		float DirtIntensity = 1.0f;
	};

	struct DepthOfFieldSettings
	{
		bool Enabled = false;
		float FocusDistance = 0.0f;
		float BlurSize = 1.0f;
	};

	struct PostProcessSettings
	{
		AmbientOcclusion AmbientOcclusionSettings;
		BloomSettings BloomSettings;
		DepthOfFieldSettings DOFSettings;
	};
}