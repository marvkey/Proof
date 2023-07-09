#pragma once
#include "Renderer.h"
namespace Proof{
	using ResourceDescriptorInfo = void*;

	class RendererResource {
	public:
		virtual int GetJustForPolyMorphysm() = 0;
	};
	class RendererViewResource  : public  RendererResource//: public Asset
	{
	public:
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const = 0;
		virtual int GetJustForPolyMorphysm() { return 0; };
	};

	class RendererBufferResource : public RendererResource
	{
	public:
		virtual int GetJustForPolyMorphysm() { return 0; };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight)const = 0;
	};
}
