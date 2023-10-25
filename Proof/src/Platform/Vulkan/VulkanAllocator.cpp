#include "Proofprch.h"
#include "VulkanAllocator.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanGraphicsContext.h"
#include "Vulkan.h"
namespace Proof
{
    struct VulkanAllocatorData
    {
        VmaAllocator Allocator;
    };
    static VulkanAllocatorData* s_Data = nullptr;
    VulkanAllocator::VulkanAllocator(const std::string& debugName):
        m_DebugName(debugName)
    {

    }
    VmaAllocator VulkanAllocator::GetVmaAllocator()
    {
        return s_Data->Allocator;
    }
    void VulkanAllocator::AllocateBuffer(VkBufferCreateInfo bufferInfo, VmaMemoryUsage usage, VulkanBuffer& buffer)
    {
        PF_CORE_ASSERT(bufferInfo.size > 0, "SIze has to be bigger than 0");
        VmaAllocationCreateInfo vmaInfo = {};
        vmaInfo.usage = usage;
        VK_CHECK_RESULT(vmaCreateBuffer(VulkanAllocator::GetVmaAllocator(), &bufferInfo, &vmaInfo, &buffer.Buffer, &buffer.Allocation, nullptr));
    }

    void VulkanAllocator::AllocateImage(VkImageCreateInfo bufferInfo, VmaMemoryUsage usage, VulkanImageAlloc& image)
    {
        VmaAllocationCreateInfo vmaInfo = {};
        vmaInfo.usage = usage;
        VK_CHECK_RESULT(vmaCreateImage(VulkanAllocator::GetVmaAllocator(), &bufferInfo, &vmaInfo, &image.Image, &image.Allocation, nullptr));
    }

    void VulkanAllocator::DestroyBuffer(VulkanBuffer buffer)
    {
        PF_CORE_ASSERT(buffer.Buffer);
        PF_CORE_ASSERT(buffer.Allocation);
        vmaDestroyBuffer(VulkanAllocator::GetVmaAllocator(), buffer.Buffer, buffer.Allocation);
    }

    void VulkanAllocator::DestroyImage(VulkanImageAlloc image)
    {
        PF_CORE_ASSERT(image.Image);
        PF_CORE_ASSERT(image.Allocation);
        vmaDestroyImage(VulkanAllocator::GetVmaAllocator(), image.Image, image.Allocation);
    }

    void VulkanAllocator::UnmapMemory(VmaAllocation allocation)
    {
        vmaUnmapMemory(VulkanAllocator::GetVmaAllocator(), allocation);
    }

    void VulkanAllocator::Init(VulkanGraphicsContext* context)
    {
        s_Data = pnew VulkanAllocatorData();
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = context->GetGPU();
        allocatorInfo.device = context->GetDevice();
        allocatorInfo.instance = context->GetInstance();
        vmaCreateAllocator(&allocatorInfo, &s_Data->Allocator);
    }

    void VulkanAllocator::ShutDown()
    {
        vmaDestroyAllocator(s_Data->Allocator);

        pdelete s_Data;
        s_Data = nullptr;
    }
}
