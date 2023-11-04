#include "Proofprch.h"
#include "WorldRenderer.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Renderer/Buffer.h"
#include "Proof/Scene/World.h"
#include "RenderMaterial.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include <tuple>
#include <variant>
#include "MeshWorkShop.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "GraphicsPipeLine.h"
#include "Proof/Scene/Mesh.h"
#include "Shader.h"
#include "Proof/Physics/PhysicsMeshCooker.h"
#include"DebugMeshRenderer.h"
#include"Font.h"
#include "ParticleSystem.h"
#include "ComputePipeline.h"
#include "RenderMaterial.h"
#include "ComputePass.h"
#include "Proof/Scene/Material.h"
#include "Proof/Math/Random.h"
#include"Vertex.h"
#include "Platform/Vulkan/VulkanFrameBuffer.h"
#include "Platform/Vulkan/VulkanImage.h"
#include "Proof/Math/MathConvert.h"	
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Scene/Material.h"
#include "Proof/Asset/AssetManager.h"

#include "VertexArray.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include<glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_inverse.hpp> 

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

/**
* set 3 (Common.glslh)
	*0: CameraData
	*1: ScreenData
* Set2  (Pbr.glslh)
	*0:SceneData
	*1:RendererData
	* (Lights.glslh)
	*2:SkyBoxData
	*3:DirectionalLightStorageBuffer 
	* 4:PointLightBuffer
	* 5:SpotLightBuffer
	* 6:LightInformationBuffer
*/

namespace Proof
{
	namespace Utils
	{

		// does not chekc if same size need to check
		inline void GenerateSSAOSampleKernal(Count<StorageBufferSet> set, uint32_t kernalSize)
		{
				//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/9.ssao/ssao.cpp
			//https://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html

			//Generating the Sample Kernel
			std::vector<glm::vec3> ssaoKernel;
			ssaoKernel.resize(kernalSize);
			for (uint32_t i = 0; i < ssaoKernel.size(); i++)
			{
				// generate sample hemisphere
				glm::vec3& vec = ssaoKernel[i];
				vec = { Random::Real<float>(-1,1),Random::Real<float>(-1,1),Random::Real<float>(0,1) };
				vec = glm::normalize(vec);

				// disturbute this across the hemisphere
				vec *= Random::Real<float>(0, 1);

				// What we actually want is for the distance from the origin to falloff as we generate more points, according to a curve like this
				//We can use an accelerating interpolation function to achieve this
				float scale = float(i) / float(kernalSize);
				scale = Math::Lerp(0.1f, 1.0f, scale * scale);
				vec *= scale;
			}
			Buffer data(ssaoKernel.data(), ssaoKernel.size() * sizeof(glm::vec3));
			for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
			{
				set->Resize(i, data);
			}

		}

		// does not check if it same size so you need to chekc
		inline void GenerateSSAONoise(Count<Texture2D>& texture, uint32_t noiseScale)
		{
			// generating noise 

			int fullNoiseSize = noiseScale * noiseScale ;
			std::vector<glm::vec3> noise;
			noise.resize(fullNoiseSize);
			for (uint32_t i = 0; i < fullNoiseSize; i++)
			{
				glm::vec3& vec = noise[i];
				vec = { Random::Real<float>(-1,1),Random::Real<float>(-1,1),0 };
				vec = glm::normalize(vec);
			}
			Buffer buffer(noise.data(), Utils::GetImageMemorySize(texture->GetSpecification().Format, noiseScale, noiseScale));
			texture->Resize(noiseScale, noiseScale, buffer);

		}
	}
	struct MeshInstanceVertex {

		glm::mat4 Transform;
	};
	#define SHADOWMAP_CASCADE_COUNT  4

	glm::vec3 convertRotationToVec3(float rotation) {
		// Convert rotation from degrees to radians
		float rotationRadians = glm::radians(rotation);

		// Calculate the x and z components of the vector using trigonometry
		float x = glm::cos(rotationRadians);
		float z = glm::sin(rotationRadians);

		// Create the glm::vec3 with the calculated components
		glm::vec3 rotationVec(x, 0.0f, z);
		return rotationVec;
	}
	WorldRenderer::~WorldRenderer() {
		for (auto& transformBuffer : m_SubmeshTransformBuffers)
			pdelete[] transformBuffer.Data;
		//PipelineLayout = nullptr;
	}
	WorldRenderer::WorldRenderer()
		:
		m_Timers(m_Stats.Timers), m_LightScene(m_Stats.LightSene)
	{
		Init();
	}
	
