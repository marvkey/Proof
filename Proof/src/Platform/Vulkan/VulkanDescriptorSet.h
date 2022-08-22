#pragma once
#include <vulkan/vulkan.h>
#include "Proof/Renderer/Renderer.h"
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{
  
    class VulkanDescriptorSetLayout {
    public:
        struct Builder {
        public:
            Builder& AddBinding(uint32_t binding,VkDescriptorType descriptorType,VkShaderStageFlags stageFlags,uint32_t count = 1);
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> Bindings{};

            Count<VulkanDescriptorSetLayout> Build();
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
            Count<VulkanDescriptorPool> Build(VkDevice device);
        };

        VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes, VkDevice& device);

        virtual ~VulkanDescriptorPool();
        bool AllocateDescriptor(VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void ResetPool();

        VkDescriptorPool GetPool() {
            return m_DescriptorPool;
        }
    private:
        VkDescriptorPool m_DescriptorPool;
        VkDevice m_Device;
        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorWriter {
    public:
        VulkanDescriptorWriter(Count<VulkanDescriptorSetLayout> setLayout, Count<VulkanDescriptorPool> pool);

        VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkanDescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo imageInfo);

        bool Build(VkDescriptorSet& set);
        void Overwrite(VkDescriptorSet& set);
        std::vector<VkWriteDescriptorSet> writes;

    private:
        Count<VulkanDescriptorSetLayout> m_SetLayout;
        Count<VulkanDescriptorPool> m_Pool;
    };

    class VulkanDescriptorSet {
    public:
        VulkanDescriptorSet();
        void Write(Count<VulkanDescriptorSetLayout> layout);
    private:
        std::vector<VulkanDescriptorSet> m_DescriptorSets;
    };
	class VulkanUniformBuffer {
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t set, uint32_t binding);
		virtual ~VulkanUniformBuffer();
        VkBuffer GetBuffer(int index) {
            return m_UniformBuffers[index].Buffer;
        }
        VkDescriptorBufferInfo GetDescriptorInfo(int index) {
            return {
                m_UniformBuffers[index].Buffer,
                0,
                m_Size,
            };
        }
		void SetData(const void* data, uint32_t size, uint32_t offset = 0, uint32_t frameIndex = Renderer::GetCurrentFrame());
	private:
        // multiple of this cause of frames in flight
        // we do not want to right to a uniform for the next frame
        // while a uniform is still being read by the gpu on the current frame
		std::vector<VulkanBuffer> m_UniformBuffers;

        uint32_t m_Size = 0;
        uint32_t m_Set;
        uint32_t m_Binding;
	};

}
