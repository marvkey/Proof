#include "Proofprch.h"	
#include "Image.h"
#include "Renderer.h"
#include "Proof/Core/Core.h"
namespace Proof
{
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