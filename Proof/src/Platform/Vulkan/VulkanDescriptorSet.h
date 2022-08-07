#pragma once
#include <vulkan/vulkan.h>
#include "Proof/Renderer/Renderer.h"

namespace Proof
{
    enum class UniformBinding {
        CameraData = 0,
        SceneData =1
    };
    class VulkanDescriptorSetLayout {
    public:
        struct Builder {
        public:
            Builder& AddBinding(uint32_t binding,VkDescriptorType descriptorType,VkShaderStageFlags stageFlags,uint32_t count = 1);
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> Bindings{};
        };

        VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        virtual ~VulkanDescriptorSetLayout();

        const VkDescriptorSetLayout& GetDescriptorSetLayout()const  { return m_DescriptorSetLayout; }

        uint32_t GetCount() {
            m_Bindings.size();
        }
    private:
        VkDescriptorSetLayout m_DescriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorPool {
    public:
        struct Builder {
        public:
            Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& SetMaxSets(uint32_t num) {
                MaxSets = num;
                return *this;
            }

            Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags) {
                PoolFlags = flags;
                return *this;

            }
            uint32_t MaxSets =1000;
            VkDescriptorPoolCreateFlags PoolFlags = 0;
            std::vector<VkDescriptorPoolSize> PoolSizes{};
            Count<VulkanDescriptorPool> Build();
        };

        VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
        VulkanDescriptorPool(const VulkanDescriptorPool::Builder& build);

        virtual ~VulkanDescriptorPool();
        bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void ResetPool();

    private:
        VkDescriptorPool m_DescriptorPool;
        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorWriter {
    public:
        VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool);

        VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkanDescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool Build(VkDescriptorSet& set);
        void Overwrite(VkDescriptorSet& set);

    private:
        VulkanDescriptorSetLayout& m_SetLayout;
        VulkanDescriptorPool& m_Pool;
        std::vector<VkWriteDescriptorSet> writes;
    };


	class VulkanUniformBuffer {
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t set, uint32_t binding);
		virtual ~VulkanUniformBuffer();

		void SetData(const void* data, uint32_t size, uint32_t offset = 0, uint32_t frameIndex = Renderer::GetCurrentFrame());
	private:
		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	};

}
