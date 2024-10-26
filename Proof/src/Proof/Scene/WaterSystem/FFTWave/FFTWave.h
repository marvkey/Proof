#pragma once
#include "Proof/Scene/WaterSystem/Wave.h"
#include <array>
#include <glm/glm.hpp>

//https://github.com/gasgiant/Ocean-URP/blob/main/Assets/OceanSystem/Runtime/OceanSimulation.cs
namespace Proof
{
	class Texture2D;
	class Image2D;
	class RenderMaterial;
	class Material;
	class MaterialTable;
	class ComputePass;
	struct TransformComponent;
	class GlobalBufferSet;
	class StorageBufferSet;
	class Entity;
	

	// TODO MOVE to Material

	
	class FFTWave : public Wave
	{

	public:
		FFTWave(Count<FFTWave> other);
		FFTWave(Count<class Water> water, WaveType type);

	public:
		void Update(float deltaTime);
	private:
		void InitPasses();
		Count<class RenderMaterial> GetRenderMaterial();
	private:

		Count<class FFTRenderer> m_FFTRenderer;
		Count<Image2D> m_InitialSpectrumTextures;
		Count<ComputePass> m_InitialSpectrumPass;
		Count<ComputePass> m_TimeDependentPass;
		struct SBSpectrumParameters
		{
			float Scale;
			float Angle;
			float SpreadBlend;
			float Swell;

			float Alpha;
			float PeakOmega;
			float Gamma;
			float ShortWavesFade;
		};

		std::array<SBSpectrumParameters, 8> m_Spectrums;
		Count<class StorageBufferSet> m_SBSpectrumBuffer;
		Count<class GlobalBufferSet> m_GlobalBufferSet;
		Count<class Mesh> m_WaveMesh;
		Count<class Material> m_Material;

		struct alignas(16) UBFFTGlobalStruct
		{
			//-----------------------Clip Map ------------------------------------------

			glm::vec3 ClipMapViewerPosition;
			float ClipMapScale;

			float ClipMapLevelHalfSize;
			int padding0;
			int padding1;
			int padding2;
			//-----------------------------------------------------------------

			//-----------------------OceanSimulation ------------------------------------------ 

			glm::vec4 ActiveCascades; //for 4 casdcaes =v
			glm::vec4 OceanLengthScales;
			//-----------------------------------------------------------------


			//----------------------- Foam ------------------------------------------

			glm::vec4 FoamTint;

			float FoamNormalDetail;
			float UnderWaterFoamParallax;
			float ContactFoam;
			int padding3;

			float OceanFoamCoverage;
			float OceanFoamDensity;
			float OceanFoamSharpness;
			float OceanFoamPersistence;

			float OceanFoamTrail;
			float OceanFoamTrailTextureStrength;
			float OceanFoamTrailBlendValue;
			float OceanFoamUnderwater;

			glm::vec4 OceanFoamCascadesWeights;

			glm::vec2 OceanFoamTrailTextureSize0;
			glm::vec2 OceanFoamTrailTextureSize1;

			glm::vec2 OceanFoamTrailDirection0;
			glm::vec2 OceanFoamTrailDirection1;

			glm::vec4 FoamAlbedo_ST;

			glm::vec4 FoamUnderwaterTexture_ST;

			glm::vec4 ContactFoamTexture_ST;
			//-----------------------------------------------------------------

			//----------------------- Simulation ------------------------------------------

			// simulation
			float OceanWindSpeed;
			float OceanWavesScale;
			float OceanWavesAlignement;
			float OceanReferenceWaveHeight;

			glm::mat4 OceanWorldToWindSpace;

			glm::vec2 OceanWindDirection;
			int padding4;
			int padding5;

			//-----------------------------------------------------------------

			//----------------------- Rougness ------------------------------------------

			float RoughnessScale;
			float SpecularStrength;
			float SpecularMinRoughness;
			float ReflectionNormalStength;

			//-----------------------------------------------------------------

			//----------------------- Distance ------------------------------------------

			float RoughnessDistance;
			float HorizonFog;
			float CascadeFadeDist;
			float UVWrapStrength;
			//-----------------------------------------------------------------

			//----------------------- color ------------------------------------------

			glm::vec3 OceanDeepScatterColor;
			int padding6;
			glm::vec3 OceanSssColor;
			int padding7;
			glm::vec3 OceanDiffuseColor;
			int padding8;

			glm::vec4 OceanAbsorptionColor0;
			glm::vec4 OceanAbsorptionColor1;
			glm::vec4 OceanAbsorptionColor2;
			glm::vec4 OceanAbsorptionColor3;
			glm::vec4 OceanAbsorptionColor4;
			glm::vec4 OceanAbsorptionColor5;
			glm::vec4 OceanAbsorptionColor6;
			glm::vec4 OceanAbsorptionColor7;

			glm::vec2 OceanAbsorptionGradientParams;
			float OceanFogDensity;
			float OceanAbsorptionDepthScale;

			//-----------------------------------------------------------------

			//----------------------- subsurface scattering ------------------------------------------


			float SssSunStrength;
			float SssEnvironmentStrength;
			float SssSpread;
			float SssNormalStrength;

			float SssHeightBias;
			float SssFadeDistance;
			int padding9;
			int padding10;

			//-----------------------------------------------------------------

			//----------------------- Refraction and Reflection ------------------------------------------

			float RefractionStrength;
			float RefractionStrengthUnderwater;
			float ReflectionMaskRadius;
			float ReflectionMaskSharpness;

			//-----------------------------------------------------------------


			glm::vec4 DistanctRoughnessMap_ST;
		} m_GlobalStruct;


		

		friend class FFTRenderer;
		friend class FFTSimulation;
	};
}