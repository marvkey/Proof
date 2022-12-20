#include "Proofprch.h"
#include "UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/Vulkan/VulkanDescriptorSet.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanTexutre.h"
#include "RendererAPI.h"
namespace Proof
{
	Count<UniformBuffer> UniformBuffer::Create(uint32_t size, DescriptorSets set, uint32_t bindingPoint) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount<OpenGLUniformBuffer>(size, bindingPoint);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanUniformBuffer>(size, set, bindingPoint);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Count<DescriptorSet> DescriptorSet::Create(DescriptorSets set, std::unordered_map<uint32_t, DescriptrLayoutBinding> Bindings) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan:
				{
					auto builder = VulkanDescriptorSet::Builder(set);

					for (auto [i, dlb] : Bindings) {
						builder.AddBinding(dlb.binding, dlb.descriptorType, dlb.shaderStage, dlb.descriptorCount);
					}
					return builder.Build();
				}
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Count<DescriptorSet> DescriptorSet::Builder::Build() {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan:
				{
					auto builder = VulkanDescriptorSet::VulkanDescriptorSetBuilder(m_Set);

					for (auto [i, dlb] : Bindings) {
						builder.AddBinding(dlb.binding, dlb.descriptorType, dlb.shaderStage, dlb.descriptorCount);
					}
					return builder.Build();
				}
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	DescriptorSet::Builder::Builder(DescriptorSets set) {
		m_Set = set;
	}
	
	DescriptorSet::Builder& DescriptorSet::Builder::AddBinding(uint32_t binding, DescriptorType descriptorType, ShaderStage shaderStage, uint32_t count) {
		DescriptrLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = count;
		layoutBinding.shaderStage = shaderStage;
		Bindings[binding] = layoutBinding;
		return *this;
	}

}


