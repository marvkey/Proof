#pragma once
#include "Proof/Renderer/UniformBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "Vulkan.h"
#include "VulkanUtils/VulkanBufferBase.h"

namespace Proof{
    class VulkanUniformBuffer : public UniformBuffer {
    public:
        VulkanUniformBuffer(uint32_t size);
        VulkanUniformBuffer(const  void* data, uint32_t size);
        virtual ~VulkanUniformBuffer();
        VkBuffer GetBuffer(int index) {
            return m_UniformBuffers[index].Buffer;
        }
        //for unifrm bufffer configuribity stuff
        void SetData(const void* data, uint32_t size, uint32_t offset = 0) {
            SetData(data, size, offset, Renderer::GetCurrentFrame().FrameinFlight);
        }
        void SetData(const void* data, uint32_t size, uint32_t offset, uint32_t frameIndex);
        const VkDescriptorBufferInfo& GetDescriptorInfoVulkan(uint32_t frmaeIndex = Renderer::GetCurrentFrame().FrameinFlight)const {
            m_BufferInfo = {
                m_UniformBuffers[frmaeIndex].Buffer,
                0,
                m_Size,
            };
            return m_BufferInfo;
        }
        virtual ResourceDescriptorInfo GetResourceDescriptorInfo(uint32_t  frameIndex = Renderer::GetCurrentFrame().FrameinFlight)const { return (ResourceDescriptorInfo)&GetDescriptorInfoVulkan(frameIndex); }
    private:
        // multiple of this cause of frames in flight
        // we do not want to right to a uniform for the next frame
        // while a uniform is still being read by the gpu on the current frame
        std::vector<VulkanBuffer> m_UniformBuffers;
        mutable VkDescriptorBufferInfo m_BufferInfo;
        uint32_t m_Size = 0;
    };

    class VulkanStorageBuffer : public StorageBuffer {
    public:
        virtual ~VulkanStorageBuffer();
        VulkanStorageBuffer(const void* data, uint32_t size, uint32_t offset = 0, uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        const VkDescriptorBufferInfo& GetDescriptorInfoVulkan(uint32_t frmaeIndex = Renderer::GetCurrentFrame().FrameinFlight)const {
            m_BufferInfo = {
                m_StorageBuffer[frmaeIndex].Buffer,
                0,
                m_Size,
            };
            return m_BufferInfo;
        }
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) {};
        virtual ResourceDescriptorInfo GetResourceDescriptorInfo(uint32_t  frameIndex = Renderer::GetCurrentFrame().FrameinFlight)const { return (ResourceDescriptorInfo)&GetDescriptorInfoVulkan(frameIndex); }
    private:
        uint32_t m_Size = 0;
        mutable VkDescriptorBufferInfo m_BufferInfo;
        std::vector<VulkanBuffer> m_StorageBuffer;
    };

}
