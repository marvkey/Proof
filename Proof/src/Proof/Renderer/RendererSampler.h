#pragma once
#include "RendererResouce.h"
namespace Proof
{
	enum class SamplerAddressMode
	{
		Wrap,
		Mirror,
		Clamp,
		Border,
		MirrorOnce,
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

	struct SamplerAddressModeMetaData
	{
		SamplerAddressModeMetaData()
		{

		}
		SamplerAddressModeMetaData(SamplerAddressMode wrap) :
			AddressU(wrap),AddressV(wrap), AddressW(wrap)
		{

		}
		SamplerAddressModeMetaData(SamplerAddressMode wrapU, SamplerAddressMode wrapV, SamplerAddressMode wrapW) :
			AddressU(wrapU), AddressV(wrapV), AddressW(wrapW)

		{

		}
		SamplerAddressMode AddressU = SamplerAddressMode::Wrap;
		SamplerAddressMode AddressV = SamplerAddressMode::Wrap;
		SamplerAddressMode AddressW = SamplerAddressMode::Wrap;

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
		SamplerAddressModeMetaData AddressMode = SamplerAddressMode::Wrap;
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

		static Count<RenderSampler> GetLinear();
		static Count<RenderSampler> GetPoint();
	private:
		static void Init();
		static void ShutDown();
		friend class Renderer;
	};
}