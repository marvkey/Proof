#include "Proofprch.h"
#include "Vertex.h"
#include "vulkan/vulkan.h"
#include "platform/Vulkan/VulkanBuffer.h"
namespace Proof
{
    VulkanVertexInput Vertex::GetVulkanDescription() {
        VulkanVertexInputData setting(sizeof(Vertex));
        setting.AddData(0, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Vertices));
        setting.AddData(1, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Normal));
        setting.AddData(2, VulkanDataFormat::Vec2, offsetof(Vertex, Vertex::TexCoords));
        setting.AddData(3, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Tangent));
        setting.AddData(4, VulkanDataFormat::Vec3, offsetof(Vertex, Vertex::Bitangent));
        return VulkanVertexInput{ setting.GetDescriptions(),setting.GetAttributes()};
    }
}
