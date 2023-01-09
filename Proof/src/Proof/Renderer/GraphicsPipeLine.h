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
		EQual,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always 
	};
	struct GraphicsPipelineConfig {
		std::string DebugName;
		Count<class Shader> Shader;
		Count<class VertexArray> VertexArray;
		Count<class PipeLineLayout> PipelineLayout;
		float LineWidth = 1.0f;
		bool WriteDepth = true;
		Count<class RenderPass> RenderPass;
		DepthCompareOperator DepthCompareOperator = DepthCompareOperator::Less;
	};
	class GraphicsPipeline {
	public:
		virtual ~GraphicsPipeline() = default;
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		static Count<GraphicsPipeline> Create(const GraphicsPipelineConfig& piplineConfig);
	};
}
