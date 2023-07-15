#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Vector.h"
namespace Proof
{
	struct ComputePassConfiguration {
		std::string DebugName;
		Count< class ComputePipeline> Pipeline;
		Vector4 DebugMarker;
	};
	
	class ComputePass
	{
	public:
		static Count<ComputePass> Create(const ComputePassConfiguration& config);
		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class Texture2D> images) = 0;
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images) = 0;
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer) = 0;
		virtual void PushData(std::string_view name, const void* data) = 0;
		virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
		virtual const ComputePassConfiguration& GetConfig()const = 0;
		virtual Count<class ComputePipeline> GetComputePipeline()const = 0;
	};
}
