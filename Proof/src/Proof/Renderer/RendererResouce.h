#pragma once
#include "Proof/Core/SmartPointer.h"
#include "Proof/Asset/Asset.h"
namespace Proof{
	using ResourceDescriptorInfo = void*;
	enum class RendererResourceType
	{
		None =0,
		Image2D,
		ImageView,
		Texture2D,
		TextureCube,
		UniformBuffer,
		//UniformBufferSet,
		StorageBuffer,
	//	StorageBufferSet
	};
	#define RENDER_VIEW_RESOURCE_CLASS_TYPE(type) static RendererResourceType GetStaticRendererResourceType() { return RendererResourceType::type; }\
								virtual RendererResourceType GetRendererResourceType() const override { return GetStaticRendererResourceType(); }
	class RendererResource : public Asset//: public Asset
	{
	public:
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const = 0;
		virtual AssetType GetAssetType() const { return AssetType::None;  }
		virtual RendererResourceType GetRendererResourceType()const = 0;
	};
}