	void WorldRenderer::Init()
	{
		//AmbientOcclusion.Enabled = true;
		const size_t TransformBufferCount = 10 * 1024; // 10240 transforms
		m_SubmeshTransformBuffers.resize(Renderer::GetConfig().FramesFlight);
		for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
		{
			m_SubmeshTransformBuffers[i].Buffer = VertexBuffer::Create(sizeof(TransformVertexData) * TransformBufferCount);
			m_SubmeshTransformBuffers[i].Data = pnew TransformVertexData[TransformBufferCount];
		}
		
		m_GlobalInputs = Count<GlobalBufferSet>::Create();

		m_UBScreenBuffer = UniformBufferSet::Create(sizeof(UBScreenData));
		m_UBRenderDataBuffer = UniformBufferSet::Create(sizeof(UBRenderData));
		m_UBSceneDataBuffer = UniformBufferSet::Create(sizeof(UBSceneData));
		m_UBCameraBuffer = UniformBufferSet::Create(sizeof(UBCameraData));
		m_UBSKyBoxBuffer = UniformBufferSet::Create(sizeof(UBSkyLight));
		m_UBCascadeProjectionBuffer = UniformBufferSet::Create(sizeof(glm::mat4) * 4);
		m_UBLightSceneBuffer = UniformBufferSet::Create(sizeof(UBLightScene) * 4);

		//storage images
		m_SBDirectionalLightsBuffer = StorageBufferSet::Create(sizeof(DirectionalLight));
		m_SBPointLightsBuffer = StorageBufferSet::Create(sizeof(PointLight)); // set to one light because we cant actaully set a buffer to size 0
		m_SBSpotLightsBuffer = StorageBufferSet::Create(sizeof(SpotLight));// set to one light because we cant actaully set a buffer to size 0

		SetViewportSize(100, 100);

		const glm::uvec2 viewportSize = m_UBScreenData.FullResolution;
		m_CommandBuffer = RenderCommandBuffer::Create("WorldRenderer");
		m_Renderer2D = Count<Renderer2D>::Create();
		m_BRDFLUT = Renderer::GenerateBRDFLut();
		m_Cube = MeshWorkShop::GenerateCube();
		m_Environment = Count<Environment>::Create(Renderer::GetBlackTextureCube(), Renderer::GetBlackTextureCube());

		switch (ShadowSetting.ShadowResolution)
		{
			
			case Proof::ShadowResolution::Low:
				m_ShadowMapResolution = 512;
				break;
			case Proof::ShadowResolution::Mediuim:
				m_ShadowMapResolution = 1024;
				break;
			case Proof::ShadowResolution::High:
				m_ShadowMapResolution = 2048;
				break;
			case Proof::ShadowResolution::Extreme:
				m_ShadowMapResolution = 4096;
				break;
			default:
				PF_CORE_ASSERT(false);
				break;
		}


		// set global input 
		{
			m_GlobalInputs->SetData("CameraData", m_UBCameraBuffer);
			m_GlobalInputs->SetData("ScreenData", m_UBScreenBuffer);
		}
		Count<VertexArray> staticVertexArray= VertexArray::Create({ { sizeof(Vertex)}, {sizeof(MeshInstanceVertex), VertexInputRate::Instance} });
		staticVertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Position));
		staticVertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
		staticVertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoord));
		staticVertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
		staticVertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));


		staticVertexArray->AddData(5, DataType::Vec4, 0, 1);
		staticVertexArray->AddData(6, DataType::Vec4, (sizeof(glm::vec4) * 1), 1);
		staticVertexArray->AddData(7, DataType::Vec4, (sizeof(glm::vec4) * 2), 1);
		staticVertexArray->AddData(8, DataType::Vec4, (sizeof(glm::vec4) * 3), 1);

		Count<VertexArray> quadVertexArray = VertexArray::Create({ sizeof(QuadVertex) });
		quadVertexArray->AddData(0, DataType::Vec3, offsetof(QuadVertex, QuadVertex::Position));
		quadVertexArray->AddData(1, DataType::Vec2, offsetof(QuadVertex, QuadVertex::TexCoord));

		//pre depth pass
		{
		
			FrameBufferConfig preDepthFramebufferSpec;
			preDepthFramebufferSpec.DebugName = "PreDepth";
			//Linear depth, reversed device depth
			//preDepthFramebufferSpec.Attachments = { ImageFormat::DEPTH32FSTENCIL8UI };
			preDepthFramebufferSpec.Attachments = { ImageFormat::DEPTH32F };
			preDepthFramebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			preDepthFramebufferSpec.DepthClearValue = 1.0f;
			
			GraphicsPipelineConfiguration pipelinelineConfig;
			//pipelinelineConfig.Attachments = { ImageFormat::DEPTH32FSTENCIL8UI };
			pipelinelineConfig.Attachments = { ImageFormat::DEPTH32F };
			pipelinelineConfig.DebugName = "PreDepth";
			pipelinelineConfig.Shader = Renderer::GetShader("PreDepth_Static");
			pipelinelineConfig.DepthCompareOperator = DepthCompareOperator::LessOrEqual;
			pipelinelineConfig.VertexArray = staticVertexArray;
			
			auto preDepthStaticPipeline = GraphicsPipeline::Create(pipelinelineConfig);
			
			RenderPassConfig renderPassConfig;
			renderPassConfig.DebugName = "PreDepth_Static";
			renderPassConfig.Pipeline = preDepthStaticPipeline;
			renderPassConfig.TargetFrameBuffer = FrameBuffer::Create(preDepthFramebufferSpec);
			
			m_PreDepthPass = RenderPass::Create(renderPassConfig);
			m_PreDepthPass->AddGlobalInput(m_GlobalInputs);

			//m_PreDepthPass->SetInput("CameraData", m_UBCameraBuffer);

		}
		//foward plus
		{
#if 0

			//https://www.3dgep.com/forward-plus/#Forward
			// look at the bottom for the zip to all code
			//https://drive.google.com/uc?export=download&id=1zCU3ahVYXOoZ3PPdsE6mYtJbHZgLmKOh

			constexpr uint32_t TILE_SIZE = 16u;
			glm::uvec2 size = m_UBScreenData.FullResolution;
			m_LightCullingWorkGroups = glm::ceil(glm::vec3(size.x / (float)TILE_SIZE, size.y / (float)TILE_SIZE, 1));
			m_LightCullingNumThreads = m_LightCullingWorkGroups * glm::uvec3{ TILE_SIZE , TILE_SIZE ,1 };

			// plane {vec3 normal, float distnace} = sizeof(float) *4
			//Frustrum{ Plane[4]] (sizeof(float) *4 ) * 4
			const float frusturmSize = (sizeof(float) * 4) * 4;
			m_FrustrumsBuffer = StorageBufferSet::Create(frusturmSize * m_LightCullingNumThreads.x * m_LightCullingNumThreads.y * m_LightCullingNumThreads.z);

			ComputePipelineConfig pipeline;
			pipeline.DebugName = "FrustrumGrid";
			pipeline.Shader = Renderer::GetShader("FrustrumGrid");
			m_FrustrumPass = ComputePass::Create({ "frustrumPass",ComputePipeline::Create(pipeline) });
			m_FrustrumPass->SetInput("OutFrustums", m_FrustrumsBuffer);
			m_FrustrumPass->AddGlobalInput(m_GlobalInputs);

			//m_FrustrumPass->SetInput("CameraData", m_UBCameraBuffer);
			//m_FrustrumPass->SetInput("ScreenData", m_UBScreenBuffer);

			ImageConfiguration imageConfig;
			imageConfig.DebugName = "PointLightGrid";
			imageConfig.Format = ImageFormat::RG32UI;
			imageConfig.Transfer = true;
			imageConfig.Usage = ImageUsage::Storage;
			
			imageConfig.Width = m_LightCullingNumThreads.x;
			imageConfig.Height = m_LightCullingNumThreads.y;
			imageConfig.Layers = m_LightCullingNumThreads.z;

			m_PointLightGrid = Image2D::Create(imageConfig);
			imageConfig.DebugName = "SpotLightGrid";
			m_SpotLightGrid = Image2D::Create(imageConfig);

			m_PointLightIndexCounterBuffer = StorageBufferSet::Create(sizeof(uint32_t));
			m_SpotLightIndexCounterBuffer = StorageBufferSet::Create(sizeof(uint32_t));

			// 
			m_PointLightIndexListBuffer = StorageBufferSet::Create(m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(uint32_t) * 1024);
			m_SpotLightIndexListBuffer = StorageBufferSet::Create(m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(uint32_t) * 1024);

			ComputePipelineConfig lightCullingpipeline;
			lightCullingpipeline.DebugName = "LightCulling";
			lightCullingpipeline.Shader = Renderer::GetShader("LightCulling");
			m_LightCullingPass = ComputePass::Create({ "LightCulling",ComputePipeline::Create(lightCullingpipeline) });

			m_LightCullingPass->SetInput("Frustrums", m_FrustrumsBuffer);
			m_LightCullingPass->SetInput("PointLightIndexCounterBuffer", m_PointLightIndexCounterBuffer);
			m_LightCullingPass->SetInput("SpotLightIndexCounterBuffer", m_SpotLightIndexCounterBuffer);
			m_LightCullingPass->SetInput("PointLightIndexListBuffer", m_PointLightIndexListBuffer);
			m_LightCullingPass->SetInput("SpotLightIndexListBuffer", m_SpotLightIndexListBuffer);

			m_LightCullingPass->SetInput("u_ImagePointLightGrid", m_PointLightGrid);
			m_LightCullingPass->SetInput("u_ImageSpotLightGrid", m_SpotLightGrid);
			m_LightCullingPass->SetInput("u_PointLightGrid", m_PointLightGrid);
			m_LightCullingPass->SetInput("u_SpotLightGrid", m_SpotLightGrid);

			m_LightCullingPass->AddGlobalInput(m_GlobalInputs);

			//m_LightCullingPass->SetInput("CameraData", m_UBCameraBuffer);
			//m_LightCullingPass->SetInput("ScreenData", m_UBScreenBuffer);
			m_LightCullingPass->SetInput("PointLightBuffer", m_SBPointLightsBuffer);
			m_LightCullingPass->SetInput("SpotLightBuffer", m_SBSpotLightsBuffer);
			m_LightCullingPass->SetInput("LightInformationBuffer", m_UBLightSceneBuffer);
			//m_LightCullingPass->SetInput("ScreenData", m_UBScreenBuffer);
#endif
			//m_LightCullingNumThreads = (viewportSize + TILE_SIZE - 1u) / TILE_SIZE;

			glm::uvec2 size = viewportSize;
			size += TILE_SIZE - viewportSize % TILE_SIZE;
			m_LightCullingWorkGroups = glm::uvec3{ size / TILE_SIZE, 1u };

			m_FrustrumsBuffer = StorageBufferSet::Create(FRUSTRUM_SIZE * m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * 1);
			// TODO are these right
			m_PointLightIndexListBuffer = StorageBufferSet::Create(m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(uint32_t) * MAX_NUM_LIGHTS_PER_TILE);
			m_PointLightGrid = StorageBufferSet::Create(m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(uint32_t));

			ComputePipelineConfig pipeline;
			pipeline.DebugName = "FrustrumGrid";
			pipeline.Shader = Renderer::GetShader("FrustrumGrid");
			m_FrustrumPass = ComputePass::Create({ "frustrumPass",ComputePipeline::Create(pipeline) });
			m_FrustrumPass->AddGlobalInput(m_GlobalInputs);
			m_FrustrumPass->SetInput("OutFrustums", m_FrustrumsBuffer);

			ComputePipelineConfig lightCullingpipeline;
			lightCullingpipeline.DebugName = "LightCulling";
			lightCullingpipeline.Shader = Renderer::GetShader("LightCulling");
			m_LightCullingPass = ComputePass::Create({ "LightCulling",ComputePipeline::Create(lightCullingpipeline) });
			m_LightCullingPass->AddGlobalInput(m_GlobalInputs);
			m_LightCullingPass->SetInput("u_DepthTexture", m_PreDepthPass->GetOutput(0).As<Image2D>());
			m_LightCullingPass->SetInput("Frustrums", m_FrustrumsBuffer);
			m_LightCullingPass->SetInput("PointLightIndexListBuffer", m_PointLightIndexListBuffer);
			m_LightCullingPass->SetInput("PointLightBuffer", m_SBPointLightsBuffer);
			m_LightCullingPass->SetInput("LightInformationBuffer", m_UBLightSceneBuffer);
			m_LightCullingPass->SetInput("PointLightGrid", m_PointLightGrid);

		}

		{
			ImageFormat depthFormat = ImageFormat::DEPTH32F;
			const uint32_t shadowMapCascade = SHADOWMAP_CASCADE_COUNT;
			ImageConfiguration depthImageConfig;
			depthImageConfig.DebugName = "Shadow Pass Image";
			depthImageConfig.Format = depthFormat;
			depthImageConfig.Usage = ImageUsage::Attachment;
			depthImageConfig.Layers = shadowMapCascade;
			depthImageConfig.Width = m_ShadowMapResolution;
			depthImageConfig.Height = m_ShadowMapResolution;

			m_ShadowPassImage = Image2D::Create(depthImageConfig);

			{
				FrameBufferConfig framebufferConfig;
				framebufferConfig.DebugName = "Shadow Debug Fraembuffer";
				framebufferConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
				framebufferConfig.Width = m_ShadowMapResolution;
				framebufferConfig.Height = m_ShadowMapResolution;
				framebufferConfig.DepthClearValue = 1.0f;

				auto shadowDebugframeBuffer = FrameBuffer::Create(framebufferConfig);

				GraphicsPipelineConfiguration shadowMapPipelineConfig;
				shadowMapPipelineConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
				shadowMapPipelineConfig.DebugName = "DebugShadowMapPipeline";
				shadowMapPipelineConfig.Shader = Renderer::GetShader("DebugShadowMap");

				m_ShadowDebugPipeline = GraphicsPipeline::Create(shadowMapPipelineConfig);

				RenderPassConfig renderpassConfig;
				renderpassConfig.DebugName = fmt::format("DebugShadowPass");
				renderpassConfig.TargetFrameBuffer = shadowDebugframeBuffer;
				renderpassConfig.Pipeline = m_ShadowDebugPipeline;
				m_ShadowDebugPass = RenderPass::Create(renderpassConfig);
				m_ShadowDebugPass->SetInput("u_ShadowMap", m_ShadowPassImage);

			}
			GraphicsPipelineConfiguration pipelineConfig;
			pipelineConfig.DebugName = "ShadowMapCascade";
			pipelineConfig.Attachments = { depthFormat };
			pipelineConfig.DepthCompareOperator = DepthCompareOperator::LessOrEqual;
			pipelineConfig.CullMode = CullMode::Back;
			pipelineConfig.Shader = Renderer::GetShader("ShadowDepthPass");
			pipelineConfig.VertexArray = staticVertexArray;

			auto pipleine = GraphicsPipeline::Create(pipelineConfig);
			for (uint32_t i = 0; i < shadowMapCascade; i++)
			{
				ImageViewConfiguration imageViewConfig;
				imageViewConfig.DebugName = fmt::format("Shadow Cascade: {}", i);
				imageViewConfig.Layer = i;
				imageViewConfig.LayerCount = 1;
				imageViewConfig.View = ImageViewType::View2D;
				imageViewConfig.Image = m_ShadowPassImage;

				Count<ImageView> imageView = ImageView::Create(imageViewConfig);
				FrameBufferConfig framebufferConfig;
				framebufferConfig.DebugName = fmt::format("Shadow map cascade {}", i);
				framebufferConfig.Attachments = { depthFormat };
				framebufferConfig.Attachments.Attachments[0].ExistingImage = imageView;
				framebufferConfig.Width = m_ShadowMapResolution;
				framebufferConfig.Height = m_ShadowMapResolution;
				framebufferConfig.DepthClearValue = 1.0f;

				Count<FrameBuffer> shadowMapPassFrameBuffer = FrameBuffer::Create(framebufferConfig);

				RenderPassConfig renderpassConfig;
				renderpassConfig.TargetFrameBuffer = shadowMapPassFrameBuffer;
				renderpassConfig.DebugName = fmt::format("Shadow render pass Cascade {}", i);
				renderpassConfig.Pipeline = pipleine;

				auto renderPass = RenderPass::Create(renderpassConfig);
				m_ShadowMapPasses[i] = renderPass;
			}
		}
	
		//geometry pass
		{
			FrameBufferConfig geoFramebufferConfig;
			geoFramebufferConfig.DebugName = "Geometry";
			geoFramebufferConfig.ClearDepthOnLoad = false;	
			geoFramebufferConfig.ClearColorOnLoad = false;
			//geoFramebufferConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32FSTENCIL8UI };
			geoFramebufferConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::RGBA16F,ImageFormat::RGBA, ImageFormat::DEPTH32F }; // color, view limuncance, metallnessroughness
			geoFramebufferConfig.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			geoFramebufferConfig.Attachments.Attachments[3].ExistingImage = m_PreDepthPass->GetOutput(0);

			auto frameBuffer = FrameBuffer::Create(geoFramebufferConfig);

			GraphicsPipelineConfiguration pipelinelineConfig;
			pipelinelineConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::RGBA16F,ImageFormat::RGBA, ImageFormat::DEPTH32F }; // color, view limuncance, metallness, roughness
			// Don't blend with luminance in the alpha channel.
			pipelinelineConfig.Attachments.Attachments[1].Blend = false;

			pipelinelineConfig.DebugName = "Geometry_Static";
			pipelinelineConfig.Shader = Renderer::GetShader("ProofPBR_Static");
			pipelinelineConfig.DepthCompareOperator = DepthCompareOperator::Equal;
			pipelinelineConfig.VertexArray = staticVertexArray;
			pipelinelineConfig.WriteDepth = false;

			Count<GraphicsPipeline> pipeline = GraphicsPipeline::Create(pipelinelineConfig);

			RenderPassConfig geopassConfig;
			geopassConfig.DebugName = "GeometryPass";
			geopassConfig.Pipeline = pipeline;
			geopassConfig.TargetFrameBuffer = frameBuffer;

			m_GeometryPass = RenderPass::Create(geopassConfig);


			m_GeometryPass->SetInput("DirectionalLightStorageBuffer", m_SBDirectionalLightsBuffer);
			m_GeometryPass->SetInput("PointLightBuffer", m_SBPointLightsBuffer);
			m_GeometryPass->SetInput("SpotLightBuffer", m_SBSpotLightsBuffer);
			m_GeometryPass->SetInput("u_IrradianceMap", m_Environment->IrradianceMap);
			m_GeometryPass->SetInput("u_PrefilterMap", m_Environment->PrefilterMap);
			m_GeometryPass->SetInput("u_BRDFLUT", m_BRDFLUT);
			m_GeometryPass->SetInput("SkyBoxData", m_UBSKyBoxBuffer);
			m_GeometryPass->SetInput("u_ShadowMap", m_ShadowPassImage);
			m_GeometryPass->SetInput("RendererData", m_UBRenderDataBuffer);
			m_GeometryPass->SetInput("SceneData", m_UBSceneDataBuffer);
			m_GeometryPass->SetInput("ShadowMapProjections", m_UBCascadeProjectionBuffer);
			//m_GeometryPass->SetInput("CameraData", m_UBCameraBuffer);
			//m_GeometryPass->SetInput("ScreenData", m_UBScreenBuffer);

			m_GeometryPass->AddGlobalInput(m_GlobalInputs);

			//m_GeometryPass->SetInput("u_PointLightGrid", m_PointLightGrid);
			//m_GeometryPass->SetInput("u_SpotLightGrid", m_SpotLightGrid);
			m_GeometryPass->SetInput("LightInformationBuffer", m_UBLightSceneBuffer);

			m_GeometryPass->SetInput("PointLightIndexListBuffer", m_PointLightIndexListBuffer);
			m_GeometryPass->SetInput("PointLightGrid", m_PointLightGrid);

			//m_GeometryPass->SetInput("SpotLightIndexListBuffer", m_SpotLightIndexListBuffer);
		}

		// skybox
		{
			GraphicsPipelineConfiguration skyBoxPipelineConfig = m_GeometryPass->GetPipeline()->GetConfig();
			skyBoxPipelineConfig.DebugName = "Sky box";
			skyBoxPipelineConfig.DepthTest = false;
			skyBoxPipelineConfig.WriteDepth = false;
			skyBoxPipelineConfig.Shader = Renderer::GetShader("SkyBox");
			skyBoxPipelineConfig.DepthCompareOperator = DepthCompareOperator::LessOrEqual;
			skyBoxPipelineConfig.VertexArray = quadVertexArray;
			auto skyBoxPipeline = GraphicsPipeline::Create(skyBoxPipelineConfig);

			RenderPassConfig renderPassConfig;
			renderPassConfig.DebugName = "skyBoxPass Pass";
			renderPassConfig.Pipeline = skyBoxPipeline;
			renderPassConfig.TargetFrameBuffer = m_GeometryPass->GetTargetFrameBuffer();
			m_SkyBoxPass = RenderPass::Create(renderPassConfig);
			//m_SkyBoxPass->SetInput("CameraData", m_UBCameraBuffer);
			m_SkyBoxPass->SetInput("SkyBoxData", m_UBSKyBoxBuffer);
			m_SkyBoxPass->AddGlobalInput(m_GlobalInputs);

		}
		
		//composite pass
		{
			/**
			A composite pass, also known as a "final render pass" or "post-processing pass," is a stage in the rendering pipeline where the final image is generated by combining various elements produced in earlier stages. 
			This pass involves taking the output from previous rendering passes (such as geometry, shading, and lighting) and applying post-processing effects, color correction, tone mapping, depth of field, motion blur, and more to create the final visual result.
			The composite pass is where all the individual elements are blended and combined to produce the image that will be presented to the user.
			*/
			FrameBufferConfig compFramebufferSpec;
			compFramebufferSpec.DebugName = "WorldComposite";
			compFramebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
			compFramebufferSpec.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
			//compFramebufferSpec.Transfer = true;
			Count<FrameBuffer> worldCompoiteframebuffer = FrameBuffer::Create(compFramebufferSpec);
			{
				GraphicsPipelineConfiguration pipelineSpecification;
				pipelineSpecification.DebugName = "WorldComposite";
				pipelineSpecification.VertexArray = quadVertexArray;
				pipelineSpecification.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
				pipelineSpecification.CullMode = CullMode::None;
				pipelineSpecification.WriteDepth = false;
				pipelineSpecification.DepthTest = false;
				pipelineSpecification.Shader = Renderer::GetShader("WorldComposite");

				Count<GraphicsPipeline> compositePipeline = GraphicsPipeline::Create(pipelineSpecification);

				RenderPassConfig renderPassSpec;
				renderPassSpec.DebugName = "WorldComposite";
				renderPassSpec.Pipeline = compositePipeline;
				renderPassSpec.TargetFrameBuffer = worldCompoiteframebuffer;

				m_CompositePass = RenderPass::Create(renderPassSpec);
				m_CompositeMaterial = RenderMaterial::Create({ "Composite", Renderer::GetShader("WorldComposite") });
			}
			#if 0
			// Ambient occlusion
			{
				//SAO
				{

					ComputePipelineConfig pipelineConfig;
					pipelineConfig.DebugName = "SSAO";
					pipelineConfig.Shader = Renderer::GetShader("SSAO");

					Count<ComputePipeline> pipeline = ComputePipeline::Create(pipelineConfig);

					ComputePassConfiguration computePassConfig;
					computePassConfig.DebugName = "SSAO";
					computePassConfig.Pipeline = pipeline;

					m_SSAOPass = ComputePass::Create(computePassConfig);
					m_SSAOPass->AddGlobalInput(m_GlobalInputs);

					{
						TextureConfiguration textureConfig;
						textureConfig.DebugName = "SSAO ";
						textureConfig.Format = ImageFormat::R;
						textureConfig.Width = m_UBScreenData.FullResolution.x;
						textureConfig.Height = m_UBScreenData.FullResolution.y;
						textureConfig.Storage = true;

						m_SSAOImage = Texture2D::Create(textureConfig);

						textureConfig.DebugName = "Noise SSAO";
						textureConfig.Format = ImageFormat::RGB32F;
						textureConfig.Filter = TextureFilter::Nearest;
						textureConfig.Width = AmbientOcclusion.SSAO.NoiseSize;
						textureConfig.Height = AmbientOcclusion.SSAO.NoiseSize;

						m_SSAONoiseImage = Texture2D::Create(textureConfig);

						Utils::GenerateSSAONoise(m_SSAONoiseImage, AmbientOcclusion.SSAO.NoiseSize);

						m_SBSSAOSampleKernalBuffer = StorageBufferSet::Create(sizeof(glm::vec3) * AmbientOcclusion.SSAO.KernelSize);
						Utils::GenerateSSAOSampleKernal(m_SBSSAOSampleKernalBuffer, AmbientOcclusion.SSAO.KernelSize);
					}
					m_SSAOMaterial = RenderMaterial::Create("SSAO", Renderer::GetShader("SSAO"));
					m_SSAOMaterial->Set("u_DepthMap", m_PreDepthPass->GetOutput(0));
					m_SSAOMaterial->Set("u_ViewNormalMap", m_GeometryPass->GetOutput(1));
					m_SSAOMaterial->Set("o_SSAOImage", m_SSAOImage);
					m_SSAOMaterial->Set("KernalBuffer", m_SBSSAOSampleKernalBuffer);
					m_SSAOMaterial->Set("u_NoiseImage", m_SSAONoiseImage);

					FrameBufferConfig blurFrameBuffer;
					blurFrameBuffer.Attachments = { ImageFormat::R16UI };
					blurFrameBuffer.DebugName = "SSAOBlur";
					blurFrameBuffer.Width = m_UBScreenData.FullResolution.x;
					blurFrameBuffer.Width = m_UBScreenData.FullResolution.y;
					blurFrameBuffer.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
					auto ssaoFrameBuffer = FrameBuffer::Create(blurFrameBuffer);

					GraphicsPipelineConfiguration pipelineSpecification;
					pipelineSpecification.DebugName = "SSAOBlur";
					pipelineSpecification.VertexArray = quadVertexArray;
					pipelineSpecification.Attachments = { ImageFormat::R16UI };
					pipelineSpecification.CullMode = CullMode::None;
					pipelineSpecification.WriteDepth = false;
					pipelineSpecification.DepthTest = false;
					//	pipelineSpecification.Blend = false;
					pipelineSpecification.Shader = Renderer::GetShader("SSAOBlur");
					auto blurPipeline = GraphicsPipeline::Create(pipelineSpecification);

					RenderPassConfig blurPassConfig;
					blurPassConfig.DebugName = "SSAOBlur";
					blurPassConfig.Pipeline = blurPipeline;
					blurPassConfig.TargetFrameBuffer = ssaoFrameBuffer;

					m_SSAOBlurPass = RenderPass::Create(blurPassConfig);
					m_SSAOBlurPass->SetInput("u_SSAOTexture", m_SSAOImage);
					m_SSAOBlurPass->AddGlobalInput(m_GlobalInputs);
				}
				//AO-compoiste
				{
					FrameBufferConfig framebufferSpec;
					framebufferSpec.DebugName = "AO-Composite";
					framebufferSpec.Attachments = { ImageFormat::RGBA32F };
					framebufferSpec.Attachments.Attachments[0].ExistingImage = m_GeometryPass->GetOutput(0);
					//framebufferSpec.Blend = true;
					framebufferSpec.ClearColorOnLoad = false;
					//framebufferSpec.BlendMode = FramebufferBlendMode::Zero_SrcColor;
					
					auto aoframeBuffer = FrameBuffer::Create(framebufferSpec);

					GraphicsPipelineConfiguration pipelineConfig;
					pipelineConfig.DebugName = "AO-Composite";
					pipelineConfig.VertexArray = quadVertexArray;
					pipelineConfig.Attachments = { ImageFormat::RGBA32F };
					//pipelineConfig.BlendMode = BlendMode::Zero_SrcColor ;
					pipelineConfig.Blend = true;
					pipelineConfig.DepthTest = false;
					pipelineConfig.Shader = Renderer::GetShader("AO-Composite");

					auto aoPipeline = GraphicsPipeline::Create(pipelineConfig);

					RenderPassConfig renderPassConfig;
					renderPassConfig.DebugName = "AO-Composite";
					renderPassConfig.Pipeline = aoPipeline;
					renderPassConfig.TargetFrameBuffer = aoframeBuffer;
					m_AmbientOcclusionCompositePass = RenderPass::Create(renderPassConfig);
				}
			}
			#endif
		}

		//External Composite 
		{
			/*
			External Composite Pass:
			An external composite pass refers to a composite pass that is performed outside of the main rendering pipeline, often in a separate step or even in a different rendering context. This can be useful for certain scenarios, such as when you want to apply post-processing effects using a different graphics library, perform specialized image manipulation, or utilize a different shader program for the final composition. 
			An external composite pass allows for more flexibility and control over how the final image is generated and processed.

			In summary, the key difference between a composite pass and an external composite pass lies in where and how the final image is generated and post-processed. 
			The composite pass is a standard stage in the rendering pipeline where various rendering elements are combined to create the final image. An external composite pass refers to performing this final composition step outside of the main pipeline, 
			providing more flexibility in how the image is generated and processed.

			User
			so the external pass is that for basically like applying ui elements
			ChatGPT
			Yes, that's one common use case for an external composite pass. Applying UI elements, like user interface overlays, HUD elements, or text, on top of the rendered scene is typically done in a separate step after the main rendering pipeline. 
			This allows for greater control over the composition of UI elements with the 3D scene.
			Here's how it might work:
			
			Main Rendering Pipeline:
			The main rendering pipeline focuses on rendering the 3D scene, including geometry, materials, lighting, and shading. It produces the 3D scene's output in the form of color and depth buffers.
			
			External Composite Pass:
			After the main rendering is complete, an external composite pass is performed. This pass involves taking the color buffer produced by the main pipeline and applying additional effects and UI elements.
			UI Elements: This is where UI elements, such as buttons, menus, health bars, crosshairs, etc., are drawn on top of the rendered scene. UI elements are often 2D and can be drawn using textures, images, or text rendering techniques.
			Post-Processing Effects: Additionally, post-processing effects like color grading, bloom, vignette, and other visual enhancements can also be applied during the external composite pass.
			By performing the UI rendering and post-processing in a separate pass after the main rendering pipeline, developers can have better control over the order of operations and ensure that UI elements appear on top of the 3D scene. 
			It also allows for flexibility in how UI elements are handled, especially if the UI system uses different rendering techniques or libraries than the main rendering pipeline. 
			*/

			FrameBufferConfig extCompFramebufferSpec;
			extCompFramebufferSpec.DebugName = "External-Composite";
			extCompFramebufferSpec.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
			extCompFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			extCompFramebufferSpec.ClearColorOnLoad = false;
			extCompFramebufferSpec.ClearDepthOnLoad = false;
			// Use the color buffer from the final compositing pass, but the depth buffer from
			// the actual 3D geometry pass, in case we want to composite elements behind meshes
			// in the scene
			extCompFramebufferSpec.Attachments.Attachments[0].ExistingImage = m_CompositePass->GetOutput(0);
			extCompFramebufferSpec.Attachments.Attachments[1].ExistingImage = m_PreDepthPass->GetOutput(0);

			m_ExternalCompositeFrameBuffer = FrameBuffer::Create(extCompFramebufferSpec);

			GraphicsPipelineConfiguration pipelineConfig;
			pipelineConfig.DebugName = "WireFrame";
			pipelineConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::DEPTH32F };
			pipelineConfig.CullMode = CullMode::None;
			pipelineConfig.DepthTest = true;
			pipelineConfig.LineWidth = 2.0f;
			pipelineConfig.VertexArray = staticVertexArray;
			pipelineConfig.FillMode = PolygonFillMode::Line;
			pipelineConfig.Shader  = Renderer::GetShader("Wireframe");
			auto geomWireframepass = GraphicsPipeline::Create(pipelineConfig);
			m_GeometryWireFramePass = RenderPass::Create(RenderPassConfig("WireFrame", geomWireframepass, m_ExternalCompositeFrameBuffer));

			pipelineConfig.DepthTest = false;
			pipelineConfig.DebugName = "Wireframe-OnTop";
			geomWireframepass = GraphicsPipeline::Create(pipelineConfig);
			m_GeometryWireFrameOnTopPass = RenderPass::Create(RenderPassConfig("WireFrameOnTop", geomWireframepass, m_ExternalCompositeFrameBuffer));
			{

				m_GeometryWireFramePassMaterialAsset = Count<Material>::Create("WireFrame", Renderer::GetShader("Wireframe"));
				AssetManager::CreateRuntimeAsset(AssetManager::CreateID(), m_GeometryWireFramePassMaterialAsset, "worldRenderer wireFrameMaterial");
				m_GeometryWireFramePassMaterial = m_GeometryWireFramePassMaterialAsset->GetRenderMaterial();
			}
			//m_GeometryWireFramePassMaterial = RenderMaterial::Create(RenderMaterialConfiguration{ "WireFrame", Renderer::GetShader("Wireframe") });

			m_GeometryWireFramePass->AddGlobalInput(m_GlobalInputs);
			m_GeometryWireFrameOnTopPass->AddGlobalInput(m_GlobalInputs);

		}

		PF_ENGINE_TRACE("World Renderer Inititilized");
	}

	void WorldRenderer::SetContext(Count<class World> world)
	{
		PF_CORE_ASSERT(!m_InContext, "Can't change world while rendering");
		m_ActiveWorld = world;

	}
	void WorldRenderer::BeginScene(const Camera& camera, const Vector& location, float nearPlane, float farPlane)
	{
		PF_PROFILE_FUNC();
		PF_CORE_ASSERT(!m_InContext);
		PF_CORE_ASSERT(m_ActiveWorld);
		PF_PROFILE_TAG("Renderer", m_ActiveWorld->GetName().c_str());
		
		m_InContext = true;

		//reset stats
		m_Stats = {};

		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		//uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex;
		if (m_NeedResize)
		{
			m_NeedResize = false;
			const glm::uvec2 viewportSize = m_UBScreenData.FullResolution;
			{

				//m_LightCullingNumThreads = (viewportSize + TILE_SIZE - 1u) / TILE_SIZE;

				glm::uvec2 size = viewportSize;
				size += TILE_SIZE - viewportSize % TILE_SIZE;
				m_LightCullingWorkGroups = { size / (uint32_t)TILE_SIZE, 1u };

				for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
				{
					m_PointLightIndexListBuffer->Resize(i, m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(uint32_t) * MAX_NUM_LIGHTS_PER_TILE);
					m_FrustrumsBuffer->Resize(i, FRUSTRUM_SIZE * m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * 1);
					m_PointLightGrid->Resize(i, m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(uint32_t));
				}

			}

			// predepth
			m_PreDepthPass->GetTargetFrameBuffer()->Resize(viewportSize);

			//geometrypass
			m_GeometryPass->GetTargetFrameBuffer()->Resize(viewportSize);

			// compoiste 
			m_CompositePass->GetTargetFrameBuffer()->Resize(viewportSize);

			#if 0
			//AO
			{
				m_SSAOImage->Resize(m_UBScreenData.FullResolution.x, m_UBScreenData.FullResolution.y);
				m_SSAOBlurPass->GetTargetFrameBuffer()->Resize(m_UBScreenData.FullResolution.x, m_UBScreenData.FullResolution.y);
			}
			#endif
			m_ExternalCompositeFrameBuffer->Resize(viewportSize);
		}

		
		switch (ShadowSetting.ShadowResolution)
		{
			case Proof::ShadowResolution::None:
				PF_CORE_ASSERT(false);
				break;
			case Proof::ShadowResolution::Low:
				m_ShadowMapResolution = 512;
				break;
			case Proof::ShadowResolution::Mediuim:
				m_ShadowMapResolution = 1024;
				break;
			case Proof::ShadowResolution::High:
				m_ShadowMapResolution = 2048;
				break;
			case Proof::ShadowResolution::Extreme:
				m_ShadowMapResolution = 4096;
				break;
		}

		if (m_ShadowMapPasses[0]->GetTargetFrameBuffer()->GetConfig().Width != m_ShadowMapResolution)
		{
			// framebuffer cannot reszize image views for now
			m_ShadowPassImage->Resize(m_ShadowMapResolution, m_ShadowMapResolution);
			for (uint32_t i = 0; i < m_ShadowMapPasses.size(); i++)
			{
				m_ShadowMapPasses[i]->GetTargetFrameBuffer()->Resize(m_ShadowMapResolution, m_ShadowMapResolution);
			}
		}

		// set buffers

		// camera buffer
		{
			m_UBCameraData.Projection = camera.GetProjectionMatrix();
			m_UBCameraData.InverseProjection = glm::inverse(m_UBCameraData.Projection);
			m_UBCameraData.UnreversedProjectionMatrix = camera.GetUnReversedProjectionMatrix();
			m_UBCameraData.View = camera.GetViewMatrix();
			m_UBCameraData.InverseView = glm::inverse(m_UBCameraData.View);
			m_UBCameraData.ViewProjection = m_UBCameraData.Projection * m_UBCameraData.View;
			m_UBCameraData.InverseViewProjection = m_UBCameraData.InverseProjection * m_UBCameraData.InverseView;
			m_UBCameraData.Position = location;
			m_UBCameraData.NearPlane = nearPlane;
			m_UBCameraData.FarPlane = farPlane;
			m_UBCameraBuffer->SetData(frameIndex, Buffer(&m_UBCameraData, sizeof(UBCameraData)));
		}
	}
	
	void WorldRenderer::EndScene()
	{
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_ActiveWorld->GetName().c_str());
		DrawScene();
		m_InContext = false;
		m_ActiveWorld = nullptr;
	}

	void WorldRenderer::DrawScene()
	{

		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("Renderer", m_ActiveWorld->GetName().c_str());

		Timer drawSceneTimer;
		// clear screen
		Renderer::BeginCommandBuffer(m_CommandBuffer);

//Renderer::BeginRenderPass(m_CommandBuffer,m_Dep, true);
//		Renderer::EndRenderPass(m_GeometryPass);
		
		if (m_UBScreenData.FullResolution.x > 0 && m_UBScreenData.FullResolution.y > 0)
		{

			SetPasses();
			ShadowPass();
			PreDepthPass();
			LightFrustrumAndCullingPass();
			GeometryPass();
			CompositePass();
		}
		Renderer::EndCommandBuffer(m_CommandBuffer);
		Renderer::SubmitCommandBuffer(m_CommandBuffer);
		// clear data
		//mesh Pass
		{
			m_MeshTransformMap.clear();

			m_MeshDrawList.clear();
			m_DynamicMeshDrawList.clear();

			m_MeshShadowDrawList.clear();
			m_DynamicMeshShadowDrawList.clear();

			m_ColliderDrawList.clear();
			m_DynamicColliderDrawList.clear();
		}

		m_Timers.TotalDrawScene	= drawSceneTimer.ElapsedMillis();

	}

	void WorldRenderer::SetViewportSize(uint32_t width, uint32_t height)
	{
		if (m_UBScreenData.FullResolution == glm::vec2{ width,height })
			return;
		if (width <= 0 || height <= 0)
		{
			width = 1;
			height = 1;
		}
		m_UBScreenData.FullResolution = { width, height };
		m_UBScreenData.InverseFullResolution = {1/ width,  1/height };
		m_UBScreenData.HalfResolution = glm::uvec2{ m_UBScreenData.FullResolution } / 2u;
		m_UBScreenData.InverseHalfResolution = glm::uvec2{ m_UBScreenData.InverseFullResolution } * 2u;
		m_UBScreenData.AspectRatio = m_UBScreenData.FullResolution.x / m_UBScreenData.FullResolution.y;

		Buffer buffer(&m_UBScreenData, sizeof(m_UBScreenData));
		for (int i =0; i<Renderer::GetConfig().FramesFlight; i++)
		{
			m_UBScreenBuffer->SetData(i, buffer);
		}
		m_NeedResize = true;
	}

	Count<Image2D> WorldRenderer::GetShadowPassDebugImage()
	{
		return m_ShadowDebugPass->GetTargetFrameBuffer()->GetOutput(0).As<Image2D>();
	}

	Count<Image2D> WorldRenderer::GetFinalPassImage()
	{
		return m_CompositePass->GetTargetFrameBuffer()->GetOutput(0).As<Image2D>(); 
	}

	void WorldRenderer::SetPasses()
	{
		PF_PROFILE_FUNC();

		Timer setPassesTimer;
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		//scene data
		{
			m_UBSceneData.CameraPosition =ProofToglmVec( m_UBCameraData.Position);
			m_UBSceneDataBuffer->SetData(frameIndex, Buffer(&m_UBSceneData, sizeof(m_UBSceneData)));
		}
		// set up shadow pass
		{
			// NOTE not everyting is set here because we are not sure yet about if they are ture
			m_UBRenderData.bShowCascades = ShadowSetting.ShowCascades;
			m_UBRenderData.bSoftShadows = ShadowSetting.SoftShadows;
			m_UBRenderData.MaxShadowDistance = ShadowSetting.MaxShadowDistance;
			m_UBRenderData.ShadowFade = ShadowSetting.ShadowFade;
			m_UBRenderData.bCascadeFading = ShadowSetting.CascadeFading;
			m_UBRenderData.CascadeTransitionFade = ShadowSetting.CascadeTransitionFade;
		}
		// light scene 
		{
			//skylight
			{
				m_UBLightData = m_LightScene;
				if (m_LightScene.SkyLightCount == 0 || m_Environment == nullptr)
				{
					UBSkyLight skyLight;
					Buffer buffer{ (void*)&skyLight, sizeof(UBSkyLight) };
					m_UBSKyBoxBuffer->SetData(frameIndex, buffer);
					m_Environment = Count<Environment>::Create(Renderer::GetBlackTextureCube(), Renderer::GetBlackTextureCube());
					m_UBLightData.SkyLightCount = 1;
				}
			}

			//directionalLight
			{
				if (m_LightScene.DirectionalLightCount == 0)
				{
					DirectionalLight directlLight = DirectionalLight();

					Buffer buffer{ &directlLight,sizeof(DirectionalLight) };
					m_SBDirectionalLightsBuffer->Resize(frameIndex, buffer);
					m_MainDirectionllLight = directlLight;
					m_UBLightData.DirectionalLightCount = 1;
				}
			}
			//point light
			{
				if (m_LightScene.PointLightCount == 0)
					m_SBPointLightsBuffer->Resize(frameIndex, sizeof(PointLight));
			}
			//spot light
			{
				if (m_LightScene.SpotLightCount == 0)
					m_SBSpotLightsBuffer->Resize(frameIndex, sizeof(SpotLight));
			}
			m_UBLightData.LightCullingWorkGroups = m_LightCullingWorkGroups;
			m_UBLightSceneBuffer->SetData(frameIndex, Buffer(&m_UBLightData, sizeof(m_UBLightData)));
		}
		// set up mesh passes
		{
			uint32_t offset = 0;
			uint64_t submeshTransformSize = m_SubmeshTransformBuffers[frameIndex].Buffer->GetVertexSize() / sizeof(TransformVertexData);
			for (auto& [key, transformData] : m_MeshTransformMap)
			{
				transformData.TransformOffset = offset * sizeof(TransformVertexData);
				for (const auto& transform : transformData.Transforms)
				{
					if (offset >= submeshTransformSize)
						PF_CORE_ASSERT(false, "Need to resize submeshTransforms to small");
					m_SubmeshTransformBuffers[frameIndex].Data[offset] = transform;
					offset++;
				}

			}
			m_SubmeshTransformBuffers[frameIndex].Buffer->SetData(m_SubmeshTransformBuffers[frameIndex].Data, offset * sizeof(TransformVertexData));
		}

		m_Timers.SetPasses = setPassesTimer.ElapsedMillis();
	}
	void WorldRenderer::CalculateCascadesManualSplit(CascadeData* cascades, const glm::vec3& lightDirection)
	{
		float scaleToOrigin = ShadowSetting.ScaleShadowCascadesToOrigin;

		glm::mat4 viewMatrix = m_UBCameraData.View;
		constexpr glm::vec4 origin = glm::vec4(glm::vec3(0.0f), 1.0f);
		viewMatrix[3] = glm::lerp(viewMatrix[3], origin, scaleToOrigin);

		auto viewProjection = m_UBCameraData.UnreversedProjectionMatrix * viewMatrix;

		const int SHADOW_MAP_CASCADE_COUNT = 4;

		float nearClip = m_UBCameraData.NearPlane;
		float farClip = m_UBCameraData.FarPlane;
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// Calculate orthographic projection matrix for each cascade
		float lastSplitDist = 0.0;
		for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float splitDist = ShadowSetting.CascadeSplits[i];
			lastSplitDist = 0.0;

			glm::vec3 frustumCorners[8] =
			{
				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f, -1.0f,  1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),
			};

			// Project frustum corners into world space
			glm::mat4 invCam = glm::inverse(viewProjection);
			for (uint32_t i = 0; i < 8; i++)
			{
				glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
				frustumCorners[i] = invCorner / invCorner.w;
			}

			for (uint32_t i = 0; i < 4; i++)
			{
				glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
				frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
				frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
			}

			// Get frustum center
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (uint32_t i = 0; i < 8; i++)
				frustumCenter += frustumCorners[i];

			frustumCenter /= 8.0f;

			//frustumCenter *= 0.01f;

			float radius = 0.0f;
			for (uint32_t i = 0; i < 8; i++)
			{
				float distance = glm::length(frustumCorners[i] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;
			radius *= ShadowSetting.CascadeSplits[1];

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 up = std::abs(glm::dot(lightDirection, glm::vec3(0.f, 1.0f, 0.0f))) > 0.9 ? glm::normalize(glm::vec3(0.01f, 1.0f, 0.01f)) : glm::vec3(0.f, 1.0f, 0.0f);
			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDirection * radius, frustumCenter, up);
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, -radius + ShadowSetting.CascadeNearPlaneOffset - ShadowSetting.CascadeFarPlaneOffset, 2 * radius);

			// Offset to texel space to avoid shimmering (from https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
			glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
			float ShadowMapResolution = (float)m_ShadowMapPasses[0]->GetTargetFrameBuffer()->GetConfig().Width;
			glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / ShadowMapResolution;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			lightOrthoMatrix[3] += roundOffset;

			// Store split distance and matrix in cascade
			cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
			cascades[i].ViewProjection = lightOrthoMatrix * lightViewMatrix;
			//cascades[i].View = lightViewMatrix;

			lastSplitDist = ShadowSetting.CascadeSplits[i];
		}
	}

	void WorldRenderer::CalculateCascades(CascadeData* cascades, const glm::vec3& lightDirection)
	{
		//https://github.com/SaschaWillems/Vulkan/blob/master/examples/shadowmappingcascade/shadowmappingcascade.cpp
		float scaleToOrigin = ShadowSetting.ScaleShadowCascadesToOrigin;

		glm::mat4 viewMatrix = m_UBCameraData.View;
		constexpr glm::vec4 origin = glm::vec4(glm::vec3(0.0f), 1.0f);
		viewMatrix[3] = glm::lerp(viewMatrix[3], origin, scaleToOrigin);

		auto viewProjection = m_UBCameraData.UnreversedProjectionMatrix * viewMatrix;

		const int SHADOW_MAP_CASCADE_COUNT = 4;
		float cascadeSplits[SHADOW_MAP_CASCADE_COUNT];

		float nearClip = m_UBCameraData.NearPlane;
		float farClip = m_UBCameraData.FarPlane;
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// Calculate split depths based on view camera frustum
		// Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
		for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float p = (i + 1) / static_cast<float>(SHADOW_MAP_CASCADE_COUNT);
			float log = minZ * std::pow(ratio, p);
			float uniform = minZ + range * p;
			float d = ShadowSetting.CascadeSplitLambda * (log - uniform) + uniform;
			cascadeSplits[i] = (d - nearClip) / clipRange;
		}

		cascadeSplits[3] = 0.3f;

		// Manually set cascades here
		// cascadeSplits[0] = 0.05f;
		// cascadeSplits[1] = 0.15f;
		// cascadeSplits[2] = 0.3f;
		// cascadeSplits[3] = 1.0f;

		// Calculate orthographic projection matrix for each cascade
		float lastSplitDist = 0.0;
		for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float splitDist = cascadeSplits[i];

			glm::vec3 frustumCorners[8] =
			{
				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f, -1.0f,  1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),
			};

			// Project frustum corners into world space
			glm::mat4 invCam = glm::inverse(viewProjection);
			for (uint32_t i = 0; i < 8; i++)
			{
				glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
				frustumCorners[i] = invCorner / invCorner.w;
			}

			for (uint32_t i = 0; i < 4; i++)
			{
				glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
				frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
				frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
			}

			// Get frustum center
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (uint32_t i = 0; i < 8; i++)
				frustumCenter += frustumCorners[i];

			frustumCenter /= 8.0f;

			//frustumCenter *= 0.01f;

			float radius = 0.0f;
			for (uint32_t i = 0; i < 8; i++)
			{
				float distance = glm::length(frustumCorners[i] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 up = std::abs(glm::dot(lightDirection, glm::vec3(0.f, 1.0f, 0.0f))) > 0.9 ? glm::normalize(glm::vec3(0.01f, 1.0f, 0.01f)) : glm::vec3(0.f, 1.0f, 0.0f);

			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDirection * radius, frustumCenter, up);
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, -radius + ShadowSetting.CascadeNearPlaneOffset - ShadowSetting.CascadeFarPlaneOffset, 2 * radius);

			// Offset to texel space to avoid shimmering (from https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
			glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
			float ShadowMapResolution = (float)m_ShadowMapPasses[0]->GetTargetFrameBuffer()->GetConfig().Width;
			glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / ShadowMapResolution;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			lightOrthoMatrix[3] += roundOffset;

			// Store split distance and matrix in cascade
			cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
			cascades[i].ViewProjection = lightOrthoMatrix * lightViewMatrix;
			//cascades[i].View = lightViewMatrix;

			lastSplitDist = cascadeSplits[i];
		}
	}

	void WorldRenderer::ShadowPass()
	{
		PF_PROFILE_FUNC();
		Timer shadowPassTimer;
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();

		if (m_MainDirectionllLight.Intensity == 0.f  || !m_MainDirectionllLight.bCastShadows)
			return;
		CascadeData cascades[SHADOWMAP_CASCADE_COUNT];
		if (ShadowSetting.UseManualCascadeSplits)
			CalculateCascadesManualSplit(cascades, m_MainDirectionllLight.Direction);
		else
			CalculateCascades(cascades, m_MainDirectionllLight.Direction);

		{
			m_UBRenderData.cascadeSplit[0] = cascades[0].SplitDepth;
			m_UBRenderData.cascadeSplit[1] = cascades[1].SplitDepth;
			m_UBRenderData.cascadeSplit[2] = cascades[2].SplitDepth;
			m_UBRenderData.cascadeSplit[3] = cascades[3].SplitDepth;
		}
		m_UBRenderDataBuffer->SetData(frameIndex, Buffer(&m_UBRenderData, sizeof(m_UBRenderData)));

		struct CascadeProjection
		{
			glm::mat4 ViewProjection[4];
		};
		CascadeProjection projections;
		glm::vec4 Splits;
		for (int i = 0; i < 4; i++)
		{
			projections.ViewProjection[i] = cascades[i].ViewProjection ;
			Splits[i] = cascades[i].SplitDepth;
		}
		

		m_UBCascadeProjectionBuffer->SetData(frameIndex, Buffer(&projections, sizeof(CascadeProjection)));
		for (uint32_t cascade = 0; cascade < SHADOWMAP_CASCADE_COUNT; cascade++)
		{
			PF_PROFILE_FUNC("ShadowPass Cascade");
			PF_PROFILE_TAG("Cascade ", cascade);

			auto cascadePass = m_ShadowMapPasses[cascade];
			cascadePass->SetInput("ShadowCascadesProjection", m_UBCascadeProjectionBuffer);

			Renderer::BeginRenderPass(m_CommandBuffer, cascadePass);
			for (auto& [meshKey, dc] : m_MeshShadowDrawList)
			{
				const auto& transformData = m_MeshTransformMap.at(meshKey);
				uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
				RenderMesh(m_CommandBuffer, dc.Mesh,cascadePass,m_SubmeshTransformBuffers[frameIndex].Buffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount, Buffer(&cascade,sizeof(uint32_t)),"u_CascadeInfo");
			}
			for (auto& [meshKey, dc] : m_DynamicMeshShadowDrawList)
			{
				const auto& transformData = m_MeshTransformMap.at(meshKey);
				uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
				RenderDynamicMesh(m_CommandBuffer, dc.Mesh, cascadePass, m_SubmeshTransformBuffers[frameIndex].Buffer, dc.SubMeshIndex, transformOffset,  dc.InstanceCount, Buffer(&cascade, sizeof(uint32_t)), "u_CascadeInfo");
			}
			Renderer::EndRenderPass(cascadePass);
		}


		// the same funtion but it is extremely slow
		//{
		//	Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_ShadowDebugPass);
		//	m_ShadowPassDebugMaterial->Set("u_CascadeInfo.CascadeIndex", debugCascade);
		//	Renderer::RenderPassPushRenderMaterial(m_ShadowDebugPass,m_ShadowPassDebugMaterial);
		//	Renderer::DrawArrays(m_CommandBuffer, 3, 1, 0, 0);
		//	Renderer::EndRenderPass(m_ShadowDebugPass);
		//}
		if(ShadowSetting.RenderDebugPass)
		{
			PF_PROFILE_FUNC("ShadowPass::DebugCascade ");

			Renderer::BeginRenderPass(m_CommandBuffer, m_ShadowDebugPass);
			m_ShadowDebugPass->PushData("u_CascadeInfo",&ShadowSetting.DebugCascade);
			m_ShadowDebugPass->SetInput("u_ShadowMap", m_ShadowPassImage);
			Renderer::DrawArrays(m_CommandBuffer, 3, 1, 0, 0);
			Renderer::EndRenderPass(m_ShadowDebugPass);
		}

		m_Timers.ShadowPass = shadowPassTimer.ElapsedMillis();
	}
	void WorldRenderer::PreDepthPass()
	{
		PF_PROFILE_FUNC();

		Timer preDepthTimer;
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();

		Renderer::BeginRenderPass(m_CommandBuffer, m_PreDepthPass, true);

		for (auto& [meshKey, dc] : m_MeshDrawList)
		{
			const auto& transformData = m_MeshTransformMap.at(meshKey);
			uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
			RenderMesh(m_CommandBuffer, dc.Mesh, m_PreDepthPass, m_SubmeshTransformBuffers[frameIndex].Buffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount);
		}
		for (auto& [meshKey, dc] : m_DynamicMeshDrawList)
		{
			const auto& transformData = m_MeshTransformMap.at(meshKey);
			uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
			RenderDynamicMesh(m_CommandBuffer, dc.Mesh, m_PreDepthPass, m_SubmeshTransformBuffers[frameIndex].Buffer, dc.SubMeshIndex, transformOffset,  dc.InstanceCount);
		}
		Renderer::EndRenderPass(m_PreDepthPass);

		m_Stats.Timers.PreDepthPass = preDepthTimer.ElapsedMillis();

	}
	void WorldRenderer::LightFrustrumAndCullingPass()
	{
		if (m_LightScene.SpotLightCount == 0 && m_LightScene.PointLightCount == 0)
			return;
		#if 1
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();

		PF_PROFILE_FUNC();
		{
			PF_PROFILE_FUNC("CalcalateFrustrumGrid");
			
			Timer timer;
			//Renderer::BeginComputePass(m_CommandBuffer, m_FrustrumPass);	
			//m_FrustrumPass->PushData("u_PushData", &m_LightCullingNumThreads);
			//m_FrustrumPass->Dispatch({ m_LightCullingNumThreadGroups,1 });
			//Renderer::EndComputePass(m_FrustrumPass);
			m_Timers.LightCalculateGridFrustum = timer.ElapsedMillis();
		}

		{
			
			PF_PROFILE_FUNC("LightCulling");
			Timer timer;


			m_LightCullingPass->SetInput("u_DepthTexture", m_PreDepthPass->GetTargetFrameBuffer()->GetDepthOutput().As<Image2D>());
			Renderer::BeginComputePass(m_CommandBuffer, m_LightCullingPass);
			//m_LightCullingPass->PushData("u_PushData", &m_LightCullingNumThreads);
			m_LightCullingPass->Dispatch(m_LightCullingWorkGroups);

			Renderer::Submit([commandBuffer = m_CommandBuffer ]()
				{

					VkMemoryBarrier barrier = {};
					barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
					barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
						0,
						1, &barrier,
						0, nullptr,
						0, nullptr);
				});

			Renderer::EndComputePass(m_LightCullingPass);

			m_Timers.LightCulling = timer.ElapsedMillis();
		}
		#endif
		
	}
	void WorldRenderer::GeometryPass()
	{
		PF_PROFILE_FUNC();

		Timer geometryPassTimer;

		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		auto transformBuffer = m_SubmeshTransformBuffers[frameIndex].Buffer;
	


		{
			PF_PROFILE_FUNC("GeometryPass::SkyBoxPass");

			Timer timer;
			m_SkyBoxPass->SetInput("u_EnvironmentMap", m_Environment->PrefilterMap);

			Renderer::BeginRenderPass(m_CommandBuffer, m_SkyBoxPass);
			Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_SkyBoxPass);
			Renderer::EndRenderPass(m_SkyBoxPass);

			m_Timers.GeometrySkyBoxPass = timer.ElapsedMillis();
		}

		{
			PF_PROFILE_FUNC("GeometryPass::MeshPass");

			Timer timer;
			m_GeometryPass->SetInput("u_IrradianceMap", m_Environment->IrradianceMap);
			m_GeometryPass->SetInput("u_PrefilterMap", m_Environment->PrefilterMap);
			Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_GeometryPass);

			{
				PF_PROFILE_FUNC("GeometryPass::Static");

				for (auto& [meshKey, dc] : m_MeshDrawList)
				{
					const auto& transformData = m_MeshTransformMap.at(meshKey);
					uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
					RenderMeshWithMaterialTable(m_CommandBuffer, dc.Mesh, dc.MaterialTable, m_GeometryPass, transformBuffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount);
				}
			}
			Renderer::EndRenderPass(m_GeometryPass);
			/*
			{
				PF_PROFILE_FUNC("GeometryPass::Dynamic");

				for (auto& [meshKey, dc] : m_DynamicMeshDrawList)
				{
					const auto& transformData = m_MeshTransformMap.at(meshKey);
					uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
					//transformData.Transforms
					RenderDynamicMeshWithMaterialTable(m_CommandBuffer, dc.Mesh, dc.MaterialTable, m_GeometryPass, transformBuffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount);
				}
			}
			*/
			m_Timers.GeometryMeshPass = timer.ElapsedMillis();
		}

		m_Timers.GeometryPass = geometryPassTimer.ElapsedMillis();
	}
	
	void WorldRenderer::CompositePass()
	{
		PF_PROFILE_FUNC();

		Timer compositeTimer;
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		
		//AmbientOcclusionPass();

		// this has to be the last thgn called
		{
			PF_PROFILE_FUNC("WorldRenderer::WorldComposite");

			Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_CompositePass, true);
			//float exposure = m_SceneData.SceneCamera.Camera.GetExposure();
			auto inputImage = m_GeometryPass->GetOutput(0);
			m_CompositeMaterial->Set("u_WorldTexture", inputImage);
			Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_CompositePass, m_CompositeMaterial);
			Renderer::EndRenderPass(m_CompositePass);
		}

		if (Options.ShowPhysicsColliders != WorldRendererOptions::PhysicsColliderView::None)
		{
			PF_PROFILE_FUNC("CompositePass::PhysicsDebugMeshes");
			Timer physicsDebugMesh;

			m_GeometryWireFramePassMaterial->GetVector4("u_MaterialUniform.Color").X = Options.PhysicsColliderColor.x;
			m_GeometryWireFramePassMaterial->GetVector4("u_MaterialUniform.Color").Y = Options.PhysicsColliderColor.y;
			m_GeometryWireFramePassMaterial->GetVector4("u_MaterialUniform.Color").Z = Options.PhysicsColliderColor.z;
			m_GeometryWireFramePassMaterial->GetVector4("u_MaterialUniform.Color").W = Options.PhysicsColliderColor.w;

			auto transformBuffer = m_SubmeshTransformBuffers[frameIndex].Buffer;

			auto wireFramePass = Options.ShowPhysicsColliders == WorldRendererOptions::PhysicsColliderView::Normal ? m_GeometryWireFramePass : m_GeometryWireFrameOnTopPass;
			Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, wireFramePass);
			for (auto& [meshKey, dc] : m_ColliderDrawList)
			{
				const auto& transformData = m_MeshTransformMap.at(meshKey);
				uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
				RenderMeshWithMaterial(m_CommandBuffer, dc.Mesh, m_GeometryWireFramePassMaterial,
					wireFramePass,
					transformBuffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount);
			}
			Renderer::EndRenderPass(wireFramePass);
			m_Timers.DrawPhysicsColliders = physicsDebugMesh.Elapsed();
		}
		m_Timers.CompositePass = compositeTimer.ElapsedMillis();
	}
	void WorldRenderer::AmbientOcclusionPass()
	{
		if (AmbientOcclusion.Enabled == false)
			return;

		PF_PROFILE_FUNC();
		
		if (AmbientOcclusion.Type == AmbientOcclusion::AmbientOcclusionType::SSAO)
		{
			
			auto& ssao = AmbientOcclusion.SSAO;

			if (m_SBSSAOSampleKernalBuffer->GetBuffer(0)->GetSize() != sizeof(glm::vec3) * ssao.KernelSize)
			{
				Utils::GenerateSSAOSampleKernal(m_SBSSAOSampleKernalBuffer, ssao.KernelSize);
			}

			if (m_SSAONoiseImage->GetWidth() != ssao.NoiseSize)
			{
				Utils::GenerateSSAONoise(m_SSAONoiseImage, ssao.NoiseSize);
			}

			Renderer::BeginRenderMaterialComputePass(m_CommandBuffer, m_SSAOPass);
			m_SSAOMaterial->Set("u_Uniforms.Radius", ssao.Radius);
			m_SSAOMaterial->Set("u_Uniforms.Bias", ssao.Bias);
			m_SSAOMaterial->Set("u_Uniforms.KernelSize", ssao.KernelSize);
			m_SSAOMaterial->Set("u_Uniforms.NoiseScale", ssao.NoiseSize);

			Renderer::ComputePassPushRenderMaterial(m_SSAOPass,m_SSAOMaterial);
			m_SSAOPass->Dispatch({ m_UBScreenData.FullResolution.x / 16,m_UBScreenData.FullResolution.y / 16,1 });
			Renderer::EndComputePass(m_SSAOPass);

			Renderer::BeginRenderPass(m_CommandBuffer, m_SSAOBlurPass);
			Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_SSAOBlurPass);
			Renderer::EndRenderPass(m_SSAOBlurPass);

			{
				PF_PROFILE_FUNC("AOComposite");
				m_AmbientOcclusionCompositePass->SetInput("u_InputAOTexture", m_SSAOBlurPass->GetOutput(0).As<Image2D>());
				Renderer::BeginRenderPass(m_CommandBuffer, m_AmbientOcclusionCompositePass);
				Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_AmbientOcclusionCompositePass);
				Renderer::EndRenderPass(m_AmbientOcclusionCompositePass);
			}
		}

		
	}
	void WorldRenderer::SubmitMesh(Count<Mesh> mesh, Count<MaterialTable> materialTable, const glm::mat4& transform, bool CastShadowws)
	{
		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("{}", mesh->GetName().c_str());
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		PF_CORE_ASSERT(mesh->GetID(), "Mesh ID cannot be zero");

		AssetID meshID = mesh->GetID();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		for (uint32_t submeshIndex : mesh->GetSubMeshes())
		{
			const auto& subMesh = meshSource->GetSubMeshes().at(submeshIndex);

			glm::mat4 subMeshTransform = transform * subMesh.Transform;

			uint32_t materialIndex = subMesh.MaterialIndex;

			AssetID materialHandle = materialTable->HasMaterial(materialIndex) ? materialTable->GetMaterial(materialIndex)->GetID() : mesh->GetMaterialTable()->GetMaterial(materialIndex)->GetID();
			PF_CORE_ASSERT(materialHandle,"Material ID cannot be zero");

			MeshKey meshKey = { meshID, materialHandle, submeshIndex, false };
			auto& transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();
			transformStorage.Transform = subMeshTransform;

			// geo pass
			{
				auto& dc = m_MeshDrawList[meshKey];
				dc.MaterialTable = materialTable;
				dc.Mesh = mesh;
				dc.SubMeshIndex = submeshIndex;
				dc.InstanceCount++;
				dc.OverrideMaterial = nullptr;
			}
			if (CastShadowws)
			{
				auto& dc = m_MeshShadowDrawList[meshKey];
				dc.MaterialTable = materialTable;
				dc.Mesh = mesh;
				dc.SubMeshIndex = submeshIndex;
				dc.InstanceCount++;
				dc.OverrideMaterial = nullptr;
			}
		}
	}
	void WorldRenderer::SubmitDynamicMesh(Count<DynamicMesh> mesh, Count<MaterialTable> materialTable, uint32_t subMeshIndex, const glm::mat4& transform, bool CastShadows)
	{
		PF_PROFILE_FUNC();
		//PF_PROFILE_TAG("{}", mesh->GetName().c_str());
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		PF_CORE_ASSERT(mesh->GetID(), "Mesh ID cannot be zero");

		AssetID meshID = mesh->GetID();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		const auto& submeshData = meshSource->GetSubMeshes();
		const auto& subMesh = meshSource->GetSubMeshes().at(subMeshIndex);

		glm::mat4 subMeshTransform = transform * subMesh.Transform;

		uint32_t materialIndex = subMesh.MaterialIndex;

		AssetID materialHandle = materialTable->HasMaterial(materialIndex) ? materialTable->GetMaterial(materialIndex)->GetID() : mesh->GetMaterialTable()->GetMaterial(materialIndex)->GetID();
		PF_CORE_ASSERT(materialHandle, "Material ID cannot be zero");

		MeshKey meshKey = { meshID, materialHandle, subMeshIndex, false };
		auto& transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();
		transformStorage.Transform = subMeshTransform;

		// geo pass
		{
			auto& dc = m_DynamicMeshDrawList[meshKey];
			dc.MaterialTable = materialTable;
			dc.Mesh = mesh;
			dc.SubMeshIndex = subMeshIndex;
			dc.InstanceCount++;
			dc.OverrideMaterial = nullptr;
		}
		if (CastShadows)
		{
			auto& dc = m_DynamicMeshShadowDrawList[meshKey];
			dc.MaterialTable = materialTable;
			dc.Mesh = mesh;
			dc.SubMeshIndex = subMeshIndex;
			dc.InstanceCount++;
			dc.OverrideMaterial = nullptr;
		}
	}

	void WorldRenderer::SubmitPhysicsDebugMesh(Count<Mesh> mesh, const glm::mat4& transform)
	{

		PF_PROFILE_FUNC();
		PF_PROFILE_TAG("{}", mesh->GetName().c_str());
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		//PF_CORE_ASSERT(mesh->GetID(), "Mesh ID cannot be zero");

		AssetID meshID = mesh->GetID();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		const auto& submeshData = meshSource->GetSubMeshes();
		for (uint32_t submeshIndex : mesh->GetSubMeshes())
		{
			const auto& subMesh = meshSource->GetSubMeshes().at(submeshIndex);

			glm::mat4 subMeshTransform = transform * subMesh.Transform;

			uint32_t materialIndex = subMesh.MaterialIndex;

			AssetID materialHandle = m_GeometryWireFramePassMaterialAsset->GetID();
			PF_CORE_ASSERT(materialHandle, "Material ID cannot be zero");

			MeshKey meshKey = { meshID, materialHandle, submeshIndex, false };
			auto& transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();
			transformStorage.Transform = subMeshTransform;

			// geo pass
			{
				auto& dc = m_ColliderDrawList[meshKey];
				dc.Mesh = mesh;
				dc.SubMeshIndex = submeshIndex;
				dc.InstanceCount++;
				dc.OverrideMaterial = nullptr;
			}
		}
	}

	void WorldRenderer::SubmitSkyLight(const UBSkyLight& skyLight, Count<class Environment> environment)
	{
		PF_PROFILE_FUNC();

		if (environment == nullptr)
		{
			PF_ENGINE_WARN("{} Submitting SKy light with null environment", m_ActiveWorld->GetName());
			return;
		}
		if (m_LightScene.SkyLightCount == 1)
			PF_ENGINE_WARN("{} Submiting mulitple sky light only the last one will be used, submit 1 to save performance", m_ActiveWorld->GetName());

		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();


		Buffer buffer{ (void*)&skyLight, sizeof(UBSkyLight) };
		m_UBSKyBoxBuffer->SetData(frameIndex, buffer);

		m_Environment = environment;
		m_LightScene.SkyLightCount = 1;
	}

	void WorldRenderer::SubmitDirectionalLight(const SBDirectionalLightsSceneData& directionaLights)
	{
		PF_PROFILE_FUNC();

		if (directionaLights.DirectionalLights.empty())
		{
			PF_ENGINE_WARN("{} Dont submit empty directionalLights", m_ActiveWorld->GetName());
			return;
		}

		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();

		m_MainDirectionllLight = directionaLights.DirectionalLights[0];

		Buffer buffer{(void*) directionaLights.DirectionalLights.data(), directionaLights.DirectionalLights.size() * sizeof(DirectionalLight)};
		m_SBDirectionalLightsBuffer->Resize(frameIndex, buffer);

		m_LightScene.DirectionalLightCount = directionaLights.DirectionalLights.size();
	}

	void WorldRenderer::SubmitPointLight(const SBPointLightSceneData& pointLights)
	{
		PF_PROFILE_FUNC();

		if (pointLights.PointLights.empty())
		{
			PF_ENGINE_WARN("{} Dont submit empty Point Lights", m_ActiveWorld->GetName());
			return;
		}
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		
		Buffer buffer{ (void*)pointLights.PointLights.data(), pointLights.PointLights.size() * sizeof(PointLight) };
		m_SBPointLightsBuffer->Resize(frameIndex, buffer);

		m_LightScene.PointLightCount = pointLights.PointLights.size();
	}
	void WorldRenderer::SubmitSpotLight(const SBSpotLightSceneData& spotLights)
	{
		PF_PROFILE_FUNC();

		if (spotLights.SpotLights.empty())
		{
			PF_ENGINE_WARN("{} Dont submit empty Spot Lights", m_ActiveWorld->GetName());
			return;
		}
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		

		Buffer buffer{ (void*)spotLights.SpotLights.data(), spotLights.SpotLights.size() * sizeof(SpotLight) };
		m_SBSpotLightsBuffer->Resize(frameIndex, buffer);

		m_LightScene.SpotLightCount = spotLights.SpotLights.size();
	}
	void WorldRenderer::RenderMesh(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset,  uint32_t instanceCount, const Buffer& pushData, const std::string& pushName)
	{
		PF_PROFILE_FUNC();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		if (pushData)
		{
			renderPass->PushData(pushName, pushData.Get());
		}
		const SubMesh& subMesh = meshSource->GetSubMeshes()[subMeshIndex];
		Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
	}
	void WorldRenderer::RenderDynamicMesh(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset,  uint32_t instanceCount, const Buffer& pushData, const std::string& pushName)
	{
		PF_PROFILE_FUNC();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		if (pushData)
		{
			renderPass->PushData(pushName, pushData.Get());
		}
		const SubMesh& subMesh = meshSource->GetSubMeshes()[subMeshIndex];
		Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
	}

	void WorldRenderer::RenderMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer,Count<Mesh>&mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass,Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount)
	{
		PF_PROFILE_FUNC();

		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		const SubMesh& subMesh = meshSource->GetSubMeshes()[subMeshIndex];
		Count<RenderMaterial> renderMaterial = material;

		Renderer::RenderPassPushRenderMaterial(renderPass, renderMaterial);
		Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
	}

	void WorldRenderer::RenderDynamicMeshWithMaterial(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<RenderMaterial>& material, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount)
	{
		PF_PROFILE_FUNC();

		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		const SubMesh& subMesh = meshSource->GetSubMeshes()[subMeshIndex];
		Count<RenderMaterial> renderMaterial = material;

		Renderer::RenderPassPushRenderMaterial(renderPass, renderMaterial);
		Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
	}

	void WorldRenderer::RenderMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer, Count<Mesh>& mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount)
	{
		PF_PROFILE_FUNC();
		#if 0
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);

		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		for (const uint32_t& index : mesh->GetSubMeshes())
		{
			const SubMesh& subMesh = meshSource->GetSubMeshes()[index];
			Count<RenderMaterial> renderMaterial = materialTable->HasMaterial(subMesh.MaterialIndex) ? materialTable->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial()
				: mesh->GetMaterialTable()->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial();

			Renderer::RenderPassPushRenderMaterial(renderPass, renderMaterial);
			Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
		}
		#endif

		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);

		const SubMesh& subMesh = meshSource->GetSubMeshes()[subMeshIndex];
		Count<RenderMaterial> renderMaterial = materialTable->HasMaterial(subMesh.MaterialIndex) ? materialTable->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial()
			: mesh->GetMaterialTable()->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial();

		Renderer::RenderPassPushRenderMaterial(renderPass, renderMaterial);
		Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
	}

	void WorldRenderer::RenderDynamicMeshWithMaterialTable(Count<RenderCommandBuffer>& commandBuffer, Count<DynamicMesh>& mesh, Count<MaterialTable>& materialTable, Count<RenderPass>& renderPass, Count<VertexBuffer>& transformBuffer, uint32_t subMeshIndex, uint32_t transformOffset, uint32_t instanceCount)
	{
		PF_PROFILE_FUNC();
		#if 0
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);

		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		for (const uint32_t& index : mesh->GetSubMeshes())
		{
			const SubMesh& subMesh = meshSource->GetSubMeshes()[index];
			Count<RenderMaterial> renderMaterial = materialTable->HasMaterial(subMesh.MaterialIndex) ? materialTable->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial()
				: mesh->GetMaterialTable()->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial();

			Renderer::RenderPassPushRenderMaterial(renderPass, renderMaterial);
			Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
		}
		#endif

		Count<MeshSource> meshSource = mesh->GetMeshSource();
		meshSource->GetVertexBuffer()->Bind(commandBuffer);
		transformBuffer->Bind(commandBuffer, 1, transformOffset);
		meshSource->GetIndexBuffer()->Bind(commandBuffer);
		const SubMesh& subMesh = meshSource->GetSubMeshes()[subMeshIndex];
		Count<RenderMaterial> renderMaterial = materialTable->HasMaterial(subMesh.MaterialIndex) ? materialTable->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial()
			: mesh->GetMaterialTable()->GetMaterial(subMesh.MaterialIndex)->GetRenderMaterial();

		Renderer::RenderPassPushRenderMaterial(renderPass, renderMaterial);
		Renderer::DrawElementIndexed(commandBuffer, subMesh.IndexCount, instanceCount, subMesh.BaseIndex, subMesh.BaseVertex);
	}
}
