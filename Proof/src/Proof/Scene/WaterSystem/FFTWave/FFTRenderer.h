#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
//https://github.com/gasgiant/Ocean-URP/blob/main/Assets/OceanSystem/Runtime/OceanRenderer.cs
namespace Proof
{
	struct FFTWaveMeshSettings
	{
		UUID EntityViewer = 0; // if attahced can make the ocean infinite to view of camera
		float MeshScale = 15;
		//min 1 max 10
		uint32_t ClipMapLevels = 7;
		//min 16 max 50
		uint32_t VertexDensity = 25;
	};

	struct FFTColorPreset
	{
		glm::vec4 DeepScatter = glm::vec4(0, 0.011764706, 0.039215688, 1);
		glm::vec4 ShallowScatter = glm::vec4(0, 1, 0.8926091, 1);
		glm::vec4 Diffuse = { 0, 0.025490196, 0.033333335, 1 };
		glm::vec4 ReflectionMask = { 0, 0.113059565, 0.1509434, 1 };

		std::array<glm::vec4, 8> Absorption =
		{
			glm::vec4{0.f, 0.2238422,0.33962262,1},
			glm::vec4{0, 0.45352036, 0.6886792, 1},
			glm::vec4{0.43915984,  0.84532803,0.9056604, 0},
			glm::vec4{1,1,1,  0},
			glm::vec4{0,0,0,  0},
			glm::vec4{0,0,0,  0},
			glm::vec4{0,0,0,  0},
			glm::vec4{0,0,0,  0},
		};
	};

	struct FFTMaterialProps
	{
		float AbsorptionDepthScale = 10;
		float FogDensity = 0.1;

		//min -0.5 max 1.1
		float ReflectionMaskRadius = -0.03;
		// min 0 max 30
		float ReflectionMaskSharpness = 3.55;
	};

	class FFTRenderer : public RefCounted
	{
	public:
		FFTRenderer(Count<class FFTWave> wave);
		Count<class Mesh> WaveMesh;
		FFTColorPreset ColorPreset;
		FFTMaterialProps MaterialProps;

	private:
		void UpdateMesh();
		void UpdateVariables();
	private:
		Count<class FFTWave> m_Wave;
		glm::ivec2 m_CurrentMeshSettings = glm::ivec2(0);
		FFTWaveMeshSettings m_MeshSettings;
		friend class FFTWave;

	};
}