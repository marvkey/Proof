#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"
#include "FrameBuffer.h"
namespace Proof{


	
	enum class DrawType 
	{
		None =0,
		Triangle,
		TriangleStrip,
		TriangleFan,
		Line,
		LineStrip,
		Point
	};
	enum class DepthCompareOperator 
	{
		None = 0,
		Never,
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
		None = 0,
		ClockWise = 1,
		CounterClockWise,
	};
	enum class BlendMode
	{
		None = 0,
		OneZero,
		SrcAlphaOneMinusSrcAlpha,
		Additive,
		Zero_SrcColor
	};
	enum class PolygonFillMode 
	{
		None = 0,
		Fill,       // Fill the polygon interior
		Line,  // Draw only the edges of the polygon (Wireframe)
		Points,     // Draw only the vertices of the polygon
		Rectangle   // Fill the polygon as rectangles (extension-specific)
	};
	struct GraphicsPipelineImageConfig {
		GraphicsPipelineImageConfig() = default;
		GraphicsPipelineImageConfig(ImageFormat format)
			: Format(format) {}

		ImageFormat Format = ImageFormat::None;
		bool PresentKHr = false;
		bool Blend = true;
	};
	struct GraphicsPipelineAttachment {
		GraphicsPipelineAttachment() = default;
		GraphicsPipelineAttachment(const FrameBufferConfig& config)
		{
			for (const auto& attach : config.Attachments.Attachments)
			{
				Attachments.emplace_back(attach.Format);
			}
		}
		GraphicsPipelineAttachment(std::initializer_list<GraphicsPipelineImageConfig> attahcments)
			:
			Attachments(attahcments)
		{

		}
		std::vector<ImageFormat> GetAttachmentsImages()const
		{
			std::vector<ImageFormat> imagesFormats;
			for (auto& attach : Attachments)
				imagesFormats.emplace_back(attach.Format);

			return imagesFormats;
		}
		std::vector<GraphicsPipelineImageConfig> Attachments;
	};
	struct GraphicsPipelineConfiguration
	{
		std::string DebugName;
		Count<class Shader> Shader;
		Count<class VertexArray> VertexArray;
		// means we can update line width even after creating a rednerPass
		// if set to true we can do that
		bool EditLineWidth = false;
		// means we can update draw type even after creating a rednerPass
		// if set to true we can do that
		bool EditDrawType = false;
		// means we can update cull type even after creating a rednerPass
		// if set to true we can do that
		bool EditCullMode = false;
		bool WriteDepth = true;
		bool DepthTest = true;

		// master swith for each ble
		bool Blend = true;
		BlendMode BlendMode = BlendMode::SrcAlphaOneMinusSrcAlpha;

		DepthCompareOperator DepthCompareOperator = DepthCompareOperator::LessOrEqual;
		FrontFace FrontFace = FrontFace::ClockWise;

		// default drawType
		DrawType DrawMode = DrawType::Triangle;
		// default CullMode
		CullMode CullMode = CullMode::Back;
		//default LineWidth
		float LineWidth = 1.0f;

		PolygonFillMode FillMode = PolygonFillMode::Fill;
		GraphicsPipelineAttachment Attachments;
		bool Multiview = false;



	};
	class GraphicsPipeline : public RefCounted {
	public:

		virtual Count<class Shader> GetShader()const = 0;
		virtual const GraphicsPipelineConfiguration& GetConfig()const = 0;
		virtual ~GraphicsPipeline() = default;
		static Count<GraphicsPipeline> Create(const GraphicsPipelineConfiguration& piplineConfig);
	};
}
