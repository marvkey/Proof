#pragma once
#include "Proof/Renderer/Renderer.h"
#include "Proof/Core/SmartPointer.h"
#include "Proof/Asset/Asset.h"
namespace Proof{
	using ResourceDescriptorInfo = void*;

	
	class RendererViewResource  : public  Asset//: public Asset
	{
	public:
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const = 0;
		virtual AssetType GetAssetType() const { return AssetType::None;  }

	};

	class RendererBufferResource : public RefCounted
	{
	public:
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight)const = 0;
	};
}
