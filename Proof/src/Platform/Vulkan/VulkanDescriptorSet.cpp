#include "Proofprch.h"
#include "VulkanDescriptorSet.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanRenderer/VulkanRenderer.h"
namespace Proof
{
	Count<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::Build() {
		return CreateCount<VulkanDescriptorSetLayout>(Bindings);
	}

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
		m_Bindings = bindings;

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

	Count<VulkanDescriptorPool> VulkanDescriptorPool::Builder::Build(VkDevice device) {
		return CreateCount<VulkanDescriptorPool>(MaxSets, PoolFlags,PoolSizes,device);
	}

	VulkanDescriptorPool::VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes, VkDevice& device):
	m_Device(device) {
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.flags = poolFlags;

		if (vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &m_DescriptorPool) !=VK_SUCCESS) {
			PF_CORE_ASSERT(false,"failed to create descriptor pool!");
		}
	}



	VulkanDescriptorPool::~VulkanDescriptorPool() {
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
	}

	bool VulkanDescriptorPool::AllocateDescriptor(VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		allocInfo.descriptorSetCount = 1;
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		
		if (vkAllocateDescriptorSets(graphicsContext->GetDevice(), &allocInfo, &descriptor) != VK_SUCCESS) {
			return false;
		}
		return true;
	}

	void VulkanDescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
		vkFreeDescriptorSets(m_Device,m_DescriptorPool,descriptors.size(),descriptors.data());
	}

	void VulkanDescriptorPool::ResetPool() {
		vkResetDescriptorPool(m_Device, m_DescriptorPool, 0);
	}

	VulkanDescriptorWriter::VulkanDescriptorWriter(Count<VulkanDescriptorSetLayout> setLayout, Count<VulkanDescriptorPool> pool):
		m_SetLayout{ setLayout }, m_Pool{ pool } 
	{
	
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
		PF_CORE_ASSERT(m_SetLayout->m_Bindings.count(binding) == 1, "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout->m_Bindings[binding];

		PF_CORE_ASSERT(bindingDescription.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo imageInfo) {
		//assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout->m_Bindings[binding];

		//assert(
		//	bindingDescription.descriptorCount == 1 &&
		//	"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = &imageInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}

	bool VulkanDescriptorWriter::Build(VkDescriptorSet& set) {
		bool success = m_Pool->AllocateDescriptor(m_SetLayout->GetDescriptorSetLayout(), set);
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


	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t set, uint32_t binding):
		m_Size(size),m_Set(set), m_Binding(binding)
	{
		/*
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		m_UniformBuffers.resize(Renderer::GetConfig().FramesFlight);

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = m_Size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
		VulkanBuffer stagingBuffer;

		for (int i = 0; i < Renderer::GetConfig().FramesFlight; i++) {
			graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
			void* stagingData;
			vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);
			memcpy(stagingData, uniformData, m_Size);
			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		}

		VulkanRenderer::Submit([&](VkCommandBuffer& cmdBuffer) {
			VkBufferCreateInfo uniformBufferInfo = {};
			uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			uniformBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			uniformBufferInfo.size = m_Size;
			//this buffer is going to be used as a UNIFORM Buffer
			// ad goign to be a desitination bit
			uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
			for (int i = 0; i < Renderer::GetConfig().FramesFlight; i++) {

				graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_UniformBuffers[i]);
				VkBufferCopy copy;
				copy.dstOffset = 0;
				copy.srcOffset = 0;
				copy.size = m_Size;
				vkCmdCopyBuffer(cmdBuffer, stagingBuffer.Buffer, m_UniformBuffers[i].Buffer, 1, &copy);
			}
		});
		*/
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		m_UniformBuffers.resize(Renderer::GetConfig().FramesFlight);
		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;

		uniformBufferInfo.size = size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		for (int i = 0; i < m_UniformBuffers.size(); i++) {
			graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_UniformBuffers[i]);
		}

	}

	VulkanUniformBuffer::~VulkanUniformBuffer() {
		//auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		//for (int i = 0; i < Renderer::GetConfig().FramesFlight; i++) {
		//	vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), m_UniformBuffers[i].Buffer, m_UniformBuffers[i].Allocation);
		//}
	}
	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset, uint32_t frameIndex) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		//void* uniformData;
		//vkMapMemory(graphicsContext->GetDevice(), m_UniformBuffersMemory[frameIndex], offset, size, 0, &uniformData);
		//
		//memcpy(uniformData, data, (size_t)size);
		//vkUnmapMemory(graphicsContext->GetDevice(), m_UniformBuffersMemory[frameIndex]);
		

			void* uniformData;
			vmaMapMemory(graphicsContext->GetVMA_Allocator(), m_UniformBuffers[frameIndex].Allocation, &uniformData);
			memcpy(uniformData, data, size);
			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), m_UniformBuffers[frameIndex].Allocation);
	}

	
	VulkanDescriptorSet::VulkanDescriptorSet() {
		m_DescriptorSets.resize(Renderer::GetConfig().FramesFlight);
	}

	void VulkanDescriptorSet::Write(Count<VulkanDescriptorSetLayout> layout) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		//for (int i = 0; i < m_DescriptorSets.size(); i++) {
		//	VulkanDescriptorWriter(*layout.get(),)
		//}
	}

}
