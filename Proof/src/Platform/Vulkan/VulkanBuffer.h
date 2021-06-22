#pragma once
#include "Proof/Renderer/Buffer.h"
#include "Proof/Core/Core.h"
#include "vulkan/vulkan.h"
#include "Platform/Window/CurrentWindow.h"
#include "VulkanGraphicsContext.h"
namespace Proof {
	class Proof_API VulkanVertexBuffer : public VertexBuffer {
		virtual void BindVertexBuffer() override;
		virtual void AddVertexBufferData(void* Data,unsigned int Size) override;
		virtual void UnBind() override;
		~VulkanVertexBuffer();
	private:
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		bool IsDeleted=false;
		Count<VulkanGraphicsContext> Graphics = std::static_pointer_cast<VulkanGraphicsContext>(CurrentWindow::GetGraphicsContext());
		uint32_t FindMemoryType(uint32_t typeFilter,VkMemoryPropertyFlags properties);
	};
}