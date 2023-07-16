#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{
	struct ComputePipelineConfig
	{
		std::string DebugName;
		Count<class Shader> Shader;
	};

	class ComputePipeline : public RefCounted
	{
	public:
		static Count< ComputePipeline> Create(const ComputePipelineConfig& config);
		virtual const ComputePipelineConfig& GetConfig()const = 0;
		virtual Count<class Shader> GetShader()const = 0;
	};
}
