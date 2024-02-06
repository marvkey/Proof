#pragma once
#include <map>
#include <glm/glm.hpp>
#include <vector>
namespace Proof
{
	struct DirectionalLight
	{
		glm::vec3 Color = glm::vec3{ 1 };
		float Intensity = 0.0f;
		glm::vec3 Direction; // Make sure TO NORMALIZED

		int bCastShadows = 0;//bool
		int bCastSoftShadows = 0;// bool
		float ShadowStrength = 0.0f; // between 0 and 1 or shadow amount how dark the shadow is
		float ShadowSoftness = 0.0f; // penubra size or light size between 0 and 1
	};

	struct UBSkyLight
	{
		glm::vec3 TintColor = glm::vec3{ 1 };
		float Lod = 0;
		//float Intensity = 1;
		float Intensity = 0;
		float Rotation = 0;
	};

	struct alignas(16) PointLight // this is what vulkan uses 16 bytes alignemnt
	{
		glm::vec3 Location{ 0 };
		float Intensity = 1; // min =0, max = 500f;
		glm::vec3 Color{ 1 };
		float MinRadius = 1; // min  = 0,maximum should be Radius
		float Radius = 10;// min is 0, max max float
		float Falloff = 1.0f; //min is 0, represent how far before the light becomes weaker
		int bCastsShadows;
		int bSoftShadows;
		float ShadowStrength = 0.5f;// 0.0 to 1.0 how dark sahdow is
		float ShadowSoftness = 0.5f;//how soft the shadow is from 0.0 to 1.0f
	};
	struct alignas(16) SpotLight
	{
		glm::vec3 Location{ 0 };
		float Intensity = 1.0f; // Range: 0.0 to positive infinity.
		glm::vec3 Direction{ 0 }; //NORMALIZE It
		// Attenuation factor for the spotlight cone's light intensity
		// as the angle between the light direction and surface normal increases.
		// A higher value results in a more rapid decrease in intensity
		// as the angle deviates from the spotlight's central direction.
		float AngleAttenuation = 5.0f; // Range: 0.0 to positive infinity.
		glm::vec3 Color{ 1.0f };
		float Range = 10.0f;// Range: 0.0 to positive infinity.
		float Angle = 60.0f;// Range: 0.0 to 180.0 degrees. // Angle of the spot light cone (in degrees).
		float Falloff = 1.0f;// Range: 0.0 to positive infinity.  factor that affects how the light intensity diminishes.
		int bCastsShadows = false;
		int bSoftShadows = false;
		float ShadowStrength = 0.5f;// 0.0 to 1.0 how dark sahdow is
		float ShadowSoftness = 0.5f;//how soft the shadow is from 0.0 to 1.0f 
	};
	struct SBDirectionalLightsSceneData
	{
		std::vector<DirectionalLight> DirectionalLights;
	};
	struct SBPointLightSceneData
	{
		std::vector<PointLight> PointLights;
	};
	struct SBSpotLightSceneData
	{
		std::vector<SpotLight> SpotLights;
	};

	struct alignas(16) UBCameraData
	{
		glm::mat4 Projection;
		glm::mat4 InverseProjection;
		glm::mat4 UnreversedProjectionMatrix;
		glm::mat4 View;
		glm::mat4 InverseView;
		glm::mat4 ViewProjection;// projection * view
		glm::mat4 InverseViewProjection;

		glm::mat4 PrevViewProjection;

		glm::vec3 Position;
		float NearPlane;

		float FarPlane;
		float Fov;
		glm::vec2 NDCToViewMul;

		glm::vec2 NDCToViewAdd;
		glm::vec2 DepthUnpackConsts;

		// Halton sequence jitter data, .xy is current frame jitter data, .zw is prev frame jitter data.
		glm::vec2 JitterData;
		glm::vec2 PrevJitterData;

		uint32_t JitterPeriod;        // jitter period for jitter data.
		int bEnableJitter;       // Is main camera enable jitter in this frame.

	};
	struct UBRenderData
	{
		glm::vec4 cascadeSplit;
		int bShowCascades = false; // bools in glsl are 4 ytes
		int bSoftShadows = (int)true;
		float MaxShadowDistance = 200.f;
		float ShadowFade = 1.0f;
		int bCascadeFading = true;
		float CascadeTransitionFade = 1.0f;
	};

	struct UBSceneData
	{
		glm::vec3 CameraPosition;
		int bShowLightGrid = (int)false;
	};
	struct UBScreenData
	{
		glm::vec2 FullResolution;
		glm::vec2 InverseFullResolution;
		glm::vec2 HalfResolution;
		glm::vec2 InverseHalfResolution;
		float AspectRatio;
		float ResolutionScaling = 1.0f;
	};

	struct CascadeData
	{
		//glm::mat4 Projection;
		glm::mat4 ViewProjection;
		float SplitDepth;
	};

	struct UBLightScene
	{
		uint32_t SkyLightCount = 0;
		uint32_t DirectionalLightCount = 0;
		uint32_t PointLightCount = 0;
		uint32_t SpotLightCount = 0;
		glm::uvec3 LightCullingWorkGroups;
	};

	struct alignas(16) UBHBAOData
	{
		glm::vec4 PerspectiveInfo;

		glm::vec2 InvQuarterResolution;
		float RadiusToScreen;
		float NegInvR2;

		float NDotVBias;
		float AOMultiplier;
		float PowExponent;
		int bIsOrtho;

		glm::vec4 Float2Offsets[16];
		glm::vec4 Jitters[16];

		glm::vec3 Padding;
		float ShadowTolerance;

	};

	struct alignas(16) UBSSR
	{
		glm::vec2 HZBUvFactor;
		glm::vec2 FadeIn = { 0.1f, 0.15f };
		float Brightness = 0.7f;
		float DepthTolerance = 0.8f;
		float FacingReflectionsFading = 0.1f;
		int MaxSteps = 70;
		uint32_t NumDepthMips;
		float RoughnessDepthTolerance = 1.0f;
		int bHalfRes = true;
		int bEnableConeTracing = true;
		float LuminanceFactor = 1.0f;
		uint32_t AmbientOcclusionType = 0; // 0 none, 1 is HBAO , 2 GTAO
	};

	struct alignas(16) UBFrameData
	{
		uint32_t FrameCount;
		float AppTimeSeconds;
	};
}