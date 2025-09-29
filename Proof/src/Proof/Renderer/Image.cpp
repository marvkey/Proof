#include "Proofprch.h"	
#include "Image.h"
#include "Renderer.h"
#include "Proof/Core/Core.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
#include "Proof/Platform/Vulkan/VulkanUtils/VulkanConvert.h"
#include "RendererSampler.h"
namespace Proof
{
	SamplerResourceConfig imageSamplerConfig("Image2DDefaultSampler", SamplerWrap::ClampEdge, SamplerFilter::Linear, SamplerBorderColor::OpaqueWhiteInt, SamplerMipMapMode::Linear);

	Count<Image2D> Image2D::Create(const ImageConfiguration& specification)
	{
		imageSamplerConfig.DebugName = "Image2DDefaultSampler";

		if (Utils::IsIntegerBased(specification.Format))
		{

			imageSamplerConfig.Filter = SamplerFilter::Nearest;
			imageSamplerConfig.MipMapMode = SamplerMipMapMode::Nearest;
		}
		else
		{

			imageSamplerConfig.Filter = SamplerFilter::Linear;
			imageSamplerConfig.MipMapMode = SamplerMipMapMode::Linear;
		}

		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImage2D>::Create(specification,SamplerFactory::GetOrAddSamplerToFactory(imageSamplerConfig));
		}
		return nullptr;
	}

	Count<Image2D> Image2D::Create(const ImageConfiguration& specification, Count<class RenderSampler> sampler)
	{


		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImage2D>::Create(specification, sampler);
		}
		return nullptr;
	}

	Count<Image2D> Image2D::Create(const ImageConfiguration& specification, SamplerWrap wrap, SamplerFilter filter)
	{
		imageSamplerConfig.DebugName = "Image2DCustomSampler";
		imageSamplerConfig.Filter = filter;
		imageSamplerConfig.Wrap	= wrap;

		if (Utils::IsIntegerBased(specification.Format))
			imageSamplerConfig.MipMapMode = SamplerMipMapMode::Nearest;
		else
			imageSamplerConfig.MipMapMode = SamplerMipMapMode::Linear;

		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImage2D>::Create(specification, SamplerFactory::GetOrAddSamplerToFactory(imageSamplerConfig));
		}
		return nullptr;
	}

	Count<ImageView> ImageView::Create(const ImageViewConfiguration& specification)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImageView>::Create(specification);
		}
		return nullptr;
	}
	ImageLayouts2D::ImageLayouts2D(Count<Image2D> image)
	{
		Images.resize(Renderer::GetConfig().MaxImageCount);
		for (int i = 0; i < Renderer::GetConfig().MaxImageCount; i++)
			Images[i] = image;
	}

	ImageLayouts2D::ImageLayouts2D(std::initializer_list<Count<Image2D>> images)
	{
		PF_CORE_ASSERT(images.size() == Renderer::GetConfig().MaxImageCount, "Initilizer list must equalt image count");

		Images.resize(Renderer::GetConfig().MaxImageCount);
		for (int i = 0; i < Renderer::GetConfig().MaxImageCount; i++)
			Images[i] = *(images.begin() + i);
	}

	ImageLayouts2D::ImageLayouts2D(const std::vector<Count<Image2D>>& images)
	{
		PF_CORE_ASSERT(images.size() == Renderer::GetConfig().MaxImageCount, "Vector must equalt image count");
		Images.resize(Renderer::GetConfig().MaxImageCount);
		for (int i = 0; i < Renderer::GetConfig().MaxImageCount; i++)
			Images[i] = images[i];
	}

	ImageLayouts2D::ImageLayouts2D(Count<ImageView> image)
	{
		Images.resize(Renderer::GetConfig().MaxImageCount);
		for (int i = 0; i < Renderer::GetConfig().MaxImageCount; i++)
			Images[i] = image;
	}

	ImageLayouts2D::ImageLayouts2D(std::initializer_list<Count<ImageView>> images)
	{
		PF_CORE_ASSERT(images.size() == Renderer::GetConfig().MaxImageCount, "Initilizer list must equalt image count");

		Images.resize(Renderer::GetConfig().MaxImageCount);
		for (int i = 0; i < Renderer::GetConfig().MaxImageCount; i++)
			Images[i] = *(images.begin() + i);
	}

	ImageLayouts2D::ImageLayouts2D(const std::vector<Count<ImageView>>& images)
	{
		PF_CORE_ASSERT(images.size() == Renderer::GetConfig().MaxImageCount, "Vector must equalt image count");
		Images.resize(Renderer::GetConfig().MaxImageCount);
		for (int i = 0; i < Renderer::GetConfig().MaxImageCount; i++)
			Images[i] = images[i];
	}
	ImageLayouts2D::ImageLayouts2D()
	{
		//Images.resize(Renderer::GetConfig().MaxImageCount);
	}
}