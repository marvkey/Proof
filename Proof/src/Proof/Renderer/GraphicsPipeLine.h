#pragma once
#include "Proof/Core/Core.h"

namespace Proof{
	enum class DrawType {
		TriangleList = 0,
		TriangleStrip,
		TriangleFan,
		LineList,
		LineStrip,
		PointList
	};
	enum class DepthCompareOperator {
		Never = 0,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always 
	};

	enum class CullMode {
		None= 0,
		Front,
		Back,
		FrontAndBck,
	};

	enum class FrontFace {
		ClockWise = 0,
		CounterClockWise,
	};
	struct GraphicsPipelineConfig {
		std::string DebugName;
		Count<class Shader> Shader;
		// should kinda remove this ngl
		Count<class VertexArray> VertexArray;
		float LineWidth = 1.0f;
		bool WriteDepth = true;
		bool DepthTest = true;
		bool Blend = false;
		CullMode CullMode = CullMode::None;
		FrontFace FrontFace = FrontFace::ClockWise;
		DepthCompareOperator DepthCompareOperator = DepthCompareOperator::Less;
		DrawType DrawMode = DrawType::TriangleList;
		Count<class FrameBuffer> TargetBuffer;

	};
	class GraphicsPipeline : public RefCounted {
	public:
		virtual Count<class Shader> GetShader()const = 0;
		virtual Count<class FrameBuffer> GetTargetBuffer()const= 0;
		virtual const GraphicsPipelineConfig& GetConfig()const = 0;
		virtual ~GraphicsPipeline() = default;
		static Count<GraphicsPipeline> Create(const GraphicsPipelineConfig& piplineConfig);
	};
}
