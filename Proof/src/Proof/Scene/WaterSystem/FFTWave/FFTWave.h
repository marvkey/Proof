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
	

	class FFTWave : public Wave
	{

	public:
		FFTWave(Count<FFTWave> other);
		FFTWave(Count<class Water> water);

	public:
		void Update(float deltaTime);
		virtual void Render(Count<class WorldRenderer> renderer);

		Count<Texture2D> GetInitialSpectrumImage() { return m_InitialSpectrumTexture; }
		Count<Texture2D> GetPingPhaseTexture() { return m_PingPhaseTexture; }
		Count<Texture2D> GetPongPhaseImage() { return m_PongPhaseTexture; };
		Count<Texture2D> GetSpectrumImage() { return m_SpectrumTexture; }
		Count<Texture2D> GetTempImage() { return m_TempTexture; }
		Count<Texture2D> GetNormalMap() { return m_NormalTexture; }
		const int OceanSize = 1024;
		struct FFTWaveInfo
		{
			float WindMagnitude = 14.142135f;
			float windAngle = 45.f;
			float Choppiness = 1.5f;

		}WaveInfo;
	private:
		void InitPasses();
		void InitTextures();
		Count<class RenderMaterial> GetRenderMaterial();
	private:

		Count<ComputePass> m_InitialSpectrumPass;
		Count<ComputePass> m_PhasePass;
		Count<ComputePass> m_SpectrumPass;
		Count<ComputePass> m_HorizontalPass;
		Count<ComputePass> m_VerticalPass;
		Count<ComputePass> m_NormalMapPass;

		Count<Texture2D> m_InitialSpectrumTexture;
		Count<Texture2D> m_PingPhaseTexture;
		Count<Texture2D> m_PongPhaseTexture;
		Count<Texture2D> m_SpectrumTexture;
		Count<Texture2D> m_TempTexture;
		Count<Texture2D> m_NormalTexture;
		Count<class RenderCommandBuffer> m_CommandBuffer;
		bool m_IsInPingPhase = true;

	};
}