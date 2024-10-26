#pragma once
#include "Proof/Core/Core.h"


namespace Proof
{
	class RenderSampler;
	struct SamplerResourceConfig;
	class SamplerFactory
	{
	public:
		//https://github.com/qiutang98/flower/blob/b32f8097ca43f02aafdf7af3c67e8987939eebe2/source/engine/graphics/base.h#L499

		static Count<RenderSampler> GetPointClampEdge();
		static Count<RenderSampler> GetPointClampBorder0000();
		static Count<RenderSampler> GetPointClampBorder1111();
		static Count<RenderSampler> GetPointRepeat();
		static Count<RenderSampler> GetLinearClampEdge();
		static Count<RenderSampler> GetLinearClampEdgeMipPoint();
		static Count<RenderSampler> GetLinearClampBorder0000MipPoint();
		static Count<RenderSampler> GetLinearClampBorder1111MipPoint();
		static Count<RenderSampler> GetLinearRepeatMipPoint();
		static Count<RenderSampler> GetLinearRepeat();

		// default sampler 
		static Count<RenderSampler> GetTrilinear();
		static Count<RenderSampler> GetBilinear();
		static Count<RenderSampler> GetPoint();

		static Count<RenderSampler> GetClampEdgeTrilinear();
		static Count<RenderSampler> GetClampEdgeBilinear();
		static Count<RenderSampler> GetClampEdgePoint();
		static void AddSamplerToFactory(Count<RenderSampler> sampler);
		static Count<RenderSampler> GetOrAddSamplerToFactory(const SamplerResourceConfig& samplerConfig);

	private:
		static void Init();
		static void ShutDown();
		friend class Renderer;
	};
}