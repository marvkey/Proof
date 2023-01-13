#include "Proofprch.h"
#include "VulkanDescriptorSet.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanPipeLineLayout.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanTexutre.h"
#include "VulkanUtils/VulkanConvert.h"
namespace Proof
{
	namespace Utils
	{
		VkDescriptorType ProofFormatToVulkanFormat(DescriptorType data) {
			switch (data) {
				case Proof::DescriptorType::ImageSampler:
					return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				case Proof::DescriptorType::UniformBuffer:
					return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				case Proof::DescriptorType::StorageBuffer:
					return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				default:
					break;
			}
			PF_CORE_ASSERT("Format not supported");
		}
	}
	
	Count<VulkanDescriptorSet> VulkanDescriptorSet::VulkanDescriptorSetBuilder::Build() {
		return CreateCount<VulkanDescriptorSet>(m_Set, Bindings);
	}
	VulkanDescriptorSet::VulkanDescriptorSetBuilder& VulkanDescriptorSet::VulkanDescriptorSetBuilder::AddBinding(uint32_t binding, DescriptorType descriptorType, ShaderStage  stageFlags, uint32_t count) {
		PF_CORE_ASSERT(Bindings.count(binding) == 0, "Binding already in use");
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = Utils::ProofFormatToVulkanFormat(descriptorType);
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags =Utils::ProofShaderToVulkanShader( stageFlags);
		Bindings[binding] = layoutBinding;
		return *this;
	}


	VulkanDescriptorSet::VulkanDescriptorSet(DescriptorSets set, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
	{
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		m_Set = set;
		m_Writer = CreateSpecial<VulkanDescriptorWriter>(this, graphicsContext->GetGlobalPool());
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

		m_DescriptorSets.resize(Renderer::GetConfig().FramesFlight);

	}

	VulkanDescriptorSet::~VulkanDescriptorSet() {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
	}

	DescriptorSet& VulkanDescriptorSet::WriteBuffer(uint32_t binding, Count<UniformBuffer> buffer) {
		auto bufferInfo = buffer->As<VulkanUniformBuffer>()->GetDescriptorInfo();
		if (bufferInfo.range == 0 || bufferInfo.buffer == nullptr)
		{
			PF_ENGINE_WARN("Cannot write Uniform Buffer to descriptor set with size 0 OR buffer == nullptr");
			return *this;
		}
		m_Writer->WriteBuffer(binding, &bufferInfo);
		return *this;
	}

	DescriptorSet& VulkanDescriptorSet::WriteBuffer(uint32_t binding, Count<StorageBuffer> buffer) {
		auto bufferInfo = buffer->As<VulkanStorageBuffer>()->GetDescriptorInfo();
		if (bufferInfo.range == 0 || bufferInfo.buffer ==nullptr)
		{
			PF_ENGINE_WARN("Cannot write Storage buffer to descriptor set with size 0 OR buffer == nullptr");
			return *this;
		}
		m_Writer->WriteBuffer(binding, &bufferInfo);
		return *this;
	}

	DescriptorSet& VulkanDescriptorSet::WriteImage(uint32_t binding, Count<class Texture2D> image) {
		auto imageInfo = image->As<VulkanTexture2D>()->GetImageBufferInfo();
		m_Writer->WriteImage(binding, &imageInfo);
		return *this;
	}

	DescriptorSet& VulkanDescriptorSet::WriteImage(uint32_t binding, std::vector<Count<class Texture2D>> images) {
		std::vector< VkDescriptorImageInfo> info;
		auto whiteImageInfo = Renderer::GetWhiteTexture()->As<VulkanTexture2D>()->GetImageBufferInfo();
		for (auto image : images) {
			if (image == nullptr) {
				//VkDescriptorImageInfo imageInfo;
				//imageInfo.imageView = nullptr;
				//imageInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				//imageInfo.sampler = nullptr;
				info.emplace_back(whiteImageInfo);
				continue;
			}
			auto imageInfo = image->As<VulkanTexture2D>()->GetImageBufferInfo();
			info.emplace_back(imageInfo);
		}
		m_Writer->WriteImage(binding, info);
		return *this;
	}

	bool VulkanDescriptorSet::Build(int frame) {
		return m_Writer->Build(m_DescriptorSets[frame]);
	}

	void VulkanDescriptorSet::Overwrite(int frame) {
		return m_Writer->Overwrite(m_DescriptorSets[frame]);
	}

	void VulkanDescriptorSet::Bind(Count<class CommandBuffer> commandBuffer, Count<class PipeLineLayout>pipeLineLayout) {
		Build();
		vkCmdBindDescriptorSets(
			commandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeLineLayout->As<VulkanPipeLineLayout>()->GetPipeLineLayout(),
			(int)m_Set,
			1,
			&m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight],
			0,
			nullptr);
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
		//VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		//descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		////descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		//descriptorPoolInfo.maxSets = maxSets;
		//descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		//descriptorPoolInfo.flags = poolFlags;
		//descriptorPoolInfo.pPoolSizes = poolSizes.data();
		//
		//if (vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &m_DescriptorPool) !=VK_SUCCESS) {
		//	PF_CORE_ASSERT(false,"failed to create descriptor pool!");
		//}
		// 
		m_PoolSizes = poolSizes;
		m_MaxSets = maxSets;
		m_PoolFlags = poolFlags;
		//there are reusable pools availible
		if (m_FreePools.size() > 0) {
			//grab pool from the back of the vector and remove it from there.
			VkDescriptorPool pool = m_FreePools.back();
			m_FreePools.pop_back();
		}
		else {
		}
		m_CurrentPool = CreatePool(device, maxSets, poolFlags);
	}



