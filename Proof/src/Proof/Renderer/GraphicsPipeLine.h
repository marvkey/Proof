#pragma once
#include "Proof/Core/Core.h"
#include "FrameBuffer.h"
namespace Proof
{


	//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/d7531ae47d8b36a5d4025663e731a47a38be882f/framework/cauldron/framework/inc/render/pipelinedesc.h#L38
	//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/d7531ae47d8b36a5d4025663e731a47a38be882f/framework/cauldron/framework/inc/render/renderdefines.h#L333
	struct BlendMetaData
	{

	};

	
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
	enum class BlendFactor
	{
		Zero = 0,
		One = 1,
		SrcColor = 2,
		OneMinusSrcColor = 3,
		DstColor = 4,
		OneMinusDstColor = 5,
		SrcAlpha = 6,
		OneMinusSrcAlpha = 7,
		DstAlpha = 8,
		OneMinusDstAlpha = 9,
		ConstantColor = 10,
		OneMinusConstantColor = 11,
		ConstantAlpha = 12,
		OneMinusConstantAlpha = 13,
		SrcAlphaSaturate = 14,
		Src1Color = 15,
		OneMinusSrc1Color = 16,
		Src1Alpha = 17,
		OneMinusSrc1Alpha = 18
	};
	enum class BlendOperation
	{
		Add = 0,
		Subtract = 1,
		ReverseSubtract = 2,
		Min = 3,
		Max = 4
	};
	struct BlendAttachmentState
	{
		BlendFactor SrcColorBlendFactor = BlendFactor::SrcAlpha;
		BlendFactor DstColorBlendFactor = BlendFactor::OneMinusSrcAlpha;
		BlendOperation ColorBlendOperation = BlendOperation::Add;

		BlendFactor SrcAlphaBlendFactor = BlendFactor::SrcAlpha;
		BlendFactor DstAlphaBlendFactor = BlendFactor::OneMinusSrcAlpha;
		BlendOperation AlphaBlendOperation = BlendOperation::Add;

		BlendAttachmentState()
		{

		};

		BlendAttachmentState(BlendFactor srcColorBlendFactor, BlendFactor dstColorBlendFactor, BlendOperation colorBlendOperation,
			BlendFactor srcAlphaBlendFactor, BlendFactor dstAlphaBlendFactor, BlendOperation alphaBlendOperation)
			: SrcColorBlendFactor(srcColorBlendFactor),
			DstColorBlendFactor(dstColorBlendFactor),
			ColorBlendOperation(colorBlendOperation),
			SrcAlphaBlendFactor(srcAlphaBlendFactor),
			DstAlphaBlendFactor(dstAlphaBlendFactor),
			AlphaBlendOperation(alphaBlendOperation)
		{
		}

		BlendAttachmentState(BlendFactor srcBlendFactor, BlendFactor dstBlendFactor, BlendOperation blendOperation)
			: 
			SrcColorBlendFactor(srcBlendFactor),
			DstColorBlendFactor(dstBlendFactor),
			ColorBlendOperation(blendOperation),
			SrcAlphaBlendFactor(srcBlendFactor),
			DstAlphaBlendFactor(dstBlendFactor),
			AlphaBlendOperation(blendOperation)
		{
		}
		BlendAttachmentState(BlendMode blendMode)
		{
			switch (blendMode)
			{
				case Proof::BlendMode::None:
				{
					SrcColorBlendFactor = BlendFactor::One;
					DstColorBlendFactor = BlendFactor::Zero;
					ColorBlendOperation = BlendOperation::Add;

					SrcAlphaBlendFactor = BlendFactor::One;
					DstAlphaBlendFactor = BlendFactor::Zero;
					AlphaBlendOperation = BlendOperation::Add;
				}
					break;
				case Proof::BlendMode::OneZero:
				{
					SrcColorBlendFactor = BlendFactor::One;
					DstColorBlendFactor = BlendFactor::Zero;
					ColorBlendOperation = BlendOperation::Add;

					SrcAlphaBlendFactor = BlendFactor::One;
					DstAlphaBlendFactor = BlendFactor::Zero;
					AlphaBlendOperation = BlendOperation::Add;
				}
					break;
				case BlendMode::SrcAlphaOneMinusSrcAlpha:
				{

					SrcColorBlendFactor = BlendFactor::SrcAlpha;
					DstColorBlendFactor = BlendFactor::OneMinusSrcAlpha;
					ColorBlendOperation = BlendOperation::Add;

					SrcAlphaBlendFactor = BlendFactor::SrcAlpha;
					DstAlphaBlendFactor = BlendFactor::OneMinusSrcAlpha;
					AlphaBlendOperation = BlendOperation::Add;
				}
					break;

				case BlendMode::Additive:
				{
					SrcColorBlendFactor = BlendFactor::SrcAlpha;
					DstColorBlendFactor = BlendFactor::One;
					ColorBlendOperation = BlendOperation::Add;

					SrcAlphaBlendFactor = BlendFactor::SrcAlpha;
					DstAlphaBlendFactor = BlendFactor::One;
					AlphaBlendOperation = BlendOperation::Add;
				}
					break;

				case BlendMode::Zero_SrcColor:
				{
					SrcColorBlendFactor = BlendFactor::Zero;
					DstColorBlendFactor = BlendFactor::SrcColor;
					ColorBlendOperation = BlendOperation::Add;

					SrcAlphaBlendFactor = BlendFactor::Zero;
					DstAlphaBlendFactor = BlendFactor::SrcAlpha;
					AlphaBlendOperation = BlendOperation::Add;
				}
					break;
				default:
					break;
			}
		}
	};
	struct GraphicsPipelineImageConfig 
	{
		GraphicsPipelineImageConfig() = default;
		GraphicsPipelineImageConfig(ImageFormat format)
			: Format(format) {}

		ImageFormat Format = ImageFormat::None;
		bool PresentKHr = false;
		bool Blend = true;
		bool OverrideBaseBlend = false;
		// override baseBled has to enabled to use this
		BlendAttachmentState BlendState{ BlendMode::SrcAlphaOneMinusSrcAlpha };
	};
	struct GraphicsPipelineAttachment 
	{
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

		// means we can update depth test even after creating a rednerPass
		bool EditDepthTest = false;

		bool WriteDepth = true;
		bool DepthTest = true;

		// master swith for each ble
		bool Blend = true;
		BlendAttachmentState BlendMode{ BlendMode::SrcAlphaOneMinusSrcAlpha };

		DepthCompareOperator DepthCompareOperator = DepthCompareOperator::GreaterOrEqual;
		FrontFace FrontFace = FrontFace::ClockWise;

		// default drawType
		DrawType DrawMode = DrawType::Triangle;
		// default CullMode
		CullMode CullMode = CullMode::Back;
		//default LineWidth if greater tahn 1 edit lien widht automatically set to true
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
