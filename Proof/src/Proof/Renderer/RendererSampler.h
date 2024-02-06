#pragma once
#include "RendererResouce.h"
namespace Proof
{
	enum class SamplerWrap
	{
		Repeat,
		MirroredRepeat,
		ClampEdge,
		MirrorClampEdge,
		ClampBorder,
	};

	enum class SamplerFilter
	{
		Linear,
		Nearest,
		Cubic
	};
	enum class SamplerBorderColor
	{
		TransperantBlack = 0,
		OpaqueBlack = 1,
		OpaqueWhite = 2,
		TransperantBlackInt,
		OpaqueBlackInt,
		OpaqueWhiteInt,
	};

	enum class SamplerMipMapMode
	{
		NEAREST = 0,
		LINEAR = 1,
	};

	struct SamplerAddressMode
	{
		SamplerAddressMode()
		{

		}
		SamplerAddressMode(SamplerWrap wrap) :
			AddressU(wrap),AddressV(wrap), AddressW(wrap)
		{

		}
		SamplerAddressMode(SamplerWrap wrapU, SamplerWrap wrapV, SamplerWrap wrapW) :
			AddressU(wrapU), AddressV(wrapV), AddressW(wrapW)

		{

		}
		SamplerWrap AddressU = SamplerWrap::Repeat;
		SamplerWrap AddressV = SamplerWrap::Repeat;
		SamplerWrap AddressW = SamplerWrap::Repeat;

	};

	enum class SamplerCompare 
	{
		Never = 0,
		Less = 1,
		Equal = 2,
		LessOrEqual = 3,
		Greater = 4,
		NotEqual = 5,
		GreaterOrEqual = 6,
		Always = 7,
	};
	struct SamplerResourceConfig
	{
		std::string DebugName;
		SamplerAddressMode AddressMode = SamplerWrap::Repeat;
		SamplerFilter Filter = SamplerFilter::Linear;
		SamplerBorderColor BorderColor = SamplerBorderColor::TransperantBlack;
		SamplerMipMapMode MipMapMode = SamplerMipMapMode::NEAREST;
		SamplerCompare CompareOp = SamplerCompare::Never;
	};
	class RenderSampler : public RendererResource
	{

	public:
		RENDER_VIEW_RESOURCE_CLASS_TYPE(Sampler)
		static Count<RenderSampler> Create(const SamplerResourceConfig& config);

		virtual const SamplerResourceConfig& GetConfig() const = 0;
	};

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
	private:
		static void Init();
		static void ShutDown();
		friend class Renderer;
	};
}