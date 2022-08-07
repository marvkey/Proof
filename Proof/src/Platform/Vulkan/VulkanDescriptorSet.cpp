#include "Proofprch.h"
#include "VulkanDescriptorSet.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"

namespace Proof
{

	VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count) {
		PF_CORE_ASSERT(Bindings.count(binding) == 0, "Binding already in use");
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = stageFlags;
		Bindings[binding] = layoutBinding;
		return *this;
	}


	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto kv : bindings) {
			setLayoutBindings.emplace_back(kv.second);
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

		if (vkCreateDescriptorSetLayout(graphicsContext->GetDevice(),&descriptorSetLayoutInfo,nullptr,&m_DescriptorSetLayout) != VK_SUCCESS) {
			PF_CORE_ASSERT(false,"failed to create descriptor set layout!");
		}
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
	}

	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::AddPoolSize(VkDescriptorType descriptorType, uint32_t count) {
		PoolSizes.push_back({ descriptorType, count });
		return *this;
	}

	Count<VulkanDescriptorPool> VulkanDescriptorPool::Builder::Build() {
		return CreateCount<VulkanDescriptorPool>(*this);
	}

	VulkanDescriptorPool::VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.flags = poolFlags;

		if (vkCreateDescriptorPool(graphicsContext->GetDevice(), &descriptorPoolInfo, nullptr, &m_DescriptorPool) !=VK_SUCCESS) {
			PF_CORE_ASSERT(false,"failed to create descriptor pool!");
		}
	}



	VulkanDescriptorPool::VulkanDescriptorPool(const VulkanDescriptorPool::Builder& build) {
		VulkanDescriptorPool(build.MaxSets, build.PoolFlags, build.PoolSizes);
	}

	VulkanDescriptorPool::~VulkanDescriptorPool() {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		vkDestroyDescriptorPool(device, m_DescriptorPool, nullptr);
	}

	bool VulkanDescriptorPool::AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		allocInfo.descriptorSetCount = 1;

		if (vkAllocateDescriptorSets(device, &allocInfo, &descriptor) != VK_SUCCESS) {
			return false;
		}
		return true;
	}

	void VulkanDescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		vkFreeDescriptorSets(device,m_DescriptorPool,descriptors.size(),descriptors.data());
	}

	void VulkanDescriptorPool::ResetPool() {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		vkResetDescriptorPool(device, m_DescriptorPool, 0);
	}

	VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool):
		m_SetLayout{ setLayout }, m_Pool{ pool } 
	{
	
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
		//PF_CORE_ASSERT(m_SetLayout.m_Bindings.count(binding) == 1 , "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout.m_Bindings[binding];

		//PF_CORE_ASSERT(bindingDescription.m_DescriptorCount == 1, "Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo) {
		//assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout.m_Bindings[binding];

		//assert(
		//	bindingDescription.descriptorCount == 1 &&
		//	"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}

	bool VulkanDescriptorWriter::Build(VkDescriptorSet& set) {
		bool success = m_Pool.AllocateDescriptor(m_SetLayout.GetDescriptorSetLayout(), set);
		if (success == false)
			return false;
		Overwrite(set);
		return true;
	}

	void VulkanDescriptorWriter::Overwrite(VkDescriptorSet& set) {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();

		for (auto& write : writes) {
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(device, writes.size(), writes.data(), 0, nullptr);
	}


	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t set, uint32_t binding) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		VkDeviceSize bufferSize = size;

		m_UniformBuffers.resize(Renderer::GetConfig().FramesFlight);
		m_UniformBuffersMemory.resize(Renderer::GetConfig().FramesFlight);

		for (size_t i = 0; i < Renderer::GetConfig().FramesFlight; i++) {
			graphicsContext->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBuffersMemory[i]);
		}
	}

	VulkanUniformBuffer::~VulkanUniformBuffer() {
		auto device= Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		for (size_t i = 0; i < Renderer::GetConfig().FramesFlight; i++) {
			vkDestroyBuffer(device, m_UniformBuffers[i], nullptr);
			vkFreeMemory(device, m_UniformBuffersMemory[i], nullptr);
		}
	}
	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset, uint32_t frameIndex) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		void* uniformData;
		vkMapMemory(graphicsContext->GetDevice(), m_UniformBuffersMemory[frameIndex], offset, size, 0, &uniformData);

		memcpy(uniformData, data, (size_t)size);
		vkUnmapMemory(graphicsContext->GetDevice(), m_UniformBuffersMemory[frameIndex]);
	}

	
}
