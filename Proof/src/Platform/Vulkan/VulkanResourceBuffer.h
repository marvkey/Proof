#pragma once
#include "Proof/Renderer/UniformBuffer.h"
#include "Vulkan.h"
#include "VulkanUtils/VulkanBufferBase.h"

namespace Proof{
    class VulkanUniformBuffer : public UniformBuffer {
    public:
        VulkanUniformBuffer(uint64_t size);
        VulkanUniformBuffer(Buffer data);
        virtual ~VulkanUniformBuffer();
        VkBuffer GetBuffer() { return m_UniformBuffer.Buffer; }
        
        void SetData(Buffer data, uint64_t offset = 0);
        const VkDescriptorBufferInfo& GetDescriptorInfoVulkan()const {
            m_BufferInfo = {
                m_UniformBuffer.Buffer,
                0,
                m_Size,
            };
            return m_BufferInfo;
        }
        void Resize(uint64_t size);
        void Resize(Buffer data);
        virtual uint64_t GetSize() { return m_Size; }
        virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const { return (ResourceDescriptorInfo)&GetDescriptorInfoVulkan(); }
    private:
        VulkanBuffer m_UniformBuffer;
        mutable VkDescriptorBufferInfo m_BufferInfo;
        uint64_t m_Size = 0;
        VulkanMemmoryUsage m_MemoryUsage;
        void Release();
        void Build();
    };

    class VulkanUniformBufferSet : public UniformBufferSet
    {
    public:
        VulkanUniformBufferSet(uint64_t size);
        VulkanUniformBufferSet(Buffer data);
        virtual ~VulkanUniformBufferSet();
        Count<UniformBuffer> GetBuffer(uint32_t index);

        void Resize(uint32_t index, uint64_t size);
        void Resize(uint32_t index,Buffer data);
        void SetData(uint32_t index,Buffer data, uint64_t offset = 0);

    private:
        std::map<uint32_t, Count<UniformBuffer>> m_Buffers;
    };

    class VulkanStorageBuffer : public StorageBuffer {
    public:
        virtual ~VulkanStorageBuffer();
        VulkanStorageBuffer(uint64_t size);
        VulkanStorageBuffer(Buffer data);
        const VkDescriptorBufferInfo& GetDescriptorInfoVulkan()const {
            m_BufferInfo = {
                m_StorageBuffer.Buffer,
                0,
                m_Size,
            };
            return m_BufferInfo;
        }
        void SetData(Buffer data, uint64_t offset = 0);

        void Resize(uint64_t size);
        void Resize(Buffer data);
        virtual uint64_t GetSize() { return m_Size; }
        virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const { return (ResourceDescriptorInfo)&GetDescriptorInfoVulkan(); }
    
    private:
        VulkanMemmoryUsage m_MemoryUsage;
        uint64_t m_Size = 0;
        mutable VkDescriptorBufferInfo m_BufferInfo;
        VulkanBuffer m_StorageBuffer;
        
        void Release();
        void Build();
    };

    class VulkanStorageBufferSet : public StorageBufferSet
    {
    public:
        VulkanStorageBufferSet(uint64_t size);
        VulkanStorageBufferSet(Buffer data);
        virtual ~VulkanStorageBufferSet();
        Count<StorageBuffer> GetBuffer(uint32_t index);

        void Resize(uint32_t index, uint64_t size);
        void Resize(uint32_t index, Buffer data);
        void SetData(uint32_t index, Buffer data, uint64_t offset = 0);

    private:
        std::map<uint32_t, Count<StorageBuffer>> m_Buffers;
    };
}
