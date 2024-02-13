#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Vector.h"
#include <glm/glm.hpp>
namespace Proof
{
	struct ComputePassConfiguration {
		std::string DebugName;
		Count< class ComputePipeline> Pipeline;
		Vector4 DebugMarker;
	};
	
	class ComputePass : public RefCounted
	{
	public:
		static Count<ComputePass> Create(const ComputePassConfiguration& config);
		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class Texture2D> iamge) = 0;
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class UniformBufferSet> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class StorageBufferSet> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class TextureCube> buffer) = 0;
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images) = 0;
		void virtual SetInput(std::string_view name, Count<class ImageView> imageView) = 0;
		void virtual SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews) = 0;
		void virtual SetInput(std::string_view name, Count<class Image2D>image) = 0;
		void virtual SetInput(std::string_view name, Count<class Image>image) = 0;
		void virtual SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images) = 0;
		virtual void PushData(std::string_view name, const void* data) = 0;
		void virtual AddGlobalInput(Count<class GlobalBufferSet> globalInputs) = 0;

		virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
		virtual void DispatchIndirect(Count<class StorageBuffer> storageBuffer, uint64_t offset) = 0;
		virtual void DispatchIndirect(Count<class UniformBuffer> buffer, uint64_t offset) = 0;
		virtual void DispatchIndirect(Count<class UniformBufferSet> buffer, uint64_t offset) = 0;
		virtual void DispatchIndirect(Count<class StorageBufferSet> buffer, uint64_t offset) = 0;
		virtual void Dispatch(glm::uvec3 group) = 0;
		virtual const ComputePassConfiguration& GetConfig()const = 0;
		virtual Count<class ComputePipeline> GetComputePipeline()const = 0;
		virtual Count<class Shader> GetShader()const = 0;

	};
}
