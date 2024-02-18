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
#include "Proof/Platform/Vulkan/VulkanSwapChain.h"
#include "GraphicsPipeLine.h"
#include "Proof/Scene/Mesh.h"
#include "Shader.h"
#include "Proof/Physics/PhysicsMeshCooker.h"
#include "DebugMeshRenderer.h"
#include "Font.h"
#include "ParticleSystem.h"
#include "ComputePipeline.h"
#include "RenderMaterial.h"
#include "ComputePass.h"
#include "Proof/Scene/Material.h"
#include "Proof/Math/Random.h"
#include "Vertex.h"
#include "Proof/Platform/Vulkan/VulkanFrameBuffer.h"
#include "Proof/Platform/Vulkan/VulkanImage.h"
#include "Proof/Math/MathConvert.h"	
#include "Proof/Platform/Vulkan/VulkanCommandBuffer.h"
#include "Proof/Platform/Vulkan/VulkanTexutre.h"
#include "Proof/Scene/Material.h"
#include "Proof/Platform/Vulkan/VulkanComputePass.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Core/Core.h"
#include "RendererSampler.h"

#include "VertexArray.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
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
	template<typename T> inline T DivideRoundingUp(T a, T b)
	{
		return (a + b - (T)1) / b;
	}
	static inline uint32_t GetGroupCount(uint32_t threadCount, uint32_t localSize)
	{
		return (threadCount + localSize - 1) / localSize;
	}
	static std::array<glm::vec4, 16> HBAOJitter()
	{
		static std::uniform_real_distribution<float> s_Jitters(0.f, 1.f);
		static std::default_random_engine s_JitterGenerator(1337u);
		constexpr float PI = 3.14159265358979323846264338f;
		const float numDir = 8.f;  // keep in sync to glsl

		std::array<glm::vec4, 16> result{};

		for (int i = 0; i < 16; i++)
		{
			float Rand1 = s_Jitters(s_JitterGenerator);
			float Rand2 = s_Jitters(s_JitterGenerator);
			// Use random rotation angles in [0,2PI/NUM_DIRECTIONS)
			const float Angle = 2.f * PI * Rand1 / numDir;
			result[i].x = cosf(Angle);
			result[i].y = sinf(Angle);
			result[i].z = Rand2;
			result[i].w = 0;
		}
		return result;
	}
	struct MeshInstanceVertex {

		glm::mat4 Transform;
		glm::mat4 PrevTransform;
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

	static const uint32_t DOF_NUM_THREADS = 32;
	static const uint32_t BLOOM_COMPUTE_WORK_GROUP_SIZE = 8;
	static const uint32_t HBAO_WORK_GROUP_SIZE = 16u;
	#define AO_RANDOMTEX_SIZE 4
	static constexpr int HBAO_RANDOM_SIZE = AO_RANDOMTEX_SIZE;
	static constexpr int HBAO_RANDOM_ELEMENTS = HBAO_RANDOM_SIZE * HBAO_RANDOM_SIZE;
	static constexpr int NUM_MRT = 8;
	WorldRenderer::~WorldRenderer() {
		for (auto& transformBuffer : m_SubmeshTransformBuffers)
			pdelete[] transformBuffer.Data;
		//PipelineLayout = nullptr;
	}
	WorldRenderer::WorldRenderer()
		:
		m_Timers(m_Stats.Timers), m_LightScene(m_Stats.LightSene),
		DebugOptions(), ShadowSetting(PreProcessSettings.ShadowSettings),
		AmbientOcclusionSettings(PostProcessSettings.AmbientOcclusionSettings), BloomSettings(PostProcessSettings.BloomSettings), DOFSettings(PostProcessSettings.DOFSettings), 
		SSRSettings(PostProcessSettings.SSRSettings)
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

		m_UBFrameBuffer = UniformBufferSet::Create(sizeof(UBFrameData));
		m_UBScreenBuffer = UniformBufferSet::Create(sizeof(UBScreenData));
		m_UBRenderDataBuffer = UniformBufferSet::Create(sizeof(UBRenderData));
		m_UBSceneDataBuffer = UniformBufferSet::Create(sizeof(UBSceneData));
		m_UBCameraBuffer = UniformBufferSet::Create(sizeof(UBCameraData));
		m_UBSKyBoxBuffer = UniformBufferSet::Create(sizeof(UBSkyLight));
		m_UBCascadeProjectionBuffer = UniformBufferSet::Create(sizeof(glm::mat4) * 4);
		m_UBLightSceneBuffer = UniformBufferSet::Create(sizeof(UBLightScene) * 4);
		m_UBHBAOBuffer = UniformBufferSet::Create(sizeof(UBHBAOData));

		//storage images
		m_SBDirectionalLightsBuffer = StorageBufferSet::Create(sizeof(DirectionalLight));
		m_SBPointLightsBuffer = StorageBufferSet::Create(sizeof(PointLight)); // set to one light because we cant actaully set a buffer to size 0
		m_SBSpotLightsBuffer = StorageBufferSet::Create(sizeof(SpotLight));// set to one light because we cant actaully set a buffer to size 0
		SetViewportSize(100, 100);

		const glm::uvec2 viewportSize = m_UBScreenData.FullResolution;
		m_CommandBuffer = RenderCommandBuffer::Create("WorldRenderer");
		m_Renderer2D = Count<Renderer2D>::Create();
		m_Cube = MeshWorkShop::GenerateCube();
		m_Environment = Count<Environment>::Create(EnvironmentTextureData());

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
			m_GlobalInputs->SetData("FrameData", m_UBFrameBuffer);

			m_GlobalInputs->SetData("u_PointClampEdgeSampler", SamplerFactory::GetPointClampEdge());
			m_GlobalInputs->SetData("u_PointClampBorder0000Sampler", SamplerFactory::GetPointClampBorder0000());
			m_GlobalInputs->SetData("u_PointRepeatSampler", SamplerFactory::GetPointRepeat());
			m_GlobalInputs->SetData("u_PointClampBorder1111Sampler", SamplerFactory::GetPointClampBorder1111());

			m_GlobalInputs->SetData("u_LinearClampEdgeSampler", SamplerFactory::GetLinearClampEdge());
			m_GlobalInputs->SetData("u_LinearClampBorder0000Sampler", SamplerFactory::GetLinearClampBorder0000MipPoint());
			m_GlobalInputs->SetData("u_LinearRepeatSampler", SamplerFactory::GetLinearRepeat());
			m_GlobalInputs->SetData("u_LinearClampBorder1111Sampler", SamplerFactory::GetLinearClampBorder1111MipPoint());

			m_GlobalInputs->SetData("u_LinearClampEdgeMipFilterSampler", SamplerFactory::GetLinearClampEdgeMipPoint());
			m_GlobalInputs->SetData("u_LinearRepeatMipFilterSampler", SamplerFactory::GetLinearRepeatMipPoint());

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

		staticVertexArray->AddData(9, DataType::Vec4, (sizeof(glm::vec4) * 4), 1);
		staticVertexArray->AddData(10, DataType::Vec4, (sizeof(glm::vec4) * 5), 1);
		staticVertexArray->AddData(11, DataType::Vec4, (sizeof(glm::vec4) * 6), 1);
		staticVertexArray->AddData(12, DataType::Vec4, (sizeof(glm::vec4) * 7), 1);

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

			ImageConfiguration imageConfig;
			imageConfig.DebugName = "PrevDepthImage";
			imageConfig.Height = 1;
			imageConfig.Width = 1;
			imageConfig.Format = ImageFormat::DEPTH32F;
			imageConfig.Usage = ImageUsage::Attachment;
			m_PrevDepthImage = Image2D::Create(imageConfig);
			m_PreDepthPass->AddGlobalInput(m_GlobalInputs);

			//m_PreDepthPass->SetInput("CameraData", m_UBCameraBuffer);

		}
		//foward plus
		{

			glm::uvec2 size = viewportSize;
			size += TILE_SIZE - viewportSize % TILE_SIZE;
			m_LightCullingWorkGroups = glm::uvec3{ size / TILE_SIZE, 1u };
			m_SBVisiblePointLightIndicesBuffer = StorageBufferSet::Create(m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(int) * MAX_NUM_LIGHTS_PER_TILE);
			m_SBVisibleSpotLightIndicesBuffer = StorageBufferSet::Create(m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(int) * MAX_NUM_LIGHTS_PER_TILE);

			ComputePipelineConfig lightCullingpipeline;
			lightCullingpipeline.DebugName = "LightCulling";
			lightCullingpipeline.Shader = Renderer::GetShader("LightCulling");
			m_LightCullingPass = ComputePass::Create({ "LightCulling",ComputePipeline::Create(lightCullingpipeline) });
			m_LightCullingPass->AddGlobalInput(m_GlobalInputs);
			m_LightCullingPass->SetInput("VisiblePointLightIndicesBuffer", m_SBVisiblePointLightIndicesBuffer);
			m_LightCullingPass->SetInput("VisibleSpotLightIndicesBuffer", m_SBVisibleSpotLightIndicesBuffer);
			m_LightCullingPass->SetInput("PointLightBuffer", m_SBPointLightsBuffer);
			m_LightCullingPass->SetInput("SpotLightBuffer", m_SBSpotLightsBuffer);
			m_LightCullingPass->SetInput("LightInformationBuffer", m_UBLightSceneBuffer);
			m_LightCullingPass->SetInput("u_DepthTexture", m_PreDepthPass->GetOutput(0));

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
			geoFramebufferConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::RGBA16F,ImageFormat::RGBA, ImageFormat::RG16F,ImageFormat::DEPTH32F }; // color, view limuncance, metallnessroughness,velocity
			geoFramebufferConfig.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			geoFramebufferConfig.Attachments.Attachments[4].ExistingImage = m_PreDepthPass->GetOutput(0);
			auto frameBuffer = FrameBuffer::Create(geoFramebufferConfig);

			GraphicsPipelineConfiguration pipelinelineConfig;
			pipelinelineConfig.Attachments = { ImageFormat::RGBA32F, ImageFormat::RGBA16F,ImageFormat::RGBA,ImageFormat::RG16F, ImageFormat::DEPTH32F }; // color, view limuncance, metallnessroughness,velocity
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
			m_GeometryPass->SetInput("u_IrradianceMap", m_Environment->GetIrradianceMap());
			m_GeometryPass->SetInput("u_PrefilterMap", m_Environment->GetPrefilterMap());
			m_GeometryPass->SetInput("u_BRDFLUT", Renderer::GetBRDFLut());
			m_GeometryPass->SetInput("SkyBoxData", m_UBSKyBoxBuffer);
			m_GeometryPass->SetInput("u_ShadowMap", m_ShadowPassImage);
			m_GeometryPass->SetInput("RendererData", m_UBRenderDataBuffer);
			m_GeometryPass->SetInput("SceneData", m_UBSceneDataBuffer);
			m_GeometryPass->SetInput("ShadowMapProjections", m_UBCascadeProjectionBuffer);

			m_GeometryPass->AddGlobalInput(m_GlobalInputs);

			m_GeometryPass->SetInput("LightInformationBuffer", m_UBLightSceneBuffer);

			m_GeometryPass->SetInput("VisiblePointLightIndicesBuffer", m_SBVisiblePointLightIndicesBuffer);
			m_GeometryPass->SetInput("VisibleSpotLightIndicesBuffer", m_SBVisibleSpotLightIndicesBuffer);


			ImageConfiguration imageConfig;
			imageConfig.DebugName = "PrevNormalMap";
			imageConfig.Height = 1;
			imageConfig.Width = 1;
			imageConfig.Format = ImageFormat::RGBA16F;
			imageConfig.Usage = ImageUsage::Attachment;
			m_PrevNormalImage = Image2D::Create(imageConfig);
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
			//AmbientOcclusion
			{
				//AO-compoiste
				{
					FrameBufferConfig framebufferSpec;
					framebufferSpec.DebugName = "AO-Composite";
					framebufferSpec.Attachments = { ImageFormat::RGBA32F };
					framebufferSpec.Attachments.Attachments[0].ExistingImage = m_GeometryPass->GetOutput(0);
					framebufferSpec.ClearColorOnLoad = false;

					auto aoframeBuffer = FrameBuffer::Create(framebufferSpec);

					GraphicsPipelineConfiguration pipelineConfig;
					pipelineConfig.DebugName = "AO-Composite";
					pipelineConfig.VertexArray = quadVertexArray;
					pipelineConfig.Attachments = { ImageFormat::RGBA32F };
					pipelineConfig.BlendMode = BlendMode::Zero_SrcColor ;
					pipelineConfig.Blend = true;
					pipelineConfig.DepthTest = false;
					pipelineConfig.Shader = Renderer::GetShader("AO-Composite");
					pipelineConfig.DepthCompareOperator = DepthCompareOperator::Equal;

					auto aoPipeline = GraphicsPipeline::Create(pipelineConfig);

					RenderPassConfig renderPassConfig;
					renderPassConfig.DebugName = "AO-Composite";
					renderPassConfig.Pipeline = aoPipeline;
					renderPassConfig.TargetFrameBuffer = aoframeBuffer;
					m_AmbientOcclusionCompositePass = RenderPass::Create(renderPassConfig);
				}

				//HBAO
				{
					//Deinterleaving
					{
						ImageConfiguration imageSpec;
						imageSpec.Format = ImageFormat::R32F;
						imageSpec.Layers = 16;
						imageSpec.Usage = ImageUsage::Attachment;
						imageSpec.DebugName = "Deinterleaved";
						Count<Image2D> image = Image2D::Create(imageSpec);
						image->CreateMipAndLayerViews();

						FrameBufferConfig deinterleavingFramebufferSpec;
						deinterleavingFramebufferSpec.Width = 1;
						deinterleavingFramebufferSpec.Height = 1;
						deinterleavingFramebufferSpec.Attachments.Attachments = { 8,ImageFormat::R32F  }; // 8 images, with RED32F
						deinterleavingFramebufferSpec.ClearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
						deinterleavingFramebufferSpec.DebugName = "Deinterleaving";

						for (auto& attachment : deinterleavingFramebufferSpec.Attachments.Attachments)
							attachment.ExistingImage = image;


						Count<Shader> shader = Renderer::GetShader("Deinterleaving");

						GraphicsPipelineConfiguration pipelineConfig;
						pipelineConfig.DebugName = "Deinterleaving";
						pipelineConfig.WriteDepth = false;
						pipelineConfig.DepthTest = false;
						pipelineConfig.Shader = shader;
						pipelineConfig.VertexArray = quadVertexArray;
						pipelineConfig.Attachments.Attachments = { 8,ImageFormat::R32F }; // 8 images, with RED32F

						Count<GraphicsPipeline> pipeline = GraphicsPipeline::Create(pipelineConfig);
						

						// 2 frame buffers, 2 render passes .. 8 attachments each
						//for (int i = 0; i < HBAO_RANDOM_ELEMENTS; i += NUM_MRT)
						for (int i = 0; i < 2; i ++)
						{
							deinterleavingFramebufferSpec.ExistingImageLayers.clear();
							for (int layer = 0; layer < NUM_MRT; layer++)
								deinterleavingFramebufferSpec.ExistingImageLayers.emplace_back( i * 8 + layer);

							Count<FrameBuffer> frameBuffer = FrameBuffer::Create(deinterleavingFramebufferSpec);

							RenderPassConfig renderPassConfig;
							renderPassConfig.TargetFrameBuffer = frameBuffer;
							renderPassConfig.DebugName = fmt::format("Deinterleaving {}",i);
							renderPassConfig.Pipeline = pipeline;

							m_AmbientOcclusion.HBAO.DeinterleavePass[i] = RenderPass::Create(renderPassConfig);
							m_AmbientOcclusion.HBAO.DeinterleavePass[i]->AddGlobalInput(m_GlobalInputs);
						}
					}

					//Reinterleaving
					{
						FrameBufferConfig reinterleavingFramebufferSpec;
						reinterleavingFramebufferSpec.Attachments = { ImageFormat::RG16F };
						reinterleavingFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
						reinterleavingFramebufferSpec.DebugName = "Reinterleaving";

						Count<FrameBuffer> framebuffer = FrameBuffer::Create(reinterleavingFramebufferSpec);

						GraphicsPipelineConfiguration pipelineConfig;
						pipelineConfig.CullMode = CullMode::None;
						pipelineConfig.Shader = Renderer::GetShader("Reinterleaving");
						pipelineConfig.DepthTest = false;
						pipelineConfig.WriteDepth = false;
						pipelineConfig.VertexArray = quadVertexArray;
						pipelineConfig.Attachments = { ImageFormat::RG16F };

						Count<GraphicsPipeline> pipeline = GraphicsPipeline::Create(pipelineConfig);

						RenderPassConfig renderPassConfig;
						renderPassConfig.DebugName = "Reinterleaving";
						renderPassConfig.TargetFrameBuffer = framebuffer;
						renderPassConfig.Pipeline = pipeline;

						m_AmbientOcclusion.HBAO.ReinterleavePass = RenderPass::Create(renderPassConfig);



					}

					//Blur
					{
						auto shader = Renderer::GetShader("HBAOBlur");

						GraphicsPipelineConfiguration pipelineSpecification;
						pipelineSpecification.VertexArray = quadVertexArray;
						pipelineSpecification.CullMode = CullMode::None;
						pipelineSpecification.DepthTest = false;
						pipelineSpecification.WriteDepth = false;
						pipelineSpecification.Shader = shader;
						pipelineSpecification.DebugName = "HBAOBlur";
						pipelineSpecification.Attachments = { ImageFormat::RG16F };

						Count<GraphicsPipeline> pipeline = GraphicsPipeline::Create(pipelineSpecification);

						FrameBufferConfig hbaoBlurFramebufferSpec;
						hbaoBlurFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
						hbaoBlurFramebufferSpec.Attachments = { ImageFormat::RG16F };

						RenderPassConfig renderPassSpec;
						renderPassSpec.Pipeline = pipeline;
						
						{
							hbaoBlurFramebufferSpec.DebugName = "HBAOBlur0";
							auto frameBuffer = FrameBuffer::Create(hbaoBlurFramebufferSpec);

							renderPassSpec.DebugName = "HBAOBlur0";
							renderPassSpec.TargetFrameBuffer = frameBuffer;

							m_AmbientOcclusion.HBAO.BlurPass[0] = RenderPass::Create(renderPassSpec);
						}

						{
							hbaoBlurFramebufferSpec.DebugName = "HBAOBlur1";
							auto frameBuffer = FrameBuffer::Create(hbaoBlurFramebufferSpec);

							renderPassSpec.DebugName = "HBAOBlur1";
							renderPassSpec.TargetFrameBuffer = frameBuffer;

							m_AmbientOcclusion.HBAO.BlurPass[1] = RenderPass::Create(renderPassSpec);
						}

					}
					//HBAO Pass
					{
						ImageConfiguration imageSpec;
						imageSpec.Format = ImageFormat::RG16F;
						imageSpec.Usage = ImageUsage::Storage;
						imageSpec.ImageLayerViewsBaseType = ImageViewType::View2D;
						imageSpec.Layers = 16;
						imageSpec.DebugName = "HBAO-Output";
						Count<Image2D> image = Image2D::Create(imageSpec);

						m_AmbientOcclusion.HBAO.HBAOOutputImage = image;

						for (int i = 0; i < 16; i++)
							m_HBAOData.Float2Offsets[i] = glm::vec4((float)(i % 4) + 0.5f, (float)(i / 4) + 0.5f, 0.0f, 1.f);

						std::memcpy(m_HBAOData.Jitters, HBAOJitter().data(), sizeof( glm::vec4) * 16);

						m_AmbientOcclusion.HBAO.HBAOPass = ComputePass::Create(ComputePassConfiguration{ "HBAO",
							ComputePipeline::Create({"HBAO",Renderer::GetShader("HBAO")}) });

						m_AmbientOcclusion.HBAO.HBAOPass->SetInput("HBAOData", m_UBHBAOBuffer);
					}
				}
			}

			//ssr
			{
				//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/host/ffx_sssr.h
				/*
					typedef enum FfxSssrPass
				{
					FFX_SSSR_PASS_DEPTH_DOWNSAMPLE = 0,             ///< A pass which performs the hierarchical depth buffer generation
					FFX_SSSR_PASS_CLASSIFY_TILES = 1,               ///< A pass which classifies which pixels require screen space ray marching
					FFX_SSSR_PASS_PREPARE_BLUE_NOISE_TEXTURE = 2,   ///< A pass which generates an optimized blue noise texture
					FFX_SSSR_PASS_PREPARE_INDIRECT_ARGS = 3,        ///< A pass which generates the indirect arguments for the intersection pass.
					FFX_SSSR_PASS_INTERSECTION = 4,                 ///< A pass which performs the actual hierarchical depth ray marching.
					FFX_SSSR_PASS_COUNT
				} FfxSssrPass;
				*/
				//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/src/components/sssr/ffx_sssr.cpp
				//ssr			
				{
					ImageConfiguration imageSpec;
					imageSpec.Format = ImageFormat::RGBA16F;
					imageSpec.Usage = ImageUsage::Storage;
					imageSpec.DebugName = "SSR";
					Count<Image2D> image = Image2D::Create(imageSpec);
					m_SSR.SSRImage = image;

					auto ssrPipeline = ComputePipeline::Create({ "SSR",Renderer::GetShader("SSR") });
					m_SSR.SSRPass = ComputePass::Create({ "SSR",ssrPipeline });
					m_SSR.SSRPass->AddGlobalInput(m_GlobalInputs);
					//ssr Composite
					{
						GraphicsPipelineConfiguration pipelineConfig;
						pipelineConfig.Attachments.Attachments.emplace_back(ImageFormat::RGBA32F);
						pipelineConfig.DebugName = "SSRComposite";
						pipelineConfig.VertexArray = quadVertexArray;
						pipelineConfig.CullMode = CullMode::None;
						pipelineConfig.DepthTest = false;
						pipelineConfig.WriteDepth = false;
						pipelineConfig.BlendMode = BlendMode::SrcAlphaOneMinusSrcAlpha;
						pipelineConfig.Shader = Renderer::GetShader("SSRComposite");

						auto compositePipeline = GraphicsPipeline::Create(pipelineConfig);

						FrameBufferConfig framebufferSpec;
						framebufferSpec.DebugName = "SSRComposite";
						framebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
						framebufferSpec.Attachments.Attachments.emplace_back(ImageFormat::RGBA32F);
						framebufferSpec.Attachments.Attachments[0].ExistingImage = m_GeometryPass->GetTargetFrameBuffer()->GetOutput(0);
						framebufferSpec.ClearColorOnLoad = false;

						RenderPassConfig renderPassConfig;
						renderPassConfig.DebugName = "SSRComposite";
						renderPassConfig.Pipeline = compositePipeline;
						renderPassConfig.TargetFrameBuffer = FrameBuffer::Create(framebufferSpec);

						m_SSR.SSRCompositePass = RenderPass::Create(renderPassConfig);
					}
				}

				// NEW SSR
				{
					
					m_NewSSR.GlobalBuffer = Count<GlobalBufferSet>::Create();
					auto ssrGlobalBuffer = m_NewSSR.GlobalBuffer;
					ssrGlobalBuffer->SetData("SobolBuffer", Renderer::GetSPP1().SBSobolBuffer);
					ssrGlobalBuffer->SetData("RankingTileBuffer", Renderer::GetSPP1().SBRankingTileBuffer);
					ssrGlobalBuffer->SetData("ScramblingTileBuffer", Renderer::GetSPP1().SBScramblingTileBuffer);
					ssrGlobalBuffer->SetData("ScramblingTileBuffer", Renderer::GetSPP1().SBScramblingTileBuffer);
					ssrGlobalBuffer->SetData("u_SobolTextureBuffer", Renderer::GetSPP1().SobolTexture);
					ssrGlobalBuffer->SetData("u_ScramblingTileTextureBuffer", Renderer::GetSPP1().ScramblingTexture);
					//buffer
					{
						//https://github.com/qiutang98/flower/blob/main/source/engine/renderer/pass/sssr_pass.cpp
						//line 330

						const uint32_t maxRayCount = m_UBScreenData.FullResolution.x * m_UBScreenData.FullResolution.y; // Max case is one pixel one ray.
						const uint32_t maxDenoiseListCount = maxRayCount / (8 * 8) + 1; // Tile run in 8x8.

						m_NewSSR.SBRayCounter = StorageBufferSet::Create(sizeof(NewSSR::RayCounterData));
						m_NewSSR.SBRayList = StorageBufferSet::Create(maxRayCount * sizeof(uint32_t));
						m_NewSSR.SBDenoiseList = StorageBufferSet::Create(maxDenoiseListCount * sizeof(uint32_t));
						m_NewSSR.SBIntersectCommand = StorageBufferSet::Create(sizeof(NewSSR::GPUDispatchIndirectCommand));
						m_NewSSR.SBDenoiseCommand = StorageBufferSet::Create(sizeof(NewSSR::GPUDispatchIndirectCommand));
						m_NewSSR.SBGlobalAtomic = StorageBufferSet::Create(sizeof(uint32_t));

						ssrGlobalBuffer->SetData("SSSRRayCounterSSBO", m_NewSSR.SBRayCounter);
						ssrGlobalBuffer->SetData("SSSRRayListSSBO", m_NewSSR.SBRayList);
						ssrGlobalBuffer->SetData("SSSRDenoiseTileListSSBO", m_NewSSR.SBDenoiseList);
						ssrGlobalBuffer->SetData("SSSRIntersectCmdSSBO", m_NewSSR.SBIntersectCommand);
						ssrGlobalBuffer->SetData("SSSRDenoiseCmdSSBO", m_NewSSR.SBDenoiseCommand);
						ssrGlobalBuffer->SetData("SSSRSPDGlobalAtomic", m_NewSSR.SBGlobalAtomic);


					}
					//textures
					{
						//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/src/components/sssr/ffx_sssr.cpp
						//line 270

						ssrGlobalBuffer->SetData("u_DepthMap", m_PreDepthPass->GetOutput(0));
						ssrGlobalBuffer->SetData("u_NormalMap", m_GeometryPass->GetOutput(1));
						//HierarchalDepthDownSampler
						{
							TextureConfiguration imageConfig;
							imageConfig.DebugName = "SSSRHierarchalDepthDownSampler";
							imageConfig.Format = ImageFormat::R32F;
							imageConfig.Width = 100;
							imageConfig.Height = 100;
							imageConfig.GenerateMips = true;
							imageConfig.Wrap = TextureWrap::ClampEdge;
							imageConfig.Filter = TextureFilter::Nearest;
							imageConfig.Storage = true;
							m_NewSSR.HierarchalDepthDownSamplerTexture = Texture2D::Create(imageConfig);
							ssrGlobalBuffer->SetData("u_HZBMap", m_NewSSR.HierarchalDepthDownSamplerTexture);

						}
						//radiance
						{
							ImageConfiguration imageConfig;
							imageConfig.DebugName = "SSSRRadiance0";
							imageConfig.Width = 1;
							imageConfig.Height = 1;
							imageConfig.Format = ImageFormat::RGBA16F;
							imageConfig.Usage = ImageUsage::Storage;

							m_NewSSR.Radiance = Image2D::Create(imageConfig);
							imageConfig.DebugName = "SSSRRadiance1";
							m_NewSSR.PreviousRadiance = Image2D::Create(imageConfig);
						}
						//extract roughness
						{
							ImageConfiguration imageConfig;
							imageConfig.DebugName = "SSSRExtractRoughness0";
							imageConfig.Width = 1;
							imageConfig.Height = 1;
							imageConfig.Format = ImageFormat::R;
							imageConfig.Usage = ImageUsage::Storage;

							m_NewSSR.ExtractRoughness = Image2D::Create(imageConfig);
							imageConfig.DebugName = "SSSRExtractRoughness1";
							m_NewSSR.PreviousExtractRoughness = Image2D::Create(imageConfig);
						}

						//blue noise
						{

							ImageConfiguration imageConfig;
							imageConfig.DebugName = "SSSR BlueNoise";
							imageConfig.Format = ImageFormat::RG;
							imageConfig.Width = 128;
							imageConfig.Height = 128;
							imageConfig.Usage = ImageUsage::Storage;
							m_NewSSR.BlueNoiseImage = Image2D::Create(imageConfig);
							ssrGlobalBuffer->SetData("u_BlueNoiseTexture", m_NewSSR.BlueNoiseImage);

						}

						///sample count
						{
							ImageConfiguration imageConfig;
							imageConfig.DebugName = "SSSRSampleCount0";
							imageConfig.Format = ImageFormat::R16F;
							imageConfig.Width = 1;
							imageConfig.Height = 1;
							imageConfig.Usage = ImageUsage::Storage;
							m_NewSSR.SampleCount = Image2D::Create(imageConfig);
							imageConfig.DebugName = "SSSRSampleCount1";
							m_NewSSR.PreviousSampleCount = Image2D::Create(imageConfig);
						}
						//reproject image
						{
							ImageConfiguration imageConfig;
							imageConfig.DebugName = "SSSRReproject0";
							imageConfig.Format = ImageFormat::RGBA16F;
							imageConfig.Width = 1;
							imageConfig.Height = 1;
							imageConfig.Usage = ImageUsage::Storage;
							m_NewSSR.ReprojectImage = Image2D::Create(imageConfig);
							imageConfig.DebugName = "SSSRReproject1";
							m_NewSSR.PreviousReprojectImage = Image2D::Create(imageConfig);
						}
						//average radiance
						{
							ImageConfiguration imageConfig;
							imageConfig.DebugName = "SSSR AverageRadiance";
							imageConfig.Format = ImageFormat::B10G11R11_UF_PACK32;
							imageConfig.Width = 1;
							imageConfig.Height = 1;
							imageConfig.Usage = ImageUsage::Storage;
							m_NewSSR.AverageRadiance = Image2D::Create(imageConfig);
						}
					}
					////hzb
					//{
					//
					//	TextureConfiguration config;
					//	config.DebugName = "SSSRHZB";
					//	config.Format = ImageFormat::R32F;
					//	config.Width = 1;
					//	config.Height = 1;
					//	config.Wrap = TextureWrap::ClampEdge;
					//	config.Filter = TextureFilter::Nearest;
					//	config.GenerateMips = true;
					//	config.Storage = true;
					//
					//	m_NewSSR.HZBTexture = Texture2D::Create(config);
					//}
					// 

					//HierarchalDepthDownSampler
					{
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRHZB";
						pipelineConfig.Shader = Renderer::GetShader("SSSRHZB");
						m_NewSSR.HierarchalDepthDownSamplerPass = ComputePass::Create(ComputePassConfiguration{ "SSSRHZB",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.HierarchalDepthDownSamplerPass->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.HierarchalDepthDownSamplerPass->AddGlobalInput(ssrGlobalBuffer);
					}
					//tile classification
					{
						ImageConfiguration imageConfig;
						imageConfig.DebugName = "SSSRVariance0";
						imageConfig.Width = 1;
						imageConfig.Height = 1;
						imageConfig.Format = ImageFormat::R16F;
						imageConfig.Usage = ImageUsage::Storage;

						m_NewSSR.Variance = Image2D::Create(imageConfig);
						imageConfig.DebugName = "SSSRVariance1";
						m_NewSSR.PreviousVariance = Image2D::Create(imageConfig);
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRTileClassification";
						pipelineConfig.Shader = Renderer::GetShader("SSSRTileClassification");
						m_NewSSR.TileClassification = ComputePass::Create(ComputePassConfiguration{ "SSSRTileClassification",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.TileClassification->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.TileClassification->AddGlobalInput(ssrGlobalBuffer);

					}
					// blue noise texture 
					{
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRBlueNoiseTextureGeneration";
						pipelineConfig.Shader = Renderer::GetShader("SSSRBlueNoiseTextureGeneration");
						m_NewSSR.BlueNoisePass = ComputePass::Create(ComputePassConfiguration{ "SSSRBlueNoiseTextureGeneration",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.BlueNoisePass->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.BlueNoisePass->AddGlobalInput(ssrGlobalBuffer);
					}
					//intersect args 
					{
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRIntersectArgs";
						pipelineConfig.Shader = Renderer::GetShader("SSSRIntersectArgs");
						m_NewSSR.IntersectArgs = ComputePass::Create(ComputePassConfiguration{ "SSSRIntersectArgs",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.IntersectArgs->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.IntersectArgs->AddGlobalInput(ssrGlobalBuffer);
					}
					//interserct
					{

						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRIntersect";
						pipelineConfig.Shader = Renderer::GetShader("SSSRIntersect");
						m_NewSSR.Intersect = ComputePass::Create(ComputePassConfiguration{ "SSSRIntersect",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.Intersect->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.Intersect->AddGlobalInput(ssrGlobalBuffer);
					}
					//reproject
					{
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSRReproject";
						pipelineConfig.Shader = Renderer::GetShader("SSRReproject");
						m_NewSSR.Reproject = ComputePass::Create(ComputePassConfiguration{ "SSRReproject",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.Reproject->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.Reproject->AddGlobalInput(ssrGlobalBuffer);
					}
					//prefilter
					{
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRPrefilter";
						pipelineConfig.Shader = Renderer::GetShader("SSSRPrefilter");
						m_NewSSR.Prefilter = ComputePass::Create(ComputePassConfiguration{ "SSSRPrefilter",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.Prefilter->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.Prefilter->AddGlobalInput(ssrGlobalBuffer);
					}

					//temporal
					{
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRTemporal";
						pipelineConfig.Shader = Renderer::GetShader("SSSRTemporal");
						m_NewSSR.Temporal = ComputePass::Create(ComputePassConfiguration{ "SSSRTemporal",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.Temporal->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.Temporal->AddGlobalInput(ssrGlobalBuffer);
					}

					//apply
					{
						ComputePipelineConfig pipelineConfig;
						pipelineConfig.DebugName = "SSSRApply";
						pipelineConfig.Shader = Renderer::GetShader("SSSRApply");
						m_NewSSR.ApplyPass = ComputePass::Create(ComputePassConfiguration{ "SSSRApply",ComputePipeline::Create(pipelineConfig) });
						m_NewSSR.ApplyPass->AddGlobalInput(m_GlobalInputs);
						m_NewSSR.ApplyPass->AddGlobalInput(ssrGlobalBuffer);
					}
				}
				//HZB
				{
					
					TextureConfiguration config;
					config.DebugName = "HierarchicalZ";
					config.Format = ImageFormat::R32F;
					config.Width = 1;
					config.Height = 1;
					config.Wrap = TextureWrap::ClampEdge;
					config.Filter = TextureFilter::Nearest;
					config.GenerateMips = true;
					config.Storage = true;

					m_SSR.HierarchicalDepthTexture = Texture2D::Create(config);

					auto computePipeline = ComputePipeline::Create({ "HierarchicalZ",Renderer::GetShader("HZB") });
					m_SSR.HierarchicalDepthPass = ComputePass::Create({ "HierarchicalZ",computePipeline });
				}

				// Pre-Integration
				{
					TextureConfiguration config;
					config.Format = ImageFormat::R;
					config.Width = 1;
					config.Height = 1;
					config.DebugName = "PreIntegration";
					config.GenerateMips = true;
					config.Storage = true;

					m_SSR.VisibilityTexture = Texture2D::Create(config);
					
					auto computePipeline = ComputePipeline::Create({ "PreIntegration",Renderer::GetShader("PreIntegration") });
					m_SSR.PreIntegrationPass = ComputePass::Create({ "PreIntegration",computePipeline });
					m_SSR.PreIntegrationPass->AddGlobalInput(m_GlobalInputs);
				}

				//PreConvolution
				{
					TextureConfiguration spec;
					spec.Format = ImageFormat::RGBA32F;
					spec.Width = 1;
					spec.Height = 1;
					spec.Wrap = TextureWrap::ClampEdge;
					spec.DebugName = "Pre-Convoluted";
					spec.GenerateMips = true;
					spec.Storage = true;

					m_SSR.PreConvolutedTexture = Texture2D::Create(spec);

					auto computePipeline = ComputePipeline::Create({ "PreConvoulution",Renderer::GetShader("PreConvoulution") });
					m_SSR.PreConvolutePass = ComputePass::Create({ "PreConvoulution",computePipeline });

				}

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
				
			}
#endif
			//bloom
			{
				m_BloomComputePass = ComputePass::Create({ "Bloom",ComputePipeline::Create({"Bloom",Renderer::GetShader("Bloom")}) });

				TextureConfiguration spec;
				spec.Format = ImageFormat::RGBA32F;
				spec.Width = 1;
				spec.Height = 1;
				spec.Wrap = TextureWrap::ClampEdge;
				spec.Storage = true;
				spec.GenerateMips = true;
				spec.DebugName = "BloomCompute-0";
				m_BloomComputeTextures[0] = Texture2D::Create(spec);
				spec.DebugName = "BloomCompute-1";
				m_BloomComputeTextures[1] = Texture2D::Create(spec);
				spec.DebugName = "BloomCompute-2";
				m_BloomComputeTextures[2] = Texture2D::Create(spec);


				//glm::uvec2 bloomSize = (viewportSize + 1u) / 2u;
				//bloomSize += BLOOM_COMPUTE_WORK_GROUP_SIZE - bloomSize % BLOOM_COMPUTE_WORK_GROUP_SIZE;
				//m_BloomComputeTextures[0]->Resize(bloomSize.x, bloomSize.y);
				//m_BloomComputeTextures[1]->Resize(bloomSize.x, bloomSize.y);
				//m_BloomComputeTextures[2]->Resize(bloomSize.x, bloomSize.y);
			}

			//DOF
			{
				m_DOFPass = ComputePass::Create({ "Depth of Field",ComputePipeline::Create({"Depth of Field",Renderer::GetShader("DOF")}) });

				TextureConfiguration config;
				config.DebugName = "DOF";
				config.Format = ImageFormat::RGBA32F;
				config.Width = 1;
				config.Height = 1;
				config.Storage = true;

				m_DOFTexture = Texture2D::Create(config);

				m_DOFPass->SetInput("u_DepthTexture", m_PreDepthPass->GetOutput(0));
				m_DOFPass->SetInput("u_Texture", m_GeometryPass->GetOutput(0));
				m_DOFPass->SetInput("o_Image", m_DOFTexture);
				m_DOFPass->AddGlobalInput(m_GlobalInputs);
			}

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
		Renderer::Submit([instance = Count<WorldRenderer>(this)]() mutable { instance->m_ResourcesCreatedGPU = true; });
		PF_ENGINE_TRACE("World Renderer Inititilized");
	}

	void WorldRenderer::SetContext(Count<class World> world)
	{
		PF_CORE_ASSERT(!m_InContext, "Can't change world while rendering");
		m_ActiveWorld = world;

	}
	static inline uint32_t GetJitterPhaseCount(uint32_t renderWidth, uint32_t displayWidth)
	{
		const float basePhaseCount = 8.0f;
		const uint32_t jitterPhaseCount = uint32_t(basePhaseCount * pow((float(displayWidth) / renderWidth), 2.0f));
		return jitterPhaseCount;
	}
	// halton sequence compute.
	static inline float Halton(uint64_t index, uint64_t base)
	{
		float f = 1; float r = 0;
		while (index > 0)
		{
			f = f / static_cast<float>(base);
			r = r + f * (index % base);
			index = index / base;
		}
		return r;
	}
	void WorldRenderer::BeginScene(const WorldRendererCamera& camera, const glm::vec3& location)
	{
		PF_PROFILE_FUNC();
		PF_CORE_ASSERT(!m_InContext);
		PF_CORE_ASSERT(m_ActiveWorld);
		PF_PROFILE_TAG("Renderer", m_ActiveWorld->GetName().c_str());
		if (m_ResourcesCreatedGPU)
			m_ResourcesCreated = true;

		if (!m_ResourcesCreated)
			return;

		m_InContext = true;
		static bool isFlip = false;
		if (!isFlip)
		{
			m_CurTransformMap = &m_MeshTransformMap[0];
			m_PrevTransformMap = &m_MeshTransformMap[1];
		}
		else
		{
			m_CurTransformMap = &m_MeshTransformMap[1];
			m_PrevTransformMap = &m_MeshTransformMap[0];
		}
		isFlip = !isFlip;
		m_CurTransformMap->clear();

		//reset stats
		m_Stats = {};

		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		//uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex;
		if (m_NeedResize)
		{
			m_NeedResize = false;
			const glm::uvec2 viewportSize = m_UBScreenData.FullResolution;
			{


				glm::uvec2 size = viewportSize;
				size += TILE_SIZE - viewportSize % TILE_SIZE;
				m_LightCullingWorkGroups = { size / (uint32_t)TILE_SIZE, 1u };

				for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
				{
					m_SBVisiblePointLightIndicesBuffer->Resize(i, m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(int) * MAX_NUM_LIGHTS_PER_TILE);
					m_SBVisibleSpotLightIndicesBuffer->Resize(i, m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * sizeof(int) * MAX_NUM_LIGHTS_PER_TILE);
				}

			}
			//bloom
			{
				glm::uvec2 bloomSize = (viewportSize + 1u) / 2u;
				bloomSize += BLOOM_COMPUTE_WORK_GROUP_SIZE - bloomSize % BLOOM_COMPUTE_WORK_GROUP_SIZE;
				m_BloomComputeTextures[0]->Resize(bloomSize.x, bloomSize.y);
				m_BloomComputeTextures[1]->Resize(bloomSize.x, bloomSize.y);
				m_BloomComputeTextures[2]->Resize(bloomSize.x, bloomSize.y);
			}
			//DOF 
			{

				glm::uvec2 size = (viewportSize + 1u) / 2u;
				size += DOF_NUM_THREADS - size % DOF_NUM_THREADS;
				m_DOFTexture->Resize(size.x, size.y);
			}

			//ssr
			{

				//ssr
				{
					constexpr uint32_t WORK_GROUP_SIZE = 8u;
					glm::uvec2 ssrSize = SSRSettings.HalfRes ? (viewportSize + 1u) / 2u : viewportSize;
					m_SSR.SSRImage->Resize(ssrSize.x, ssrSize.y);
					m_SSR.PreConvolutedTexture->Resize(ssrSize.x, ssrSize.y);
					ssrSize += WORK_GROUP_SIZE - ssrSize % WORK_GROUP_SIZE;
					m_SSR.WorkGroups.x = ssrSize.x / WORK_GROUP_SIZE;
					m_SSR.WorkGroups.y = ssrSize.y / WORK_GROUP_SIZE;

					m_SSR.VisibilityTexture->Resize(viewportSize.x, viewportSize.y);
					m_SSR.SSRCompositePass->GetTargetFrameBuffer()->Resize(viewportSize);

				}

				//HZB
				{

					const glm::uvec2 numMips = glm::ceil(glm::log2(glm::vec2(viewportSize)));
					m_UBSSR.NumDepthMips = glm::max(numMips.x, numMips.y);

					const glm::uvec2 hzbSize = 1u << numMips;
					m_SSR.HierarchicalDepthTexture->Resize(hzbSize.x, hzbSize.y);

					const glm::vec2 hzbUVFactor = { (glm::vec2)viewportSize / (glm::vec2)hzbSize };
					m_UBSSR.HZBUvFactor = hzbUVFactor;
				}
			
			}

			//new ssr
			{
				const uint32_t maxRayCount = viewportSize.x * viewportSize.y; // Max case is one pixel one ray.
				const uint32_t maxDenoiseListCount = maxRayCount / (8 * 8) + 1; // Tile run in 8x8.


				for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
				{
					m_NewSSR.SBRayList->Resize(i,maxRayCount * sizeof(uint32_t));
					m_NewSSR.SBDenoiseList->Resize(i, maxDenoiseListCount * sizeof(uint32_t));
				}
				m_NewSSR.Variance->Resize(viewportSize.x, viewportSize.y);
				m_NewSSR.PreviousVariance->Resize(viewportSize.x, viewportSize.y);

				m_NewSSR.Radiance->Resize(viewportSize.x, viewportSize.y);
				m_NewSSR.PreviousRadiance->Resize(viewportSize.x, viewportSize.y);
			

				m_NewSSR.ExtractRoughness->Resize(viewportSize.x, viewportSize.y);
				m_NewSSR.PreviousExtractRoughness->Resize(viewportSize.x, viewportSize.y);

				m_NewSSR.SampleCount->Resize(viewportSize.x, viewportSize.y);
				m_NewSSR.PreviousSampleCount->Resize(viewportSize.x, viewportSize.y);

				m_NewSSR.ReprojectImage->Resize(viewportSize.x, viewportSize.y);
				m_NewSSR.PreviousReprojectImage->Resize(viewportSize.x, viewportSize.y);

				m_NewSSR.AverageRadiance->Resize(DivideRoundingUp(viewportSize.x,8u), DivideRoundingUp(viewportSize.y, 8u));

				m_NewSSR.HierarchalDepthDownSamplerTexture->Resize(viewportSize.x, viewportSize.y);

				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.Variance);
				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.PreviousVariance);

				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.Radiance);
				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.PreviousRadiance);
					
				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.ExtractRoughness);
				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.PreviousExtractRoughness);

				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.SampleCount);
				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.PreviousSampleCount);

				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.ReprojectImage);
				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.PreviousReprojectImage);

				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.AverageRadiance);

				Renderer::ClearImage(Renderer::GetRendererCommandBuffer(), m_NewSSR.HierarchalDepthDownSamplerTexture->GetImage());
			}

			//HBAO
			{
				m_AmbientOcclusion.HBAO.ReinterleavePass->GetTargetFrameBuffer()->Resize(viewportSize);
				m_AmbientOcclusion.HBAO.BlurPass[0]->GetTargetFrameBuffer()->Resize(viewportSize);
				m_AmbientOcclusion.HBAO.BlurPass[1]->GetTargetFrameBuffer()->Resize(viewportSize);
				m_AmbientOcclusionCompositePass->GetTargetFrameBuffer()->Resize(viewportSize);

				glm::uvec2 quarterSize = (viewportSize + 3u) / 4u;

				m_AmbientOcclusion.HBAO.DeinterleavePass[0]->GetTargetFrameBuffer()->Resize(quarterSize);
				m_AmbientOcclusion.HBAO.DeinterleavePass[1]->GetTargetFrameBuffer()->Resize(quarterSize);

				m_AmbientOcclusion.HBAO.HBAOOutputImage->Resize(quarterSize.x, quarterSize.y);
				m_AmbientOcclusion.HBAO.HBAOOutputImage->CreateMipAndLayerViews();

				quarterSize += HBAO_WORK_GROUP_SIZE - quarterSize % HBAO_WORK_GROUP_SIZE;
				m_AmbientOcclusion.HBAO.WorkGroups.x = quarterSize.x / 16u;
				m_AmbientOcclusion.HBAO.WorkGroups.y = quarterSize.y / 16u;
				m_AmbientOcclusion.HBAO.WorkGroups.z = 16;
			}
			// predepth
			m_PreDepthPass->GetTargetFrameBuffer()->Resize(viewportSize);
			m_PrevDepthImage->Resize(viewportSize.x, viewportSize.y);
			m_PrevNormalImage->Resize(viewportSize.x, viewportSize.y);

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
		//frameBuffer
		{
			m_UBFrameData.FrameCount = FrameTime::GetFrameCount();
			m_UBFrameData.AppTimeSeconds = FrameTime::GetTime();
			m_UBFrameBuffer->SetData(frameIndex, Buffer(&m_UBFrameData, sizeof(m_UBFrameData)));
		}
		// camera buffer
		{
			m_UBCameraData.PrevJitterData = m_UBCameraData.JitterData;
			const bool bEnableCameraJitter = true;
			if (bEnableCameraJitter)
			{
				m_UBCameraData.JitterPeriod = GetJitterPhaseCount(m_UBScreenData.FullResolution.x, m_UBScreenData.FullResolution.x);

				// halton23 sequence
				m_UBCameraData.JitterData.x = Halton((FrameTime::GetFrameCount() % m_UBCameraData.JitterPeriod) + 1, 2) - 0.5f;
				m_UBCameraData.JitterData.y = Halton((FrameTime::GetFrameCount() % m_UBCameraData.JitterPeriod) + 1, 3) - 0.5f;

				m_UBCameraData.bEnableJitter = true;

			}
			else
			{
				m_UBCameraData.JitterData.x = 0.0f;
				m_UBCameraData.JitterData.y = 0.0f;

				m_UBCameraData.JitterPeriod = 1;
				m_UBCameraData.bEnableJitter = false;

			}
			m_UBCameraData.PrevViewProjection = m_UBCameraData.ViewProjection;

			m_UBCameraData.Projection = camera.Camera.GetProjectionMatrix();
			m_UBCameraData.InverseProjection = glm::inverse(m_UBCameraData.Projection);
			m_UBCameraData.UnreversedProjectionMatrix = camera.Camera.GetUnReversedProjectionMatrix();
			m_UBCameraData.View = camera.Camera.GetViewMatrix();
			m_UBCameraData.InverseView = glm::inverse(m_UBCameraData.View);
			m_UBCameraData.ViewProjection = m_UBCameraData.Projection * m_UBCameraData.View;
			m_UBCameraData.InverseViewProjection = m_UBCameraData.InverseProjection * m_UBCameraData.InverseView;
			m_UBCameraData.Position = location;
			m_UBCameraData.NearPlane = camera.NearPlane;
			m_UBCameraData.FarPlane = camera.FarPlane;
			m_UBCameraData.Fov = camera.Fov;
			float depthLinearizeMul = -(camera.FarPlane * camera.NearPlane) / (camera.FarPlane - camera.NearPlane);
			float depthLinearizeAdd = camera.FarPlane / (camera.FarPlane - camera.NearPlane);
			m_UBCameraData.DepthUnpackConsts = { depthLinearizeMul, depthLinearizeAdd };
			const float* P = glm::value_ptr(camera.Camera.GetProjectionMatrix());
			const glm::vec4 projInfoPerspective = {
					 2.0f / (P[4 * 0 + 0]),                  // (x) * (R - L)/N
					 2.0f / (P[4 * 1 + 1]),                  // (y) * (T - B)/N
					-(1.0f - P[4 * 2 + 0]) / P[4 * 0 + 0],  // L/N
					-(1.0f + P[4 * 2 + 1]) / P[4 * 1 + 1],  // B/N
			};


			m_UBCameraData.NDCToViewMul = { projInfoPerspective[0], projInfoPerspective[1] };
			m_UBCameraData.NDCToViewAdd = { projInfoPerspective[2], projInfoPerspective[3] };
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

//Renderer::BeginRenderPass(m_CommandBuffer,m_Dep, true);
//		Renderer::EndRenderPass(m_GeometryPass);
		
		if (m_ResourcesCreated && m_UBScreenData.FullResolution.x > 0 && m_UBScreenData.FullResolution.y > 0)
		{
			Renderer::BeginCommandBuffer(m_CommandBuffer);

			SetPasses();
			ShadowPass();
			PreDepthPass();
			HZBPass();
			//PreIntegrationPass();
			LightFrustrumAndCullingPass();
			GeometryPass();
			CompositePass();

			Renderer::EndCommandBuffer(m_CommandBuffer);
			Renderer::SubmitCommandBuffer(m_CommandBuffer);
		}
		else
		{
			Renderer::BeginCommandBuffer(m_CommandBuffer);

			Renderer::BeginRenderPass(m_CommandBuffer, m_PreDepthPass, true);
			Renderer::EndRenderPass(m_PreDepthPass);

			Renderer::BeginRenderPass(m_CommandBuffer, m_CompositePass, true);
			Renderer::EndRenderPass(m_CompositePass);

			Renderer::EndCommandBuffer(m_CommandBuffer);
			Renderer::SubmitCommandBuffer(m_CommandBuffer);

		}
		// clear data
		//mesh Pass
		{
			//m_MeshTransformMap.clear();

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
		//can only have a minum of 100 as the size of viewpoart
		if (width <= 100)
			width = 100;

		if (height <= 100)
			height = 100;

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
		auto image = m_CompositePass->GetTargetFrameBuffer()->GetOutput(0).As<Image2D>();
		if (image == nullptr)
			return Renderer::GetBlackTexture()->GetImage();

		return image;
	}

	void WorldRenderer::SetPasses()
	{
		PF_PROFILE_FUNC();

		Timer setPassesTimer;
		uint32_t frameIndex = Renderer::GetCurrentFrameInFlight();
		//scene data
		{
			m_UBSceneData.CameraPosition =m_UBCameraData.Position;
			m_UBSceneData.bShowLightGrid = DebugOptions.LightDebugOptions.ShowLightGrid;
			m_UBSceneDataBuffer->SetData(frameIndex, Buffer(&m_UBSceneData, sizeof(m_UBSceneData)));
		}
		// set up shadow pass
		{
			// NOTE not everyting is set here because we are not sure yet about if they are ture
			m_UBRenderData.bShowCascades = DebugOptions.ShadowDebugOptions.ShowCascades;
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
					m_Environment = Count<Environment>::Create(EnvironmentTextureData());
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
			for (auto& [key, transformData] : *m_CurTransformMap)
			{
				transformData.TransformOffset = offset * sizeof(TransformVertexData);
				auto& prevTransformData = (*m_PrevTransformMap)[key];

				uint32_t transformIndex = 0;
				for (const auto& transform : transformData.Transforms)
				{
					PF_CORE_ASSERT(offset <= submeshTransformSize, "Need to resize SubMeshTransforms to small");
					m_SubmeshTransformBuffers[frameIndex].Data[offset] = transform;
					offset++;

					//prev model matrix

					PF_CORE_ASSERT(offset <= submeshTransformSize, "Need to resize SubMeshTransforms to small");
					if(prevTransformData.Transforms.size() > transformIndex)
					m_SubmeshTransformBuffers[frameIndex].Data[offset] = prevTransformData.Transforms[transformIndex];
					else
						m_SubmeshTransformBuffers[frameIndex].Data[offset] = transform;

					offset++;
					transformIndex++;
				}

			}
			m_SubmeshTransformBuffers[frameIndex].Buffer->SetData(m_SubmeshTransformBuffers[frameIndex].Data, offset * sizeof(TransformVertexData));
		}
		Renderer::CopyImage(m_CommandBuffer, m_GeometryPass->GetOutput(1).As<Image2D>(), m_PrevNormalImage);

		//very important because its not cleared and images are recreating for the limunance
		// it affects the HBAO so much
		Renderer::ClearImage(m_CommandBuffer, m_GeometryPass->GetOutput(1).As<Image2D>());
		Renderer::ClearImage(m_CommandBuffer, m_GeometryPass->GetOutput(2).As<Image2D>()); //clear metallnessRougness
		Renderer::ClearImage(m_CommandBuffer, m_GeometryPass->GetOutput(3).As<Image2D>()); //clear velocity
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
				const auto& transformData = m_CurTransformMap->at(meshKey);
				uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
				RenderMesh(m_CommandBuffer, dc.Mesh,cascadePass,m_SubmeshTransformBuffers[frameIndex].Buffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount, Buffer(&cascade,sizeof(uint32_t)),"u_CascadeInfo");
			}
			for (auto& [meshKey, dc] : m_DynamicMeshShadowDrawList)
			{
				const auto& transformData = m_CurTransformMap->at(meshKey);
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
		Renderer::CopyImage(m_CommandBuffer, m_PreDepthPass->GetOutput(0).As<Image2D>(), m_PrevDepthImage);
		Renderer::BeginRenderPass(m_CommandBuffer, m_PreDepthPass, true);

		for (auto& [meshKey, dc] : m_MeshDrawList)
		{
			const auto& transformData = m_CurTransformMap->at(meshKey);
			uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
			RenderMesh(m_CommandBuffer, dc.Mesh, m_PreDepthPass, m_SubmeshTransformBuffers[frameIndex].Buffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount);
		}
		for (auto& [meshKey, dc] : m_DynamicMeshDrawList)
		{
			const auto& transformData = m_CurTransformMap->at(meshKey);
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
	//set
		{
		}

		{
			PF_PROFILE_FUNC("GeometryPass::SkyBoxPass");

			Timer timer;
			m_SkyBoxPass->SetInput("u_EnvironmentMap", m_Environment->GetPrefilterMap());

			Renderer::BeginRenderPass(m_CommandBuffer, m_SkyBoxPass);
			Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_SkyBoxPass);
			Renderer::EndRenderPass(m_SkyBoxPass);

			m_Timers.GeometrySkyBoxPass = timer.ElapsedMillis();
		}

		{
			PF_PROFILE_FUNC("GeometryPass::MeshPass");

			Timer timer;
			m_GeometryPass->SetInput("u_IrradianceMap", m_Environment->GetPrefilterMap());
			m_GeometryPass->SetInput("u_PrefilterMap", m_Environment->GetPrefilterMap());
			Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_GeometryPass);

			{
				PF_PROFILE_FUNC("GeometryPass::Static");

				for (auto& [meshKey, dc] : m_MeshDrawList)
				{
					const auto& transformData = m_CurTransformMap->at(meshKey);
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
		{

			//PreConvolutePass();
			//SSRPass();
			NewSSRPass();
		}
		BloomPass();
		//DOFPass();

		// this has to be the last thgn called
		{
			PF_PROFILE_FUNC("WorldRenderer::WorldComposite");

			auto blackTexture = Renderer::GetBlackTexture();
			Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, m_CompositePass, true);
			//float exposure = m_SceneData.SceneCamera.Camera.GetExposure();
			auto inputImage = m_GeometryPass->GetOutput(0);
			m_CompositeMaterial->Set("u_WorldTexture", inputImage);

			m_CompositeMaterial->Set("u_BloomTexture", m_BloomComputeTextures[2]);

			if (BloomSettings.Enabled)
			{
				m_CompositeMaterial->Set("u_Uniforms.BloomIntensity", BloomSettings.Intensity);
				m_CompositeMaterial->Set("u_BloomTexture", m_BloomComputeTextures[2]);
			}
			else
			{
				m_CompositeMaterial->Set("u_Uniforms.BloomIntensity", 0.0f);
				m_CompositeMaterial->Set("u_BloomTexture", blackTexture);
			}

			if(DOFSettings.Enabled)
				m_CompositeMaterial->Set("u_DOFTexture", m_DOFTexture);
			else
				m_CompositeMaterial->Set("u_DOFTexture", blackTexture);


			Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_CompositePass, m_CompositeMaterial);
			Renderer::EndRenderPass(m_CompositePass);
		}

		if (DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders != WorldRendererDebugOptions::PhysicsColliderView::None)
		{
			PF_PROFILE_FUNC("CompositePass::PhysicsDebugMeshes");
			Timer physicsDebugMesh;

			m_GeometryWireFramePassMaterial->GetVector4("u_MaterialUniform.Color") = DebugOptions.PhysicsDebugOptions.PhysicsColliderColor;

			auto transformBuffer = m_SubmeshTransformBuffers[frameIndex].Buffer;

			auto wireFramePass = DebugOptions.PhysicsDebugOptions.ShowPhysicsColliders == WorldRendererDebugOptions::PhysicsColliderView::Normal ? m_GeometryWireFramePass : m_GeometryWireFrameOnTopPass;
			Renderer::BeginRenderMaterialRenderPass(m_CommandBuffer, wireFramePass);
			for (auto& [meshKey, dc] : m_ColliderDrawList)
			{
				const auto& transformData = m_CurTransformMap->at(meshKey);
				uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
				RenderMeshWithMaterial(m_CommandBuffer, dc.Mesh, m_GeometryWireFramePassMaterial,
					wireFramePass,
					transformBuffer, dc.SubMeshIndex, transformOffset, dc.InstanceCount);
			}

			for (auto& [meshKey, dc] : m_DynamicColliderDrawList)
			{
				const auto& transformData = m_CurTransformMap->at(meshKey);
				uint32_t transformOffset = transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData);
				RenderDynamicMeshWithMaterial(m_CommandBuffer, dc.Mesh, m_GeometryWireFramePassMaterial,
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
		if (!AmbientOcclusionSettings.Enabled)
			return;

		PF_PROFILE_FUNC();

		if (AmbientOcclusionSettings.Type == AmbientOcclusion::AmbientOcclusionType::HBAO)
		{
			HBAOPass();
			m_AmbientOcclusionCompositePass->SetInput("u_InputAOTexture", m_AmbientOcclusion.HBAO.BlurPass[1]->GetOutput(0));
		}

#if 0
		if (AmbientOcclusionSettings.Type == AmbientOcclusion::AmbientOcclusionType::SSAO)
		{
			
			auto& ssao = AmbientOcclusionSettings.SSAO;

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

			
		}
#endif
		{
			PF_PROFILE_FUNC("AOComposite");
			Renderer::BeginRenderPass(m_CommandBuffer, m_AmbientOcclusionCompositePass);

			if (AmbientOcclusionSettings.Type == AmbientOcclusion::AmbientOcclusionType::HBAO)
			{
				int data = 0;
				m_AmbientOcclusionCompositePass->PushData("u_PushData", &data);
			}
			Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_AmbientOcclusionCompositePass);
			Renderer::EndRenderPass(m_AmbientOcclusionCompositePass);
		}
		
	}

	void WorldRenderer::HBAOPass()
	{
		if (!AmbientOcclusionSettings.Enabled)
			return;

		PF_PROFILE_FUNC();

		{
			PF_PROFILE_FUNC("SetHBAOPass");
			//https://github.com/nvpro-samples/gl_ssao/blob/f6b010dc7a05346518cd13d3368d8d830a382ed9/ssao.cpp#L705
			//UPDATA HBAODATA
			// radius
			const float meters2viewSpace = 1.0f;
			const float R = AmbientOcclusionSettings.HBAO.Radius * meters2viewSpace;
			const float R2 = R * R;
			m_HBAOData.NegInvR2 = -1.0f / R2;
			m_HBAOData.InvQuarterResolution = 1.f / glm::vec2{ (float)m_UBScreenData.FullResolution.x / 4, (float)m_UBScreenData.FullResolution.y / 4 };
			m_HBAOData.RadiusToScreen = R * 0.5f * (float)m_UBScreenData.FullResolution.y / (tanf(m_UBCameraData.Fov * 0.5f) * 2.0f);

			const float* P = glm::value_ptr(m_UBCameraData.Projection);
			const glm::vec4 projInfoPerspective = {
					2.0f / (P[4 * 0 + 0]),                  // (x) * (R - L)/N
					2.0f / (P[4 * 1 + 1]),                  // (y) * (T - B)/N
					-(1.0f - P[4 * 2 + 0]) / P[4 * 0 + 0],  // L/N
					-(1.0f + P[4 * 2 + 1]) / P[4 * 1 + 1],  // B/N
			};
			m_HBAOData.PerspectiveInfo = projInfoPerspective;
			m_HBAOData.bIsOrtho = false; //TODO: change depending on camera
			m_HBAOData.PowExponent = glm::max(AmbientOcclusionSettings.HBAO.Intensity, 0.f);
			m_HBAOData.NDotVBias = glm::min(std::max(0.f, AmbientOcclusionSettings.HBAO.Bias), 1.f);
			m_HBAOData.AOMultiplier = 1.f / (1.f - m_HBAOData.NDotVBias);
			m_HBAOData.ShadowTolerance = AmbientOcclusionSettings.ShadowTolerance;

			m_UBHBAOBuffer->SetData(Renderer::GetCurrentFrameInFlight(), Buffer{ &m_HBAOData,sizeof(m_HBAOData) });
		}
		//deinter leave
		{
			PF_PROFILE_FUNC("Deinterleave");

			//https://github.com/nvpro-samples/gl_ssao/blob/master/ssao.cpp @ line 914

			for (int i = 0; i < 2; i++)
			{

				Count<RenderPass> deinterleavePass = m_AmbientOcclusion.HBAO.DeinterleavePass[i];
				deinterleavePass->SetInput("u_Depth", m_PreDepthPass->GetOutput(0));

				Renderer::BeginRenderPass(m_CommandBuffer, deinterleavePass);
				deinterleavePass->PushData("u_Info", &i);
				Renderer::SubmitFullScreenQuad(m_CommandBuffer, deinterleavePass);
				Renderer::EndRenderPass(deinterleavePass);
			}
		}
		//HBAO Main Pass
		{
			PF_PROFILE_FUNC("HBAOMainPass");
			auto hbaoPass = m_AmbientOcclusion.HBAO.HBAOPass;

			hbaoPass->SetInput("u_LinearDepthTexArray", m_AmbientOcclusion.HBAO.DeinterleavePass[0]->GetOutput(0));
			hbaoPass->SetInput("u_ViewNormalsMaskTex", m_GeometryPass->GetOutput(1));
			hbaoPass->SetInput("o_Color", m_AmbientOcclusion.HBAO.HBAOOutputImage);

			Renderer::BeginComputePass(m_CommandBuffer, hbaoPass);
			hbaoPass->Dispatch(m_AmbientOcclusion.HBAO.WorkGroups);
			Renderer::EndComputePass(hbaoPass);
		}

		//Reinterleave
		{
			PF_PROFILE_FUNC("Reinterleave");
			auto reinterleavePass = m_AmbientOcclusion.HBAO.ReinterleavePass;
			ClearPass(reinterleavePass,true);
			reinterleavePass->SetInput("u_TexResultsArray", m_AmbientOcclusion.HBAO.HBAOOutputImage);
			Renderer::BeginRenderPass(m_CommandBuffer, reinterleavePass);
			Renderer::SubmitFullScreenQuad(m_CommandBuffer, reinterleavePass);
			Renderer::EndRenderPass(reinterleavePass);
		}

		//Blur pass
		{
			PF_PROFILE_FUNC("BlurPass");
			struct PushData
			{
				glm::vec2 invesDirection;
				float sharpness;
			}pushData;

			//auto inafdadas = sizeof(pushData);
			{
				PF_PROFILE_FUNC("BlurPass0");

				auto blurPass0 = m_AmbientOcclusion.HBAO.BlurPass[0];
				blurPass0->SetInput("u_InputTex", m_AmbientOcclusion.HBAO.ReinterleavePass->GetOutput(0));

				Renderer::BeginRenderPass(m_CommandBuffer, blurPass0);
				pushData = { {m_UBScreenData.InverseFullResolution.x,0},PostProcessSettings.AmbientOcclusionSettings.HBAO.BlurSharpness };
				blurPass0->PushData("u_PushData", &pushData);
				Renderer::SubmitFullScreenQuad(m_CommandBuffer, blurPass0);
				Renderer::EndRenderPass(blurPass0);
			}

			{
				PF_PROFILE_FUNC("BlurPass1");
				auto blurPass1 = m_AmbientOcclusion.HBAO.BlurPass[1];
				blurPass1->SetInput("u_InputTex", m_AmbientOcclusion.HBAO.BlurPass[0]->GetOutput(0));

				Renderer::BeginRenderPass(m_CommandBuffer, blurPass1);
				pushData = { {0,m_UBScreenData.InverseFullResolution.y},PostProcessSettings.AmbientOcclusionSettings.HBAO.BlurSharpness };

				blurPass1->PushData("u_PushData", &pushData);
				Renderer::SubmitFullScreenQuad(m_CommandBuffer, blurPass1);
				Renderer::EndRenderPass(blurPass1);
			}
		}
		
	}

	void WorldRenderer::BloomPass()
	{
		PF_PROFILE_FUNC();
		if (!BloomSettings.Enabled)
			return;
		struct BloomComputePushConstants
		{
			glm::vec4 Params;
			float LOD = 0.0f;
			int Mode = 0; // 0 = prefilter, 1 = downsample, 2 = firstUpsample, 3 = upsample
		} bloomComputePushConstants;
		bloomComputePushConstants.Params = { BloomSettings.Threshold, BloomSettings.Threshold - BloomSettings.Knee, BloomSettings.Knee * 2.0f, 0.25f / BloomSettings.Knee };
		Renderer::BeginComputePass(m_CommandBuffer, m_BloomComputePass);

		uint32_t workGroupsX;
		uint32_t workGroupsY;
		//prefilter
		{

			bloomComputePushConstants.Mode = 0;

			m_BloomComputePass->SetInput("u_SourceTexture", m_GeometryPass->GetOutput(0));
			m_BloomComputePass->SetInput("u_BloomTexture", m_GeometryPass->GetOutput(0));
			m_BloomComputePass->SetInput("o_Image", m_BloomComputeTextures[0]);

			workGroupsX = m_BloomComputeTextures[0]->GetWidth() / BLOOM_COMPUTE_WORK_GROUP_SIZE;
			workGroupsY = m_BloomComputeTextures[0]->GetHeight() / BLOOM_COMPUTE_WORK_GROUP_SIZE;

			m_BloomComputePass->PushData("u_Uniforms", &bloomComputePushConstants);
			m_BloomComputePass->Dispatch(workGroupsX, workGroupsY, 1);
			Renderer::Submit([image = m_BloomComputeTextures[0].As<VulkanTexture2D>()->GetImage().As<VulkanImage2D>(),commandBuffer = m_CommandBuffer]
				{
					VkImageMemoryBarrier imageMemoryBarrier = {};
					imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
					imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1};
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
					imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					vkCmdPipelineBarrier(
						commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						0,
						0, nullptr,
						0, nullptr,
						1, &imageMemoryBarrier);
				});
		}
		uint32_t mips = m_BloomComputeTextures[0]->GetMipLevelCount() - 2;
		{
			// downsample
			bloomComputePushConstants.Mode = 1;

			for (uint32_t i = 1; i < mips; i++)
			{
				auto imageView = m_BloomComputeTextures[0]->GetImageMip(i);
				auto mipDimension = imageView->GetMipSize();
				workGroupsX = (uint32_t)glm::ceil((float)mipDimension.x / (float)BLOOM_COMPUTE_WORK_GROUP_SIZE);
				workGroupsY = (uint32_t)glm::ceil((float)mipDimension.y / (float)BLOOM_COMPUTE_WORK_GROUP_SIZE);

				m_BloomComputePass->SetInput("o_Image", m_BloomComputeTextures[1]->GetImageMip(i));
				m_BloomComputePass->SetInput("u_SourceTexture", m_BloomComputeTextures[0]);
				m_BloomComputePass->SetInput("u_BloomTexture", m_GeometryPass->GetOutput(0));
				bloomComputePushConstants.LOD = i - 1.0f;

				m_BloomComputePass->PushData("u_Uniforms", &bloomComputePushConstants);
				m_BloomComputePass->Dispatch(workGroupsX, workGroupsY, 1);
				{
					Renderer::Submit([image = m_BloomComputeTextures[1].As<VulkanTexture2D>()->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer]
						{
							VkImageMemoryBarrier imageMemoryBarrier = {};
							imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
							imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
							imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
							imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
							imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1 };
							imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
							imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
							vkCmdPipelineBarrier(
								commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
								VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
								VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
								0,
								0, nullptr,
								0, nullptr,
								1, &imageMemoryBarrier);
						});
				}

				m_BloomComputePass->SetInput("o_Image", m_BloomComputeTextures[0]->GetImageMip(i));
				m_BloomComputePass->SetInput("u_SourceTexture", m_BloomComputeTextures[1]);
				m_BloomComputePass->SetInput("u_BloomTexture", m_GeometryPass->GetOutput(0));
				bloomComputePushConstants.LOD = (float)i;
				m_BloomComputePass->PushData("u_Uniforms", &bloomComputePushConstants);
				m_BloomComputePass->Dispatch(workGroupsX, workGroupsY, 1);


				{
					Renderer::Submit([image = m_BloomComputeTextures[1].As<VulkanTexture2D>()->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer]
						{
							VkImageMemoryBarrier imageMemoryBarrier = {};
							imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
							imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
							imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
							imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
							imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1 };
							imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
							imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
							vkCmdPipelineBarrier(
								commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
								VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
								VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
								0,
								0, nullptr,
								0, nullptr,
								1, &imageMemoryBarrier);
						});
				}
			}
		}


		//upsample first
		{
			bloomComputePushConstants.Mode = 2;
			workGroupsX *= 2;
			workGroupsY *= 2;
			m_BloomComputePass->SetInput("o_Image", m_BloomComputeTextures[2]->GetImageMip(mips - 2));
			m_BloomComputePass->SetInput("u_SourceTexture", m_BloomComputeTextures[0]);
			m_BloomComputePass->SetInput("u_BloomTexture", m_GeometryPass->GetOutput(0).As<Image2D>());
			bloomComputePushConstants.LOD--;
			m_BloomComputePass->PushData("u_Uniforms", &bloomComputePushConstants);
			auto dimension = m_BloomComputeTextures[2]->GetImageMip(mips - 2)->GetMipSize();
			workGroupsX = (uint32_t)glm::ceil((float)dimension.x / (float)BLOOM_COMPUTE_WORK_GROUP_SIZE);
			workGroupsY = (uint32_t)glm::ceil((float)dimension.y / (float)BLOOM_COMPUTE_WORK_GROUP_SIZE);
			m_BloomComputePass->Dispatch(workGroupsX, workGroupsY, 1);

			{
				Renderer::Submit([image = m_BloomComputeTextures[2].As<VulkanTexture2D>()->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer]
					{
						VkImageMemoryBarrier imageMemoryBarrier = {};
						imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
						imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
						imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
						imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
						imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1 };
						imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
						imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
						vkCmdPipelineBarrier(
							commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
							VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
							VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
							0,
							0, nullptr,
							0, nullptr,
							1, &imageMemoryBarrier);
					});
			}
		}

		//upsample
		{
			bloomComputePushConstants.Mode = 3;
			// Upsample
			for (int32_t mip = mips - 3; mip >= 0; mip--)
			{
				auto dimension = m_BloomComputeTextures[2]->GetImageMip(mip)->GetMipSize();
				workGroupsX = (uint32_t)glm::ceil((float)dimension.x / (float)BLOOM_COMPUTE_WORK_GROUP_SIZE);
				workGroupsY = (uint32_t)glm::ceil((float)dimension.y / (float)BLOOM_COMPUTE_WORK_GROUP_SIZE);

				m_BloomComputePass->SetInput("o_Image", m_BloomComputeTextures[2]->GetImageMip(mip));
				m_BloomComputePass->SetInput("u_SourceTexture", m_BloomComputeTextures[0]);
				m_BloomComputePass->SetInput("u_BloomTexture", m_BloomComputeTextures[2]);

				bloomComputePushConstants.LOD = (float)mip;
				m_BloomComputePass->PushData("u_Uniforms", &bloomComputePushConstants);
				m_BloomComputePass->Dispatch(workGroupsX, workGroupsY, 1);

				{
					Renderer::Submit([image = m_BloomComputeTextures[2].As<VulkanTexture2D>()->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer]
						{
							VkImageMemoryBarrier imageMemoryBarrier = {};
							imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
							imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
							imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
							imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
							imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1 };
							imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
							imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
							vkCmdPipelineBarrier(
								commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
								VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
								VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
								0,
								0, nullptr,
								0, nullptr,
								1, &imageMemoryBarrier);
						});
				}
			}
		}
		Renderer::EndComputePass(m_BloomComputePass);

	}

	void WorldRenderer::DOFPass()
	{
		PF_PROFILE_FUNC();

		if (!DOFSettings.Enabled)
			return;
		glm::uvec2 workgroupCount = (glm::uvec2(m_UBScreenData.FullResolution) + glm::uvec2(DOF_NUM_THREADS) - 1u) / glm::uvec2(DOF_NUM_THREADS);

		Renderer::BeginComputePass(m_CommandBuffer, m_DOFPass);
		glm::vec2 data{ DOFSettings.FocusDistance,DOFSettings.BlurSize };
		m_DOFPass->PushData("u_Uniforms", &data);
		m_DOFPass->Dispatch(m_DOFTexture->GetWidth()/DOF_NUM_THREADS, m_DOFTexture->GetHeight() / DOF_NUM_THREADS, 1);

		Renderer::Submit([image = m_DOFTexture.As<VulkanTexture2D>()->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer]
			{
				VkImageMemoryBarrier imageMemoryBarrier = {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageMemoryBarrier.image = image->Getinfo().ImageAlloc.Image;
				imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetSpecification().Mips, 0, 1 };
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				vkCmdPipelineBarrier(
					commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			});
		Renderer::EndComputePass(m_DOFPass);
	}
	void WorldRenderer::SSRPass()
	{
		if (!SSRSettings.Enabled)
			return;
		PF_PROFILE_FUNC();

		//set up
		{
			PF_PROFILE_FUNC("Set Up");

			m_UBSSR.HZBUvFactor = SSRSettings.HZBUvFactor;
			m_UBSSR.FadeIn = SSRSettings.FadeIn;
			m_UBSSR.Brightness = SSRSettings.Brightness;
			m_UBSSR.DepthTolerance = SSRSettings.DepthTolerance;
			m_UBSSR.FacingReflectionsFading = SSRSettings.FacingReflectionsFading;
			m_UBSSR.MaxSteps = SSRSettings.MaxSteps;
			m_UBSSR.NumDepthMips = SSRSettings.NumDepthMips;
			m_UBSSR.RoughnessDepthTolerance = SSRSettings.RoughnessDepthTolerance;
			m_UBSSR.bHalfRes = SSRSettings.HalfRes;
			m_UBSSR.bEnableConeTracing = SSRSettings.EnableConeTracing;
			m_UBSSR.LuminanceFactor = SSRSettings.LuminanceFactor;

			m_UBSSR.AmbientOcclusionType = !AmbientOcclusionSettings.Enabled ? 0 : (uint32_t)AmbientOcclusionSettings.Type;
			// it does not resize every frame because images only resize when the size is different
			//constexpr uint32_t WORK_GROUP_SIZE = 8u;
			//glm::uvec2 ssrSize = SSRSettings.HalfRes ? (m_UBScreenData.FullResolution + 1.f) / 2.f : m_UBScreenData.FullResolution;
			//m_SSR.SSRImage->Resize(ssrSize.x, ssrSize.y);
			//m_SSR.PreConvolutedTexture->Resize(ssrSize.x, ssrSize.y);
			//ssrSize += WORK_GROUP_SIZE - ssrSize % WORK_GROUP_SIZE;
			//m_SSR.WorkGroups.x = ssrSize.x / WORK_GROUP_SIZE;
			//m_SSR.WorkGroups.y = ssrSize.y / WORK_GROUP_SIZE;
		}
		//ssr
		{
			PF_PROFILE_FUNC("ssr");

			auto renderPass = m_SSR.SSRPass;
			renderPass->SetInput("o_Color", m_SSR.SSRImage);
			renderPass->SetInput("u_PreConvultedMap", m_SSR.PreConvolutedTexture);
			renderPass->SetInput("u_VisibilityMap", m_SSR.VisibilityTexture);
			renderPass->SetInput("u_HIZMap", m_SSR.HierarchicalDepthTexture);
			renderPass->SetInput("u_NormalMap", m_GeometryPass->GetOutput(1));
			renderPass->SetInput("u_MetalnessRoughnessMap", m_GeometryPass->GetOutput(2));

			if (AmbientOcclusionSettings.Enabled)
			{

				switch (AmbientOcclusionSettings.Type)
				{
					case AmbientOcclusion::AmbientOcclusionType::HBAO:
						renderPass->SetInput("u_AmbientOcclusionMap", m_AmbientOcclusion.HBAO.BlurPass[1]->GetOutput(0));
					break;
				}
			}

			Renderer::BeginComputePass(m_CommandBuffer, renderPass);

			renderPass->PushData("u_PushData", &m_UBSSR);
			renderPass->Dispatch(m_SSR.WorkGroups);
			Renderer::EndComputePass(renderPass);

		}
		//ssr composite
		{
			auto renderPass = m_SSR.SSRCompositePass;

			renderPass->SetInput("u_SSR", m_SSR.SSRImage);

			Renderer::BeginRenderPass(m_CommandBuffer, renderPass);

			Renderer::SubmitFullScreenQuad(m_CommandBuffer, renderPass);

			Renderer::EndRenderPass(renderPass);
		}
	}

	void WorldRenderer::HZBPass()
	{
		//HZB
		{
			PF_PROFILE_FUNC("HZB");

			auto hierarchicalDepthPass = m_SSR.HierarchicalDepthPass;
			auto hierarchicalZTexture = m_SSR.HierarchicalDepthTexture;
			constexpr uint32_t maxMipBatchSize = 4;
			const uint32_t hzbMipCount = hierarchicalZTexture->GetMipLevelCount();

			Renderer::BeginComputePass(m_CommandBuffer, hierarchicalDepthPass);

			auto ReduceHZB = [hzbMipCount, maxMipBatchSize, &hierarchicalZTexture, hierarchicalDepthPass]
			(const uint32_t startDestMip, const uint32_t parentMip, Count<Image2D> parentImage, const glm::vec2& DispatchThreadIdToBufferUV, const glm::vec2& InputViewportMaxBound, const bool isFirstPass)
			{
				struct HierarchicalZComputePushConstants
				{
					glm::vec2 DispatchThreadIdToBufferUV;
					glm::vec2 InputViewportMaxBound;
					glm::vec2 InvSize;
					int FirstLod;
					int bIsFirstPass;
				} hierarchicalZComputePushConstants;

				hierarchicalZComputePushConstants.bIsFirstPass = isFirstPass;
				hierarchicalZComputePushConstants.FirstLod = (int)startDestMip;
				hierarchicalZComputePushConstants.DispatchThreadIdToBufferUV = DispatchThreadIdToBufferUV;
				hierarchicalZComputePushConstants.InputViewportMaxBound = InputViewportMaxBound;

				std::array<Count<ImageView>, 4> hbImageViews{};

				const uint32_t endDestMip = glm::min(startDestMip + maxMipBatchSize, hzbMipCount);
				uint32_t destMip;
				for (destMip = startDestMip; destMip < endDestMip; ++destMip)
				{
					uint32_t idx = destMip - startDestMip;
					hbImageViews[idx] = hierarchicalZTexture->GetImageMip(destMip);
				}
				destMip -= startDestMip;
				for (; destMip < maxMipBatchSize; ++destMip)
				{
					hbImageViews[destMip] = hierarchicalZTexture->GetImageMip(hzbMipCount - 1);
				}

				std::vector<Count<ImageView>> imageViews;
				for (uint32_t i = 0; i < maxMipBatchSize; ++i)
				{
					imageViews.emplace_back(hbImageViews[i]);
				}
				//hbImageViews[4] = parentImage
				hierarchicalDepthPass->SetInput("o_HZB", imageViews);
				hierarchicalDepthPass->SetInput("u_InputDepth", parentImage);

				const glm::ivec2 srcSize{ Math::DivideAndRoundUp(glm::uvec2(parentImage->GetSize().X,parentImage->GetSize().Y), 1u << parentMip) };
				const glm::ivec2 dstSize = Math::DivideAndRoundUp(glm::uvec2(hierarchicalZTexture->GetSize().X, hierarchicalZTexture->GetSize().Y), 1u << startDestMip);
				hierarchicalZComputePushConstants.InvSize = glm::vec2{ 1.0f / (float)srcSize.x, 1.0f / (float)srcSize.y };
				hierarchicalDepthPass->PushData("u_PushData", &hierarchicalZComputePushConstants);

				hierarchicalDepthPass->Dispatch(Math::DivideAndRoundUp(dstSize.x, 8), Math::DivideAndRoundUp(dstSize.y, 8), 1);

			};

			auto srcDepthImage = m_PreDepthPass->GetOutput(0).As<Image2D>();

			auto srcSize = srcDepthImage->GetSize();


			ReduceHZB(0, 0, srcDepthImage, { 1.0f / glm::vec2{ srcSize.X,srcSize.Y } }, { (glm::vec2{ srcSize.X,srcSize.Y } - 0.5f) / glm::vec2{ srcSize.X,srcSize.Y } }, true);

			for (uint32_t startDestMip = maxMipBatchSize; startDestMip < hzbMipCount; startDestMip += maxMipBatchSize)
			{
				auto newSize = Math::DivideAndRoundUp(glm::uvec2{ hierarchicalZTexture->GetSize().X,hierarchicalZTexture->GetSize().Y }, 1u << uint32_t(startDestMip - 1));

				srcSize = { newSize.x, newSize.y };

				ReduceHZB(startDestMip, startDestMip - 1, hierarchicalZTexture->GetImage(), { 2.0f / glm::vec2{srcSize.X,srcSize.Y} }, glm::vec2{ 1.0f }, false);

			}
			Renderer::EndComputePass(hierarchicalDepthPass);
		}
	}
	void WorldRenderer::PreIntegrationPass()
	{
		//preIntegration
		{
			PF_PROFILE_FUNC("preIntegration");

			auto visibilityTexture = m_SSR.VisibilityTexture;

			struct PreIntegrationComputePushConstants
			{
				glm::vec2 HZBResFactor;
				glm::vec2 ResFactor;
				glm::vec2 ProjectionParams; //(x) = Near plane, (y) = Far plane
				int PrevLod = 0;
			};
			PreIntegrationComputePushConstants preIntegrationComputePushConstants;
			//Renderer::ClearImage(m_CommandBuffer, visibilityTexture->GetImage());

			glm::vec2 projectionParams = { m_UBCameraData.FarPlane, m_UBCameraData.NearPlane }; // Reversed 

			auto preIntegration = m_SSR.PreIntegrationPass;
		

			Renderer::BeginComputePass(m_CommandBuffer, preIntegration);

			const uint32_t mipLevelCount = visibilityTexture->GetMipLevelCount();
			for (uint32_t i = 0; i < mipLevelCount; i++)
			{
				auto [mipWidth, mipHeight] = visibilityTexture->GetImage()->GetMipSize(i);
				const auto workGroupsX = (uint32_t)glm::ceil((float)mipWidth / 8.0f);
				const auto workGroupsY = (uint32_t)glm::ceil((float)mipHeight / 8.0f);

				auto imageView = visibilityTexture->GetImage()->CreateOrGetImageMip(i);

				preIntegration->SetInput("u_HZB", m_SSR.HierarchicalDepthTexture);
				preIntegration->SetInput("u_VisibilityTex", m_SSR.VisibilityTexture->GetImage());
				auto [width, height] = visibilityTexture->GetImage()->GetMipSize(i);
				glm::vec2 resFactor = 1.0f / glm::vec2{ width, height };
				preIntegrationComputePushConstants.HZBResFactor = resFactor * SSRSettings.HZBUvFactor;
				preIntegrationComputePushConstants.ResFactor = resFactor;
				preIntegrationComputePushConstants.ProjectionParams = projectionParams;
				if(i == 0)
					preIntegrationComputePushConstants.PrevLod = (int)i;
				else
					preIntegrationComputePushConstants.PrevLod = (int)i - 1;

				preIntegration->SetInput("o_VisibilityImage", imageView);
				
				preIntegration->PushData("u_Info", &preIntegrationComputePushConstants);
				preIntegration->Dispatch(workGroupsX, workGroupsY, 1);

			}
			Renderer::EndComputePass(preIntegration);
		}
	}
	void WorldRenderer::PreConvolutePass()
	{
		//preConvulution
		{
			PF_PROFILE_FUNC("PreConvulute");

			struct PreConvolutionComputePushConstants
			{
				int PrevLod = 0;
				int Mode = 0; // 0 = Copy, 1 = GaussianHorizontal, 2 = GaussianVertical
			} preConvolutionComputePushConstants;

			//Might change to be maximum res used by other techniques other than SSR.
			int halfRes = int(SSRSettings.HalfRes);
			auto preConvolutePass = m_SSR.PreConvolutePass;
			Renderer::BeginComputePass(m_CommandBuffer, preConvolutePass);


			Count<Image2D> preConvolutedImage = m_SSR.PreConvolutedTexture->GetImage();

			auto inputColorImage = m_GeometryPass->GetOutput(0);
			uint32_t workGroupsX = (uint32_t)glm::ceil((float)inputColorImage->GetWidth() / 16.0f);
			uint32_t workGroupsY = (uint32_t)glm::ceil((float)inputColorImage->GetHeight() / 16.0f);

			preConvolutePass->PushData("u_Info", &preConvolutionComputePushConstants);
			preConvolutePass->SetInput("u_Input", inputColorImage);
			preConvolutePass->SetInput("o_Image", preConvolutedImage->CreateOrGetImageMip(0));
			preConvolutePass->Dispatch(workGroupsX, workGroupsY, 1);


			const uint32_t mipCount = m_SSR.PreConvolutedTexture->GetMipLevelCount();

			for (uint32_t mip = 1; mip < mipCount; mip++)
			{
				auto [mipWidth, mipHeight] = preConvolutedImage->GetMipSize(mip);
				workGroupsX = (uint32_t)glm::ceil((float)mipWidth / 16.0f);
				workGroupsY = (uint32_t)glm::ceil((float)mipHeight / 16.0f);

				auto imageView = preConvolutedImage->CreateOrGetImageMip(mip);

				preConvolutePass->SetInput("u_Input", preConvolutedImage);
				preConvolutePass->SetInput("o_Image", imageView);
				preConvolutionComputePushConstants.PrevLod = (int)mip - 1;

				auto blur = [&](const int mode)
				{
					preConvolutionComputePushConstants.Mode = (int)mode;
					preConvolutePass->PushData("u_Info", &preConvolutionComputePushConstants);
					preConvolutePass->Dispatch(workGroupsX, workGroupsY, 1);
				};



				blur(1); // Horizontal blur
				blur(2); // Vertical Blur
			}
			Renderer::EndComputePass(preConvolutePass);

		}
	}

	void WorldRenderer::NewSSRPass()
	{
		struct SSSRPush
		{
			uint32_t SamplesPerQuad;
			uint32_t TemporalVarianceGuidedTracingEnabled;
			uint32_t MostDetailedMip = 0;
			float RoughnessThreshold; // Max roughness stop to reflection sample.
			float TemporalVarianceThreshold;
		};

		SSSRPush pushConst =
		{
			.SamplesPerQuad = 1,
			.TemporalVarianceGuidedTracingEnabled = 1,
			.MostDetailedMip = 0,
			.RoughnessThreshold = 0.2f,
			.TemporalVarianceThreshold = 0.0f,
		};

		
		//set up
		{
			auto sbRayCounter = m_NewSSR.SBRayCounter->GetBuffer(Renderer::GetCurrentFrameInFlight());
			Buffer buffer(sbRayCounter->GetSize());
			buffer.Fill(0);
			sbRayCounter->SetData(buffer);
			buffer.Release();

			Renderer::ClearImage(m_CommandBuffer, m_NewSSR.Radiance);
			Renderer::ClearImage(m_CommandBuffer, m_NewSSR.Variance);
			Renderer::ClearImage(m_CommandBuffer, m_NewSSR.ExtractRoughness);
			Renderer::ClearImage(m_CommandBuffer, m_NewSSR.SampleCount);
			Renderer::ClearImage(m_CommandBuffer, m_NewSSR.ReprojectImage);
			Renderer::ClearImage(m_CommandBuffer, m_NewSSR.AverageRadiance);
		}
		//new ssr pass
		{
			std::vector<Count<ImageView>> imageViews(m_NewSSR.HierarchalDepthDownSamplerTexture->GetMipLevelCount());

			for (int i = 0; i < m_NewSSR.HierarchalDepthDownSamplerTexture->GetMipLevelCount(); i++)
				imageViews[i] = m_NewSSR.HierarchalDepthDownSamplerTexture->GetImageMip(i);

			for (int i = imageViews.size(); i < 13; i++)
			{
				// want size == 13
				imageViews.push_back(m_NewSSR.HierarchalDepthDownSamplerTexture->GetImageMip(0));
			}

			m_NewSSR.HierarchalDepthDownSamplerPass->SetInput("o_DepthHierarchy", imageViews);
			Renderer::BeginComputePass(m_CommandBuffer, m_NewSSR.HierarchalDepthDownSamplerPass);
			m_NewSSR.HierarchalDepthDownSamplerPass->PushData("u_PushData", &pushConst);
			m_NewSSR.HierarchalDepthDownSamplerPass->Dispatch(DivideRoundingUp((uint32_t)m_UBScreenData.FullResolution.x, 8u), DivideRoundingUp((uint32_t)m_UBScreenData.FullResolution.y, 8u), 1);
			Renderer::EndComputePass(m_NewSSR.HierarchalDepthDownSamplerPass);
		}

		//Blue noise
		{

			//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/d7531ae47d8b36a5d4025663e731a47a38be882f/sdk/src/components/sssr/ffx_sssr.cpp#L36
			//shedule dispatch
			m_NewSSR.BlueNoisePass->SetInput("o_BlueNoiseTexture", m_NewSSR.BlueNoiseImage);
			Renderer::BeginComputePass(m_CommandBuffer, m_NewSSR.BlueNoisePass);
			m_NewSSR.BlueNoisePass->Dispatch(128 / 8u, 128 / 8u, 1);
			Renderer::EndComputePass(m_NewSSR.BlueNoisePass);
		}


		//Tile Classification
		{
			m_NewSSR.TileClassification->SetInput("u_CubeMapPrefilter", m_Environment->GetPrefilterMap());
			m_NewSSR.TileClassification->SetInput("u_SSRVarianceHistory", m_NewSSR.PreviousVariance);
			m_NewSSR.TileClassification->SetInput("o_SSRIntersection", m_NewSSR.Radiance);
			m_NewSSR.TileClassification->SetInput("o_SSRExtractRoughness", m_NewSSR.ExtractRoughness);
			m_NewSSR.TileClassification->SetInput("u_MetalnessRoughness", m_GeometryPass->GetOutput(2));
			Renderer::BeginComputePass(m_CommandBuffer, m_NewSSR.TileClassification);
			m_NewSSR.TileClassification->PushData("u_PushData", &pushConst);
			m_NewSSR.TileClassification->Dispatch(GetGroupCount(m_UBScreenData.FullResolution.x, 8), GetGroupCount(m_UBScreenData.FullResolution.y, 8), 1.);
			Renderer::EndComputePass(m_NewSSR.TileClassification);
		}

		
		// Intersect Args
		{
			Renderer::BeginComputePass(m_CommandBuffer, m_NewSSR.IntersectArgs);
			m_NewSSR.IntersectArgs->PushData("u_PushData", &pushConst);
			m_NewSSR.IntersectArgs->Dispatch(1, 1, 1);
			Renderer::EndComputePass(m_NewSSR.IntersectArgs);
		}
		//auto data = m_NewSSR.SBIntersectCommand->GetBuffer(Renderer::GetCurrentFrameInFlight())->GetDataRaw().As<NewSSR::GPUDispatchIndirectCommand>();
		//PF_ENGINE_INFO("SBINTERSECT GPUDispatchIndirectCommand X: {} Y: {} Z:{}", data->x, data->y, data->z);
		//intersect
		{
			auto intersect = m_NewSSR.Intersect;

			intersect->SetInput("u_GeometryTexture", m_CompositePass->GetOutput(0));
			intersect->SetInput("o_SSRIntersectionImage", m_NewSSR.Radiance);
			intersect->SetInput("u_SSSRExtractRoughnessImage", m_NewSSR.ExtractRoughness);
			intersect->SetInput("u_CubeMapPrefilter", m_Environment->GetPrefilterMap());
			intersect->SetInput("u_VelocityMap", m_GeometryPass->GetOutput(3));

			Renderer::BeginComputePass(m_CommandBuffer, intersect);
			intersect->PushData("u_PushData", &pushConst);
			intersect->DispatchIndirect(m_NewSSR.SBIntersectCommand, 0);
			Renderer::EndComputePass(intersect);
		}
		//auto data = m_NewSSR.SBDenoiseCommand->GetBuffer(Renderer::GetCurrentFrameInFlight())->GetDataRaw().As<NewSSR::GPUDispatchIndirectCommand>();
		//PF_ENGINE_INFO("SBDENOISE GPUDispatchIndirectCommand X: {} Y: {} Z:{}", data->x, data->y, data->z);
		//reproject
		{
			/*
			auto reproject = m_NewSSR.Reproject;


			reproject->SetInput("u_SSSRIntersectionMap", m_NewSSR.Radiance);
			reproject->SetInput("u_PreviewDepthMap", m_PrevDepthImage);
			reproject->SetInput("u_PrevSSSRRadianceMap", m_NewSSR.PreviousRadiance);
			reproject->SetInput("u_PrevNormalMap", m_PrevNormalImage);
			reproject->SetInput("u_SSSRVarianceHistory", m_NewSSR.PreviousVariance);
			reproject->SetInput("u_SSSRExtractRoughness", m_NewSSR.ExtractRoughness);
			reproject->SetInput("u_PrevSampleCount", m_NewSSR.PreviousSampleCount);

			reproject->SetInput("o_SSSRReprojectedRadiance", m_NewSSR.ReprojectImage);
			reproject->SetInput("o_SSSRAverageRadiance", m_NewSSR.AverageRadiance);
			reproject->SetInput("o_SSRVariance", m_NewSSR.Variance);
			reproject->SetInput("o_SSRSampleCount", m_NewSSR.SampleCount);

			reproject->SetInput("u_SSSRPrevExtractRoughness", m_NewSSR.PreviousExtractRoughness);
			reproject->SetInput("u_VelocityMap", m_GeometryPass->GetOutput(3));

			Renderer::BeginComputePass(m_CommandBuffer, reproject);
			reproject->PushData("u_PushData", &pushConst);
			reproject->DispatchIndirect(m_NewSSR.SBDenoiseCommand, 0);
			Renderer::EndComputePass(reproject);
			*/
		}
		/*
		//prefilter
		{
			auto prefilter = m_NewSSR.Prefilter;

			prefilter->SetInput("u_SSSRAverageRadiance", m_NewSSR.AverageRadiance);
			prefilter->SetInput("u_SSSRIntersection", m_NewSSR.Radiance);
			prefilter->SetInput("u_SSSRVariance", m_NewSSR.Variance);
			prefilter->SetInput("u_SSSRExtractRoughness", m_NewSSR.ExtractRoughness);

			prefilter->SetInput("o_SSSRTemporalFilterRadiance", m_NewSSR.Radiance);
			prefilter->SetInput("o_SSSRTemporalFilterVariance", m_NewSSR.Variance);

			Renderer::BeginComputePass(m_CommandBuffer, prefilter);
			prefilter->PushData("u_PushData", &pushConst);
			prefilter->DispatchIndirect(m_NewSSR.SBDenoiseCommand, 0);
			Renderer::EndComputePass(prefilter);
		}
		
		//Temporal
		{
			auto temporal = m_NewSSR.Temporal;

			temporal->SetInput("u_SSSRAverageRadiance", m_NewSSR.AverageRadiance);
			temporal->SetInput("U_SSSRPrefilterRadiance", m_NewSSR.PreviousRadiance);
			temporal->SetInput("u_SSSRReprojectedRadiance", m_NewSSR.ReprojectImage);
			temporal->SetInput("u_SSSRPrefilterVariance", m_NewSSR.PreviousVariance);
			temporal->SetInput("u_PrevSampleCount", m_NewSSR.PreviousSampleCount);
			temporal->SetInput("u_SSSRExtractRoughness", m_NewSSR.ExtractRoughness);


			temporal->SetInput("o_SSSRTemporalFilterRadiance", m_NewSSR.Radiance);
			temporal->SetInput("o_SSSRTemporalFilterVariance", m_NewSSR.Variance);

			Renderer::BeginComputePass(m_CommandBuffer, temporal);
			temporal->PushData("u_PushData", &pushConst);
			temporal->DispatchIndirect(m_NewSSR.SBDenoiseCommand, 0);
			Renderer::EndComputePass(temporal);
		
		}

		{
			auto apply = m_NewSSR.ApplyPass;

			apply->SetInput("u_BRDFLUT", Renderer::GetBRDFLut());
			apply->SetInput("o_OutputGeometryImage", m_GeometryPass->GetOutput(0));
			apply->SetInput("u_MetalnessRoughnessMap", m_GeometryPass->GetOutput(2));
			apply->SetInput("u_WorldTexture", m_GeometryPass->GetOutput(0));
			apply->SetInput("u_SSSRIntersectionMap", m_NewSSR.Radiance);
			Renderer::BeginComputePass(m_CommandBuffer, apply);
			apply->PushData("u_PushData", &pushConst);
			apply->Dispatch(GetGroupCount(m_UBScreenData.FullResolution.x, 8), GetGroupCount(m_UBScreenData.FullResolution.y, 8), 1);
			Renderer::EndComputePass(apply);
		}
		*/
		//endindg
		{
			Math::Swap(m_NewSSR.Variance, m_NewSSR.PreviousVariance);
			Math::Swap(m_NewSSR.Radiance, m_NewSSR.PreviousRadiance);
			Math::Swap(m_NewSSR.ExtractRoughness, m_NewSSR.PreviousExtractRoughness);
			Math::Swap(m_NewSSR.SampleCount, m_NewSSR.PreviousSampleCount);
			Math::Swap(m_NewSSR.ReprojectImage, m_NewSSR.PreviousReprojectImage);

			
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
			auto& transformStorage = (*m_CurTransformMap)[meshKey].Transforms.emplace_back();
			transformStorage.Transform = subMeshTransform;

			if ((*m_PrevTransformMap).find(meshKey) == (*m_PrevTransformMap).end())
			{
				(*m_PrevTransformMap)[meshKey] = (*m_CurTransformMap)[meshKey];
			}
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
		auto& transformStorage = (*m_CurTransformMap)[meshKey].Transforms.emplace_back();
		transformStorage.Transform = subMeshTransform;

		if ((*m_PrevTransformMap).find(meshKey) == (*m_PrevTransformMap).end())
		{
			(*m_PrevTransformMap)[meshKey] = (*m_CurTransformMap)[meshKey];
		}
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
			auto& transformStorage = (*m_CurTransformMap)[meshKey].Transforms.emplace_back();
			transformStorage.Transform = subMeshTransform;

			if ((*m_PrevTransformMap).find(meshKey) == (*m_PrevTransformMap).end())
			{
				(*m_PrevTransformMap)[meshKey] = (*m_CurTransformMap)[meshKey];
			}
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

	void WorldRenderer::SubmitPhysicsDynamicDebugMesh(Count<DynamicMesh> mesh, uint32_t subMeshIndex, const glm::mat4& transform)
	{

		PF_PROFILE_FUNC();
		//TODO FASTER HASH FUNCTION FOR MESHKEY
		//PF_CORE_ASSERT(mesh->GetID(), "Mesh ID cannot be zero");

		AssetID meshID = mesh->GetID();
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		const auto& submeshData = meshSource->GetSubMesh(subMeshIndex);
		const auto& subMesh = meshSource->GetSubMeshes().at(subMeshIndex);

		glm::mat4 subMeshTransform = transform * subMesh.Transform;

		uint32_t materialIndex = subMesh.MaterialIndex;

		AssetID materialHandle = m_GeometryWireFramePassMaterialAsset->GetID();
		PF_CORE_ASSERT(materialHandle, "Material ID cannot be zero");

		MeshKey meshKey = { meshID, materialHandle, subMeshIndex, false };
		auto& transformStorage = (*m_CurTransformMap)[meshKey].Transforms.emplace_back();
		transformStorage.Transform = subMeshTransform;

		if ((*m_PrevTransformMap).find(meshKey) == (*m_PrevTransformMap).end())
		{
			(*m_PrevTransformMap)[meshKey] = (*m_CurTransformMap)[meshKey];
		}
		// geo pass
		{
			auto& dc = m_DynamicColliderDrawList[meshKey];
			dc.Mesh = mesh;
			dc.SubMeshIndex = subMeshIndex;
			dc.InstanceCount++;
			dc.OverrideMaterial = nullptr;
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

		if (environment->GetPrefilterMap() == nullptr || environment->GetIrradianceMap() == nullptr)
			return;


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
		
		Buffer buffer{ spotLights.SpotLights.data(), spotLights.SpotLights.size() * sizeof(SpotLight) };
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
	
	void WorldRenderer::ClearPass(Count<RenderPass> renderPass, bool explicitClear)
	{
		PF_PROFILE_FUNC();
		Renderer::BeginRenderPass(m_CommandBuffer, renderPass, explicitClear);
		Renderer::EndRenderPass(renderPass);
	}
}
