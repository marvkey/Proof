#pragma once
#include <glm/glm.hpp>
namespace Proof
{
	struct WorldRendererDebugOptions
	{
		enum class PhysicsColliderView
		{
			None = 0, Normal = 1, OnTop = 2
		};
		struct PhysicsDebugOptions
		{
			PhysicsColliderView ShowPhysicsColliders = PhysicsColliderView::None;
			glm::vec4 PhysicsColliderColor = glm::vec4{ 0.2f, 1.0f, 0.2f, 1.0f };

		} PhysicsDebugOptions;

		struct LightDebugOptions
		{
			bool ShowLightGrid = false;
		} LightDebugOptions;

		struct ShadowDebugOptions
		{
			bool ShowCascades = false;
		} ShadowDebugOptions;

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

		bool SoftShadows = false; // temporarily set to false for performance reasons
		ShadowResolution ShadowResolution = ShadowResolution::Mediuim;
		bool RenderDebugPass = false;
		int DebugCascade = 0; // min 0 max 3
	};

	struct PreProcessSettings
	{
		ShadowSetting ShadowSettings;
	};
	struct AmbientOcclusion
	{
		bool Enabled = true;
		float ShadowTolerance = 0.15f;
		enum class AmbientOcclusionType
		{
			//SSAO, // Scren space Ambient Occlusion
			HBAO = 1, // High 
			//GTAO //Ground Truth Ambient Occlusion
		};

		AmbientOcclusionType Type = AmbientOcclusionType::HBAO;
		/*
		struct SSAOSettings
		{
			float Radius = 0.5f;
			float Bias = 0.25f;
			int KernelSize = 64;
			int NoiseSize = 4;
		};
		*/

		struct HBAOSettings
		{
			float Intensity = 1.5f;
			float Radius = 0.275f;
			float Bias = 0.6f;
			float BlurSharpness = 1.0f;
		};

		//SSAOSettings SSAO;
		HBAOSettings HBAO;
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

	struct SSRSettings
	{
		bool Enabled = true;
		glm::vec2 HZBUvFactor;
		glm::vec2 FadeIn = { 0.1f, 0.15f };
		float Brightness = 0.7f;
		float DepthTolerance = 0.8f;
		float FacingReflectionsFading = 0.1f;
		int MaxSteps = 70;
		uint32_t NumDepthMips;
		float RoughnessDepthTolerance = 1.0f;
		bool HalfRes = true;
		bool EnableConeTracing = true;
		float LuminanceFactor = 1.0f;
	};
	struct PostProcessSettings
	{
		AmbientOcclusion AmbientOcclusionSettings;
		BloomSettings BloomSettings;
		DepthOfFieldSettings DOFSettings;
		SSRSettings SSRSettings;
	};


}