	VulkanDescriptorPool::~VulkanDescriptorPool() {
		//vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
	}



	void VulkanDescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
		//vkFreeDescriptorSets(m_Device,m_DescriptorPool,descriptors.size(),descriptors.data());
	}
	VkDescriptorSetLayout TextreuLayout;
	VkSampler fontSampler;
	void InitTextureLayout() {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		if (fontSampler == nullptr){
			VkSamplerCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.magFilter = VK_FILTER_LINEAR;
			info.minFilter = VK_FILTER_LINEAR;
			info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.minLod = -1000;
			info.maxLod = 1000;
			info.maxAnisotropy = 1.0f;
			VkResult err = vkCreateSampler(graphicsContext->GetDevice(), &info, nullptr, &fontSampler);
		}
		VkSampler sampler[1] = { fontSampler };
		VkDescriptorSetLayoutBinding binding[1] = {};
		binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		binding[0].descriptorCount = 1;
		binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		binding[0].pImmutableSamplers = sampler;
		VkDescriptorSetLayoutCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.bindingCount = 1;
		info.pBindings = binding;
		VkResult err = vkCreateDescriptorSetLayout(graphicsContext->GetDevice(), &info, nullptr, &TextreuLayout);
	}
	VkDescriptorSet VulkanDescriptorPool::AddTexture(VkSampler sampler, VkImageView image_view, VkImageLayout image_layout) {
		if (TextreuLayout == nullptr)
			InitTextureLayout();
		// Create Descriptor Set:
		VkDescriptorSet descriptor_set;
		{
			Allocate(&descriptor_set, TextreuLayout);
		}

		// Update the Descriptor Set:
		{
			VkDescriptorImageInfo desc_image[1] = {};
			desc_image[0].sampler = sampler;
			desc_image[0].imageView = image_view;
			desc_image[0].imageLayout = image_layout;
			VkWriteDescriptorSet write_desc[1] = {};
			write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_desc[0].dstSet = descriptor_set;
			write_desc[0].descriptorCount = 1;
			write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_desc[0].pImageInfo = desc_image;
			vkUpdateDescriptorSets(m_Device, 1, write_desc, 0, NULL);
		}
		return descriptor_set;
	}

	void VulkanDescriptorPool::ResetPool() {
		//reset all used pools and add them to the free pools
		for (auto p : m_UsedPools) {
			vkResetDescriptorPool(m_Device, p, 0);
			m_FreePools.push_back(p);
		}

		//clear the used pools, since we've put them all in the free pools
		m_UsedPools.clear();

		//reset the current pool handle back to null
		m_CurrentPool = VK_NULL_HANDLE;
	}

	bool VulkanDescriptorPool::Allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout) {
		//initialize the currentPool handle if it's null
		if (m_CurrentPool == VK_NULL_HANDLE) {

			m_CurrentPool = GrabPool();
			m_UsedPools.push_back(m_CurrentPool);
		}

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;

		allocInfo.pSetLayouts = &layout;
		allocInfo.descriptorPool = m_CurrentPool;
		allocInfo.descriptorSetCount = 1;

		//try to allocate the descriptor set
		VkResult allocResult = vkAllocateDescriptorSets(m_Device, &allocInfo, set);
		bool needReallocate = false;

		switch (allocResult) {
			case VK_SUCCESS:
				//all good, return
				return true;
			case VK_ERROR_FRAGMENTED_POOL:
			case VK_ERROR_OUT_OF_POOL_MEMORY:
				//reallocate pool
				needReallocate = true;
				break;
			default:
				//unrecoverable error
				return false;
		}

		if (needReallocate) {
			//allocate a new pool and retry
			m_CurrentPool = GrabPool();
			m_UsedPools.push_back(m_CurrentPool);

			allocResult = vkAllocateDescriptorSets(m_Device, &allocInfo, set);

			//if it still fails then we have big issues
			if (allocResult == VK_SUCCESS) {
				return true;
			}
		}
		PF_CORE_ASSERT(false);
		return false;
	}

	VkDescriptorPool VulkanDescriptorPool::CreatePool(VkDevice device, int count, VkDescriptorPoolCreateFlags flags) {
		std::vector<VkDescriptorPoolSize> sizes;
		sizes.reserve(m_PoolSizes.size());
		for (auto sz : m_PoolSizes) {
			sizes.push_back({ sz.type, uint32_t(sz.descriptorCount * count) });
		}
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = flags;
		pool_info.maxSets = count;
		pool_info.poolSizeCount = (uint32_t)sizes.size();
		pool_info.pPoolSizes = sizes.data();

		VkDescriptorPool descriptorPool;
		vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptorPool);

		return descriptorPool;
	}

	VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSet* setLayout, Count<VulkanDescriptorPool> pool):
		m_SetLayout{ setLayout }, m_Pool{ pool } 
	{
	
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
		PF_CORE_ASSERT(m_SetLayout->m_Bindings.count(binding) == 1, "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout->m_Bindings[binding];

		PF_CORE_ASSERT(bindingDescription.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple");
		VkDescriptorBufferInfo copyBuffer = *bufferInfo;
		m_Buffers.push_back(copyBuffer);

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = new VkDescriptorBufferInfo(copyBuffer); // leave this the way it is we will fix in future 
		// very very very very very very very buggy
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo) {
		PF_CORE_ASSERT(m_SetLayout->m_Bindings.count(binding) == 1,"Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout->m_Bindings[binding];

		PF_CORE_ASSERT(bindingDescription.descriptorCount == 1,"Binding single descriptor info, but binding expects multiple");
		m_Images.emplace_back(*imageInfo);

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = &m_Images[m_Images.size()-1];
		write.descriptorCount = 1;

		writes.push_back(write);
		return *this;
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteImage(uint32_t binding, std::vector<VkDescriptorImageInfo> imageInfo) {
		 PF_CORE_ASSERT(m_SetLayout->m_Bindings.count(binding) == 1, "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout->m_Bindings[binding];

		uint32_t firstPos = m_Images.size();
		for (auto i : imageInfo) {
			m_Images.emplace_back(i);
		}

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = &m_Images[firstPos];// the first element it started on
		write.descriptorCount = imageInfo.size();

		writes.push_back(write);
		return *this;
	}

	bool VulkanDescriptorWriter::Build(VkDescriptorSet& set) {
		const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		graphicsContext->GetGlobalPool()->Allocate(&m_SetLayout->m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight], m_SetLayout->m_DescriptorSetLayout);
		Overwrite(m_SetLayout->m_DescriptorSets[Renderer::GetCurrentFrame().FrameinFlight]);
		return true;
	}



	void VulkanDescriptorWriter::Overwrite(VkDescriptorSet& set) {
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
		for (auto& write : writes) {
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(device, writes.size(), writes.data(), 0, nullptr);
		for (auto & write : writes) {
			delete write.pBufferInfo;
		}
		writes.clear();
		m_Buffers.clear();
		m_Images.clear();
	}


	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, DescriptorSets set, uint32_t binding):
		m_Size(size),m_Set(set), m_Binding(binding)
	{
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		m_UniformBuffers.resize(Renderer::GetConfig().FramesFlight);
		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;

		uniformBufferInfo.size = m_Size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		for (int i = 0; i < m_UniformBuffers.size(); i++) {
			graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_UniformBuffers[i]);
		}
	}
	VulkanUniformBuffer::VulkanUniformBuffer(const void* data, uint32_t size, DescriptorSets set, uint32_t binding)
	{
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		m_UniformBuffers.resize(Renderer::GetConfig().FramesFlight);
		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;

		uniformBufferInfo.size = m_Size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		for (int i = 0; i < m_UniformBuffers.size(); i++)
		{
			graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_UniformBuffers[i]);
		}
		SetData(data, size);
	}
	VkDescriptorBufferInfo VulkanUniformBuffer::GetDescriptorInfo(uint32_t index)
	{
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		return {
			m_UniformBuffers[Renderer::GetCurrentFrame().FrameinFlight].Buffer,
			0,
			m_Size,
		};
	}

	VulkanUniformBuffer::~VulkanUniformBuffer() {
		for (int i = 0; i < m_UniformBuffers.size(); i++)
		{
			Renderer::SubmitDatafree([buffer = m_UniformBuffers[i]]() {
				auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
				vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), buffer.Buffer, buffer.Allocation);
			});
		}
	}
	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset, uint32_t frameIndex) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		VulkanBuffer stagingBuffer;

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, size);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);

		Renderer::Submit([&](CommandBuffer* cmdBuffer){
			VkBufferCopy copy;
			copy.dstOffset = offset;
			copy.srcOffset = 0;
			copy.size = size;
			vkCmdCopyBuffer((VkCommandBuffer)cmdBuffer->Get(), stagingBuffer.Buffer, m_UniformBuffers[Renderer::GetCurrentFrame().FrameinFlight].Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);

	}
	

	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
		//for (int i = 0; i < m_StorageBuffer.size(); i++)
		//{
		//	Renderer::SubmitDatafree([buffer = m_StorageBuffer[i]]() {
		//		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		//		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), buffer.Buffer, buffer.Allocation);
		//	});
		//}
	}

	VulkanStorageBuffer::VulkanStorageBuffer(DescriptorSets set, uint32_t binding, const void* data, uint32_t size, uint32_t offset, uint32_t frameIndex)
		:
		m_Set(set), m_Binding(binding)

	{
		m_StorageBuffer.resize(Renderer::GetConfig().FramesFlight);
		if (size == 0)
		{
			PF_ENGINE_WARN("Cannot set storage buffer with size 0");
			return;
		}
		m_Size = size;
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		{
			Renderer::Submit([&](CommandBuffer* cmdBuffer) {

				m_StorageBuffer.resize(Renderer::GetConfig().FramesFlight);
				VkBufferCreateInfo uniformBufferInfo = {};
				uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				uniformBufferInfo.pNext = nullptr;

				uniformBufferInfo.size = m_Size;
				uniformBufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				VmaAllocationCreateInfo vmaallocInfo = {};
				vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
				for (int i = 0; i < m_StorageBuffer.size(); i++)
				{
					graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_StorageBuffer[i]);
				}
			});
		}

		VulkanBuffer stagingBuffer;

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = m_Size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, m_Size);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		Renderer::Submit([&](CommandBuffer* cmdBuffer) {
			VkBufferCopy copy;
			copy.dstOffset = offset;
			copy.srcOffset = 0;
			copy.size = m_Size;
			vkCmdCopyBuffer((VkCommandBuffer)cmdBuffer->Get(), stagingBuffer.Buffer, m_StorageBuffer[Renderer::GetCurrentFrame().FrameinFlight].Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);

	}

	VkDescriptorBufferInfo VulkanStorageBuffer::GetDescriptorInfo(uint32_t index)
	{
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		return {
				m_StorageBuffer[Renderer::GetCurrentFrame().FrameinFlight].Buffer,
				0,
				m_Size,
		};
	}
}
