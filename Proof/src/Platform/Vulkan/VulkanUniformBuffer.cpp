#include "Proofprch.h"
#include "VulkanUniformBuffer.h"
#include "VulkanSwapChain.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t binding) {
        m_Size = size;
        m_Binding = binding;
        for (size_t i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->CreateBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffer, m_UniformBufferMemory);
        }       
	}
    VulkanUniformBuffer::~VulkanUniformBuffer() {
        for (size_t i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_UniformBuffer, nullptr);
            vkFreeMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_UniformBufferMemory, nullptr);
        }
    }
    void VulkanUniformBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
        if (size == VK_WHOLE_SIZE) {
            memcpy(m_Mapped, data, size);
        }
        else {
            vkMapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_UniformBufferMemory, 0, size, 0, &data);
            vkUnmapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_UniformBufferMemory);
            /*
            char* memOffset = (char*)m_Mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
            */
        }

        Bind();
    }
    void VulkanUniformBuffer::Bind() {
        vkBindBufferMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_UniformBuffer,m_UniformBufferMemory, 0);
    }
    void VulkanUniformBuffer::Recreate() {
        *this = VulkanUniformBuffer(m_Size, m_Binding);
    }
}
