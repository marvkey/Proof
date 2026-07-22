#pragma once
#include "Proof/Core/Core.h"
#include "RendererResouce.h"
#include "Proof/Core/Assert.h"
#include "Proof/Resources/EnumReflection.h"
#include "ImageUtils.h"
#include <glm/glm.hpp>
#include <iostream>
namespace Proof
{

	enum class ImageUsage 
	{
		None = 0,
		Texture,
		Attachment,
		Storage,
		SwapChain, //TODO Remove dont use it only for swapchain going to be removes
		HostRead //HostRead
	};
	enum class ImageViewType
	{
		None = 0,
		View1D,
		View2D,
		View3D,
		ViewCube,
		View1DArray,
		View2DArray,
		ViewCubeArray
	};
	struct ImageConfiguration
	{
		std::string DebugName;
		bool Transfer = false;
		ImageFormat Format = ImageFormat::RGBA;
		ImageUsage Usage = ImageUsage::Texture;
		//represent the imageViewType for images that are created using 
		ImageViewType ImageLayerViewsBaseType = ImageViewType::View2D; 
		uint32_t Width = 1, Height = 1, Depth = 1;
		uint32_t Mips = 1;
		uint32_t Layers = 1;
	};


	class Image : public RendererResource
	{
	public:
		virtual ~Image() = default;	
		virtual glm::uvec2 GetSize() = 0;
		virtual float GetAspectRatio() = 0;;
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
	};
	class Image2D : public Image
	{
	public:
		virtual ~Image2D() = default;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Resize(glm::uvec2 size) = 0;
		virtual const ImageConfiguration& GetSpecification()const = 0;
		static Count<Image2D>Create(const ImageConfiguration& specification);
		static Count<Image2D>Create(const ImageConfiguration& specification, Count<class RenderSampler> sampler);
		static Count<Image2D>Create(const ImageConfiguration& specification, SamplerWrap wrap, SamplerFilter filter);
		virtual Count<class RenderSampler> GetSampler() = 0;

		virtual void ClearImageViews() = 0;
		virtual Count<class ImageView> CreateOrGetImageMip(uint32_t mip, uint32_t layer = 0) = 0;
		virtual void CreateMipAndLayerViews() = 0;
		virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const = 0;

		virtual void GenerateMips() = 0;
		virtual struct Buffer GetStoredDataAsBuffer() = 0;
		RENDER_VIEW_RESOURCE_CLASS_TYPE(Image2D);
	};
	
	struct ImageViewConfiguration
	{
		std::string DebugName;
		Count<Image2D> Image;

		uint32_t Layer = 0;
		uint32_t Mip = 0;

		uint32_t LayerCount = 1;
		uint32_t MipCount = 1;

		ImageViewType View = ImageViewType::View2D;
	};
	class ImageView : public Image 
	{
	public:
		virtual ~ImageView() = default;
		static Count<ImageView>Create(const ImageViewConfiguration& specification);
		virtual const ImageViewConfiguration& GetSpecification()const = 0;
		virtual Count<Image2D> GetImage()const = 0;
		virtual glm::uvec2 GetMipSize() = 0;
		RENDER_VIEW_RESOURCE_CLASS_TYPE(ImageView);

	};

	// basiclly for framebuffers and swapchain which store 3 iamges
	// image index
	struct ImageLayouts2D 
	{

		ImageLayouts2D(const ImageLayouts2D&) = default;
		ImageLayouts2D(Count<Image2D> image);
		ImageLayouts2D(std::initializer_list<Count<Image2D>> images);
		ImageLayouts2D(const std::vector<Count<Image2D>>& images);
		ImageLayouts2D(Count<ImageView> image);
		ImageLayouts2D(std::initializer_list<Count<ImageView>> images);
		ImageLayouts2D(const std::vector<Count<ImageView>>& images);
		ImageLayouts2D();

		bool HasImages()const {
			return Images.size() > 0;
		}

		Count<Image> GetImageIndex(uint32_t index)const
		{
			PF_CORE_ASSERT(index < Images.size(), " Does not contain image Index");
			return Images[index];
		}
		// only image2D AND ImageView
		std::vector<Count<Image>> Images;
	};

}