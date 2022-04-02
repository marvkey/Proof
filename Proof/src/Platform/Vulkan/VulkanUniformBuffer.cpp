#include "Proofprch.h"
#include "VulkanUniformBuffer.h"
#include "VulkanSwapChain.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size, uint32_t binding) {

        for (size_t i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->CreateBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffer, m_UniformBufferMemory);
        }
	}
    VulkanUniformBuffer::~VulkanUniformBuffer() {
        for (size_t i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_UniformBuffer, nullptr);
            vkFreeMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_UniformBufferMemory, nullptr);
        }
    }
    void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
        if (size == VK_WHOLE_SIZE) {
            memcpy(m_Mapped, data, size);
        }
        else {
            char* memOffset = (char*)m_Mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
        }
    }
}
