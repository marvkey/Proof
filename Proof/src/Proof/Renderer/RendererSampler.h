#pragma once
#include "RendererResouce.h"
#include "ImageUtils.h"
namespace Proof
{
	

	struct SamplerFilterMetaData
	{
		SamplerFilter MinFilter = SamplerFilter::Linear;
		SamplerFilter MagFilter = SamplerFilter::Nearest;
		SamplerFilterMetaData()
		{

		}
		SamplerFilterMetaData(SamplerFilter filter) :
			MinFilter(filter),MagFilter(filter)
		{

		}
		SamplerFilterMetaData(SamplerFilter minFilter, SamplerFilter magFilter) :
			MinFilter(minFilter),MagFilter(magFilter)
		{

		}

		bool operator<(const SamplerFilterMetaData& other) const
		{
			if (MinFilter < other.MinFilter)
				return true;

			if (MinFilter > other.MinFilter)
				return false;

			return MagFilter < other.MagFilter;
		}

		bool operator>(const SamplerFilterMetaData& other) const 
		{
			return other < *this;
		}
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
		Linear = 0,
		Nearest = 1,
	};

	struct SamplerWrapMetaData
	{
		SamplerWrapMetaData()
		{

		}
		SamplerWrapMetaData(SamplerWrap wrap) :
			AddressU(wrap),AddressV(wrap), AddressW(wrap)
		{

		}
		SamplerWrapMetaData(SamplerWrap wrapU, SamplerWrap wrapV, SamplerWrap wrapW) :
			AddressU(wrapU), AddressV(wrapV), AddressW(wrapW)

		{

		}
		SamplerWrap AddressU = SamplerWrap::Repeat;
		SamplerWrap AddressV = SamplerWrap::Repeat;
		SamplerWrap AddressW = SamplerWrap::Repeat;

		bool operator<(const SamplerWrapMetaData& other) const
		{
			if (AddressU < other.AddressU)
				return true;

			if (AddressU > other.AddressU)
				return false;

			if (AddressV < other.AddressV)
				return true;

			if (AddressV > other.AddressV)
				return false;

			return AddressW < other.AddressW;
		}

		bool operator>(const SamplerWrapMetaData& other) const
		{
			return other < *this;
		}
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
		SamplerWrapMetaData Wrap = SamplerWrap::Repeat;
		SamplerFilterMetaData Filter = SamplerFilter::Linear;
		SamplerBorderColor BorderColor = SamplerBorderColor::TransperantBlack;
		SamplerMipMapMode MipMapMode = SamplerMipMapMode::Linear;
		SamplerCompare CompareOp = SamplerCompare::Never;
		float Anisotropy = 1.0f;

		bool operator<(const SamplerResourceConfig& other) const;
	};
	class RenderSampler : public RendererResource
	{

	public:
		RENDER_VIEW_RESOURCE_CLASS_TYPE(Sampler)
		static Count<RenderSampler> Create(const SamplerResourceConfig& config);

		virtual const SamplerResourceConfig& GetConfig() const = 0;
	};


}